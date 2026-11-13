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

#include <utility/engine.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/event/event_manager.hpp"
#include "guillaume/event/event_bus.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/mouse_hover_interaction.hpp"
#include "guillaume/components/transform.hpp"

#include <utility/event/mouse_motion_event.hpp>

namespace guillaume::systems
{
	/**
	 * @brief Tracks which entities the mouse pointer is over.
	 *
	 * Runs in the Event phase on entities that have a
	 * components::MouseHoverInteraction, a Transform and a Bound. It turns
	 * the position of the current mouse motion event into a world ray
	 * (through the engine's view) and tests it against the rectangle of the
	 * entity, grown by its accessibility margin. It writes the hovered flag
	 * of components::MouseHoverInteraction and calls its hover handler when
	 * the pointer enters and its unhover handler when it leaves. A disabled
	 * entity is never hovered.
	 *
	 * Every entity is tested on its own: when two entities overlap under
	 * the pointer, both are hovered. Nothing is tested until a first mouse
	 * motion event has been received.
	 *
	 * @see components::MouseHoverInteraction
	 * @see systems::hitTest
	 * @see systems::StyleState
	 */
	class MouseMotion:
		public ecs::SystemFiller<components::MouseHoverInteraction,
								 components::Transform, components::Bound>,
		public event::EventManager<utility::event::MouseMotionEvent>
	{
		private:
		utility::Engine &_engine;	 ///< Engine whose view converts a pointer
									 ///< position into a world ray

		public:
		/**
		 * @brief Create the system, in the Event phase, and subscribe it to
		 * mouse motion events.
		 * @param eventBus The event bus delivering the mouse motion events.
		 * @param engine The engine whose view converts the pointer position
		 * into a world ray. It must outlive the system.
		 */
		MouseMotion(event::EventBus &eventBus, utility::Engine &engine);

		/**
		 * @brief Destructor.
		 */
		~MouseMotion(void);

		/**
		 * @brief Take the next pending mouse motion event, once per frame.
		 *
		 * When none is pending, the previous event stays the current one.
		 */
		void prepare(void) override;

		/**
		 * @brief Does nothing.
		 */
		void cleanup(void) override;

		/**
		 * @brief Test the pointer against one entity and update its hovered
		 * flag.
		 *
		 * Calls the hover or unhover handler when the flag changes. When
		 * the view cannot convert the pointer position (it throws
		 * `std::out_of_range`), a warning is logged and the entity is left
		 * as it was.
		 * @param entityIdentifier The entity holding the
		 * components::MouseHoverInteraction.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems