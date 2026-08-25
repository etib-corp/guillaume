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
#include <memory>

#include <utility/logging/loggable.hpp>
#include <utility/logging/default_logger.hpp>

#include <utility/event/event.hpp>

namespace guillaume
{

	/**
	 * @brief Interface for event handlers.
	 *
	 * This class provides an abstract interface for handling events in the
	 * Guillaume framework. Implementations should poll or process
	 * platform-specific events and convert them to Guillaume Event objects.
	 *
	 * @code
	 * class MyEventHandler : public event::EventHandler {
	 *   public:
	 *     void pollEvents(void) override {
	 *         // Convert platform events to utility::event::Event and call
	 * callback.
	 *         // getEventCallback()(event);
	 *     }
	 * };
	 * @endcode
	 *
	 * @see EventBus
	 */
	class EventHandler:
		protected utility::logging::Loggable<EventHandler,
											 utility::logging::DefaultLogger>
	{
		public:
		/**
		 * @brief Type alias for an event handler callback function.
		 *
		 * Takes a unique pointer to an Event and performs an action.
		 */
		using Handler =
			std::function<void(std::shared_ptr<utility::event::Event> &)>;

		private:
		Handler _callback;	  ///< Event callback function

		protected:
		/**
		 * @brief Get the current event callback function.
		 * @return Reference to the event callback function.
		 */
		Handler &getEventCallback(void);

		public:
		/**
		 * @brief Default constructor
		 */
		EventHandler(void);

		/**
		 * @brief Default destructor
		 */
		virtual ~EventHandler(void) = default;

		/**
		 * @brief Set the event callback function.
		 *
		 * The callback will be invoked for each event when pollEvents() is
		 * called.
		 *
		 * @param callback Function to call when an event is received.
		 */
		void setEventCallback(const Handler &callback);

		/**
		 * @brief Poll for events and dispatch them.
		 *
		 * This method should check for pending events from the underlying
		 * platform, convert them to Event objects, and call the registered
		 * callback for each event.
		 */
		virtual void pollEvents(void) = 0;
	};

	/**
	 * @brief Concept to ensure a type inherits from EventHandler.
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromEventHandler = std::is_base_of_v<EventHandler, Type>;

}	 // namespace guillaume
