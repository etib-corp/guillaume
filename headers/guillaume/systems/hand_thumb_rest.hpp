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

#include "guillaume/components/hand_thumb_rest_interaction.hpp"

#include <utility/event/hand_thumb_rest_event.hpp>

namespace guillaume::systems
{
	/**
	 * @brief Receives XR thumb-rest events (the thumb touching the rest
	 * pad of a controller).
	 *
	 * Runs in the Event phase on entities that have a
	 * components::HandThumbRestInteraction, a Transform and a Bound, and
	 * takes one thumb-rest event per frame. It does not override
	 * HandEventSystem::update, so it writes no component and calls no
	 * handler: the thumb-rest interaction component is not driven by this
	 * system.
	 *
	 * @see components::HandThumbRestInteraction
	 * @see systems::HandEventSystem
	 */
	class HandThumbRest:
		public HandEventSystem<components::HandThumbRestInteraction,
							   utility::event::HandThumbRestEvent>
	{
		public:
		/**
		 * @brief Create the system, in the Event phase, and subscribe it to
		 * hand thumb-rest events.
		 * @param eventBus The event bus delivering the thumb-rest events.
		 */
		explicit HandThumbRest(event::EventBus &eventBus);

		/**
		 * @brief Destructor.
		 */
		~HandThumbRest(void) override;
	};

}	 // namespace guillaume::systems
