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
#include "guillaume/entities/interaction_helpers.hpp"

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
#include "guillaume/systems/switch.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		constexpr float TrackWidth	= 52.0f;
		constexpr float TrackHeight = 32.0f;
		constexpr float ThumbSize	= components::Switch::ThumbSize;

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
		auto entity = Switch::create(this->getComponentRegistry(), _config,
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

	std::shared_ptr<Switch> Switch::Builder::makeSwitch(
		std::shared_ptr<ecs::Entity> parent, Variant variant, bool checked,
		const std::string &iconGlyph, const std::string &label)
	{
		return withVariant(variant)
			.withChecked(checked)
			.withIcon(iconGlyph)
			.withLabel(label)
			.registerEntity(parent);
	}

	Switch::Switch(ecs::ComponentRegistry &registry,
				   const SurfaceConfig &config, Variant variant, bool checked,
				   const std::string &iconGlyph, const std::string &label)
		: SurfaceBase<components::Value, components::MouseButtonInteraction,
					  components::HandButtonInteraction, components::Switch>(
			  registry, config)
	{
		getComponentRegistry()
			.getComponent<components::Switch>(getIdentifier())
			.setVariant(variant)
			.setIconGlyph(iconGlyph)
			.setLabel(label);
		systems::Switch::install(getComponentRegistry(), getIdentifier());
		systems::Switch::setChecked(getComponentRegistry(), getIdentifier(),
									checked);
		systems::Switch::apply(getComponentRegistry(), getIdentifier());
	}

	Switch &Switch::setVariant(Variant variant)
	{
		getComponentRegistry()
			.getComponent<components::Switch>(getIdentifier())
			.setVariant(variant);
		systems::Switch::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Switch::Variant Switch::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Switch>(getIdentifier())
			.getVariant();
	}

	Switch &Switch::setChecked(bool checked)
	{
		systems::Switch::setChecked(getComponentRegistry(), getIdentifier(),
									checked);
		return *this;
	}

	bool Switch::isChecked(void) const
	{
		return systems::Switch::isChecked(getComponentRegistry(),
										  getIdentifier());
	}

	Switch &Switch::toggle(void)
	{
		systems::Switch::toggle(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Switch &Switch::setOnValueChange(std::function<void(bool)> onValueChange)
	{
		getComponentRegistry()
			.getComponent<components::Switch>(getIdentifier())
			.setOnValueChange(std::move(onValueChange));
		return *this;
	}

	bool Switch::isDisabled(void) const
	{
		return systems::Switch::isDisabled(getComponentRegistry(),
										   getIdentifier());
	}

	Switch &Switch::setLabel(const std::string &label)
	{
		auto &registry = getComponentRegistry();
		auto &config =
			registry.getComponent<components::Switch>(getIdentifier());

		config.setLabel(label);
		if (registry.hasComponent<components::Text>(config.getLabelEntity())) {
			registry.getComponent<components::Text>(config.getLabelEntity())
				.setContent(label);
		}
		systems::Switch::apply(registry, getIdentifier());
		return *this;
	}

	void Switch::build(void)
	{
		auto &registry = getComponentRegistry();
		auto &config =
			registry.getComponent<components::Switch>(getIdentifier());

		const auto thumb = std::make_shared<SwitchThumb>(
			registry, schemeColor(SchemeColorRole::Outline));
		thumb->setParent(shared_from_this());
		this->addEntity(thumb);
		config.setThumb(thumb->getIdentifier());

		if (!config.getIconGlyph().empty()) {
			const auto icon =
				buildIcon(registry, *this, thumb, config.getIconGlyph(), 16.0f,
						  schemeColor(SchemeColorRole::OnPrimary));
			if (icon != nullptr) {
				config.setIcon(icon->getIdentifier());
			}
		}

		if (!config.getLabel().empty()) {
			const auto label =
				buildText(registry, *this, shared_from_this(),
						  config.getLabel(), theme::TypeRole::BodyLarge,
						  schemeColor(SchemeColorRole::OnSurface));
			if (label != nullptr) {
				config.setLabelEntity(label->getIdentifier());
			}
		}

		// The thumb and the label are positioned by systems::Switch; neither
		// is a layout child of the track.
		setChildren({});
		systems::Switch::apply(registry, getIdentifier());
	}

}	 // namespace guillaume::entities
