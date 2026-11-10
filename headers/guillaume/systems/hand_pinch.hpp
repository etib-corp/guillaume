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

#include "guillaume/components/hand_pinch_interaction.hpp"

#include <utility/event/hand_pinch_event.hpp>

namespace guillaume::systems
{
	/**
	 * @brief System handling hand pinch interactions.
	 */
	class HandPinch:
		public HandRaySystem<components::HandPinchInteraction,
							 utility::event::HandPinchEvent>
	{
		public:
		/**
		 * @brief Construct the HandPinch system and subscribe to hand
		 * pinch events.
		 * @param eventBus Reference to the event bus for subscribing to events.
		 */
		explicit HandPinch(event::EventBus &eventBus);

		/**
		 * @brief Default destructor for the HandPinch system.
		 */
		~HandPinch(void) override;

		protected:
		/**
		 * @brief Store the pinch state on the interaction component.
		 * @param interaction Interaction component of the entity.
		 * @param engaged True when the hand ray hits the entity.
		 */
		void setEngaged(components::HandPinchInteraction &interaction,
						bool engaged) override;
	};

}	 // namespace guillaume::systems
