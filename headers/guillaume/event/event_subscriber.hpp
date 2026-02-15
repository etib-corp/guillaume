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

namespace guillaume::event {

/**
 * @brief Base class for subscribing to events from the event bus.
 * @tparam EventType The type of event to subscribe to.
 *
 * This class provides a base for subscribing to specific event types from the
 * event bus. It maintains a queue of received events for processing.
 * @see EventBus
 */
template <utility::event::InheritFromEvent EventType> class EventSubscriber {
  private:
    std::queue<std::unique_ptr<EventType>>
        _eventQueue; ///< Queue of received events

  public:
    /**
     * @brief Construct an event subscriber and register it to the event bus.
     * @param eventBus The event bus to subscribe to.
     */
    EventSubscriber(EventBus &eventBus) {
        eventBus.subscribe<EventType>(
            [this](std::unique_ptr<utility::event::Event> event) {
                this->_eventQueue.push(std::unique_ptr<EventType>(
                    static_cast<EventType *>(event.release())));
            });
    }

    /**
     * @brief Destructor for the EventSubscriber base class.
     */
    ~EventSubscriber(void) = default;

    /**
     * @brief Check if there are pending events.
     * @return True if there are pending events, false otherwise.
     */
    bool hasPendingEvents(void) const { return !_eventQueue.empty(); }

    /**
     * @brief Get the next event from the queue.
     * @return The next event.
     * @retval nullptr If the queue is empty.
     */
    std::unique_ptr<EventType> getNextEvent(void) {
        if (_eventQueue.empty()) {
            return nullptr;
        }
        std::unique_ptr<EventType> event = std::move(_eventQueue.front());
        _eventQueue.pop();
        return event;
    }

    /**
     * @brief Add an event to the queue.
     * @param event The event to add.
     */
    void pushUnhandledEvent(std::unique_ptr<EventType> event) {
        _eventQueue.push(std::move(event));
    }
};

} // namespace guillaume::event
