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
#include "guillaume/components/mouse_button_interaction.hpp"
#include "guillaume/components/transform.hpp"

#include <utility/event/mouse_button_event.hpp>

namespace guillaume::systems
{
	/**
	 * @brief Tracks which mouse buttons are held on which entities.
	 *
	 * Runs in the Event phase on entities that have a
	 * components::MouseButtonInteraction, a Transform and a Bound. It turns
	 * the position of the current mouse button event into a world ray and
	 * tests it against the rectangle of the entity, grown by its
	 * accessibility margin (a disabled entity is never hit). Then, for the
	 * button of the event:
	 * - button down on the entity: the button is marked pressed and its
	 *   press handler is called;
	 * - button up on the entity while it was pressed there: the button is
	 *   marked released and its release handler is called. This is what a
	 *   widget treats as a click;
	 * - button up elsewhere: the button is marked released, no handler is
	 *   called, so dragging the pointer off a widget cancels the click.
	 *
	 * Every entity is tested on its own: overlapping entities under the
	 * pointer all receive the press. systems::Drag, systems::Focus and
	 * systems::StyleState read the pressed flags later in the same phase.
	 *
	 * @see components::MouseButtonInteraction
	 * @see systems::interaction::makeInteractive
	 * @see systems::hitTest
	 */
	class MouseButton:

		public ecs::SystemFiller<components::MouseButtonInteraction,
								 components::Transform, components::Bound>,
		public event::EventManager<utility::event::MouseButtonEvent>
	{
		private:
		utility::Engine &_engine;	 ///< Engine whose view converts a pointer
									 ///< position into a world ray

		public:
		/**
		 * @brief Create the system, in the Event phase, and subscribe it to
		 * mouse button events.
		 * @param eventBus The event bus delivering the mouse button events.
		 * @param engine The engine whose view converts the pointer position
		 * into a world ray. It must outlive the system.
		 */
		MouseButton(event::EventBus &eventBus, utility::Engine &engine);

		/**
		 * @brief Destructor.
		 */
		~MouseButton(void);

		/**
		 * @brief Take the next pending mouse button event, once per frame.
		 *
		 * When none is pending, the previous event stays the current one.
		 */
		void prepare(void) override;

		/**
		 * @brief Does nothing.
		 */
		void cleanup(void) override;

		/**
		 * @brief Apply the current mouse button event to one entity.
		 *
		 * Events with an unknown button are ignored. When the view cannot
		 * convert the pointer position (it throws `std::out_of_range`), a
		 * warning is logged and the entity is left as it was.
		 * @param entityIdentifier The entity holding the
		 * components::MouseButtonInteraction.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems