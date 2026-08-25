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

#include <utility/graphic/size.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component handling hand hover interactions.
	 * @see systems::HandMotion
	 */
	class HandHoverInteraction: public ecs::Component
	{
		public:
		using HoverHandler =
			std::function<void(void)>;	  ///< Type for hover handlers
		using UnhoverHandler =
			std::function<void(void)>;	  ///< Type for unhover handlers

		private:
		HoverHandler
			_onHover;	 ///< Handler function to call when a hover event occurs
		UnhoverHandler _onUnhover;	  ///< Handler function to call when an
									  ///< unhover event occurs
		bool _isHovered { false };	  ///< Current hover state of the hand
		utility::graphic::SizeUI _accessibilityMargin {
			0, 0
		};	  ///< Margin around the hand's bounds to increase hover
			  ///< accessibility

		public:
		/**
		 * @brief Construct a HandHoverInteraction component with default
		 * handlers and states.
		 */
		HandHoverInteraction(void);

		/**
		 * @brief Default destructor for HandHoverInteraction.
		 */
		~HandHoverInteraction(void) = default;

		/**
		 * @brief Set the handler for a hover event.
		 * @param handler The function to call when a hover event occurs.
		 * @return Reference to this HandHoverInteraction for method chaining.
		 */
		HandHoverInteraction &setOnHoverHandler(const HoverHandler &handler);

		/**
		 * @brief Set the handler for an unhover event.
		 * @param handler The function to call when an unhover event occurs.
		 * @return Reference to this HandHoverInteraction for method chaining.
		 */
		HandHoverInteraction &
			setOnUnhoverHandler(const UnhoverHandler &handler);

		/**
		 * @brief Get the handler for hover events.
		 * @return The function set as the hover event handler.
		 */
		HoverHandler getOnHoverHandler(void) const;

		/**
		 * @brief Get the handler for unhover events.
		 * @return The function set as the unhover event handler.
		 */
		UnhoverHandler getOnUnhoverHandler(void) const;

		/**
		 * @brief Check if the hand is currently hovered.
		 * @return True if the hand is hovered, false otherwise.
		 */
		bool isHovered(void) const;

		/**
		 * @brief Set the hovered state of the hand.
		 * @param isHovered The new hovered state to set.
		 * @return Reference to this HandHoverInteraction for method chaining.
		 */
		HandHoverInteraction &setHovered(bool isHovered);

		/**
		 * @brief Get the accessibility margin around the hand's bounds.
		 * @return The current accessibility margin as a SizeUI object.
		 */
		utility::graphic::SizeUI getAccessibilityMargin(void) const;

		/**
		 * @brief Set the accessibility margin around the hand's bounds.
		 * @param margin The new accessibility margin to set as a SizeUI object.
		 * @return Reference to this HandHoverInteraction for method chaining.
		 */
		HandHoverInteraction &
			setAccessibilityMargin(const utility::graphic::SizeUI &margin);
	};

}	 // namespace guillaume::components
