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

#include "guillaume/components/hand_trigger_interaction.hpp"

#include <utility/event/hand_trigger_event.hpp>

namespace guillaume::systems
{
	/**
	 * @brief Receives XR trigger events (the index trigger of a
	 * controller).
	 *
	 * Runs in the Event phase on entities that have a
	 * components::HandTriggerInteraction, a Transform and a Bound, and
	 * takes one trigger event per frame. It does not override
	 * HandEventSystem::update, so it writes no component and calls no
	 * handler: the trigger interaction component is not driven by this
	 * system.
	 *
	 * @see components::HandTriggerInteraction
	 * @see systems::HandEventSystem
	 */
	class HandTrigger:
		public HandEventSystem<components::HandTriggerInteraction,
							   utility::event::HandTriggerEvent>
	{
		public:
		/**
		 * @brief Create the system, in the Event phase, and subscribe it to
		 * hand trigger events.
		 * @param eventBus The event bus delivering the hand trigger events.
		 */
		explicit HandTrigger(event::EventBus &eventBus);

		/**
		 * @brief Destructor.
		 */
		~HandTrigger(void) override;
	};

}	 // namespace guillaume::systems
