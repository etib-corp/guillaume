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

#include <map>
#include <memory>
#include <string>

#include <logger.hpp>
#include <standard_logger.hpp>

#include "ecs.hpp"
#include "event_bus.hpp"
#include "event_handler.hpp"
#include "metadata.hpp"
#include "renderer.hpp"
#include "window.hpp"

namespace guillaume {

/**
 * @brief Application base class.
 *
 * @tparam WindowType The type of the window used by the application.
 * @tparam RendererType The type of the renderer used by the windows.
 * @tparam EventHandlerType The type of the event handler used by the
 * application.
 * @tparam LoggerType The type of the logger used by the application.
 */
template <typename WindowType, InheritFromRenderer RendererType,
          InheritFromEventHandler EventHandlerType,
          utility::InheritFromLogger LoggerType = utility::StandardLogger>
class Application {
  private:
    std::map<std::string, std::unique_ptr<WindowType>>
        _windows;                   ///< Application windows
    ECS _ecs;                       ///< Application ECS
    EventHandlerType _eventHandler; ///< Application event handler
    EventBus _eventBus;             ///< Event bus dispatching to systems

    /**
     * @brief Add a window to the application.
     * @param name The name of the window.
     */
    void addWindow(const std::string &name) {
        _windows[name] = std::make_unique<WindowType>();
    }

  protected:
    LoggerType _logger; ///< Application logger

    /**
     * @brief Remove a window by name.
     * @param name The name of the window.
     */
    void removeWindow(const std::string &name) { _windows.erase(name); }

    /**
     * @brief Main application routine (single frame render).
     */
    void routine(void) {
        _eventHandler.pollEvents();
        _ecs.routine();
    }

  public:
    /**
     * @brief Default constructor
     */
    Application(void) {
        _eventHandler.setEventCallback(
            [this](Event &event) { this->_eventBus.publish(event); });
    }

    /**
     * @brief Default destructor
     */
    virtual ~Application(void) = default;

    /**
     * @brief Run the application main loop.
     * @return Exit code.
     */
    int run(void) {
        while (true) {
            try {
                routine();
            } catch (const std::exception &exception) {
                _logger.error(std::string("Application error: ") +
                              exception.what());
                return EXIT_FAILURE;
            }
        }
        return EXIT_SUCCESS;
    }
};

} // namespace guillaume