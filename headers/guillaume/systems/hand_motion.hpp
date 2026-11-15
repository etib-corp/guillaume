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
	 * @brief Tracks which entities an XR hand is pointing at.
	 *
	 * The hand counterpart of systems::MouseMotion. Runs in the Event phase
	 * on entities that have a components::HandHoverInteraction, a Transform
	 * and a Bound. It casts the forward ray of the aim pose of the current
	 * hand motion event and tests it against the rectangle of the entity,
	 * grown by its accessibility margin. It writes the hovered flag of
	 * components::HandHoverInteraction and calls its hover handler when the
	 * ray enters and its unhover handler when it leaves. A disabled entity
	 * is never hovered.
	 *
	 * Every entity is tested on its own: overlapping entities on the ray
	 * are all hovered. Nothing is tested until a first hand motion event
	 * has been received.
	 *
	 * @see components::HandHoverInteraction
	 * @see systems::MouseMotion
	 * @see systems::hitTest
	 */
	class HandMotion:
		public ecs::SystemFiller<components::HandHoverInteraction,
								 components::Transform, components::Bound>,
		public event::EventManager<utility::event::HandMotionEvent>
	{
		public:
		/**
		 * @brief Create the system, in the Event phase, and subscribe it to
		 * hand motion events.
		 * @param eventBus The event bus delivering the hand motion events.
		 */
		HandMotion(event::EventBus &eventBus);

		/**
		 * @brief Destructor.
		 */
		~HandMotion(void);

		/**
		 * @brief Take the next pending hand motion event, once per frame.
		 *
		 * When none is pending, the previous event stays the current one.
		 */
		void prepare(void) override;

		/**
		 * @brief Does nothing.
		 */
		void cleanup(void) override;

		/**
		 * @brief Test the hand ray against one entity and update its
		 * hovered flag, calling the hover or unhover handler when the flag
		 * changes.
		 * @param entityIdentifier The entity holding the
		 * components::HandHoverInteraction.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems