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
#include <map>

#include <utility/event/mouse_button_event.hpp>
#include <utility/event/controller_button_event.hpp>
#include <utility/event/hand_pinch_event.hpp>
#include <utility/event/hand_poke_event.hpp>

#include "guillaume/ecs/component.hpp"

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
		using MouseHoverHandler =
			std::function<void(void)>;	  ///< Hover event handler type
		using MouseUnhoverHandler =
			std::function<void(void)>;	  ///< Unhover event handler type

		using MouseButtonClickHandler =
			std::function<void(void)>;	  ///< Button click event handler type
		using MouseButtonClickReleaseHandler =
			std::function<void(void)>;	  ///< Button release event handler type

		using ControllerHoverHandler   = std::function<void(
			void)>;	   ///< Controller hover event handler type
		using ControllerUnhoverHandler = std::function<void(
			void)>;	   ///< Controller unhover event handler type

		using ControllerButtonClickHandler =
			std::function<void(void)>;	  ///< Controller
										  ///< button
										  ///< click
										  ///< event
		using ControllerButtonClickReleaseHandler =
			std::function<void(void)>;	  ///< Controller
										  ///< button
										  ///< release
										  ///< event

		using HandPinchHandler =
			std::function<void(void)>;	  ///< Hand pinch event handler type

		using HandPokeHandler =
			std::function<void(void)>;	  ///< Hand poke event handler type

		private:
		MouseHoverHandler _onMouseHover;		///< Hover enter event handler
		MouseUnhoverHandler _onMouseUnhover;	///< Hover leave event handler
		bool _isMouseHovered {
			false
		};	  ///< Flag indicating if the entity is currently hovered

		std::map<utility::event::MouseButtonEvent::MouseButton,
				 MouseButtonClickHandler>
			_onMouseButtonClickHandlers {};	   ///< Click event handlers
		std::map<utility::event::MouseButtonEvent::MouseButton,
				 MouseButtonClickReleaseHandler>
			_onMouseButtonClickReleaseHandlers {};	  ///< Release event
													  ///< handlers
		std::map<utility::event::MouseButtonEvent::MouseButton, bool>
			_isMouseButtonClicked {};	 ///< Flag indicating if the entity is
										 ///< currently clicked

		ControllerHoverHandler
			_onControllerHover;	   ///< Controller hover event handler
		ControllerUnhoverHandler
			_onControllerUnhover;	 ///< Controller unhover event
									 ///< handler
		bool _isControllerHovered {
			false
		};	  ///< Flag indicating if the entity is currently hovered by a
			  ///< controller

		std::map<utility::event::ControllerButtonEvent::Button,
				 ControllerButtonClickHandler>
			_onControllerButtonClickHandlers {};	///< Controller button click
													///< event handlers
		std::map<utility::event::ControllerButtonEvent::Button,
				 ControllerButtonClickReleaseHandler>
			_onControllerButtonClickReleaseHandlers {};	   ///< Controller
														   ///< button release
														   ///< event handlers
		std::map<utility::event::ControllerButtonEvent::Button, bool>
			_isControllerButtonClicked {};	  ///< Flag indicating if the entity
											  ///< is currently clicked for a
											  ///< controller button

		HandPinchHandler _onHandPinchHandler;	 ///< Hand pinch event handler
		bool _isHandPinched {
			false
		};	  ///< Flag indicating if the entity is currently clicked for a hand
			  ///< pinch
		HandPokeHandler _onHandPokeHandler;	   ///< Hand poke event handler
		bool _isHandPoked { false };	///< Flag indicating if the entity is
										///< currently clicked for a hand poke

		utility::math::Vector2F _accessibilityMargin {
			0.0f, 0.0f
		};	  ///< Margin to increase the interactive area of entities for
			  ///< better accessibility

		public:
		/**
		 * @brief Default constructor for the Interaction component.
		 */
		Interaction(void);

		/**
		 * @brief Default destructor for the Interaction component.
		 */
		~Interaction(void) = default;

		/**
		 * @brief Set the mouse onHover event handler.
		 *
		 * This handler is called when the cursor enters the entity's bounds.
		 * @param handler The function to call on hover enter.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setMouseOnHoverHandler(const MouseHoverHandler &handler);

		/**
		 * @brief Set the mouse onUnhover event handler.
		 * @param handler The function to call on hover leave.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &
			setMouseOnUnhoverHandler(const MouseUnhoverHandler &handler);

		/**
		 * @brief Get the mouse onHover event handler.
		 * @return The hover enter handler.
		 */
		MouseHoverHandler getMouseOnHoverHandler(void) const;

		/**
		 * @brief Get the mouse onUnhover event handler.
		 * @return The hover leave handler.
		 */
		MouseUnhoverHandler getMouseOnUnhoverHandler(void) const;

		/**
		 * @brief Check if the entity is currently hovered.
		 * @return True when cursor is inside entity bounds.
		 */
		bool isMouseHovered(void) const;

		/**
		 * @brief Set current hovered state.
		 * @param isHovered Current hovered state.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setMouseHovered(bool isHovered);

		/**
		 * @brief Set the onClick event handler for one mouse button.
		 * @param button The mouse button to associate with the handler.
		 * @param handler The function to call on click events.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setMouseButtonOnClickHandler(
			const utility::event::MouseButtonEvent::MouseButton &button,
			const MouseButtonClickHandler &handler);

		/**
		 * @brief Get click handlers indexed by mouse button.
		 * @return A map of mouse buttons to click handlers.
		 */
		const std::map<utility::event::MouseButtonEvent::MouseButton,
					   MouseButtonClickHandler> &
			getMouseButtonOnClickHandlers() const;

		/**
		 * @brief Set the onRelease event handler for one mouse button.
		 * @param button The mouse button to associate with the handler.
		 * @param handler The function to call on release events.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setMouseButtonOnClickReleaseHandler(
			const utility::event::MouseButtonEvent::MouseButton &button,
			const MouseButtonClickReleaseHandler &handler);

		/**
		 * @brief Get click release handlers indexed by mouse button.
		 * @return A map of mouse buttons to release handlers.
		 */
		const std::map<utility::event::MouseButtonEvent::MouseButton,
					   MouseButtonClickReleaseHandler> &
			getMouseButtonOnClickReleaseHandlers() const;

		/**
		 * @brief Check if the entity is currently clicked for one mouse button.
		 * @param button The mouse button to check.
		 * @return True if clicked for the provided button.
		 */
		bool isMouseButtonClicked(
			const utility::event::MouseButtonEvent::MouseButton &button) const;

		/**
		 * @brief Set clicked state for one mouse button.
		 * @param button The mouse button to update.
		 * @param clicked Clicked state to set.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setMouseButtonClicked(
			const utility::event::MouseButtonEvent::MouseButton &button,
			bool clicked);

		/**
		 * @brief Set the onClick event handler for one controller button.
		 * @param button The controller button to associate with the handler.
		 * @param handler The function to call on click events.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setControllerButtonOnClickHandler(
			const utility::event::ControllerButtonEvent::Button &button,
			const ControllerButtonClickHandler &handler);

		/**
		 * @brief Get click handlers indexed by controller button.
		 * @return A map of controller buttons to click handlers.
		 */
		const std::map<utility::event::ControllerButtonEvent::Button,
					   ControllerButtonClickHandler> &
			getControllerButtonOnClickHandlers() const;

		/**
		 * @brief Set the onRelease event handler for one controller button.
		 * @param button The controller button to associate with the handler.
		 * @param handler The function to call on release events.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setControllerButtonOnClickReleaseHandler(
			const utility::event::ControllerButtonEvent::Button &button,
			const ControllerButtonClickReleaseHandler &handler);

		/**
		 * @brief Get click release handlers indexed by controller button.
		 * @return A map of controller buttons to release handlers.
		 */
		const std::map<utility::event::ControllerButtonEvent::Button,
					   ControllerButtonClickReleaseHandler> &
			getControllerButtonOnClickReleaseHandlers() const;

		/**
		 * @brief Check if the entity is currently clicked for one controller
		 * button.
		 * @param button The controller button to check.
		 * @return True if clicked for the provided button.
		 */
		bool isControllerButtonClicked(
			const utility::event::ControllerButtonEvent::Button &button) const;

		/**
		 * @brief Set clicked state for one controller button.
		 * @param button The controller button to update.
		 * @param clicked Clicked state to set.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setControllerButtonClicked(
			const utility::event::ControllerButtonEvent::Button &button,
			bool clicked);

		/**
		 * @brief Set the onHover event handler for controllers.
		 *
		 * This handler is called when the controller pointer enters the
		 * entity's bounds.
		 * @param handler The function to call on hover enter.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setControllerOnHoverHandler(const ControllerHoverHandler &handler);

		/**
		 * @brief Set the controller onUnhover event handler.
		 * @param handler The function to call on hover leave.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &
			setControllerOnUnhoverHandler(const ControllerUnhoverHandler &handler);

		/**
		 * @brief Get the controller onHover event handler.
		 * @return The hover enter handler.
		 */
		ControllerHoverHandler getControllerOnHoverHandler(void) const;

		/**
		 * @brief Get the controller onUnhover event handler.
		 * @return The hover leave handler.
		 */
		ControllerUnhoverHandler getControllerOnUnhoverHandler(void) const;

		/**
		 * @brief Check if the entity is currently hovered.
		 * @return True when cursor is inside entity bounds.
		 */
		bool isControllerHovered(void) const;

		/**
		 * @brief Set current hovered state.
		 * @param isHovered Current hovered state.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setControllerHovered(bool isHovered);

		/**
		 * @brief Set the onHandPinch event handler.
		 * @param handler The function to call on hand pinch events.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setHandPinchHandler(const HandPinchHandler &handler);

		/**
		 * @brief Get the onHandPinch event handler.
		 * @return The hand pinch event handler.
		 */
		HandPinchHandler getHandPinchHandler(void) const;

		/**
		 * @brief Check if the entity is currently clicked for a hand pinch.
		 * @return True if clicked for a hand pinch.
		 */
		bool isHandPinched(void) const;

		/**
		 * @brief Set clicked state for a hand pinch.
		 * @param isHandPinched Clicked state to set.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setHandPinched(bool isHandPinched);

		/**
		 * @brief Set the onHandPoke event handler.
		 * @param handler The function to call on hand poke events.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setHandPokeHandler(const HandPokeHandler &handler);

		/**
		 * @brief Get the onHandPoke event handler.
		 * @return The hand poke event handler.
		 */
		HandPokeHandler getHandPokeHandler(void) const;

		/**
		 * @brief Check if the entity is currently clicked for a hand poke.
		 * @return True if clicked for a hand poke.
		 */
		bool isHandPoked(void) const;

		/**
		 * @brief Set clicked state for a hand poke.
		 * @param isHandPoked Clicked state to set.
		 * @return Reference to this Interaction component for chaining.
		 */
		Interaction &setHandPoked(bool isHandPoked);

		/**
		 * @brief Get the Accessibility Margin object
		 *
		 * @return The margin to increase the interactive area of entities for
		 * better
		 */
		const utility::math::Vector2F &getAccessibilityMargin() const;

		/**
		 * @brief Set the Accessibility Margin object
		 *
		 * @param margin The margin to increase the interactive area of entities
		 * for better accessibility
		 */
		void setAccessibilityMargin(const utility::math::Vector2F &margin);
	};

}	 // namespace guillaume::components
