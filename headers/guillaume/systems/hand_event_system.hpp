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

#include <concepts>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/event/event_bus.hpp"
#include "guillaume/event/event_manager.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Base of the XR hand input systems: the wiring they share.
	 *
	 * A hand input system pairs one hand event type with one interaction
	 * component. This base runs in the Event phase on entities that have
	 * that component, a Transform and a Bound, and takes one event of its
	 * type per frame. What is done with the event is left to update(),
	 * which does nothing here: systems::HandSqueeze, HandThumbRest,
	 * HandThumbStick and HandTrigger use the base as is and therefore only
	 * subscribe to their event.
	 *
	 * @code
	 * class HandSqueeze:
	 * 	public HandEventSystem<components::HandSqueezeInteraction,
	 * 						   utility::event::HandSqueezeEvent>
	 * {
	 * 	// constructor forwarding the event bus
	 * };
	 * @endcode
	 *
	 * @tparam InteractionType The interaction component an entity must have
	 * to be visited.
	 * @tparam EventType The hand event the system subscribes to.
	 * @see systems::HandRaySystem
	 */
	template<ecs::InheritFromComponent InteractionType,
			 utility::event::InheritFromEvent EventType>
	class HandEventSystem:
		public ecs::SystemFiller<InteractionType, components::Transform,
								 components::Bound>,
		public event::EventManager<EventType>
	{
		public:
		/**
		 * @brief Create the system, in the Event phase, and subscribe it to
		 * events of type `EventType`.
		 * @param eventBus The event bus delivering the hand events.
		 */
		explicit HandEventSystem(event::EventBus &eventBus);

		/**
		 * @brief Default destructor.
		 */
		~HandEventSystem(void) override = default;

		/**
		 * @brief Take the next pending event, once per frame.
		 *
		 * When none is pending, the previous event stays the current one.
		 */
		void prepare(void) override;

		/**
		 * @brief Does nothing.
		 */
		void cleanup(void) override;

		/**
		 * @brief React to the current event for one entity. Does nothing
		 * here; a derived system overrides it.
		 * @param entityIdentifier The entity holding the interaction
		 * component.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

	/**
	 * @brief Base of the hand input systems that point at an entity: a
	 * gesture counts for the entity the hand ray hits.
	 *
	 * For each visited entity, a ray is cast from the hand position of the
	 * current event along the forward axis of the hand pose and tested
	 * against the rectangle of the entity, grown by its accessibility
	 * margin (a disabled entity is never hit). The result is stored on the
	 * interaction component through setEngaged(), and on a hit the
	 * component's event handler is called. Nothing happens until a first
	 * event has been received.
	 *
	 * systems::HandPinch and systems::HandPoke derive from it and only say
	 * which flag setEngaged() writes.
	 *
	 * @tparam InteractionType The interaction component an entity must have
	 * to be visited. It must provide `getOnEventHandler()`.
	 * @tparam EventType The hand event the system subscribes to. It must
	 * provide the hand pose through `getPose()`.
	 * @see systems::hitTest
	 */
	template<ecs::InheritFromComponent InteractionType,
			 utility::event::InheritFromEvent EventType>
	class HandRaySystem: public HandEventSystem<InteractionType, EventType>
	{
		public:
		using HandEventSystem<InteractionType, EventType>::HandEventSystem;

		/**
		 * @brief Test the hand ray of the current event against one entity,
		 * store the result with setEngaged() and call the event handler of
		 * the interaction component on a hit.
		 * @param entityIdentifier The entity holding the interaction
		 * component.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		protected:
		/**
		 * @brief Write the result of the hit test on the interaction
		 * component. Called for every visited entity, hit or not.
		 * @param interaction The interaction component of the entity.
		 * @param engaged True when the hand ray of the current event hits
		 * the entity.
		 */
		virtual void setEngaged(InteractionType &interaction, bool engaged) = 0;
	};

}	 // namespace guillaume::systems

#include "guillaume/systems/hand_event_system.tpp"
