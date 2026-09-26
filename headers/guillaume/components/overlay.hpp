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
	 * @brief Component marking an entity as a transient overlay.
	 *
	 * Overlays (dialogs, menus, sheets, snackbars, tooltips) are stacked by
	 * `systems::Overlay`, which handles modal input capture, dismissal on
	 * outside click or Escape, and optional auto-dismiss timing.
	 *
	 * @see systems::Overlay
	 */
	class Overlay: public ecs::Component
	{
		public:
		using DismissHandler =
			std::function<void(void)>;	  ///< Dismiss handler.

		private:
		bool _visible { false };	///< Whether the overlay is shown.
		bool _modal { true };		///< Whether the overlay captures input.
		bool _dismissOnOutsideClick {
			true
		};	  ///< Whether an outside click dismisses the overlay.
		bool _dismissOnEscape { true };	   ///< Whether Escape dismisses it.
		float _autoDismiss { 0.0f };	   ///< Auto-dismiss delay (0 = never).
		float _elapsed { 0.0f };		   ///< Elapsed time while visible.
		DismissHandler _onDismiss {};	   ///< Dismiss handler.

		public:
		/**
		 * @brief Default constructor for the Overlay component.
		 */
		Overlay(void) = default;

		/**
		 * @brief Default destructor for the Overlay component.
		 */
		~Overlay(void) = default;

		/**
		 * @brief Check whether the overlay is visible.
		 * @return True when visible.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Set the visibility of the overlay.
		 * @param visible The new visibility.
		 * @return Reference to this Overlay for chaining.
		 */
		Overlay &setVisible(bool visible);

		/**
		 * @brief Check whether the overlay is modal.
		 * @return True when modal.
		 */
		bool isModal(void) const;

		/**
		 * @brief Set whether the overlay is modal.
		 * @param modal Whether the overlay captures input.
		 * @return Reference to this Overlay for chaining.
		 */
		Overlay &setModal(bool modal);

		/**
		 * @brief Check whether an outside click dismisses the overlay.
		 * @return True when dismiss-on-outside-click is enabled.
		 */
		bool dismissesOnOutsideClick(void) const;

		/**
		 * @brief Set whether an outside click dismisses the overlay.
		 * @param dismiss Whether to dismiss on outside click.
		 * @return Reference to this Overlay for chaining.
		 */
		Overlay &setDismissOnOutsideClick(bool dismiss);

		/**
		 * @brief Check whether Escape dismisses the overlay.
		 * @return True when dismiss-on-Escape is enabled.
		 */
		bool dismissesOnEscape(void) const;

		/**
		 * @brief Set whether Escape dismisses the overlay.
		 * @param dismiss Whether to dismiss on Escape.
		 * @return Reference to this Overlay for chaining.
		 */
		Overlay &setDismissOnEscape(bool dismiss);

		/**
		 * @brief Get the auto-dismiss delay.
		 * @return The delay in seconds (0 means never).
		 */
		float getAutoDismiss(void) const;

		/**
		 * @brief Set the auto-dismiss delay.
		 * @param delay The delay in seconds (0 means never).
		 * @return Reference to this Overlay for chaining.
		 */
		Overlay &setAutoDismiss(float delay);

		/**
		 * @brief Check whether the overlay auto-dismisses.
		 * @return True when an auto-dismiss delay is set.
		 */
		bool hasAutoDismiss(void) const;

		/**
		 * @brief Get the elapsed visible time.
		 * @return The elapsed time, in seconds.
		 */
		float getElapsed(void) const;

		/**
		 * @brief Set the elapsed visible time.
		 * @param elapsed The new elapsed time, in seconds.
		 * @return Reference to this Overlay for chaining.
		 */
		Overlay &setElapsed(float elapsed);

		/**
		 * @brief Set the dismiss handler.
		 * @param handler The handler to call when the overlay is dismissed.
		 * @return Reference to this Overlay for chaining.
		 */
		Overlay &setOnDismissHandler(const DismissHandler &handler);

		/**
		 * @brief Get the dismiss handler.
		 * @return The dismiss handler.
		 */
		DismissHandler getOnDismissHandler(void) const;
	};

}	 // namespace guillaume::components
