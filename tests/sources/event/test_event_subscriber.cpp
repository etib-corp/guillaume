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

#include "event/test_event_subscriber.hpp"

#include <memory>
#include <type_traits>

#include <utility/event/quit_event.hpp>

namespace guillaume::event::tests
{
	using QuitSubscriber = EventSubscriber<utility::event::QuitEvent>;

	TEST_F(TestEventSubscriber, QueuesPublishedEventsInOrder)
	{
		EventBus bus;
		QuitSubscriber subscriber(bus);
		const auto first  = std::make_shared<utility::event::QuitEvent>();
		const auto second = std::make_shared<utility::event::QuitEvent>();

		EXPECT_FALSE(subscriber.hasPendingEvents());
		bus.publish(first);
		bus.publish(second);

		EXPECT_TRUE(subscriber.hasPendingEvents());
		EXPECT_EQ(subscriber.getNextEvent(), first);
		EXPECT_EQ(subscriber.getNextEvent(), second);
		EXPECT_EQ(subscriber.getNextEvent(), nullptr);
	}

	// #232: a copy or a move would leave the listener of the bus pointing
	// at the original.
	TEST_F(TestEventSubscriber, IsNeitherCopyableNorMovable)
	{
		EXPECT_FALSE(std::is_copy_constructible_v<QuitSubscriber>);
		EXPECT_FALSE(std::is_copy_assignable_v<QuitSubscriber>);
		EXPECT_FALSE(std::is_move_constructible_v<QuitSubscriber>);
		EXPECT_FALSE(std::is_move_assignable_v<QuitSubscriber>);
	}

	// #232: the listener left on the bus must not touch a destroyed
	// subscriber. Before the fix this wrote to freed memory.
	TEST_F(TestEventSubscriber, PublishingAfterDestructionIsSafe)
	{
		EventBus bus;
		auto doomed = std::make_unique<QuitSubscriber>(bus);
		QuitSubscriber survivor(bus);
		const auto event = std::make_shared<utility::event::QuitEvent>();

		doomed.reset();
		bus.publish(event);

		// Only the survivor holds the event besides this test.
		EXPECT_EQ(event.use_count(), 2);
		EXPECT_EQ(survivor.getNextEvent(), event);
	}

	TEST_F(TestEventSubscriber, MayOutliveTheBus)
	{
		auto bus = std::make_unique<EventBus>();
		QuitSubscriber subscriber(*bus);
		bus->publish(std::make_shared<utility::event::QuitEvent>());

		bus.reset();

		EXPECT_TRUE(subscriber.hasPendingEvents());
		EXPECT_NE(subscriber.getNextEvent(), nullptr);
	}
}	 // namespace guillaume::event::tests