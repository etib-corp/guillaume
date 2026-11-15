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

#include <cstdint>
#include <optional>

#include "guillaume/ecs/component.hpp"
#include "guillaume/theme/tokens.hpp"

namespace guillaume::components
{

	/**
	 * @brief The interaction state of an entity in one place: enabled or
	 * disabled, and whether it is hovered, pressed, focused or dragged.
	 *
	 * The enabled flag is set by user code or a widget
	 * (systems::interaction::setEnabled). The four other flags are derived:
	 * systems::StyleState rewrites hovered, pressed and dragged every frame
	 * in the Event phase from the hover, button, pinch, poke and drag
	 * interaction components of the entity, and systems::Focus writes
	 * focused. Widget systems read getActiveLayer() to tint the entity,
	 * systems::FocusRingRender draws a ring around a focused entity, and the
	 * input systems ignore a disabled entity.
	 *
	 * @code
	 * const auto &state =
	 * 	registry.getComponent<components::StyleState>(identifier);
	 * if (state.getActiveLayer() == theme::StateLayer::Pressed) { ... }
	 * @endcode
	 *
	 * A value written with setHovered(), setPressed() or setDragged() is
	 * overwritten on the next frame. An entity without StyleState is treated
	 * as enabled.
	 *
	 * @see systems::StyleState, systems::Focus, systems::interaction
	 */
	class StyleState: public ecs::Component
	{
		public:
		/**
		 * @brief Construct an enabled, idle state.
		 */
		StyleState(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~StyleState(void) = default;

		/**
		 * @brief Check whether the entity accepts interaction.
		 * @return True when enabled, which is the default.
		 */
		bool isEnabled(void) const;

		/**
		 * @brief Enable or disable the entity.
		 *
		 * Disabling also clears the hovered, pressed, focused and dragged
		 * flags, and they stay false while the entity is disabled. Marks the
		 * component changed only when a flag differs.
		 *
		 * @param enabled False to make the input systems ignore the entity.
		 * @return This component, to chain calls.
		 */
		StyleState &setEnabled(bool enabled);

		/**
		 * @brief Check whether the mouse pointer or a hand ray is over the
		 * entity.
		 * @return True when hovered; always false when disabled.
		 */
		bool isHovered(void) const;

		/**
		 * @brief Record whether the entity is hovered.
		 *
		 * Called every frame by systems::StyleState. Stores false when the
		 * entity is disabled; marks the component changed only when the flag
		 * differs.
		 *
		 * @param hovered True when a pointer or a hand ray is over the entity.
		 * @return This component, to chain calls.
		 */
		StyleState &setHovered(bool hovered);

		/**
		 * @brief Check whether the entity is pressed: the left mouse button
		 * or hand button A is held on it, or a hand pinches or pokes it.
		 * @return True when pressed; always false when disabled.
		 */
		bool isPressed(void) const;

		/**
		 * @brief Record whether the entity is pressed.
		 *
		 * Called every frame by systems::StyleState. Stores false when the
		 * entity is disabled; marks the component changed only when the flag
		 * differs.
		 *
		 * @param pressed True when the entity is pressed.
		 * @return This component, to chain calls.
		 */
		StyleState &setPressed(bool pressed);

		/**
		 * @brief Check whether the entity owns the input focus.
		 * @return True when focused; always false when disabled.
		 */
		bool isFocused(void) const;

		/**
		 * @brief Record whether the entity owns the input focus.
		 *
		 * Called by systems::Focus when the focus moves. Stores false when
		 * the entity is disabled; marks the component changed only when the
		 * flag differs.
		 *
		 * @param focused True when the entity owns the focus.
		 * @return This component, to chain calls.
		 */
		StyleState &setFocused(bool focused);

		/**
		 * @brief Check whether a drag gesture is in progress on the entity.
		 * @return True when dragged; always false when disabled.
		 */
		bool isDragged(void) const;

		/**
		 * @brief Record whether the entity is being dragged.
		 *
		 * Called every frame by systems::StyleState from
		 * DragInteraction::isDragging. Stores false when the entity is
		 * disabled; marks the component changed only when the flag differs.
		 *
		 * @param dragged True when a drag is in progress on the entity.
		 * @return This component, to chain calls.
		 */
		StyleState &setDragged(bool dragged);

		/**
		 * @brief Get the Material 3 state layer to paint over the entity.
		 *
		 * When several flags are set the strongest one wins: dragged, then
		 * pressed, then focused, then hovered.
		 *
		 * @return The layer of the strongest active state, or `std::nullopt`
		 * when the entity is idle or disabled.
		 */
		std::optional<theme::StateLayer> getActiveLayer(void) const;

		private:
		bool _enabled { true };		///< Whether interaction is accepted.
		bool _hovered { false };	///< Pointer or hand hovers the entity.
		bool _pressed { false };	///< Primary button is held on the entity.
		bool _focused { false };	///< Entity owns the input focus.
		bool _dragged { false };	///< Entity is being dragged.

		/**
		 * @brief Store a flag and mark the component changed when it differs.
		 * @param flag The member to update.
		 * @param value The value to store in it.
		 */
		void setFlag(bool &flag, bool value);
	};

}	 // namespace guillaume::components
