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

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Makes an entity able to own the input focus, and holds whether it
	 * owns it and what to call when it gains or loses it.
	 *
	 * At most one entity is focused at a time; systems::Focus decides which.
	 * It runs in the Event phase, gives the focus to an entity that is
	 * clicked (left mouse button, hand button A, B, X or Y, pinch or poke) or
	 * reached with Tab / Shift+Tab, then writes the focused flag here and in
	 * StyleState and calls the handlers. An entity disabled through
	 * StyleState cannot take or keep the focus.
	 *
	 * @code
	 * auto &focus = registry.getComponent<components::Focus>(identifier);
	 * focus.setOnFocusGainedHandler([] { showCaret(); });
	 * focus.setOnFocusLostHandler([] { hideCaret(); });
	 * @endcode
	 *
	 * To move the focus from code, call systems::Focus::setFocus or
	 * systems::Focus::clearFocus: setFocused() only stores the flag and does
	 * not take the focus away from the entity that owns it. Tab order and
	 * exclusion from the Tab traversal are set on Accessibility.
	 *
	 * @see systems::Focus, StyleState, Accessibility
	 */
	class Focus: public ecs::Component
	{
		public:
		using Handler = std::function<void(void)>;	  ///< Focus change callback

		private:
		Handler _onFocusGained;		///< Called when the entity gains the focus
		Handler _onFocusLost;		///< Called when the entity loses the focus
		bool _focused { false };	///< Whether the entity owns the focus

		public:
		/**
		 * @brief Construct an unfocused component without handler.
		 */
		Focus(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Focus(void) = default;

		/**
		 * @brief Store the callback run when the entity gains the focus.
		 *
		 * systems::Focus calls it each time it sets the focus on the entity,
		 * including when the entity already owned it. Always marks the
		 * component changed.
		 *
		 * @param handler The callback, or `nullptr` to remove the current one.
		 */
		void setOnFocusGainedHandler(const Handler &handler);

		/**
		 * @brief Get the callback run when the entity gains the focus.
		 * @return A copy of the handler; empty when none was set.
		 */
		Handler getOnFocusGainedHandler(void) const;

		/**
		 * @brief Store the callback run when the entity loses the focus.
		 *
		 * systems::Focus calls it when the focus moves to another entity or
		 * is cleared. Always marks the component changed.
		 *
		 * @param handler The callback, or `nullptr` to remove the current one.
		 */
		void setOnFocusLostHandler(const Handler &handler);

		/**
		 * @brief Get the callback run when the entity loses the focus.
		 * @return A copy of the handler; empty when none was set.
		 */
		Handler getOnFocusLostHandler(void) const;

		/**
		 * @brief Check whether the entity owns the input focus.
		 * @return True when systems::Focus last gave the focus to this entity.
		 */
		bool isFocused(void) const;

		/**
		 * @brief Record whether the entity owns the input focus.
		 *
		 * Called by systems::Focus; it calls no handler and does not change
		 * which entity the system considers focused. Marks the component
		 * changed only when the flag differs.
		 *
		 * @param focused True when the entity owns the focus.
		 */
		void setFocused(bool focused);
	};

}	 // namespace guillaume::components
