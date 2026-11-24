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
#include <queue>

#include "guillaume/event/event_bus.hpp"
#include <utility/event/event.hpp>

namespace guillaume::event
{

	/**
	 * @brief A queue of the events of one type published on an EventBus,
	 * waiting to be read.
	 *
	 * The bus delivers events the moment the engine polls them. A subscriber
	 * stores them, in arrival order, until its owner is ready: Application
	 * reads its quit-event subscriber before each frame, and each input
	 * system reads its own (through EventManager) during the Event phase.
	 *
	 * @code
	 * event::EventSubscriber<utility::event::QuitEvent> quit(bus);
	 * ...
	 * while (quit.hasPendingEvents()) {
	 * 	auto event = quit.getNextEvent();
	 * }
	 * @endcode
	 *
	 * @note The bus has no way to drop a listener, so the listener of a
	 * subscriber does not point to the subscriber: it shares the queue and
	 * does nothing once the subscriber is destroyed. A subscriber may
	 * therefore be destroyed before or after the bus. It cannot be copied
	 * or moved: one subscription fills one queue.
	 * @tparam EventType The exact event type to queue.
	 * @see EventBus
	 * @see EventManager
	 */
	template<utility::event::InheritFromEvent EventType> class EventSubscriber
	{
		private:
		using Queue =
			std::queue<std::shared_ptr<EventType>>;	   ///< Events, oldest first

		std::shared_ptr<Queue>
			_eventQueue;	///< Events received and not read yet; shared
							///< with the listener registered on the bus,
							///< which only holds it weakly

		public:
		/**
		 * @brief Create an empty queue and subscribe it to a bus.
		 * @param eventBus The bus whose events of type `EventType` are
		 * queued, starting with the next one published.
		 */
		EventSubscriber(EventBus &eventBus);

		/**
		 * @brief Destroy the queue. The listener left on the bus stops
		 * queuing: see the note of the class.
		 */
		~EventSubscriber(void) = default;

		/**
		 * @brief Not copyable: the copy would share no subscription.
		 */
		EventSubscriber(const EventSubscriber &) = delete;

		/**
		 * @brief Not copy-assignable.
		 * @return Nothing: the operator is deleted.
		 */
		EventSubscriber &operator=(const EventSubscriber &) = delete;

		/**
		 * @brief Not movable: the queue stays with its subscription.
		 */
		EventSubscriber(EventSubscriber &&) = delete;

		/**
		 * @brief Not move-assignable.
		 * @return Nothing: the operator is deleted.
		 */
		EventSubscriber &operator=(EventSubscriber &&) = delete;

		/**
		 * @brief Tell whether at least one event is waiting.
		 * @return True when getNextEvent() would return an event.
		 */
		bool hasPendingEvents(void) const;

		/**
		 * @brief Take the oldest waiting event out of the queue.
		 * @return The event, or nullptr when the queue is empty.
		 */
		std::shared_ptr<EventType> getNextEvent(void);
	};

}	 // namespace guillaume::event

// Include the implementation of the EventSubscriber template class
#include "guillaume/event/event_subscriber.tpp"