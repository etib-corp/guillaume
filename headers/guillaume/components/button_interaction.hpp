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

#include <array>
#include <functional>
#include <map>

#include <utility/event/event.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Concept to check if a type is an enum.
	 * @tparam T The type to check.
	 * @return True if T is an enum type, false otherwise.
	 */
	template<typename T>
	concept IsEnum = std::is_enum_v<T>;

	/**
	 * @brief Generic component handling button interactions for any input type.
	 *
	 * @tparam EventType The event type (must satisfy
	 * utility::event::InheritFromEvent).
	 * @tparam ButtonEnum The enum type representing buttons in the event.
	 * @tparam N Number of buttons in the compile-time array.
	 * @tparam ButtonsArray A compile-time array of valid button values.
	 */
	template<utility::event::InheritFromEvent EventType, IsEnum ButtonEnum,
			 std::size_t N, const std::array<ButtonEnum, N> &ButtonsArray>
	class ButtonInteraction: public ecs::Component
	{
		public:
		/**
		 * @brief Type for button press handlers, which are functions that take
		 * no arguments and return void. These handlers will be called when the
		 * corresponding button is pressed.
		 */
		using ButtonPressHandler = std::function<void(void)>;
		/**
		 * @brief Type for button release handlers, which are functions that
		 * take no arguments and return void. These handlers will be called when
		 * the corresponding button is released.
		 */
		using ButtonReleaseHandler =
			std::function<void(void)>;	  ///< Type for button release handlers

		private:
		std::map<ButtonEnum, ButtonPressHandler>
			_onButtonPressHandlers {};	  ///< Map of button press handlers
		std::map<ButtonEnum, ButtonReleaseHandler>
			_onButtonReleaseHandlers {};	///< Map of button release handlers
		std::map<ButtonEnum, bool>
			_isButtonPressed {};	///< Map of button pressed states

		public:
		/**
		 * @brief Constructs a ButtonInteraction component.
		 */
		ButtonInteraction(void);

		/**
		 * @brief Destructor for the ButtonInteraction component.
		 */
		~ButtonInteraction(void) = default;

		/**
		 * @brief Sets the handler function to be called when a button is
		 * pressed.
		 *
		 * @param button The button for which to set the press handler. Must be
		 * one of the values in ButtonsArray.
		 * @param handler The function to be called when the specified button is
		 * pressed. This can be used to trigger specific actions or events in
		 * response to the button press.
		 *
		 * @return A reference to the ButtonInteraction instance, allowing for
		 * method chaining.
		 */
		ButtonInteraction &
			setOnButtonPressHandler(const ButtonEnum &button,
									const ButtonPressHandler &handler);

		/**
		 * @brief Retrieves the current handler function for a button press.
		 * @param button The button for which to get the press handler. Must be
		 * one of the values in ButtonsArray.
		 * @return The current button press handler function for the specified
		 * button, which will be called when that button is pressed.
		 */
		const ButtonPressHandler &
			getOnButtonPressHandler(const ButtonEnum &button) const;

		/**
		 * @brief Sets the handler function to be called when a button is
		 * released.
		 * @param button The button for which to set the release handler. Must
		 * be one of the values in ButtonsArray.
		 * @param handler The function to be called when the specified button is
		 * released. This can be used to trigger specific actions or events in
		 * response to the button release.
		 * @return A reference to the ButtonInteraction instance, allowing for
		 * method chaining.
		 */
		ButtonInteraction &
			setOnButtonReleaseHandler(const ButtonEnum &button,
									  const ButtonReleaseHandler &handler);

		/**
		 * @brief Retrieves the current handler function for a button release.
		 * @param button The button for which to get the release handler. Must
		 * be one of the values in ButtonsArray.
		 * @return The current button release handler function for the specified
		 * button, which will be called when that button is released.
		 */
		const ButtonReleaseHandler &
			getOnButtonReleaseHandler(const ButtonEnum &button) const;

		/**
		 * @brief Checks if a button is currently pressed.
		 * @param button The button to check. Must be one of the values in
		 * ButtonsArray.
		 * @return True if the specified button is currently pressed, false
		 * otherwise.
		 */
		bool isButtonPressed(const ButtonEnum &button) const;

		/**
		 * @brief Sets the pressed state of a button.
		 * @param button The button for which to set the pressed state. Must be
		 * one of the values in ButtonsArray.
		 * @param pressed The new pressed state to set for the specified button.
		 * @return A reference to the ButtonInteraction instance, allowing for
		 * method chaining.
		 */
		ButtonInteraction &setButtonPressed(const ButtonEnum &button,
											bool pressed);
	};

}	 // namespace guillaume::components

// Include the implementation of the ButtonInteraction template class
#include "guillaume/components/button_interaction.tpp"
