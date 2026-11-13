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
	 * @note The bus keeps a pointer to the subscriber and has no way to
	 * drop it: a subscriber must not be copied or moved, and must live at
	 * least as long as events are published on the bus.
	 * @tparam EventType The exact event type to queue.
	 * @see EventBus
	 * @see EventManager
	 */
	template<utility::event::InheritFromEvent EventType> class EventSubscriber
	{
		private:
		std::queue<std::shared_ptr<EventType>>
			_eventQueue;	///< Events received and not read yet, oldest
							///< first

		public:
		/**
		 * @brief Create an empty queue and subscribe it to a bus.
		 * @param eventBus The bus whose events of type `EventType` are
		 * queued, starting with the next one published.
		 */
		EventSubscriber(EventBus &eventBus);

		/**
		 * @brief Destroy the queue. The bus is not told: see the note of
		 * the class.
		 */
		~EventSubscriber(void) = default;

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