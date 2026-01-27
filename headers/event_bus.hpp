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
#include <vector>

#include "event.hpp"

namespace guillaume {

/**
 * @brief Routes events to subscribed listeners.
 */
class EventBus {
  public:
    using Listener = std::function<void(Event &)>; ///< Event listener type
    using ListenerList = std::vector<Listener>;    ///< List of event listeners

  private:
    std::map<EventType, ListenerList> _typedListeners; ///< Per-type listeners
    ListenerList _globalListeners; ///< Listeners receiving all events

    /**
     * @brief Dispatch an event to a list of listeners.
     * @param event Event to dispatch.
     * @param listeners List of listeners to notify.
     */
    void dispatchToListeners(Event &event, ListenerList &listeners);

  public:
    /**
     * @brief Construct an event bus.
     */
    EventBus(void);

    /**
     * @brief Dispatch an event to listeners and systems.
     * @param event Event to dispatch.
     */
    void publish(Event &event);

    /**
     * @brief Subscribe a listener for a specific event type.
     * @param eventType Event type to listen for.
     * @param listener Callback invoked when the event is published.
     */
    void subscribe(EventType eventType, const Listener &listener);
};

} // namespace guillaume
