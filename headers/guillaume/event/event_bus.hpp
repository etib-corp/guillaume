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

#include <functional>
#include <map>
#include <memory>
#include <typeindex>
#include <vector>

#include <utility/event/event.hpp>

namespace guillaume::event
{

	/**
	 * @brief Hands each engine event to the listeners subscribed to its
	 * type.
	 *
	 * Application owns one bus and passes to publish() every input event
	 * the engine polls at the start of a frame; the listeners are called at
	 * once, in subscription order. The systems do not subscribe directly:
	 * they use EventManager (or EventSubscriber), whose listener only
	 * queues the event so the system can process it later in the frame,
	 * during its phase.
	 *
	 * @code
	 * event::EventBus bus;
	 * bus.subscribe<utility::event::KeyboardEvent>(
	 * 	[](std::shared_ptr<utility::event::Event> event) {
	 * 		auto keyboardEvent = std::dynamic_pointer_cast<
	 * 			utility::event::KeyboardEvent>(event);
	 * 	});
	 * bus.publish(event);	  // a std::shared_ptr<utility::event::Event>
	 * @endcode
	 *
	 * Things to watch out for:
	 * - The match is on the exact type of the event object: a listener
	 *   subscribed to a base event type does not receive derived events.
	 * - A listener cannot be removed. Whatever it captures must stay alive
	 *   as long as events are published.
	 *
	 * @see EventSubscriber
	 * @see EventManager
	 */
	class EventBus
	{
		public:
		using Listener = std::function<void(
			std::shared_ptr<utility::event::Event>)>;	 ///< Function called
														 ///< with each event
														 ///< of the subscribed
														 ///< type
		using ListenerList =
			std::vector<Listener>;	  ///< Listeners of one event type, in
									  ///< subscription order

		private:
		std::map<std::type_index, ListenerList>
			_typedListeners;	///< Listeners, by exact event type

		/**
		 * @brief Call every listener of a list with an event, in order.
		 * @param event The event handed to each listener.
		 * @param listeners The listeners to call. Empty functions are
		 * skipped.
		 */
		void dispatchToListeners(std::shared_ptr<utility::event::Event> event,
								 ListenerList &listeners);

		public:
		/**
		 * @brief Create a bus with no listener.
		 */
		EventBus(void) = default;

		/**
		 * @brief Destroy the bus and its listeners.
		 */
		~EventBus(void) = default;

		/**
		 * @brief Call, before returning, every listener subscribed to the
		 * exact type of an event.
		 *
		 * Nothing happens when the event is null or when its type has no
		 * listener. An exception thrown by a listener propagates and the
		 * remaining listeners are not called.
		 * @param event The event to deliver. It is shared with the
		 * listeners, which may keep it.
		 */
		void publish(std::shared_ptr<utility::event::Event> event);

		/**
		 * @brief Add a listener for one event type.
		 * @tparam EventType The exact event type to listen to.
		 * @param listener The function to call with each published event of
		 * that type. It receives the event as a base `Event` pointer and
		 * casts it itself.
		 */
		template<utility::event::InheritFromEvent EventType>
		void subscribe(Listener listener);
	};

}	 // namespace guillaume::event

// Include the EventBus template implementation
#include "guillaume/event/event_bus.tpp"