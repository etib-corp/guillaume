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

#include "guillaume/entities/radio_button.hpp"

#include <memory>
#include <vector>

#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/ellipse.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/placement_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/components/hand_hover_interaction.hpp"
#include "guillaume/components/mouse_hover_interaction.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/radio_button.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		/**
		 * @brief Inner filled dot of a selected radio button.
		 */
		class RadioDot:
			public ecs::EntityFiller<components::Transform, components::Bound,
									 components::Color, components::Ellipse>
		{
			public:
			/**
			 * @brief Construct a radio dot.
			 * @param registry The component registry.
			 * @param color The dot color.
			 */
			RadioDot(ecs::ComponentRegistry &registry,
					 const utility::graphic::Color32Bit &color)
				: ecs::EntityFiller<components::Transform, components::Bound,
									components::Color, components::Ellipse>(
					  registry)
			{
				getComponentRegistry()
					.getComponent<components::Bound>(getIdentifier())
					.setWidth(RadioButton::DotSize)
					.setHeight(RadioButton::DotSize);

				getComponentRegistry()
					.getComponent<components::Color>(getIdentifier())
					.setColor(color);
			}

			/**
			 * @brief Default destructor.
			 */
			~RadioDot(void) override = default;
		};
	}	 // namespace

	RadioButton::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								  ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<RadioButton>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<RadioButton> RadioButton::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<RadioButton>(
			this->getComponentRegistry(), _config, _variant, _label);
		return entity;
	}

	void RadioButton::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Enabled;
		_label.clear();

		_config.borderRadius = getShapeRadius(theme::ShapeRole::Full);
		_config.axis		 = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.spacing = 8.0f;
		_config.padding = 0.0f;
		_config.setFixedWidth(20.0f);
		_config.setFixedHeight(20.0f);
	}

	RadioButton::Builder &
		RadioButton::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	RadioButton::Builder &RadioButton::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	RadioButton::Builder &
		RadioButton::Builder::withLabel(const std::string &label)
	{
		_label = label;
		return *this;
	}

	std::shared_ptr<RadioButton> RadioButton::Builder::makeRadioButton(
		std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::string &label)
	{
		return withVariant(variant).withLabel(label).registerEntity(parent);
	}

	RadioButton::RadioButton(ecs::ComponentRegistry &registry,
							 const SurfaceConfig &config, Variant variant,
							 const std::string &label)
		: SurfaceBase<components::Selectable, components::Ring,
					  components::RadioButton>(registry, config)
	{
		getComponentRegistry()
			.getComponent<components::RadioButton>(getIdentifier())
			.setVariant(variant)
			.setLabel(label);
		systems::RadioButton::install(getComponentRegistry(), getIdentifier());
		systems::RadioButton::apply(getComponentRegistry(), getIdentifier());
	}

	bool RadioButton::isDisabled(void) const
	{
		return systems::RadioButton::isDisabled(getComponentRegistry(),
												getIdentifier());
	}

	RadioButton &RadioButton::setOnPress(std::function<void(void)> onPress)
	{
		getComponentRegistry()
			.getComponent<components::RadioButton>(getIdentifier())
			.setOnPress(std::move(onPress));
		return *this;
	}

	RadioButton &RadioButton::setVariant(Variant variant)
	{
		getComponentRegistry()
			.getComponent<components::RadioButton>(getIdentifier())
			.setVariant(variant);
		systems::RadioButton::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	RadioButton::Variant RadioButton::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::RadioButton>(getIdentifier())
			.getVariant();
	}

	RadioButton &RadioButton::setLabel(const std::string &label)
	{
		auto &registry = getComponentRegistry();
		auto &radio =
			registry.getComponent<components::RadioButton>(getIdentifier());

		radio.setLabel(label);
		if (registry.hasComponent<components::Text>(radio.getLabelEntity())) {
			registry.getComponent<components::Text>(radio.getLabelEntity())
				.setContent(label);
		}
		systems::RadioButton::apply(registry, getIdentifier());
		return *this;
	}

	bool RadioButton::isSelected(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.isSelected();
	}

	RadioButton &RadioButton::select(bool selected)
	{
		systems::RadioButton::select(getComponentRegistry(), getIdentifier(),
									 selected);
		return *this;
	}

	void RadioButton::initialize(void)
	{
		auto &registry = getComponentRegistry();
		auto &radio =
			registry.getComponent<components::RadioButton>(getIdentifier());

		const auto dot = std::make_shared<RadioDot>(
			registry, schemeColor(SchemeColorRole::Primary));
		dot->setParent(shared_from_this());
		this->addEntity(dot);
		radio.setDot(dot->getIdentifier());

		if (!radio.getLabel().empty()) {
			const auto label =
				buildText(registry, *this, shared_from_this(), radio.getLabel(),
						  theme::TypeRole::BodyLarge,
						  schemeColor(SchemeColorRole::OnSurface));
			if (label != nullptr) {
				radio.setLabelEntity(label->getIdentifier());
			}
		}

		// The dot is centered and the label placed beside the ring by
		// systems::RadioButton; neither is a layout child of the surface.
		setChildren({});
		systems::RadioButton::apply(registry, getIdentifier());
	}

}	 // namespace guillaume::entities
