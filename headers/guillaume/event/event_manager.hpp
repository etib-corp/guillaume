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

#include <memory>
#include <type_traits>

#include <utility/event/event.hpp>

#include "guillaume/event/event_subscriber.hpp"

namespace guillaume::event
{
	/**
	 * @brief Base class that gives a system the input events of one type,
	 * one event per frame.
	 *
	 * The input systems (systems::MouseButton, systems::MouseMotion, the
	 * hand systems, ...) derive from both ecs::SystemFiller and
	 * EventManager. The manager subscribes to the bus and queues the events
	 * as the engine polls them. The system then:
	 * - calls consumeNextEvent() once in ecs::System::prepare, which moves
	 *   the oldest queued event into the "last event" slot;
	 * - reads that event with getLastEvent() in ecs::System::update, for
	 *   each entity it visits.
	 *
	 * @code
	 * class Clicks:
	 * 	public ecs::SystemFiller<components::Bound>,
	 * 	public event::EventManager<utility::event::MouseButtonEvent>
	 * {
	 * 	public:
	 * 	Clicks(event::EventBus &eventBus)
	 * 		: ecs::SystemFiller<components::Bound>(ecs::Phase::Event)
	 * 		, event::EventManager<utility::event::MouseButtonEvent>(
	 * 			  eventBus)
	 * 	{
	 * 	}
	 *
	 * 	void prepare(void) override
	 * 	{
	 * 		consumeNextEvent();
	 * 	}
	 *
	 * 	void update(const ecs::Entity::Identifier &identifier) override
	 * 	{
	 * 		auto event = getLastEvent();
	 * 		if (!event) {
	 * 			return;
	 * 		}
	 * 		// test the event against the entity
	 * 	}
	 * };
	 * @endcode
	 *
	 * Things to watch out for:
	 * - One call to consumeNextEvent() takes one event. When several events
	 *   of the type arrive in the same frame, the others wait for the next
	 *   frames.
	 * - The last event is kept when the queue is empty: getLastEvent()
	 *   returns the same event again on the following frames, until a new
	 *   one arrives.
	 * - To listen to several event types, derive from several EventManager
	 *   instantiations.
	 *
	 * @tparam EventType The exact event type to receive.
	 * @see EventSubscriber
	 * @see EventBus
	 */
	template<utility::event::InheritFromEvent EventType> class EventManager
	{
		private:
		EventBus &_eventBus;	///< The bus the manager is subscribed to

		std::shared_ptr<EventSubscriber<EventType>>
			_subscriber;	///< Queue of the events received and not
							///< consumed yet

		std::shared_ptr<EventType>
			_lastEvent;	   ///< The event taken by the last successful
						   ///< consumeNextEvent(); nullptr before the first

		protected:
		/**
		 * @brief Take the oldest queued event and make it the last event.
		 *
		 * When no event is queued, nothing changes: the last event stays the
		 * one of a previous call. Call it once per frame, in
		 * ecs::System::prepare.
		 */
		void consumeNextEvent(void);

		/**
		 * @brief Get the event taken by the most recent consumeNextEvent()
		 * that found one.
		 *
		 * The event is not removed: every call returns the same event until
		 * consumeNextEvent() replaces it.
		 * @return The last event, or nullptr when no event was ever
		 * consumed.
		 */
		std::shared_ptr<EventType> getLastEvent(void);

		public:
		/**
		 * @brief Subscribe to a bus; events of type `EventType` published
		 * after this call are queued.
		 * @param eventBus The bus to subscribe to, normally the
		 * application's. It must outlive the manager's use.
		 */
		EventManager(EventBus &eventBus);

		/**
		 * @brief Virtual destructor.
		 */
		virtual ~EventManager(void) = default;

		/**
		 * @brief Tell whether at least one event is queued, waiting for
		 * consumeNextEvent().
		 * @return True when the queue is not empty. The last event does not
		 * count.
		 */
		bool hasPendingEvents(void) const;
	};
}	 // namespace guillaume::event

// Include the implementation of the EventManager template class
#include "guillaume/event/event_manager.tpp"