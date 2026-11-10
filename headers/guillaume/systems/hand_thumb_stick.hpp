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

#include "guillaume/systems/hand_event_system.hpp"

#include "guillaume/components/hand_thumb_stick_interaction.hpp"

#include <utility/event/hand_thumb_stick_event.hpp>

namespace guillaume::systems
{
	/**
	 * @brief System handling hand thumb-stick interactions.
	 */
	class HandThumbStick:
		public HandEventSystem<components::HandThumbStickInteraction,
							   utility::event::HandThumbStickEvent>
	{
		public:
		/**
		 * @brief Construct the HandThumbStick system and subscribe to hand
		 * thumb-stick events.
		 * @param eventBus Reference to the event bus for subscribing to events.
		 */
		explicit HandThumbStick(event::EventBus &eventBus);

		/**
		 * @brief Default destructor for the HandThumbStick system.
		 */
		~HandThumbStick(void) override;
	};

}	 // namespace guillaume::systems
