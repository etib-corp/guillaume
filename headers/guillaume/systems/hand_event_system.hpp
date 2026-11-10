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
	 * @brief Shared base of the hand input systems.
	 *
	 * Every hand input system matches entities carrying its interaction
	 * component plus a Transform and a Bound, runs in the Event phase and
	 * consumes one event of its type per frame. This base holds that common
	 * wiring; the per-entity reaction is left to update(), which does nothing
	 * by default.
	 * @tparam InteractionType Interaction component the system reacts on.
	 * @tparam EventType Hand event consumed by the system.
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
		 * @brief Construct the system and subscribe to its hand event.
		 * @param eventBus Reference to the event bus for subscribing to events.
		 */
		explicit HandEventSystem(event::EventBus &eventBus);

		/**
		 * @brief Default destructor.
		 */
		~HandEventSystem(void) override = default;

		/**
		 * @brief Consume the next pending event before entities are updated.
		 */
		void prepare(void) override;

		/**
		 * @brief Called after every entity was updated. Does nothing.
		 */
		void cleanup(void) override;

		/**
		 * @brief Update one matching entity. Does nothing by default.
		 * @param entityIdentifier Identifier of the entity being updated.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

	/**
	 * @brief Hand input system that hit-tests the hand ray against the entity.
	 *
	 * The event type must expose the hand pose through getPose(). When the
	 * ray hits an interactive entity the interaction is engaged and its event
	 * handler is invoked; otherwise the interaction is released.
	 * @tparam InteractionType Interaction component the system reacts on.
	 * @tparam EventType Hand event consumed by the system.
	 */
	template<ecs::InheritFromComponent InteractionType,
			 utility::event::InheritFromEvent EventType>
	class HandRaySystem: public HandEventSystem<InteractionType, EventType>
	{
		public:
		using HandEventSystem<InteractionType, EventType>::HandEventSystem;

		/**
		 * @brief Hit-test the entity against the hand ray of the last event.
		 * @param entityIdentifier Identifier of the entity being updated.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		protected:
		/**
		 * @brief Store the engaged state on the interaction component.
		 * @param interaction Interaction component of the entity.
		 * @param engaged True when the hand ray hits the entity.
		 */
		virtual void setEngaged(InteractionType &interaction, bool engaged) = 0;
	};

}	 // namespace guillaume::systems

#include "guillaume/systems/hand_event_system.tpp"
