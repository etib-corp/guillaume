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
	 * @brief Shared interaction state of an entity.
	 *
	 * `StyleState` is the single source of truth for the enabled flag and the
	 * transient hover/press/focus/drag states. Interaction systems and
	 * `systems::Focus` skip entities whose state is disabled, and
	 * `systems::StyleState` derives the transient flags from the interaction
	 * components every frame. Entities read it to pick their state layer.
	 *
	 * @see systems::StyleState
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
		 * @return True when enabled.
		 */
		bool isEnabled(void) const;

		/**
		 * @brief Enable or disable the entity.
		 *
		 * Disabling also clears the transient hover/press/focus/drag flags.
		 *
		 * @param enabled New enabled flag.
		 * @return Reference to this StyleState for chaining.
		 */
		StyleState &setEnabled(bool enabled);

		/** @brief Check the hover flag. @return True when hovered. */
		bool isHovered(void) const;

		/** @brief Set the hover flag. @param hovered New flag. @return *this. */
		StyleState &setHovered(bool hovered);

		/** @brief Check the press flag. @return True when pressed. */
		bool isPressed(void) const;

		/** @brief Set the press flag. @param pressed New flag. @return *this. */
		StyleState &setPressed(bool pressed);

		/** @brief Check the focus flag. @return True when focused. */
		bool isFocused(void) const;

		/** @brief Set the focus flag. @param focused New flag. @return *this. */
		StyleState &setFocused(bool focused);

		/** @brief Check the drag flag. @return True when dragged. */
		bool isDragged(void) const;

		/** @brief Set the drag flag. @param dragged New flag. @return *this. */
		StyleState &setDragged(bool dragged);

		/**
		 * @brief Get the state layer to paint, by M3 priority
		 * (dragged > pressed > focused > hovered).
		 * @return The active layer, or none when idle or disabled.
		 */
		std::optional<theme::StateLayer> getActiveLayer(void) const;

		private:
		bool _enabled { true };		///< Whether interaction is accepted.
		bool _hovered { false };	///< Pointer or hand hovers the entity.
		bool _pressed { false };	///< Primary button is held on the entity.
		bool _focused { false };	///< Entity owns the input focus.
		bool _dragged { false };	///< Entity is being dragged.

		/**
		 * @brief Update a flag and mark the component changed.
		 * @param flag Flag to update.
		 * @param value New value.
		 */
		void setFlag(bool &flag, bool value);
	};

}	 // namespace guillaume::components
