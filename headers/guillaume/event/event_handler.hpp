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

#include <utility/logging/logger.hpp>
#include <utility/logging/standard_logger.hpp>

#include <utility/event/event.hpp>

namespace guillaume::event {

/**
 * @brief Interface for event handlers.
 * @param LoggerType The type of logger to use.
 *
 * This class provides an abstract interface for handling events in the
 * Guillaume framework. Implementations should poll or process platform-specific
 * events and convert them to Guillaume Event objects.
 */
template <utility::logging::InheritFromLogger LoggerType =
              utility::logging::StandardLogger>
class EventHandler {
  public:
    using Handler = std::function<void(
        std::unique_ptr<utility::event::Event> &)>; ///< Event handler type

  private:
    LoggerType logger; ///< Logger instance
    Handler _callback; ///< Event callback function

  protected:
    /**
     * @brief Get the logger instance.
     * @return Reference to the logger.
     */
    LoggerType &getLogger(void) { return logger; }

    /**
     * @brief Get the current event callback function.
     * @return Reference to the event callback function.
     */
    Handler &getEventCallback(void) { return _callback; }

  public:
    /**
     * @brief Default constructor
     */
    EventHandler(void) { getLogger().setName("EventHandler"); }

    /**
     * @brief Default destructor
     */
    virtual ~EventHandler(void) = default;

    /**
     * @brief Set the event callback function.
     *
     * The callback will be invoked for each event when pollEvents() is called.
     *
     * @param callback Function to call when an event is received.
     */
    void setEventCallback(const Handler &callback) { _callback = callback; }

    /**
     * @brief Poll for events and dispatch them.
     *
     * This method should check for pending events from the underlying platform,
     * convert them to Event objects, and call the
     * registered callback for each event.
     *
     * @return True if events were processed, false if no events are pending.
     */
    virtual bool pollEvents(void) = 0;
};

} // namespace guillaume::event
