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

#include "guillaume/event/event_manager.hpp"

namespace guillaume::event
{
	template<utility::event::InheritFromEvent EventType>
	void EventManager<EventType>::consumeNextEvent(void)
	{
		if (_subscriber->hasPendingEvents()) {
			_lastEvent = _subscriber->getNextEvent();
		}
	}

	template<utility::event::InheritFromEvent EventType>
	std::shared_ptr<EventType> EventManager<EventType>::getLastEvent(void)
	{
		if (!_lastEvent) {
			return nullptr;
		}
		return _lastEvent;
	}

	template<utility::event::InheritFromEvent EventType>
	EventManager<EventType>::EventManager(EventBus &eventBus)
		: _eventBus(eventBus)
		, _subscriber(std::make_unique<EventSubscriber<EventType>>(_eventBus))
	{
	}

	template<utility::event::InheritFromEvent EventType>
	bool EventManager<EventType>::hasPendingEvents(void) const
	{
		return _subscriber->hasPendingEvents();
	}
}	 // namespace guillaume::event