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

#include "guillaume/components/button_interaction.hpp"

namespace guillaume::components
{

	template<utility::event::InheritFromEvent EventType, IsEnum ButtonEnum,
			 std::size_t N, const std::array<ButtonEnum, N> &ButtonsArray>
	ButtonInteraction<EventType, ButtonEnum, N,
					  ButtonsArray>::ButtonInteraction(void)
		: ecs::Component()
	{
		for (const auto button: ButtonsArray) {
			_isButtonPressed[button]		 = false;
			_onButtonPressHandlers[button]	 = nullptr;
			_onButtonReleaseHandlers[button] = nullptr;
		}
	}

	template<utility::event::InheritFromEvent EventType, IsEnum ButtonEnum,
			 std::size_t N, const std::array<ButtonEnum, N> &ButtonsArray>
	ButtonInteraction<EventType, ButtonEnum, N, ButtonsArray> &
		ButtonInteraction<EventType, ButtonEnum, N, ButtonsArray>::
			setOnButtonPressHandler(const ButtonEnum &button,
									const ButtonPressHandler &handler)
	{
		_onButtonPressHandlers[button] = handler;
		setHasChanged(true);
		return *this;
	}

	template<utility::event::InheritFromEvent EventType, IsEnum ButtonEnum,
			 std::size_t N, const std::array<ButtonEnum, N> &ButtonsArray>
	const typename ButtonInteraction<EventType, ButtonEnum, N,
									 ButtonsArray>::ButtonPressHandler &
		ButtonInteraction<EventType, ButtonEnum, N, ButtonsArray>::
			getOnButtonPressHandler(const ButtonEnum &button) const
	{
		auto it = _onButtonPressHandlers.find(button);
		if (it == _onButtonPressHandlers.end()) {
			throw std::invalid_argument("Invalid button");
		}
		return it->second;
	}

	template<utility::event::InheritFromEvent EventType, IsEnum ButtonEnum,
			 std::size_t N, const std::array<ButtonEnum, N> &ButtonsArray>
	ButtonInteraction<EventType, ButtonEnum, N, ButtonsArray> &
		ButtonInteraction<EventType, ButtonEnum, N, ButtonsArray>::
			setOnButtonReleaseHandler(const ButtonEnum &button,
									  const ButtonReleaseHandler &handler)
	{
		_onButtonReleaseHandlers[button] = handler;
		setHasChanged(true);
		return *this;
	}

	template<utility::event::InheritFromEvent EventType, IsEnum ButtonEnum,
			 std::size_t N, const std::array<ButtonEnum, N> &ButtonsArray>
	const typename ButtonInteraction<EventType, ButtonEnum, N,
									 ButtonsArray>::ButtonReleaseHandler &
		ButtonInteraction<EventType, ButtonEnum, N, ButtonsArray>::
			getOnButtonReleaseHandler(const ButtonEnum &button) const
	{
		auto it = _onButtonReleaseHandlers.find(button);
		if (it == _onButtonReleaseHandlers.end()) {
			throw std::invalid_argument("Invalid button");
		}
		return it->second;
	}

	template<utility::event::InheritFromEvent EventType, IsEnum ButtonEnum,
			 std::size_t N, const std::array<ButtonEnum, N> &ButtonsArray>
	bool ButtonInteraction<EventType, ButtonEnum, N, ButtonsArray>::
		isButtonPressed(const ButtonEnum &button) const
	{
		auto it = _isButtonPressed.find(button);
		if (it == _isButtonPressed.end()) {
			return false;
		}
		return it->second;
	}

	template<utility::event::InheritFromEvent EventType, IsEnum ButtonEnum,
			 std::size_t N, const std::array<ButtonEnum, N> &ButtonsArray>
	ButtonInteraction<EventType, ButtonEnum, N, ButtonsArray> &
		ButtonInteraction<EventType, ButtonEnum, N, ButtonsArray>::
			setButtonPressed(const ButtonEnum &button, bool pressed)
	{
		if (_isButtonPressed[button] == pressed) {
			return *this;
		}
		_isButtonPressed[button] = pressed;
		setHasChanged(true);
		return *this;
	}

}	 // namespace guillaume::components
