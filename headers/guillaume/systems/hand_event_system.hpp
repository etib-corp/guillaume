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
	 * type per frame. On the frame an event is taken, update() casts a ray
	 * from the hand position of the event along the forward axis of the
	 * hand pose; for the entity it hits (a disabled entity is never hit),
	 * the state of the event is copied into the interaction component,
	 * which derives from the event type, and the event handler of the
	 * component is called. systems::HandSqueeze, HandThumbRest,
	 * HandThumbStick and HandTrigger use the base as is.
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
	 * to be visited. It must derive from `EventType` and provide
	 * `getOnEventHandler()`, as components::Interaction does.
	 * @tparam EventType The hand event the system subscribes to. It must
	 * provide the hand pose through `getPose()`.
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
		 * When none is pending, there is no current event this frame.
		 */
		void prepare(void) override;

		/**
		 * @brief Does nothing.
		 */
		void cleanup(void) override;

		/**
		 * @brief Give the event of this frame to one entity when the hand
		 * ray hits it: copy the state of the event into the interaction
		 * component and call its event handler.
		 *
		 * Nothing happens on a frame without event, or for an entity the
		 * ray misses.
		 * @param entityIdentifier The entity holding the interaction
		 * component.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		protected:
		/**
		 * @brief Tell whether the hand ray of an event hits an entity.
		 *
		 * The ray starts at the hand position and follows the forward axis
		 * of the hand pose. It is tested against the rectangle of the
		 * entity, grown by its accessibility margin.
		 * @param handEvent The event giving the hand pose.
		 * @param entityIdentifier The entity to test. It must have a
		 * Transform and a Bound.
		 * @return True on a hit. Always false for a disabled entity.
		 */
		bool isHit(const EventType &handEvent,
				   const ecs::Entity::Identifier &entityIdentifier);
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
	 * component's event handler is called. A gesture event counts for the
	 * frame it is taken in: on a frame without event, setEngaged() is
	 * called with false for every visited entity.
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
		 * @brief Test the hand ray of the event of this frame against one
		 * entity, store the result with setEngaged() and call the event
		 * handler of the interaction component on a hit. Without event this
		 * frame, the entity is disengaged.
		 * @param entityIdentifier The entity holding the interaction
		 * component.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		protected:
		/**
		 * @brief Write the result of the hit test on the interaction
		 * component. Called for every visited entity, hit or not.
		 * @param interaction The interaction component of the entity.
		 * @param engaged True when the hand ray of the event of this frame
		 * hits the entity; false when it misses or no event arrived.
		 */
		virtual void setEngaged(InteractionType &interaction, bool engaged) = 0;
	};

}	 // namespace guillaume::systems

#include "guillaume/systems/hand_event_system.tpp"
