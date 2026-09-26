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

#include "guillaume/event/event_bus.hpp"
#include "guillaume/event/event_manager.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/scrollable.hpp"

#include <utility/event/mouse_wheel_event.hpp>

namespace guillaume::systems
{

	/**
	 * @brief System handling scrollable content from the mouse wheel.
	 *
	 * Consumes mouse wheel events, advances the `components::Scrollable`
	 * content offset by the wheel delta scaled by the scroll step, then clamps
	 * the offset to the entity `components::Bound` viewport.
	 *
	 * @see components::Scrollable
	 */
	class Scroll:
		public ecs::SystemFiller<components::Scrollable, components::Bound>,
		public event::EventManager<utility::event::MouseWheelEvent>
	{
		public:
		/**
		 * @brief Construct the Scroll system and subscribe to wheel events.
		 * @param eventBus The event bus to subscribe to.
		 */
		Scroll(event::EventBus &eventBus);

		/**
		 * @brief Default destructor for the Scroll system.
		 */
		~Scroll(void);

		/**
		 * @brief Consume the pending wheel event before the pass.
		 */
		void prepare(void) override;

		/**
		 * @brief Update the Scroll system for one entity.
		 * @param entityIdentifier The target entity identifier.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
