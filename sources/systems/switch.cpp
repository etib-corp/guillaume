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
#include "guillaume/systems/switch.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/value.hpp"
#include "guillaume/systems/interaction_helpers.hpp"
#include "guillaume/systems/placement_helpers.hpp"
#include "guillaume/systems/style_helpers.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Variant = components::Switch::Variant;
		using style::disabledContainerColor;
		using style::disabledContentColor;
		using style::schemeColor;
		using style::transparentColor;

		/**
		 * @brief Set the color of a child, when the child exists.
		 * @param registry The registry holding the components.
		 * @param child The child identifier (may be invalid).
		 * @param color The color to write.
		 */
		void colorChild(ecs::ComponentRegistry &registry,
						ecs::Entity::Identifier child,
						const utility::graphic::Color32Bit &color)
		{
			if (registry.hasComponent<components::Color>(child)) {
				registry.getComponent<components::Color>(child).setColor(color);
			}
		}
	}	 // namespace

	Switch::Switch(void)
		: ecs::SystemFiller<components::Switch>(ecs::Phase::Layout)
	{
	}

	void Switch::install(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		interaction::makeInteractive(
			componentRegistry, identifier,
			components::AccessibilityRole::Switch,
			[registry, identifier]() {
				toggle(*registry, identifier);
			},
			[registry, identifier]() {
				apply(*registry, identifier);
			});

		auto &value =
			componentRegistry.getComponent<components::Value>(identifier);
		value.setMin(0.0f).setMax(1.0f).setStep(1.0f);
		value.setOnChangedHandler([registry, identifier](float) {
			apply(*registry, identifier);
		});
	}

	bool Switch::isDisabled(const ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier)
	{
		return componentRegistry
				   .getComponent<components::Switch>(entityIdentifier)
				   .getVariant()
			== Variant::Disabled
			|| !interaction::isEnabled(componentRegistry, entityIdentifier);
	}

	bool Switch::isChecked(const ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier)
	{
		return componentRegistry
				   .getComponent<components::Value>(entityIdentifier)
				   .getValue()
			>= 0.5f;
	}

	void Switch::setChecked(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							bool checked)
	{
		componentRegistry.getComponent<components::Value>(entityIdentifier)
			.setValue(checked ? 1.0f : 0.0f);
	}

	void Switch::toggle(ecs::ComponentRegistry &componentRegistry,
						const ecs::Entity::Identifier &entityIdentifier)
	{
		if (isDisabled(componentRegistry, entityIdentifier)) {
			return;
		}

		setChecked(componentRegistry, entityIdentifier,
				   !isChecked(componentRegistry, entityIdentifier));
		// Copy: the callback may replace itself.
		const auto onValueChange =
			componentRegistry.getComponent<components::Switch>(entityIdentifier)
				.getOnValueChange();
		if (onValueChange) {
			onValueChange(isChecked(componentRegistry, entityIdentifier));
		}
	}

	void Switch::apply(ecs::ComponentRegistry &componentRegistry,
					   const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &config = componentRegistry.getComponent<components::Switch>(
			entityIdentifier);
		const bool on	 = isChecked(componentRegistry, entityIdentifier);
		const auto layer = Layout::layerOf(componentRegistry, entityIdentifier);
		const auto thumb = config.getThumb();
		const auto label = config.getLabelEntity();

		// Placement: the thumb slides along the track, the icon stays
		// centered on the thumb, the label sits beside the track.
		if (componentRegistry.hasComponent<components::Color>(thumb)) {
			placement::placeNormalized(componentRegistry, entityIdentifier,
									   thumb, on ? 1.0f : 0.0f,
									   components::Switch::ThumbInset, layer);
			colorChild(componentRegistry, thumb,
					   config.getVariant() == Variant::Disabled
						   ? disabledContentColor()
						   : schemeColor(on ? SchemeColorRole::OnPrimary
											: SchemeColorRole::Outline));
			placement::centerChild(componentRegistry, thumb, config.getIcon(),
								   Layout::layerOf(componentRegistry, thumb));
		}
		if (componentRegistry.hasComponent<components::Bound>(label)) {
			const auto &track =
				componentRegistry.getComponent<components::Bound>(
					entityIdentifier);
			const auto &labelBound =
				componentRegistry.getComponent<components::Bound>(label);
			placement::placeChild(
				componentRegistry, entityIdentifier, label,
				utility::graphic::PositionF(
					track.getWidth() + components::Switch::LabelGap,
					(track.getHeight() - labelBound.getHeight()) / 2.0f, 0.0f),
				layer);
			placement::reserveTrailingWidth(componentRegistry, entityIdentifier,
											components::Switch::LabelGap
												+ labelBound.getWidth());
		} else {
			placement::reserveTrailingWidth(componentRegistry, entityIdentifier,
											0.0f);
		}

		// Style.
		interaction::setEnabled(componentRegistry, entityIdentifier,
								config.getVariant() != Variant::Disabled);
		componentRegistry
			.getComponent<components::Accessibility>(entityIdentifier)
			.setChecked(on ? components::CheckedState::Checked
						   : components::CheckedState::Unchecked)
			.setLabel(config.getLabel());

		const bool disabled = isDisabled(componentRegistry, entityIdentifier);
		auto &color =
			componentRegistry.getComponent<components::Color>(entityIdentifier);
		if (disabled) {
			color.setColor(disabledContainerColor());
		} else {
			color.setColor(schemeColor(on ? SchemeColorRole::Primary
										  : SchemeColorRole::SurfaceVariant));
		}
		componentRegistry.getComponent<components::Borders>(entityIdentifier)
			.setColor(on ? transparentColor()
						 : schemeColor(SchemeColorRole::Outline));

		colorChild(componentRegistry, config.getIcon(),
				   disabled
					   ? disabledContentColor()
					   : schemeColor(on ? SchemeColorRole::OnPrimary
										: SchemeColorRole::OnSurfaceVariant));
		colorChild(componentRegistry, label,
				   disabled ? disabledContentColor()
							: schemeColor(SchemeColorRole::OnSurface));
	}

	void Switch::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Switch>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
