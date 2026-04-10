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

#include "guillaume/components/click.hpp"
#include "guillaume/components/hover.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component combining hover and click interactions in a single
	 * component.
	 * @see systems::Interaction
	 */
	class Interaction: public ecs::Component
	{
		public:
		using ClickHandler = Click::Handler;    ///< Type alias for click event handlers
		using HoverHandler = Hover::Handler;    ///< Type alias for hover event handlers

		private:
		Click _click;   ///< Click component to manage click interactions
		Hover _hover;   ///< Hover component to manage hover interactions

        utility::math::Vector2F _accessibilityMargin {
			0.0f, 0.0f
		};	  ///< Margin to increase the interactive area of entities for
			  ///< better accessibility

		public:
		/**
		 * @brief Default constructor for the Interaction component.
		 */
		Interaction(void) = default;

		/**
		 * @brief Default destructor for the Interaction component.
		 */
		~Interaction(void) = default;

		/**
		 * @brief Set the onClick event handler.
		 * @param button The mouse button to associate with the handler.
		 * @param handler The function to call on click events.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setOnClickHandler(
			const utility::event::MouseButtonEvent::MouseButton &button,
			const ClickHandler &handler)
		{
			_click.setOnClickHandler(button, handler);
			return *this;
		}

		/**
		 * @brief Set the onRelease event handler.
		 * @param button The mouse button to associate with the handler.
		 * @param handler The function to call on release events.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setOnReleaseHandler(
			const utility::event::MouseButtonEvent::MouseButton &button,
			const ClickHandler &handler)
		{
			_click.setOnReleaseHandler(button, handler);
			return *this;
		}

		/**
		 * @brief Get click handlers indexed by mouse button.
		 * @return A map of mouse buttons to click handlers.
		 */
		const std::map<utility::event::MouseButtonEvent::MouseButton,
					   ClickHandler> &
			getOnClickHandlers() const
		{
			return _click.getOnClickHandlers();
		}

		/**
		 * @brief Get release handlers indexed by mouse button.
		 * @return A map of mouse buttons to release handlers.
		 */
		const std::map<utility::event::MouseButtonEvent::MouseButton,
					   ClickHandler> &
			getOnReleaseHandlers() const
		{
			return _click.getOnReleaseHandlers();
		}

		/**
		 * @brief Check if the entity is currently clicked for one button.
		 * @param button The mouse button to check.
		 * @return True if clicked for the provided button.
		 */
		bool isClicked(
			const utility::event::MouseButtonEvent::MouseButton &button) const
		{
			return _click.isClicked(button);
		}

		/**
		 * @brief Set clicked state for one mouse button.
		 * @param button The mouse button to update.
		 * @param clicked Clicked state to set.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setClicked(
			const utility::event::MouseButtonEvent::MouseButton &button,
			bool clicked)
		{
			_click.setClicked(button, clicked);
			return *this;
		}

		/**
		 * @brief Check if the entity is clicked for any button.
		 * @return True if at least one button is in clicked state.
		 */
		bool isEntityClicked() const
		{
			return _click.isEntityClicked();
		}

		/**
		 * @brief Set whether a button press started inside entity bounds.
		 * @param button The mouse button to update.
		 * @param pressedInside Whether the press started inside bounds.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setPressedInside(
			const utility::event::MouseButtonEvent::MouseButton &button,
			bool pressedInside)
		{
			_click.setPressedInside(button, pressedInside);
			return *this;
		}

		/**
		 * @brief Check whether a button press started inside entity bounds.
		 * @param button The mouse button to check.
		 * @return True if pressed inside bounds for this button.
		 */
		bool isPressedInside(
			const utility::event::MouseButtonEvent::MouseButton &button) const
		{
			return _click.isPressedInside(button);
		}

		/**
		 * @brief Set the onHover event handler.
		 * @param handler The function to call on hover enter.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setOnHoverHandler(const HoverHandler &handler)
		{
			_hover.setOnHoverHandler(handler);
			return *this;
		}

		/**
		 * @brief Set the onUnhover event handler.
		 * @param handler The function to call on hover leave.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setOnUnhoverHandler(const HoverHandler &handler)
		{
			_hover.setOnUnhoverHandler(handler);
			return *this;
		}

		/**
		 * @brief Get the onHover event handler.
		 * @return The hover enter handler.
		 */
		HoverHandler getOnHoverHandler(void) const
		{
			return _hover.getOnHoverHandler();
		}

		/**
		 * @brief Get the onUnhover event handler.
		 * @return The hover leave handler.
		 */
		HoverHandler getOnUnhoverHandler(void) const
		{
			return _hover.getOnUnhoverHandler();
		}

		/**
		 * @brief Check if the entity is currently hovered.
		 * @return True when cursor is inside entity bounds.
		 */
		bool isHovered(void) const
		{
			return _hover.isHovered();
		}

		/**
		 * @brief Set current hovered state.
		 * @param isHovered Current hovered state.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setHovered(bool isHovered)
		{
			_hover.setHovered(isHovered);
			return *this;
		}

        /**
         * @brief Get the Accessibility Margin object
         *
         * @return The margin to increase the interactive area of entities for better
         */
        const utility::math::Vector2F &getAccessibilityMargin() const
        {
            return _accessibilityMargin;
        }

        /**
		 * @brief Set the Accessibility Margin object
		 *
		 * @param margin The margin to increase the interactive area of entities
		 * for better accessibility
		 */
		void setAccessibilityMargin(const utility::math::Vector2F &margin)
		{
			_accessibilityMargin = margin;
		}

	};

}    // namespace guillaume::components
