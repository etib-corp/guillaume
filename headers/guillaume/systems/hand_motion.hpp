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

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/event/event_manager.hpp"
#include "guillaume/event/event_bus.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/hand_hover_interaction.hpp"
#include "guillaume/components/transform.hpp"

#include <utility/event/hand_motion_event.hpp>

namespace guillaume::systems
{
	/**
	 * @brief System handling hand motion interactions.
	 */
	class HandMotion:
		public ecs::SystemFiller<components::HandHoverInteraction,
								 components::Transform, components::Bound>,
		public event::EventManager<utility::event::HandMotionEvent>
	{
		public:
		/**
		 * @brief Construct the HandMotion system and subscribe to hand motion
		 * events.
		 * @param eventBus Reference to the event bus for subscribing to events.
		 */
		HandMotion(event::EventBus &eventBus);

		/**
		 * @brief Default destructor for the HandMotion system.
		 */
		~HandMotion(void);

		/**
		 * @brief This function is called before call update on all entities and
		 * can be used to set up any necessary state or resources.
		 */
		void prepare(void) override;

		/**
		 * @brief This function is called after call update on all entities and
		 * can be used to release any resources or reset state.
		 */
		void cleanup(void) override;

		/**
		 * @brief Update method called for each entity with the relevant
		 * components. Processes hand motion events and updates interaction
		 * states accordingly.
		 * @param entityIdentifier Identifier of the entity being updated.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems