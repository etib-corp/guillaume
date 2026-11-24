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

#include <gtest/gtest.h>

#include <memory>

#include <utility/event/mouse_wheel_event.hpp>

#include <guillaume/event/event_bus.hpp>
#include <guillaume/event/event_manager.hpp>

namespace guillaume::event::tests
{

	/**
	 * @brief Opens the protected interface of EventManager to the tests.
	 */
	class EventManagerProbe:
		public EventManager<utility::event::MouseWheelEvent>
	{
		public:
		using EventManager<utility::event::MouseWheelEvent>::EventManager;
		using EventManager<utility::event::MouseWheelEvent>::consumeNextEvent;
		using EventManager<utility::event::MouseWheelEvent>::getLastEvent;
		using EventManager<utility::event::MouseWheelEvent>::getRetainedEvent;
	};

	class TestEventManager: public ::testing::Test
	{
		protected:
		EventBus _eventBus;	   ///< Bus the probe listens to.
		EventManagerProbe _manager { _eventBus };	 ///< Manager under test.

		TestEventManager(void)			 = default;
		~TestEventManager(void) override = default;

		/**
		 * @brief Publish a new wheel event.
		 * @return The event.
		 */
		std::shared_ptr<utility::event::MouseWheelEvent> publish(void)
		{
			auto event = std::make_shared<utility::event::MouseWheelEvent>();
			_eventBus.publish(event);
			return event;
		}
	};

}	 // namespace guillaume::event::tests
