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
#include <typeindex>
#include <vector>

#include <utility/event/event.hpp>

namespace guillaume::event {

/**
 * @brief Routes events to subscribed listeners.
 */
class EventBus {
  public:
    using Listener = std::function<void(
        std::unique_ptr<utility::event::Event>)>; ///< Event listener type
    using ListenerList = std::vector<Listener>;   ///< List of event listeners

  private:
    std::map<std::type_index, ListenerList>
        _typedListeners; ///< Per-type listeners

    /**
     * @brief Dispatch an event to a list of listeners.
     * @param event Event to dispatch.
     * @param listeners List of listeners to notify.
     */
    void dispatchToListeners(std::unique_ptr<utility::event::Event> event,
                             ListenerList &listeners) {
        for (auto &listener : listeners) {
            if (!listener) {
                continue;
            }
            listener(std::move(event));
        }
    }

  public:
    /**
     * @brief Default constructor.
     */
    EventBus(void) {}

    /**
     * @brief Default destructor.
     */
    ~EventBus(void) = default;

    /**
     * @brief Dispatch an event to listeners and systems.
     * @param event Event to dispatch.
     */
    void publish(std::unique_ptr<utility::event::Event> event) {
        auto it = _typedListeners.find(typeid(event.get()));
        if (it != _typedListeners.end()) {
            dispatchToListeners(std::move(event), it->second);
        }
    }

    /**
     * @brief Subscribe a listener to all events.
     * @tparam EventType The type of event to subscribe to.
     * @param listener Listener to notify on all events.
     */
    template <utility::event::InheritFromEvent EventType>
    void subscribe(const Listener &listener) {
        _typedListeners[typeid(EventType)].push_back(listener);
    }
};

} // namespace guillaume::event