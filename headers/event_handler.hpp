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

#include <vector.hpp>

#include "event.hpp"

namespace guillaume {

/**
 * @brief Event callback function type.
 */
using EventCallbackFunction = std::function<void(Event &)>;

/**
 * @brief Interface for event handlers.
 *
 * This class provides an abstract interface for handling events in the
 * Guillaume framework. Implementations should poll or process platform-specific
 * events and convert them to Guillaume Event objects.
 */
class EventHandler {
public:
  virtual ~EventHandler(void) = default;

  /**
   * @brief Poll for events and dispatch them.
   *
   * This method should check for pending events from the underlying platform,
   * convert them to Guillaume Event objects, and call the registered callback
   * for each event.
   *
   * @return True if events were processed, false if no events are pending.
   */
  virtual bool pollEvents(void) = 0;

  /**
   * @brief Set the event callback function.
   *
   * The callback will be invoked for each event when pollEvents() is called.
   *
   * @param callback Function to call when an event is received.
   */
  virtual void setEventCallback(const EventCallbackFunction &callback) = 0;

  /**
   * @brief Check if a quit event has been received.
   *
   * @return True if a quit event was received.
   */
  virtual bool shouldQuit(void) const = 0;
};

} // namespace guillaume
