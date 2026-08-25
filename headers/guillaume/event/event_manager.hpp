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
	 * @brief Base class providing typed event subscription and last-event
	 * storage.
	 *
	 * EventManager manages one event subscription and stores the last received
	 * event for that type. Systems that need to respond to events can inherit
	 * from EventManager to automatically register a subscriber for a specific
	 * event type.
	 *
	 * @tparam EventType The event type to subscribe to. Must satisfy
	 * utility::event::InheritFromEvent constraint.
	 *
	 * @note This is a header-only template class. No instantiation of
	 * subscribers occurs until EventManager is instantiated by a derived class.
	 *
	 * @see EventSubscriber
	 * @see EventBus
	 * @see MouseMotion
	 * @see MouseButton
	 * @see HandMotion
	 * @see HandButton
	 * @see HandPinch
	 * @see HandPoke
	 *
	 * @code
	 * class MySystem : public EventManager<MouseButtonEvent>
	 * {
	 *   public:
	 *     MySystem(EventBus &eventBus) : EventManager(eventBus) { }
	 *     void processEvents()
	 *     {
	 *         consumeNextEvent<MouseButtonEvent>();
	 *         auto *lastButtonEvent = getLastEvent<MouseButtonEvent>();
	 *         if (lastButtonEvent) {
	 *             // Process the last button event...
	 *         }
	 *     }
	 * };
	 * @endcode
	 */
	template<utility::event::InheritFromEvent EventType> class EventManager
	{
		private:
		EventBus &_eventBus;	///< Reference to the event bus for managing
								///< event subscriptions and dispatching

		std::shared_ptr<EventSubscriber<EventType>> _subscriber;

		std::shared_ptr<EventType>
			_lastEvent;	   ///< Stores the most recent event for the subscribed
						   ///< type

		protected:
		/**
		 * @brief Consume the next pending event of a given type and store it.
		 *
		 * This method processes only the next event in the queue for the
		 * specified type and stores it for retrieval. If no pending events
		 * exist, the previously stored event remains unchanged.
		 *

		 * @code
		 * void update()
		 * {
		 *     consumeNextEvent<MouseButtonEvent>();
		 *     auto *lastButton = getLastEvent();
		 *     if (lastButton) {
		 *         // Process button event...
		 *     }
		 * }
		 * @endcode
		 */
		void consumeNextEvent(void);

		/**
		 * @brief Retrieve the most recent event of a given type, if available.
		 *
		 * Returns a pointer to the last event stored for a type, or nullptr if
		 * no event has been stored yet. Call consumeNextEvent() only after
		 * using the event content.
		 *
		 * @tparam EventType The event type to retrieve.
		 * @return Pointer to the last stored event, or nullptr if none exists.
		 *
		 * @note The returned pointer remains valid until consumeNextEvent is
		 * called for the same type, or until EventManager is destroyed.
		 *

		 * @code
		 * auto *motionEvent = getLastEvent<MouseMotionEvent>();
		 * if (motionEvent) {
		 *     float x = motionEvent->getX();
		 *     float y = motionEvent->getY();
		 *     consumeNextEvent<MouseMotionEvent>();
		 * }
		 * @endcode
		 */
		std::shared_ptr<EventType> getLastEvent(void);

		public:
		/**
		 * @brief Construct the EventManager and register subscribers for all
		 * event types.
		 *
		 * The constructor initializes the subscriber tuple and registers each
		 * subscriber with the provided event bus. This must be called by
		 * derived classes in their initializer list.
		 *
		 * @param eventBus The event bus to subscribe to.
		 *

		 * @code
		 * class MySystem : public EventManager<MouseButtonEvent,
		 * MouseMotionEvent>
		 * {
		 *   public:
		 *     MySystem(EventBus &bus) : EventManager(bus) { }
		 * };
		 * @endcode
		 */
		EventManager(EventBus &eventBus);

		/**
		 * @brief Virtual destructor for proper cleanup of derived classes.
		 */
		virtual ~EventManager(void) = default;

		/**
		 * @brief Check if there are pending events.
		 * @return True if there are pending events, false otherwise.
		 */
		bool hasPendingEvents(void) const;
	};
}	 // namespace guillaume::event

// Include the implementation of the EventManager template class
#include "guillaume/event/event_manager.tpp"