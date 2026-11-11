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
#include "guillaume/systems/radio_button.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/hand_hover_interaction.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/mouse_hover_interaction.hpp"
#include "guillaume/components/ring.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/systems/interaction_helpers.hpp"
#include "guillaume/systems/placement_helpers.hpp"
#include "guillaume/systems/style_helpers.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Variant = components::RadioButton::Variant;
		using style::disabledContentColor;
		using style::schemeColor;
		using style::transparentColor;
	}	 // namespace

	RadioButton::RadioButton(void)
		: ecs::SystemFiller<components::RadioButton>(ecs::Phase::Layout)
	{
	}

	void RadioButton::install(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		interaction::makeInteractive(
			componentRegistry, identifier, components::AccessibilityRole::Radio,
			[registry, identifier]() {
				press(*registry, identifier);
			},
			[registry, identifier]() {
				apply(*registry, identifier);
			});

		// Grow the 20 dp ring to the 40 dp target without growing it.
		const float width =
			componentRegistry.getComponent<components::Layout>(identifier)
				.getFixedWidth();
		const auto margin = static_cast<unsigned int>(
			(components::RadioButton::TouchTarget - width) / 2.0f);
		componentRegistry
			.getComponent<components::MouseHoverInteraction>(identifier)
			.setAccessibilityMargin(utility::graphic::SizeUI(margin, margin));
		componentRegistry
			.getComponent<components::HandHoverInteraction>(identifier)
			.setAccessibilityMargin(utility::graphic::SizeUI(margin, margin));

		componentRegistry.getComponent<components::Selectable>(identifier)
			.setOnSelectionChangedHandler([registry, identifier](bool) {
				apply(*registry, identifier);
			});
	}

	bool
		RadioButton::isDisabled(const ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier)
	{
		return componentRegistry
				   .getComponent<components::RadioButton>(entityIdentifier)
				   .getVariant()
			== Variant::Disabled
			|| !interaction::isEnabled(componentRegistry, entityIdentifier);
	}

	void RadioButton::select(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier,
							 bool selected)
	{
		componentRegistry.getComponent<components::Selectable>(entityIdentifier)
			.setSelected(selected);
		apply(componentRegistry, entityIdentifier);
	}

	void RadioButton::press(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier)
	{
		if (isDisabled(componentRegistry, entityIdentifier)) {
			return;
		}

		select(componentRegistry, entityIdentifier, true);
		// Copy: the callback may replace itself.
		const auto onPress =
			componentRegistry
				.getComponent<components::RadioButton>(entityIdentifier)
				.getOnPress();
		if (onPress) {
			onPress();
		}
	}

	void RadioButton::apply(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &radio =
			componentRegistry.getComponent<components::RadioButton>(
				entityIdentifier);
		const Variant variant = radio.getVariant();

		componentRegistry.getComponent<components::Ring>(entityIdentifier)
			.setThickness(2.0f);
		interaction::setEnabled(componentRegistry, entityIdentifier,
								variant != Variant::Disabled);

		const bool disabled = isDisabled(componentRegistry, entityIdentifier);
		const bool errored	= (variant == Variant::Error);
		const bool selected =
			componentRegistry
				.getComponent<components::Selectable>(entityIdentifier)
				.isSelected();

		auto &color =
			componentRegistry.getComponent<components::Color>(entityIdentifier);
		if (disabled) {
			color.setColor(disabledContentColor());
		} else if (errored) {
			color.setColor(schemeColor(SchemeColorRole::Error));
		} else if (selected) {
			color.setColor(schemeColor(SchemeColorRole::Primary));
		} else {
			color.setColor(schemeColor(SchemeColorRole::OnSurfaceVariant));
		}

		componentRegistry
			.getComponent<components::Accessibility>(entityIdentifier)
			.setLabel(radio.getLabel())
			.setSelected(selected)
			.setChecked(selected ? components::CheckedState::Checked
								 : components::CheckedState::Unchecked);

		const auto layer = Layout::layerOf(componentRegistry, entityIdentifier);
		const auto dot	 = radio.getDot();
		if (componentRegistry.hasComponent<components::Color>(dot)) {
			const auto dotColor = disabled
				? disabledContentColor()
				: schemeColor(errored ? SchemeColorRole::Error
									  : SchemeColorRole::Primary);
			componentRegistry.getComponent<components::Color>(dot).setColor(
				selected ? dotColor : transparentColor());
			if (selected) {
				placement::centerChild(componentRegistry, entityIdentifier, dot,
									   layer);
			}
		}

		const auto label = radio.getLabelEntity();
		if (componentRegistry.hasComponent<components::Color>(label)) {
			componentRegistry.getComponent<components::Color>(label).setColor(
				disabled ? disabledContentColor()
						 : schemeColor(errored ? SchemeColorRole::Error
											   : SchemeColorRole::OnSurface));
		}
		if (componentRegistry.hasComponent<components::Bound>(label)) {
			const auto &ring =
				componentRegistry.getComponent<components::Bound>(
					entityIdentifier);
			const auto &labelBound =
				componentRegistry.getComponent<components::Bound>(label);
			const float x =
				(ring.getWidth() + components::RadioButton::TouchTarget) / 2.0f
				+ components::RadioButton::LabelGap;
			const float y = (ring.getHeight() - labelBound.getHeight()) / 2.0f;
			placement::placeChild(componentRegistry, entityIdentifier, label,
								  utility::graphic::PositionF(x, y, 0.0f),
								  layer);
		}
	}

	void RadioButton::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::RadioButton>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
