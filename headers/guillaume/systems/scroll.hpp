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
	 * @brief Scrolls content with the mouse wheel.
	 *
	 * Runs in the Event phase on entities that have a components::Scrollable
	 * and a components::Bound. It reads the wheel offset of the current
	 * mouse wheel event and the scroll step of the component, adds
	 * `offset * step` (per axis) to the content offset of
	 * components::Scrollable, then clamps the result with the Bound as the
	 * viewport size.
	 *
	 * @note The system does not test where the pointer is: every scrollable
	 * entity of the scene receives the wheel event.
	 * @note The current event is the last one received. It is not cleared
	 * when a frame brings no new wheel event, so its offset is applied
	 * again on that frame.
	 *
	 * @see components::Scrollable
	 */
	class Scroll:
		public ecs::SystemFiller<components::Scrollable, components::Bound>,
		public event::EventManager<utility::event::MouseWheelEvent>
	{
		public:
		/**
		 * @brief Create the system, in the Event phase, and subscribe it to
		 * mouse wheel events.
		 * @param eventBus The event bus delivering the mouse wheel events.
		 */
		Scroll(event::EventBus &eventBus);

		/**
		 * @brief Destructor.
		 */
		~Scroll(void);

		/**
		 * @brief Take the next pending mouse wheel event, once per frame.
		 *
		 * When none is pending, the previous event stays the current one.
		 */
		void prepare(void) override;

		/**
		 * @brief Scroll one entity by the current wheel event and clamp its
		 * content offset to its Bound.
		 * @param entityIdentifier The entity holding the
		 * components::Scrollable.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
