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
#include "guillaume/components/drag_interaction.hpp"
#include "guillaume/components/hand_button_interaction.hpp"
#include "guillaume/components/mouse_button_interaction.hpp"
#include "guillaume/components/transform.hpp"

#include <utility/event/mouse_motion_event.hpp>

namespace guillaume::systems
{

	/**
	 * @brief Turns "button held while the pointer moves" into a drag gesture.
	 *
	 * Runs in the Event phase on entities that have a
	 * components::DragInteraction, Transform, Bound, MouseButtonInteraction
	 * and HandButtonInteraction. An entity is grabbed while its left mouse
	 * button or its hand button A is pressed (as recorded by
	 * systems::MouseButton and systems::HandButton, registered before this
	 * system) and it is not disabled. The system writes the dragging flag,
	 * the last position and the delta of components::DragInteraction, and
	 * calls its handlers:
	 * - drag start, on the first frame the entity is grabbed;
	 * - drag, on each following frame, with the pointer movement since the
	 *   previous frame;
	 * - drag end, when the button is released or the entity is disabled.
	 *
	 * The system does not move the entity: the drag handler decides what
	 * the movement does.
	 *
	 * @note Positions always come from the last mouse motion event, also
	 * when the grab comes from the hand button. Nothing happens until a
	 * first mouse motion event has been received.
	 *
	 * @see components::DragInteraction
	 * @see systems::MouseButton
	 */
	class Drag:
		public ecs::SystemFiller<components::DragInteraction,
								 components::Transform, components::Bound,
								 components::MouseButtonInteraction,
								 components::HandButtonInteraction>,
		public event::EventManager<utility::event::MouseMotionEvent>
	{
		public:
		/**
		 * @brief Create the system, in the Event phase, and subscribe it to
		 * mouse motion events.
		 * @param eventBus The event bus delivering the mouse motion events.
		 */
		Drag(event::EventBus &eventBus);

		/**
		 * @brief Destructor.
		 */
		~Drag(void);

		/**
		 * @brief Take the next pending mouse motion event, once per frame.
		 *
		 * When none is pending, the previous event stays the current one.
		 */
		void prepare(void) override;

		/**
		 * @brief Start, continue or end the drag of one entity.
		 *
		 * Calls the drag start, drag or drag end handler of its
		 * components::DragInteraction when the gesture reaches that step.
		 * @param entityIdentifier The entity holding the
		 * components::DragInteraction.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
