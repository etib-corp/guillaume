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

#include <utility/event/hand_motion_event.hpp>
#include <utility/event/mouse_motion_event.hpp>

namespace guillaume::systems
{

	/**
	 * @brief Turns "button held while the pointer moves" into a drag gesture.
	 *
	 * Runs in the Event phase on entities that have a
	 * components::DragInteraction, a Transform and a Bound. An entity is
	 * grabbed while the left mouse button of its MouseButtonInteraction or
	 * the button A of its HandButtonInteraction is pressed (as recorded by
	 * systems::MouseButton and systems::HandButton, registered before this
	 * system; an entity needs only one of the two components) and it is
	 * not disabled. The system writes the dragging flag, the last position
	 * and the delta of components::DragInteraction, and calls its
	 * handlers:
	 * - drag start, on the first frame the entity is grabbed;
	 * - drag, on each following frame that brings a motion event, with the
	 *   pointer movement since the previous one;
	 * - drag end, on the frame the button is released or the entity is
	 *   disabled, whether the pointer moves or not.
	 *
	 * The pointer is the one that holds the entity:
	 * - for a mouse grab, the position of the last mouse motion event, in
	 *   view coordinates;
	 * - for a hand grab, the point where the aim ray of the last hand
	 *   motion event meets the plane of the entity, measured along the
	 *   right and up axes of the entity.
	 *
	 * The system does not move the entity: the drag handler decides what
	 * the movement does.
	 *
	 * @note A drag only starts once the position of its pointer is known,
	 * that is after a first motion event of that pointer.
	 *
	 * @see components::DragInteraction
	 * @see systems::MouseButton
	 */
	class Drag:
		public ecs::SystemFiller<components::DragInteraction,
								 components::Transform, components::Bound>,
		public event::EventManager<utility::event::MouseMotionEvent>,
		public event::EventManager<utility::event::HandMotionEvent>
	{
		public:
		/**
		 * @brief Create the system, in the Event phase, and subscribe it to
		 * mouse motion and hand motion events.
		 * @param eventBus The event bus delivering the motion events.
		 */
		Drag(event::EventBus &eventBus);

		/**
		 * @brief Destructor.
		 */
		~Drag(void);

		/**
		 * @brief Take the next pending mouse motion event and the next
		 * pending hand motion event, once per frame.
		 *
		 * When none is pending, the pointer keeps its last known position.
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
