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
		: SurfaceBase<components::Overlay>(registry, config)
		, _variant(variant)
		, _text(text)
		, _title(title)
		, _titleEntity()
		, _textEntity()
	{
		applyVariant();
		applyOverlay();
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
		config.borderRadius	  = (_variant == Variant::Rich) ? 12.0f : 4.0f;
		config.hasFixedHeight = true;
		config.fixedHeight	  = getVariantHeight();

		setSurfaceConfig(config);
		setColor(schemeColor(SchemeColorRole::InverseSurface));
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
		const auto foregroundColor = schemeColor(SchemeColorRole::OnSurface);

		if (_variant == Variant::Rich && _titleEntity == nullptr
			&& !_title.empty()) {
			_titleEntity = buildText(this->getComponentRegistry(), *this,
									 this->shared_from_this(), _title, 16.0f,
									 foregroundColor);
		}

		if (_textEntity == nullptr && !_text.empty()) {
			_textEntity = buildText(this->getComponentRegistry(), *this,
									this->shared_from_this(), _text, 14.0f,
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
	}

	void Tooltip::update(void)
	{
		SurfaceBase::update();
		applyVariant();
		applyOverlay();
		buildContent();
	}

}	 // namespace guillaume::entities
