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

#include "event_bus.hpp"

namespace guillaume {

EventBus::EventBus() {}

void EventBus::publish(Event &event) {
    auto typedListenerIterator = _typedListeners.find(event.type);
    if (typedListenerIterator != _typedListeners.end()) {
        dispatchToListeners(event, typedListenerIterator->second);
    }

    if (!event.handled) {
        dispatchToListeners(event, _globalListeners);
    }
}

void EventBus::subscribe(EventType eventType, const Listener &listener) {
    _typedListeners[eventType].push_back(listener);
}

void EventBus::dispatchToListeners(Event &event, ListenerList &listeners) {
    for (auto &listener : listeners) {
        if (!listener) {
            continue;
        }
        listener(event);
        if (event.handled) {
            break;
        }
    }
}

} // namespace guillaume
