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
	 * @brief True when a type is an enumeration.
	 *
	 * Constrains the button type of ButtonInteraction.
	 *
	 * @tparam T The type to check.
	 */
	template<typename T>
	concept IsEnum = std::is_enum_v<T>;

	/**
	 * @brief Which buttons of an input device are held on an entity, and what
	 * to call when one is pressed or released on it.
	 *
	 * It is used through its two aliases, MouseButtonInteraction and
	 * HandButtonInteraction. The system of the device (systems::MouseButton,
	 * systems::HandButton) writes it in the Event phase, when a button event
	 * arrives:
	 * - a press whose ray hits the entity sets the button pressed and calls
	 *   the press handler;
	 * - a release that hits the entity while the button is pressed clears it
	 *   and calls the release handler: this is the "click";
	 * - a release anywhere else clears the button and calls nothing.
	 *
	 * systems::StyleState, systems::Focus and systems::Drag read the pressed
	 * state of the primary button (left mouse button, hand button A).
	 *
	 * @code
	 * using Button = utility::event::MouseButtonEvent::Button;
	 * registry.getComponent<components::MouseButtonInteraction>(identifier)
	 * 	.setOnButtonReleaseHandler(Button::Left, [] { save(); });
	 * @endcode
	 *
	 * Widgets wire the primary button with
	 * systems::interaction::makeInteractive, which replaces the press and
	 * release handlers of that button.
	 *
	 * @tparam EventType The button event of the device. It only makes the
	 * mouse and hand components distinct types.
	 * @tparam ButtonEnum The enumeration naming the buttons of the device.
	 * @tparam N Number of buttons in `ButtonsArray`.
	 * @tparam ButtonsArray The buttons the component tracks. Each one starts
	 * released and without handler.
	 * @see MouseButtonInteraction, HandButtonInteraction, StyleState
	 */
	template<utility::event::InheritFromEvent EventType, IsEnum ButtonEnum,
			 std::size_t N, const std::array<ButtonEnum, N> &ButtonsArray>
	class ButtonInteraction: public ecs::Component
	{
		public:
		/**
		 * @brief Callback run when a button is pressed on the entity.
		 */
		using ButtonPressHandler = std::function<void(void)>;
		/**
		 * @brief Callback run when a button pressed on the entity is released
		 * on it.
		 */
		using ButtonReleaseHandler = std::function<void(void)>;

		private:
		std::map<ButtonEnum, ButtonPressHandler>
			_onButtonPressHandlers {};	  ///< Press handler of each button
		std::map<ButtonEnum, ButtonReleaseHandler>
			_onButtonReleaseHandlers {};	///< Release handler of each button
		std::map<ButtonEnum, bool>
			_isButtonPressed {};	///< Whether each button is held on the
									///< entity

		public:
		/**
		 * @brief Construct the component with every button of `ButtonsArray`
		 * released and without handler.
		 */
		ButtonInteraction(void);

		/**
		 * @brief Default destructor.
		 */
		~ButtonInteraction(void) = default;

		/**
		 * @brief Store the callback run when a button is pressed on the
		 * entity.
		 *
		 * Always marks the component changed.
		 *
		 * @param button The button the handler is for.
		 * @param handler The callback, or `nullptr` to remove the current one.
		 * @return This component, to chain calls.
		 */
		ButtonInteraction &
			setOnButtonPressHandler(const ButtonEnum &button,
									const ButtonPressHandler &handler);

		/**
		 * @brief Get the callback run when a button is pressed on the entity.
		 * @param button The button to look up.
		 * @return The handler; empty (`nullptr`) when none was set.
		 * @throws std::invalid_argument If @p button is neither in
		 * `ButtonsArray` nor a button a handler was set for.
		 */
		const ButtonPressHandler &
			getOnButtonPressHandler(const ButtonEnum &button) const;

		/**
		 * @brief Store the callback run when a button pressed on the entity is
		 * released on it.
		 *
		 * Always marks the component changed.
		 *
		 * @param button The button the handler is for.
		 * @param handler The callback, or `nullptr` to remove the current one.
		 * @return This component, to chain calls.
		 */
		ButtonInteraction &
			setOnButtonReleaseHandler(const ButtonEnum &button,
									  const ButtonReleaseHandler &handler);

		/**
		 * @brief Get the callback run when a button pressed on the entity is
		 * released on it.
		 * @param button The button to look up.
		 * @return The handler; empty (`nullptr`) when none was set.
		 * @throws std::invalid_argument If @p button is neither in
		 * `ButtonsArray` nor a button a handler was set for.
		 */
		const ButtonReleaseHandler &
			getOnButtonReleaseHandler(const ButtonEnum &button) const;

		/**
		 * @brief Check whether a button is held on the entity.
		 * @param button The button to check.
		 * @return True from the press on the entity until the release; false
		 * for a button the component does not track.
		 */
		bool isButtonPressed(const ButtonEnum &button) const;

		/**
		 * @brief Record whether a button is held on the entity.
		 *
		 * Called by the input system of the device; it calls no handler. Marks
		 * the component changed only when the state differs.
		 *
		 * @param button The button to update.
		 * @param pressed True when the button is held on the entity.
		 * @return This component, to chain calls.
		 */
		ButtonInteraction &setButtonPressed(const ButtonEnum &button,
											bool pressed);
	};

}	 // namespace guillaume::components

// Include the implementation of the ButtonInteraction template class
#include "guillaume/components/button_interaction.tpp"
