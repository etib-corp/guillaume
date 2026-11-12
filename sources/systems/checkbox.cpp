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
#include "guillaume/systems/checkbox.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/hand_hover_interaction.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/line.hpp"
#include "guillaume/components/mouse_hover_interaction.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/entities/placement_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using State	  = components::Checkbox::State;
		using Variant = components::Checkbox::Variant;
		using entities::disabledContentColor;
		using entities::schemeColor;
		using entities::transparentColor;

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

		/**
		 * @brief Place the label beside the touch target, centered on the box.
		 * @param registry The registry holding the components.
		 * @param identifier The checkbox entity.
		 * @param label The label child (may be invalid).
		 */
		void placeLabel(ecs::ComponentRegistry &registry,
						ecs::Entity::Identifier identifier,
						ecs::Entity::Identifier label)
		{
			if (!registry.hasComponent<components::Bound>(label)) {
				return;
			}

			const auto &box =
				registry.getComponent<components::Bound>(identifier);
			const auto &labelBound =
				registry.getComponent<components::Bound>(label);

			// The label starts after the 40 dp target, vertically centered
			// on the box, so it never overlaps the 18 dp surface.
			const float x =
				(box.getWidth() + components::Checkbox::TouchTarget) / 2.0f
				+ components::Checkbox::LabelGap;
			const float y = (box.getHeight() - labelBound.getHeight()) / 2.0f;
			entities::placeChild(registry, identifier, label,
								 utility::graphic::PositionF(x, y, 0.0f),
								 Layout::layerOf(registry, identifier));
		}
	}	 // namespace

	Checkbox::Checkbox(void)
		: ecs::SystemFiller<components::Checkbox>(ecs::Phase::Layout)
	{
	}

	void Checkbox::install(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		entities::makeInteractive(
			componentRegistry, identifier,
			components::AccessibilityRole::Checkbox,
			[registry, identifier]() {
				toggle(*registry, identifier);
			},
			[registry, identifier]() {
				apply(*registry, identifier);
			});

		// Grow the 18 dp box to the 40 dp target without growing it visually.
		const float width =
			componentRegistry.getComponent<components::Layout>(identifier)
				.getFixedWidth();
		const auto margin = static_cast<unsigned int>(
			(components::Checkbox::TouchTarget - width) / 2.0f);
		componentRegistry
			.getComponent<components::MouseHoverInteraction>(identifier)
			.setAccessibilityMargin(utility::graphic::SizeUI(margin, margin));
		componentRegistry
			.getComponent<components::HandHoverInteraction>(identifier)
			.setAccessibilityMargin(utility::graphic::SizeUI(margin, margin));

		componentRegistry.getComponent<components::Selectable>(identifier)
			.setOnSelectionChangedHandler([registry,
										   identifier](bool selected) {
				auto &checkbox =
					registry->getComponent<components::Checkbox>(identifier);
				if (checkbox.getState() != State::Indeterminate) {
					checkbox.setState(selected ? State::Checked
											   : State::Unchecked);
				}
				apply(*registry, identifier);
			});
	}

	bool Checkbox::isDisabled(const ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier)
	{
		return componentRegistry
				   .getComponent<components::Checkbox>(entityIdentifier)
				   .getVariant()
			== Variant::Disabled
			|| !entities::isEnabled(componentRegistry, entityIdentifier);
	}

	void Checkbox::setState(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							components::Checkbox::State state)
	{
		componentRegistry.getComponent<components::Checkbox>(entityIdentifier)
			.setState(state);
		componentRegistry.getComponent<components::Selectable>(entityIdentifier)
			.setSelected(state != State::Unchecked);
		apply(componentRegistry, entityIdentifier);
	}

	void Checkbox::toggle(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier)
	{
		if (isDisabled(componentRegistry, entityIdentifier)) {
			return;
		}

		const auto &checkbox =
			componentRegistry.getComponent<components::Checkbox>(
				entityIdentifier);
		setState(componentRegistry, entityIdentifier,
				 checkbox.getState() == State::Checked ? State::Unchecked
													   : State::Checked);
		// Copy: the callback may replace itself.
		const auto onValueChange = checkbox.getOnValueChange();
		if (onValueChange) {
			onValueChange(checkbox.getState());
		}
	}

	void Checkbox::apply(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &checkbox =
			componentRegistry.getComponent<components::Checkbox>(
				entityIdentifier);
		const State state	  = checkbox.getState();
		const Variant variant = checkbox.getVariant();
		auto &color =
			componentRegistry.getComponent<components::Color>(entityIdentifier);
		auto &borders = componentRegistry.getComponent<components::Borders>(
			entityIdentifier);
		auto &line =
			componentRegistry.getComponent<components::Line>(entityIdentifier);

		entities::setEnabled(componentRegistry, entityIdentifier,
							 variant != Variant::Disabled);

		const bool disabled = isDisabled(componentRegistry, entityIdentifier);
		const bool errored	= (variant == Variant::Error);

		componentRegistry
			.getComponent<components::Accessibility>(entityIdentifier)
			.setLabel(checkbox.getLabel())
			.setChecked(state == State::Checked
							? components::CheckedState::Checked
							: (state == State::Indeterminate
								   ? components::CheckedState::Mixed
								   : components::CheckedState::Unchecked));

		line.setThickness(state == State::Indeterminate ? 2.0f : 0.0f);

		const auto onColorRole =
			errored ? SchemeColorRole::OnError : SchemeColorRole::OnPrimary;
		// The indeterminate dash shares the box entity, so it must carry its
		// own color instead of the container fill.
		line.setColor(schemeColor(onColorRole));
		const auto containerRole =
			errored ? SchemeColorRole::Error : SchemeColorRole::Primary;

		switch (state) {
			case State::Checked:
			case State::Indeterminate: {
				if (disabled) {
					color.setColor(disabledContentColor());
				} else {
					color.setColor(entities::stateLayerColor(
						componentRegistry, entityIdentifier,
						errored ? schemeColor(containerRole)
								: checkbox.getCheckedColor().value_or(
									  schemeColor(containerRole)),
						schemeColor(onColorRole)));
				}
				borders.setColor(transparentColor()).setWidth(0.0f);
				break;
			}
			case State::Unchecked:
			default: {
				const auto outline = errored
					? schemeColor(SchemeColorRole::Error)
					: checkbox.getUncheckedColor().value_or(
						  schemeColor(SchemeColorRole::OnSurfaceVariant));
				color.setColor(disabled
								   ? transparentColor()
								   : entities::stateLayerColor(
										 componentRegistry, entityIdentifier,
										 transparentColor(), outline));
				borders.setWidth(2.0f);
				borders.setColor(disabled ? disabledContentColor() : outline);
				break;
			}
		}

		colorChild(componentRegistry, checkbox.getCheckIcon(),
				   state == State::Checked ? schemeColor(onColorRole)
										   : transparentColor());
		colorChild(componentRegistry, checkbox.getLabelEntity(),
				   disabled
					   ? disabledContentColor()
					   : schemeColor(errored ? SchemeColorRole::Error
											 : SchemeColorRole::OnSurface));

		placeLabel(componentRegistry, entityIdentifier,
				   checkbox.getLabelEntity());
	}

	void Checkbox::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Checkbox>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
