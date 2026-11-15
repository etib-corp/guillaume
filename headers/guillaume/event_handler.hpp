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
	 * @brief Interface of an event source: something that reads platform
	 * input and reports it as `utility::event::Event` objects through a
	 * callback.
	 *
	 * An implementation overrides pollEvents() to read what the platform
	 * has pending, convert it and call the callback once per event. The
	 * owner sets the callback, typically to forward to
	 * event::EventBus::publish.
	 *
	 * @code
	 * class MyEventHandler: public guillaume::EventHandler
	 * {
	 * 	public:
	 * 	void pollEvents(void) override
	 * 	{
	 * 		std::shared_ptr<utility::event::Event> event = readPlatform();
	 * 		if (event && getEventCallback()) {
	 * 			getEventCallback()(event);
	 * 		}
	 * 	}
	 * };
	 * @endcode
	 *
	 * @note Application does not use this class: it receives its events
	 * from `utility::Engine` (`setEventCallback` and `pollEvents`), which
	 * has the same shape.
	 * @see event::EventBus
	 */
	class EventHandler:
		protected utility::logging::Loggable<EventHandler,
											 utility::logging::DefaultLogger>
	{
		public:
		/**
		 * @brief The callback type: a function called with each event read
		 * from the platform.
		 *
		 * It receives the shared pointer by reference and may move from it.
		 */
		using Handler =
			std::function<void(std::shared_ptr<utility::event::Event> &)>;

		private:
		Handler _callback;	  ///< Called with each event; empty until set

		protected:
		/**
		 * @brief Get the callback, for pollEvents() to call.
		 * @return Reference to the callback. It is an empty function until
		 * setEventCallback() is called: test it before calling it.
		 */
		Handler &getEventCallback(void);

		public:
		/**
		 * @brief Create an event source with no callback.
		 */
		EventHandler(void);

		/**
		 * @brief Virtual destructor.
		 */
		virtual ~EventHandler(void) = default;

		/**
		 * @brief Choose the function that receives the events.
		 * @param callback The function pollEvents() calls once per event.
		 * It replaces the previous one; an empty function removes it.
		 */
		void setEventCallback(const Handler &callback);

		/**
		 * @brief Read the events the platform has pending and report each
		 * one through the callback.
		 *
		 * Implemented by each event source. It is meant to be called once
		 * per frame and to return without waiting when nothing is pending.
		 */
		virtual void pollEvents(void) = 0;
	};

	/**
	 * @brief Constrains a template parameter to a type derived from
	 * EventHandler.
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromEventHandler = std::is_base_of_v<EventHandler, Type>;

}	 // namespace guillaume
