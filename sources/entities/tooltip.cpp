/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <memory>

#include "guillaume/entities/tooltip.hpp"
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/motion/easing.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	Tooltip::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							  ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Tooltip>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Tooltip> Tooltip::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<Tooltip>(
			this->getComponentRegistry(), _config, _variant, _text, _title);
		return entity;
	}

	void Tooltip::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Plain;
		_text.clear();
		_title.clear();

		_config.axis = components::Layout::Axis::Vertical;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Center;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		_config.padding = 8.0f;
		_config.spacing = 2.0f;
	}

	Tooltip::Builder &
		Tooltip::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Tooltip::Builder &Tooltip::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Tooltip::Builder &Tooltip::Builder::withText(const std::string &text)
	{
		_text = text;
		return *this;
	}

	Tooltip::Builder &Tooltip::Builder::withTitle(const std::string &title)
	{
		_title = title;
		return *this;
	}

	Tooltip::Builder &Tooltip::Builder::withFixedWidth(float width)
	{
		_config.hasFixedWidth = true;
		_config.fixedWidth	  = width;
		return *this;
	}

	std::shared_ptr<Tooltip> Tooltip::Director::makeTooltip(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::string &text, const std::string &title)
	{
		return builder.withVariant(variant)
			.withText(text)
			.withTitle(title)
			.registerEntity(parent);
	}

	Tooltip::Tooltip(ecs::ComponentRegistry &registry,
					 const SurfaceConfig &config, Variant variant,
					 const std::string &text, const std::string &title)
		: SurfaceBase<components::Overlay, components::Elevation,
					  components::Animation, components::Accessibility>(registry,
															  config)
		, _variant(variant)
		, _text(text)
		, _title(title)
		, _titleEntity()
		, _textEntity()
	{
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setRole(components::AccessibilityRole::Text)
			.setLiveRegion(components::LiveRegion::Polite)
			.setLabel(text);
		getComponentRegistry()
			.getComponent<components::Animation>(getIdentifier())
			.setDuration(1.0f)
			.setEasing(motion::Easing::Linear)
			.setLooping(true)
			.setOnSampleHandler([this](float progress) {
				float delta = progress - _lastProgress;
				if (delta < 0.0f) {
					delta += 1.0f;
				}
				_lastProgress = progress;
				advance(delta);
			});
		getComponentRegistry()
			.getComponent<components::Overlay>(getIdentifier())
			.setOnDismissHandler([this]() {
				if (_visible) {
					_visible = false;
					applyVisibility();
				}
			});
		applyVariant();
		applyOverlay();
		applyVisibility();
	}

	Tooltip &Tooltip::setAnchor(std::shared_ptr<ecs::Entity> anchor)
	{
		_anchor		  = anchor;
		_hoverTime	  = 0.0f;
		_leaveTime	  = 0.0f;
		_lastProgress = 0.0f;
		auto &ticker =
			getComponentRegistry().getComponent<components::Animation>(
				getIdentifier());
		if (anchor != nullptr) {
			ticker.play();
		} else {
			ticker.stop();
		}
		return *this;
	}

	Tooltip &Tooltip::setEnterDelay(float seconds)
	{
		if (seconds < 0.0f) {
			throw std::invalid_argument("Tooltip delay must be >= 0");
		}
		_enterDelay = seconds;
		return *this;
	}

	Tooltip &Tooltip::setLeaveDelay(float seconds)
	{
		if (seconds < 0.0f) {
			throw std::invalid_argument("Tooltip delay must be >= 0");
		}
		_leaveDelay = seconds;
		return *this;
	}

	float Tooltip::getEnterDelay(void) const
	{
		return _enterDelay;
	}

	float Tooltip::getLeaveDelay(void) const
	{
		return _leaveDelay;
	}

	Tooltip &Tooltip::setViewport(float width, float height)
	{
		_viewport = std::make_pair(width, height);
		if (_visible) {
			place();
		}
		return *this;
	}

	Tooltip &Tooltip::show(void)
	{
		_visible = true;
		place();
		applyVisibility();
		return *this;
	}

	Tooltip &Tooltip::hide(void)
	{
		_visible = false;
		applyVisibility();
		return *this;
	}

	bool Tooltip::isVisible(void) const
	{
		return _visible;
	}

	void Tooltip::advance(float deltaTime)
	{
		const auto anchor = _anchor.lock();
		if (anchor == nullptr) {
			return;
		}

		if (isHovered(getComponentRegistry(), anchor->getIdentifier())) {
			_leaveTime = 0.0f;
			_hoverTime += deltaTime;
			if (!_visible && _hoverTime >= _enterDelay) {
				show();
			}
			return;
		}

		_hoverTime = 0.0f;
		if (_visible) {
			_leaveTime += deltaTime;
			if (_leaveTime >= _leaveDelay) {
				_leaveTime = 0.0f;
				hide();
			}
		}
	}

	void Tooltip::place(void)
	{
		const auto anchor = _anchor.lock();
		if (anchor == nullptr) {
			return;
		}
		auto &registry = getComponentRegistry();
		if (!registry.hasComponent<components::Transform>(
				anchor->getIdentifier())
			|| !registry.hasComponent<components::Bound>(
				anchor->getIdentifier())) {
			return;
		}

		const auto anchorPose =
			registry.getComponent<components::Transform>(anchor->getIdentifier())
				.getPose();
		const auto &anchorBound =
			registry.getComponent<components::Bound>(anchor->getIdentifier());
		const auto &bound =
			registry.getComponent<components::Bound>(getIdentifier());

		const float width  = bound.getWidth();
		const float height = bound.getHeight();
		float x = anchorPose.getPosition().getX()
			+ (anchorBound.getWidth() - width) / 2.0f;
		float y = anchorPose.getPosition().getY() + anchorBound.getHeight();

		if (_viewport.has_value()) {
			const float viewportWidth  = _viewport->first;
			const float viewportHeight = _viewport->second;
			if (y + height > viewportHeight) {
				y = anchorPose.getPosition().getY() - height;
			}
			x = std::clamp(x, 0.0f, std::max(0.0f, viewportWidth - width));
			y = std::clamp(y, 0.0f, std::max(0.0f, viewportHeight - height));
		}

		setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(x, y,
										anchorPose.getPosition().getZ()),
			anchorPose.getOrientation()));
	}

	void Tooltip::applyVisibility(void)
	{
		auto &overlay =
			getComponentRegistry().getComponent<components::Overlay>(
				getIdentifier());
		if (overlay.isVisible() != _visible) {
			overlay.setVisible(_visible);
		}
		applyVariant();
		const bool rich = (_variant == Variant::Rich);
		if (_titleEntity != nullptr) {
			_titleEntity->setColor(_visible
									   ? schemeColor(SchemeColorRole::OnSurface)
									   : transparentColor());
		}
		if (_textEntity != nullptr) {
			const auto role = rich ? SchemeColorRole::OnSurfaceVariant
								   : SchemeColorRole::InverseOnSurface;
			_textEntity->setColor(_visible ? schemeColor(role)
										   : transparentColor());
		}
	}

	Tooltip::~Tooltip(void)
	{
	}

	float Tooltip::getVariantHeight(void) const
	{
		switch (_variant) {
			case Variant::Rich:
				return 64.0f;
			case Variant::Plain:
			default:
				return 32.0f;
		}
	}

	void Tooltip::applyVariant(void)
	{
		auto config = getSurfaceConfig();

		config.axis = components::Layout::Axis::Vertical;
		config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Center;
		config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		config.padding		  = 8.0f;
		config.spacing		  = 2.0f;
		getComponentRegistry()
			.getComponent<components::Elevation>(getIdentifier())
			.setElevationLevel(_variant == Variant::Rich && _visible
								   ? theme::ElevationLevel::Level2
								   : theme::ElevationLevel::Level0);
		config.borderRadius	  = (_variant == Variant::Rich)
			? getShapeRadius(theme::ShapeRole::Medium)
			: getShapeRadius(theme::ShapeRole::ExtraSmall);
		config.hasFixedHeight = true;
		config.fixedHeight	  = getVariantHeight();

		setSurfaceConfig(config);
		const auto container = (_variant == Variant::Rich)
			? schemeColor(SchemeColorRole::SurfaceContainer)
			: schemeColor(SchemeColorRole::InverseSurface);
		setColor(_visible ? container : transparentColor());
	}

	void Tooltip::applyOverlay(void)
	{
		auto &overlay =
			this->getComponentRegistry().getComponent<components::Overlay>(
				this->getIdentifier());

		overlay.setModal(false);
		overlay.setDismissOnOutsideClick(false);
		overlay.setDismissOnEscape(true);
	}

	void Tooltip::buildContent(void)
	{
		const auto foregroundColor = transparentColor();

		if (_variant == Variant::Rich && _titleEntity == nullptr
			&& !_title.empty()) {
			_titleEntity = buildText(this->getComponentRegistry(), *this,
									 this->shared_from_this(), _title, theme::TypeRole::TitleSmall,
									 foregroundColor);
		}

		if (_textEntity == nullptr && !_text.empty()) {
			_textEntity = buildText(this->getComponentRegistry(), *this,
									this->shared_from_this(), _text, theme::TypeRole::BodySmall,
									foregroundColor);
		}

		std::vector<std::shared_ptr<ecs::Entity>> children;

		if (_variant == Variant::Rich && _titleEntity != nullptr) {
			children.push_back(_titleEntity);
		}

		if (_textEntity != nullptr) {
			children.push_back(_textEntity);
		}

		setChildren(children);
	}

	Tooltip &Tooltip::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		return *this;
	}

	Tooltip::Variant Tooltip::getVariant(void) const
	{
		return _variant;
	}

	Tooltip &Tooltip::setText(const std::string &text)
	{
		_text = text;
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setLabel(text);

		if (_textEntity != nullptr) {
			_textEntity->setContent(text);
		}

		return *this;
	}

	void Tooltip::initialize(void)
	{
		SurfaceBase::initialize();
		applyVariant();
		applyOverlay();
		buildContent();
		applyVisibility();
	}

	void Tooltip::update(void)
	{
		SurfaceBase::update();
		applyVariant();
		applyOverlay();
		buildContent();
		applyVisibility();
	}

}	 // namespace guillaume::entities
