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
#include "guillaume/components/hand_button_interaction.hpp"
#include "guillaume/components/transform.hpp"

#include <utility/event/hand_button_event.hpp>

namespace guillaume::systems
{
	/**
	 * @brief Tracks which XR controller buttons are held on which entities.
	 *
	 * The hand counterpart of systems::MouseButton. Runs in the Event phase
	 * on entities that have a components::HandButtonInteraction, a
	 * Transform and a Bound. It casts the forward ray of the aim pose of
	 * the current hand button event and tests it against the rectangle of
	 * the entity, grown by its accessibility margin (a disabled entity is
	 * never hit). Then, for the button of the event:
	 * - button down on the entity: the button is marked pressed and its
	 *   press handler is called;
	 * - button up on the entity while it was pressed there: the button is
	 *   marked released and its release handler is called. With button A,
	 *   this is what a widget treats as a click;
	 * - button up elsewhere: the button is marked released and no handler
	 *   is called.
	 *
	 * systems::Drag, systems::Focus and systems::StyleState read the
	 * pressed flags later in the same phase.
	 *
	 * @see components::HandButtonInteraction
	 * @see systems::MouseButton
	 * @see systems::interaction::makeInteractive
	 */
	class HandButton:

		public ecs::SystemFiller<components::HandButtonInteraction,
								 components::Transform, components::Bound>,
		public event::EventManager<utility::event::HandButtonEvent>
	{
		public:
		/**
		 * @brief Create the system, in the Event phase, and subscribe it to
		 * hand button events.
		 * @param eventBus The event bus delivering the hand button events.
		 */
		HandButton(event::EventBus &eventBus);

		/**
		 * @brief Destructor.
		 */
		~HandButton(void);

		/**
		 * @brief Take the next pending hand button event, once per frame.
		 *
		 * When none is pending, the previous event stays the current one.
		 */
		void prepare(void) override;

		/**
		 * @brief Does nothing.
		 */
		void cleanup(void) override;

		/**
		 * @brief Apply the current hand button event to one entity. Events
		 * with an unknown button are ignored.
		 * @param entityIdentifier The entity holding the
		 * components::HandButtonInteraction.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems