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
	 * @brief Marks an entity as something shown temporarily above the rest
	 * of the interface (a dialog, a menu, a drawer, a snackbar, a tooltip)
	 * and holds how it is dismissed.
	 *
	 * The widget systems (systems::Dialog, systems::Menu, ...) write the
	 * flags, and their `show` and `hide` functions write the visibility.
	 * systems::Overlay reads the component in the Event phase: it keeps the
	 * visible overlays in a stack and dismisses the top one on Escape or
	 * when its auto-dismiss delay has elapsed. A click outside the top
	 * overlay dismisses it only when the application reports the click
	 * with systems::Overlay::handleOutsideClick; no system calls it.
	 *
	 * Dismissing sets the overlay invisible, resets its elapsed time and
	 * calls the dismiss handler. Setting a shown overlay invisible by hand
	 * has the same effect on the next frame, handler included.
	 *
	 * The component holds no drawing state: it is up to the widget system
	 * to draw a hidden overlay transparent.
	 *
	 * @see systems::Overlay, components::Dialog, components::Menu
	 */
	class Overlay: public ecs::Component
	{
		public:
		using DismissHandler =
			std::function<void(void)>;	  ///< Callback run on dismissal.

		private:
		bool _visible { false };	///< Whether the overlay is shown.
		bool _modal { true };		///< Whether the overlay is modal.
		bool _dismissOnOutsideClick {
			true
		};	  ///< Whether an outside click dismisses the overlay.

		bool _dismissOnEscape { true };	   ///< Whether Escape dismisses it.
		float _autoDismiss { 0.0f };	   ///< Auto-dismiss delay (0 = never).
		float _elapsed { 0.0f };		   ///< Elapsed time while visible.
		DismissHandler _onDismiss {};	   ///< Dismiss handler.

		public:
		/**
		 * @brief Create a hidden, modal overlay that is dismissed on Escape
		 * and on an outside click, and never by itself.
		 */
		Overlay(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Overlay(void) = default;

		/**
		 * @brief Tell whether the overlay is shown.
		 * @return True when shown; false by default.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Show or hide the overlay.
		 *
		 * Hiding also resets the elapsed time to 0. On its next pass,
		 * systems::Overlay puts a newly shown overlay on top of its stack,
		 * and dismisses (handler included) one that was hidden this way.
		 * Marks the component changed only when the flag differs.
		 * @param visible True to show the overlay.
		 * @return This component, for chaining.
		 */
		Overlay &setVisible(bool visible);

		/**
		 * @brief Tell whether the overlay is modal.
		 * @return True when modal, which is the default.
		 */
		bool isModal(void) const;

		/**
		 * @brief Declare the overlay modal (it must be answered before the
		 * rest of the interface is used) or not.
		 *
		 * The flag is a declaration made by the widget system: a dialog is
		 * modal, a snackbar or a tooltip is not. Marks the component
		 * changed only when the flag differs.
		 * @param modal True for a modal overlay.
		 * @return This component, for chaining.
		 */
		Overlay &setModal(bool modal);

		/**
		 * @brief Tell whether a click outside the overlay dismisses it.
		 * @return True when it does, which is the default.
		 */
		bool dismissesOnOutsideClick(void) const;

		/**
		 * @brief Choose whether a click outside the overlay dismisses it.
		 *
		 * Read by systems::Overlay::handleOutsideClick, for the top
		 * overlay only. Marks the component changed only when the flag
		 * differs.
		 * @param dismiss True to dismiss on an outside click.
		 * @return This component, for chaining.
		 */
		Overlay &setDismissOnOutsideClick(bool dismiss);

		/**
		 * @brief Tell whether the Escape key dismisses the overlay.
		 * @return True when it does, which is the default.
		 */
		bool dismissesOnEscape(void) const;

		/**
		 * @brief Choose whether the Escape key dismisses the overlay.
		 *
		 * Escape only acts on the top overlay. Marks the component changed
		 * only when the flag differs.
		 * @param dismiss True to dismiss on Escape.
		 * @return This component, for chaining.
		 */
		Overlay &setDismissOnEscape(bool dismiss);

		/**
		 * @brief Get how long the overlay stays shown before it is
		 * dismissed by itself.
		 * @return The delay, in seconds; 0 means it never is.
		 */
		float getAutoDismiss(void) const;

		/**
		 * @brief Set how long the overlay stays shown before it is
		 * dismissed by itself.
		 *
		 * The countdown only runs while the overlay is the top one. Marks
		 * the component changed only when the value differs.
		 * @param delay The delay, in seconds; 0 or less disables
		 * auto-dismissal.
		 * @return This component, for chaining.
		 */
		Overlay &setAutoDismiss(float delay);

		/**
		 * @brief Tell whether the overlay is dismissed by itself after a
		 * delay.
		 * @return True when the auto-dismiss delay is greater than 0.
		 */
		bool hasAutoDismiss(void) const;

		/**
		 * @brief Get how long the overlay has been counting towards its
		 * auto-dismissal.
		 * @return The time, in seconds; 0 for an overlay without an
		 * auto-dismiss delay.
		 */
		float getElapsed(void) const;

		/**
		 * @brief Store the time counted towards auto-dismissal.
		 *
		 * systems::Overlay adds the frame time to it; write 0 to restart
		 * the countdown. Does not mark the component changed.
		 * @param elapsed The new elapsed time, in seconds.
		 * @return This component, for chaining.
		 */
		Overlay &setElapsed(float elapsed);

		/**
		 * @brief Set the callback run when systems::Overlay dismisses the
		 * overlay.
		 *
		 * Widget systems install their own here (for example to hide the
		 * widget and call the user's callback), so replacing it on a widget
		 * disconnects that. Does not mark the component changed.
		 * @param handler The callback; an empty one removes it.
		 * @return This component, for chaining.
		 */
		Overlay &setOnDismissHandler(const DismissHandler &handler);

		/**
		 * @brief Get the callback run when the overlay is dismissed.
		 * @return A copy of the callback; empty when none is set.
		 */
		DismissHandler getOnDismissHandler(void) const;
	};

}	 // namespace guillaume::components
