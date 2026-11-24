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

#include "event/test_event_manager.hpp"

namespace guillaume::event::tests
{
	TEST_F(TestEventManager, HasNoEventBeforeTheFirstOne)
	{
		_manager.consumeNextEvent();

		EXPECT_EQ(_manager.getLastEvent(), nullptr);
		EXPECT_EQ(_manager.getRetainedEvent(), nullptr);
	}

	// #176: the last event is the one of the frame, it is not handed out
	// again on the next frames.
	TEST_F(TestEventManager, LastEventIsClearedOnAFrameWithoutEvent)
	{
		const auto event = publish();

		_manager.consumeNextEvent();
		EXPECT_EQ(_manager.getLastEvent(), event);

		_manager.consumeNextEvent();
		EXPECT_EQ(_manager.getLastEvent(), nullptr);
	}

	// #176: a state reader (pointer position) still finds the latest event.
	TEST_F(TestEventManager, RetainedEventSurvivesFramesWithoutEvent)
	{
		const auto event = publish();

		_manager.consumeNextEvent();
		_manager.consumeNextEvent();
		_manager.consumeNextEvent();

		EXPECT_EQ(_manager.getRetainedEvent(), event);
	}

	TEST_F(TestEventManager, TakesOneEventPerFrameInOrder)
	{
		const auto first  = publish();
		const auto second = publish();

		_manager.consumeNextEvent();
		EXPECT_EQ(_manager.getLastEvent(), first);
		EXPECT_TRUE(_manager.hasPendingEvents());

		_manager.consumeNextEvent();
		EXPECT_EQ(_manager.getLastEvent(), second);
		EXPECT_EQ(_manager.getRetainedEvent(), second);
		EXPECT_FALSE(_manager.hasPendingEvents());
	}
}	 // namespace guillaume::event::tests
