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
	 * @brief Component handling mouse hover interactions.
	 * @see systems::MouseMotion
	 */
	class MouseHoverInteraction: public ecs::Component
	{
		public:
		using HoverHandler =
			std::function<void(void)>;	  ///< Type for hover event handlers
		using UnhoverHandler =
			std::function<void(void)>;	  ///< Type for unhover event handlers

		private:
		HoverHandler _onHover;	  ///< Hover event handler for when the mouse
								  ///< starts hovering
		UnhoverHandler _onUnhover;	  ///< Unhover event handler for when the
									  ///< mouse stops hovering
		bool _isHovered {
			false
		};	  ///< Whether the mouse is currently hovering over the entity
		utility::graphic::SizeUI _accessibilityMargin {
			0, 0
		};	  ///< Additional margin around the entity for hover detection to
			  ///< improve accessibility

		public:
		/**
		 * @brief Default constructor for the MouseHoverInteraction component.
		 */
		MouseHoverInteraction(void);

		/**
		 * @brief Default destructor for the MouseHoverInteraction component.
		 */
		~MouseHoverInteraction(void) = default;

		/**
		 * @brief Set the hover event handler.
		 * @param handler The function to call when the mouse starts hovering.
		 * @return Reference to this MouseHoverInteraction for method chaining.
		 */
		MouseHoverInteraction &setOnHoverHandler(const HoverHandler &handler);
		/**
		 * @brief Set the unhover event handler.
		 * @param handler The function to call when the mouse stops hovering.
		 * @return Reference to this MouseHoverInteraction for method chaining.
		 */
		MouseHoverInteraction &
			setOnUnhoverHandler(const UnhoverHandler &handler);

		/**
		 * @brief Get the hover event handler.
		 * @return The hover event handler.
		 */
		HoverHandler getOnHoverHandler(void) const;

		/**
		 * @brief Get the unhover event handler.
		 * @return The unhover event handler.
		 */
		UnhoverHandler getOnUnhoverHandler(void) const;

		/**
		 * @brief Check if the mouse is currently hovering over the entity.
		 * @return True if the mouse is hovering, false otherwise.
		 */
		bool isHovered(void) const;

		/**
		 * @brief Set whether the mouse is currently hovering over the entity.
		 * @param isHovered True if the mouse is hovering, false otherwise.
		 * @return Reference to this MouseHoverInteraction for method chaining.
		 */
		MouseHoverInteraction &setHovered(bool isHovered);

		/**
		 * @brief Get the accessibility margin for hover detection.
		 * @return The accessibility margin as a SizeUI object.
		 */
		utility::graphic::SizeUI getAccessibilityMargin(void) const;

		/**
		 * @brief Set the accessibility margin for hover detection.
		 * @param margin The new accessibility margin as a SizeUI object.
		 * @return Reference to this MouseHoverInteraction for method chaining.
		 */
		MouseHoverInteraction &
			setAccessibilityMargin(const utility::graphic::SizeUI &margin);
	};

}	 // namespace guillaume::components
