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

#pragma once

#include <functional>

#include <utility/graphic/color.hpp>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::systems::interaction
{

	/**
	 * @file
	 * @brief Helpers of the namespace systems::interaction: what "pressable"
	 * means for a widget, and how its input state becomes a color.
	 *
	 * Contents:
	 * - makeInteractive(): gives an entity the components that make it
	 *   hoverable, pressable and focusable, and wires its handlers.
	 * - setEnabled(), isEnabled(), isHovered(), isPressed(): the input state
	 *   of an entity, read from its components.
	 * - stateLayerColor(): the container color of a widget with the tint
	 *   of its current state (hovered, focused, pressed, dragged).
	 *
	 * Used by widget systems such as systems::Checkbox, RadioButton, Switch,
	 * Chip, Card, List and Menu: typically their `install` calls
	 * makeInteractive() once, and their `apply` calls the others each time
	 * the widget is styled.
	 */

	/**
	 * @brief Make an entity hoverable, pressable and focusable.
	 *
	 * Adds, when missing, components::StyleState, Focus, Accessibility
	 * (its role is set to @p role), MouseHoverInteraction,
	 * HandHoverInteraction, MouseButtonInteraction and
	 * HandButtonInteraction, then sets these handlers, replacing any
	 * already set for the same event:
	 * - hover and unhover (mouse and hand) call @p onStateChange;
	 * - pressing the left mouse button or hand button A calls
	 *   @p onStateChange;
	 * - releasing either calls @p onPress then @p onStateChange, unless the
	 *   entity is disabled, in which case neither is called.
	 *
	 * The input systems then keep those components up to date each frame.
	 *
	 * @code
	 * ecs::ComponentRegistry *registry = &componentRegistry;
	 * interaction::makeInteractive(
	 * 	componentRegistry, identifier,
	 * 	components::AccessibilityRole::Checkbox,
	 * 	[registry, identifier]() { toggle(*registry, identifier); },
	 * 	[registry, identifier]() { apply(*registry, identifier); });
	 * @endcode
	 *
	 * @note The handlers are stored in the components and run in later
	 * frames: capture the registry and the identifier, not references to
	 * components or locals. The registry must outlive the entity.
	 *
	 * @param registry The registry holding the components.
	 * @param entityIdentifier The entity to make interactive.
	 * @param role What the entity is for assistive technology (button,
	 * checkbox, ...).
	 * @param onPress Called when the entity is activated. May be empty.
	 * @param onStateChange Called when the hover or press state changes, so
	 * the widget can restyle at once. May be empty.
	 */
	void makeInteractive(ecs::ComponentRegistry &registry,
						 const ecs::Entity::Identifier &entityIdentifier,
						 components::AccessibilityRole role,
						 std::function<void(void)> onPress,
						 std::function<void(void)> onStateChange = {});

	/**
	 * @brief Enable or disable an entity.
	 *
	 * Writes the enabled flag of its components::StyleState, adding the
	 * component when missing. Disabling also clears the hovered, pressed,
	 * focused and dragged flags, and the input systems skip the entity
	 * until it is enabled again. The component is marked changed only when
	 * a flag really changes.
	 *
	 * @param registry The registry holding the components.
	 * @param entityIdentifier The entity to enable or disable.
	 * @param enabled True to accept input, false to ignore it.
	 */
	void setEnabled(ecs::ComponentRegistry &registry,
					const ecs::Entity::Identifier &entityIdentifier,
					bool enabled);

	/**
	 * @brief Whether an entity accepts input.
	 * @param registry The registry holding the components.
	 * @param entityIdentifier The entity to test.
	 * @return True when the entity has no components::StyleState or an
	 * enabled one.
	 */
	bool isEnabled(const ecs::ComponentRegistry &registry,
				   const ecs::Entity::Identifier &entityIdentifier);

	/**
	 * @brief Whether the mouse pointer or a hand is over an entity.
	 *
	 * Reads components::MouseHoverInteraction and HandHoverInteraction
	 * directly, so the answer is that of the current frame.
	 * @param registry The registry holding the components.
	 * @param entityIdentifier The entity to test.
	 * @return True when the entity is enabled and one of its hover
	 * components is hovered. False when it has neither.
	 */
	bool isHovered(const ecs::ComponentRegistry &registry,
				   const ecs::Entity::Identifier &entityIdentifier);

	/**
	 * @brief Whether an entity is being pressed.
	 *
	 * Reads components::MouseButtonInteraction (left button) and
	 * HandButtonInteraction (button A) directly. Pinch and poke are not
	 * taken into account here; components::StyleState covers them.
	 * @param registry The registry holding the components.
	 * @param entityIdentifier The entity to test.
	 * @return True when the entity is enabled and one of those buttons is
	 * held on it. False when it has neither component.
	 */
	bool isPressed(const ecs::ComponentRegistry &registry,
				   const ecs::Entity::Identifier &entityIdentifier);

	/**
	 * @brief Container color of a widget in its current input state.
	 *
	 * Material 3 shows hover, focus, press and drag by blending a
	 * translucent layer of the content color over the container (the
	 * "state layer"). This picks the layer of the entity and blends it at
	 * the opacity the active theme gives that state.
	 *
	 * The state is chosen in this order: pressed (isPressed()), then the
	 * active layer of components::StyleState (dragged, pressed, focused,
	 * hovered), then hovered (isHovered()). The interaction components are
	 * consulted as well as StyleState because a handler can restyle a
	 * widget before systems::StyleState has run in the frame.
	 *
	 * @code
	 * color.setColor(interaction::stateLayerColor(registry, identifier,
	 * 	style::schemeColor(SchemeColorRole::Primary),
	 * 	style::schemeColor(SchemeColorRole::OnPrimary)));
	 * @endcode
	 *
	 * @param registry The registry holding the components.
	 * @param entityIdentifier The entity whose state is read.
	 * @param base The container color of the idle widget.
	 * @param content The color of what sits on the container (label,
	 * icon); it tints the layer.
	 * @return @p base when the entity is disabled or idle. Otherwise
	 * @p base with the layer blended over it, or, when @p base is fully
	 * transparent, @p content at the layer opacity.
	 */
	utility::graphic::Color32Bit
		stateLayerColor(const ecs::ComponentRegistry &registry,
						const ecs::Entity::Identifier &entityIdentifier,
						const utility::graphic::Color32Bit &base,
						const utility::graphic::Color32Bit &content);

}	 // namespace guillaume::systems::interaction
