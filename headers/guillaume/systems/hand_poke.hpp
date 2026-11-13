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

#include "guillaume/components/hand_poke_interaction.hpp"

#include <utility/event/hand_poke_event.hpp>

namespace guillaume::systems
{
	/**
	 * @brief Marks the entities an XR hand pokes (a fingertip pushed
	 * toward them).
	 *
	 * Runs in the Event phase on entities that have a
	 * components::HandPokeInteraction, a Transform and a Bound. The hit
	 * test and the handler call are those of systems::HandRaySystem; this
	 * class only says that the result is written to the poking flag of the
	 * component. An entity is therefore poked while the hand ray of the
	 * current poke event hits it, and its event handler is called on every
	 * frame that is true.
	 *
	 * systems::Focus and systems::StyleState read the flag later in the
	 * same phase: a poke focuses an entity and shows it pressed.
	 *
	 * @see components::HandPokeInteraction
	 * @see systems::HandRaySystem
	 */
	class HandPoke:
		public HandRaySystem<components::HandPokeInteraction,
							 utility::event::HandPokeEvent>
	{
		public:
		/**
		 * @brief Create the system, in the Event phase, and subscribe it to
		 * hand poke events.
		 * @param eventBus The event bus delivering the hand poke events.
		 */
		explicit HandPoke(event::EventBus &eventBus);

		/**
		 * @brief Destructor.
		 */
		~HandPoke(void) override;

		protected:
		/**
		 * @brief Write the poking flag of the interaction component.
		 * @param interaction The poke interaction component of the entity.
		 * @param engaged True when the hand ray of the current poke event
		 * hits the entity.
		 */
		void setEngaged(components::HandPokeInteraction &interaction,
						bool engaged) override;
	};

}	 // namespace guillaume::systems
