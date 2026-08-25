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

#include <utility/event/mouse_button_event.hpp>

#include "guillaume/components/button_interaction.hpp"

namespace guillaume::components
{

	inline constexpr std::array<utility::event::MouseButtonEvent::Button, 5>
		MouseButtons = { utility::event::MouseButtonEvent::Button::Left,
						 utility::event::MouseButtonEvent::Button::Middle,
						 utility::event::MouseButtonEvent::Button::Right,
						 utility::event::MouseButtonEvent::Button::X1,
						 utility::event::MouseButtonEvent::Button::X2 };

	/**
	 * @brief Component handling mouse button interactions.
	 * @see systems::MouseButton
	 */
	using MouseButtonInteraction =
		ButtonInteraction<utility::event::MouseButtonEvent,
						  utility::event::MouseButtonEvent::Button,
						  MouseButtons.size(), MouseButtons>;

}	 // namespace guillaume::components
