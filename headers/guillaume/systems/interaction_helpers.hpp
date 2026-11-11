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
	 * @brief Make an entity pressable through the shared interaction model.
	 *
	 * Adds (when missing) mouse/hand hover and button interactions, a
	 * `components::Focus`, a `components::StyleState` and a
	 * `components::Accessibility` with @p role. Releasing the primary mouse
	 * button or hand button A calls @p onPress, unless the entity is disabled.
	 * Hover and press changes call @p onStateChange so the entity can restyle.
	 *
	 * @param registry Component registry.
	 * @param entityIdentifier Entity identifier.
	 * @param role Semantic role.
	 * @param onPress Activation handler (may be empty).
	 * @param onStateChange Restyle handler (may be empty).
	 */
	void makeInteractive(ecs::ComponentRegistry &registry,
						 const ecs::Entity::Identifier &entityIdentifier,
						 components::AccessibilityRole role,
						 std::function<void(void)> onPress,
						 std::function<void(void)> onStateChange = {});

	/**
	 * @brief Enable or disable an entity through its StyleState.
	 * @param registry Component registry.
	 * @param entityIdentifier Entity identifier (a StyleState is added when
	 * missing).
	 * @param enabled New enabled flag.
	 */
	void setEnabled(ecs::ComponentRegistry &registry,
					const ecs::Entity::Identifier &entityIdentifier,
					bool enabled);

	/**
	 * @brief Check whether an entity is enabled.
	 * @param registry Component registry.
	 * @param entityIdentifier Entity identifier.
	 * @return True when the entity has no StyleState or an enabled one.
	 */
	bool isEnabled(const ecs::ComponentRegistry &registry,
				   const ecs::Entity::Identifier &entityIdentifier);

	/**
	 * @brief Check whether the pointer or a hand hovers an entity.
	 * @param registry Component registry.
	 * @param entityIdentifier Entity identifier.
	 * @return True when hovered and enabled.
	 */
	bool isHovered(const ecs::ComponentRegistry &registry,
				   const ecs::Entity::Identifier &entityIdentifier);

	/**
	 * @brief Check whether an entity is pressed.
	 * @param registry Component registry.
	 * @param entityIdentifier Entity identifier.
	 * @return True when pressed and enabled.
	 */
	bool isPressed(const ecs::ComponentRegistry &registry,
				   const ecs::Entity::Identifier &entityIdentifier);

	/**
	 * @brief Blend the M3 state layer of an entity over its container color.
	 *
	 * Pressed and dragged take precedence over focus, focus over hover. A
	 * disabled or idle entity keeps @p base.
	 *
	 * @param registry Component registry.
	 * @param entityIdentifier Entity identifier.
	 * @param base Container color.
	 * @param content Content color tinting the layer.
	 * @return The container color with the state layer applied.
	 */
	utility::graphic::Color32Bit
		stateLayerColor(const ecs::ComponentRegistry &registry,
						const ecs::Entity::Identifier &entityIdentifier,
						const utility::graphic::Color32Bit &base,
						const utility::graphic::Color32Bit &content);

}	 // namespace guillaume::systems::interaction
