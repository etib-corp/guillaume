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

#include "guillaume/entities/switch.hpp"

#include <functional>
#include <memory>
#include <vector>

#include <utility/event/hand_button_event.hpp>
#include <utility/event/mouse_button_event.hpp>

#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/ellipse.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/placement_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		constexpr float TrackWidth	= 52.0f;
		constexpr float TrackHeight = 32.0f;
		constexpr float ThumbSize	= 24.0f;
		constexpr float ThumbInset	= 4.0f;

		/**
		 * @brief Thumb of a switch, a filled ellipse.
		 */
		class SwitchThumb:
			public ecs::EntityFiller<components::Transform, components::Bound,
									 components::Color, components::Ellipse>
		{
			public:
			/**
			 * @brief Construct a switch thumb.
			 * @param registry The component registry.
			 * @param color The thumb color.
			 */
			SwitchThumb(ecs::ComponentRegistry &registry,
						const utility::graphic::Color32Bit &color)
				: ecs::EntityFiller<components::Transform, components::Bound,
									components::Color, components::Ellipse>(
					  registry)
			{
				getComponentRegistry()
					.getComponent<components::Bound>(getIdentifier())
					.setWidth(ThumbSize)
					.setHeight(ThumbSize);

				getComponentRegistry()
					.getComponent<components::Color>(getIdentifier())
					.setColor(color);
			}

			/**
			 * @brief Default destructor.
			 */
			~SwitchThumb(void) override = default;
		};
	}	 // namespace

	Switch::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							 ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Switch>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Switch> Switch::Builder::buildEntity(void)
	{
		auto entity =
			std::make_shared<Switch>(this->getComponentRegistry(), _config,
									 _variant, _checked, _iconGlyph, _label);
		return entity;
	}

	void Switch::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Enabled;
		_checked = false;
		_iconGlyph.clear();
		_label.clear();

		_config.borderRadius = TrackHeight * 0.5f;
		_config.axis		 = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.spacing = 8.0f;
		_config.padding = 0.0f;
		_config.setFixedWidth(TrackWidth);
		_config.setFixedHeight(TrackHeight);
	}

	Switch::Builder &
		Switch::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Switch::Builder &Switch::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Switch::Builder &Switch::Builder::withChecked(bool checked)
	{
		_checked = checked;
		return *this;
	}

	Switch::Builder &Switch::Builder::withIcon(const std::string &iconGlyph)
	{
		_iconGlyph = iconGlyph;
		return *this;
	}

	Switch::Builder &Switch::Builder::withLabel(const std::string &label)
	{
		_label = label;
		return *this;
	}

	std::shared_ptr<Switch> Switch::Director::makeSwitch(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		bool checked, const std::string &iconGlyph, const std::string &label)
	{
		return builder.withVariant(variant)
			.withChecked(checked)
			.withIcon(iconGlyph)
			.withLabel(label)
			.registerEntity(parent);
	}

	Switch::Switch(ecs::ComponentRegistry &registry,
				   const SurfaceConfig &config, Variant variant, bool checked,
				   const std::string &iconGlyph, const std::string &label)
		: SurfaceBase<components::Value, components::MouseButtonInteraction,
					  components::HandButtonInteraction>(registry, config)
		, _variant(variant)
		, _iconGlyph(iconGlyph)
		, _label(label)
		, _thumb()
		, _icon()
		, _labelEntity()
	{
		auto &value = getComponentRegistry().getComponent<components::Value>(
			getIdentifier());
		value.setMin(0.0f).setMax(1.0f).setStep(1.0f);
		value.setOnChangedHandler([this](float) {
			applyState();
		});
		value.setValue(checked ? 1.0f : 0.0f);

		applyVariant();
	}

	Switch::~Switch(void)
	{
	}

	void Switch::applyVariant(void)
	{
		const bool disabled = (_variant == Variant::Disabled);
		const bool on		= isChecked();

		if (disabled) {
			setColor(withAlpha(schemeColor(SchemeColorRole::OnSurface), 31U));
		} else if (on) {
			setColor(schemeColor(SchemeColorRole::Primary));
		} else {
			setColor(schemeColor(SchemeColorRole::SurfaceVariant));
		}

		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setColor(on ? transparentColor()
						 : schemeColor(SchemeColorRole::Outline));

		if (_icon != nullptr) {
			_icon->setColor(
				disabled
					? withAlpha(schemeColor(SchemeColorRole::OnSurface), 97U)
					: schemeColor(on ? SchemeColorRole::OnPrimary
									 : SchemeColorRole::OnSurfaceVariant));
		}

		if (_labelEntity != nullptr) {
			if (disabled) {
				_labelEntity->setColor(
					withAlpha(schemeColor(SchemeColorRole::OnSurface), 97U));
			} else {
				_labelEntity->setColor(schemeColor(SchemeColorRole::OnSurface));
			}
		}
	}

	void Switch::applyState(void)
	{
		const bool on = getComponentRegistry()
							.getComponent<components::Value>(getIdentifier())
							.getValue()
			>= 0.5f;
		const bool disabled = (_variant == Variant::Disabled);

		if (_thumb != nullptr) {
			placeNormalized(getComponentRegistry(), getIdentifier(),
							_thumb->getIdentifier(), on ? 1.0f : 0.0f,
							ThumbInset);

			const auto thumbColor = disabled
				? withAlpha(schemeColor(SchemeColorRole::OnSurface), 97U)
				: schemeColor(on ? SchemeColorRole::OnPrimary
								 : SchemeColorRole::Outline);

			getComponentRegistry()
				.getComponent<components::Color>(_thumb->getIdentifier())
				.setColor(thumbColor);

			if (_icon != nullptr) {
				centerChild(getComponentRegistry(), _thumb->getIdentifier(),
							_icon->getIdentifier());
			}
		}

		applyVariant();
	}

	Switch &Switch::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		return *this;
	}

	Switch::Variant Switch::getVariant(void) const
	{
		return _variant;
	}

	Switch &Switch::setChecked(bool checked)
	{
		getComponentRegistry()
			.getComponent<components::Value>(getIdentifier())
			.setValue(checked ? 1.0f : 0.0f);
		return *this;
	}

	bool Switch::isChecked(void) const
	{
		return getComponentRegistry()
				   .getComponent<components::Value>(getIdentifier())
				   .getValue()
			>= 0.5f;
	}

	Switch &Switch::toggle(void)
	{
		return setChecked(!isChecked());
	}

	Switch &Switch::setLabel(const std::string &label)
	{
		_label = label;

		if (_labelEntity != nullptr) {
			_labelEntity->setContent(label);
		}

		applyState();

		return *this;
	}

	void Switch::initialize(void)
	{
		SurfaceBase::initialize();

		_thumb = std::make_shared<SwitchThumb>(
			getComponentRegistry(), schemeColor(SchemeColorRole::Outline));
		_thumb->setParent(shared_from_this());
		this->addEntity(_thumb);

		if (!_iconGlyph.empty()) {
			_icon = buildIcon(getComponentRegistry(), *this, _thumb, _iconGlyph,
							  16.0f, schemeColor(SchemeColorRole::OnPrimary));
		}

		if (!_label.empty()) {
			_labelEntity = buildText(getComponentRegistry(), *this,
									 shared_from_this(), _label, 14.0f,
									 schemeColor(SchemeColorRole::OnSurface));
		}

		// The thumb is positioned manually; only the label participates in
		// the surface layout.
		std::vector<std::shared_ptr<ecs::Entity>> children;
		if (_labelEntity != nullptr) {
			children.push_back(_labelEntity);
		}
		setChildren(children);

		getComponentRegistry()
			.getComponent<components::MouseButtonInteraction>(getIdentifier())
			.setOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left,
				std::bind(&Switch::toggle, this));

		getComponentRegistry()
			.getComponent<components::HandButtonInteraction>(getIdentifier())
			.setOnButtonReleaseHandler(
				utility::event::HandButtonEvent::Button::A,
				std::bind(&Switch::toggle, this));

		applyState();
	}

	void Switch::update(void)
	{
		SurfaceBase::update();
		applyState();
	}

}	 // namespace guillaume::entities
