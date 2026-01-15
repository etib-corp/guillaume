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
#include <type_traits>

#include <logger.hpp>
#include <standard_logger.hpp>

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
template <typename WindowType, typename RendererType,
          typename EventHandlerType = EventHandler,
          typename LoggerType = utility::StandardLogger>
    requires std::is_base_of_v<Window<RendererType>, WindowType> &&
             std::is_base_of_v<Renderer, RendererType> &&
             std::is_base_of_v<EventHandler, EventHandlerType> &&
             std::is_base_of_v<utility::Logger, LoggerType>
class Application {
  private:
    std::map<std::string, std::unique_ptr<WindowType>>
        _windows; ///< Application windowsc

  protected:
    EventHandlerType _eventHandler; ///< Application event handler
    LoggerType _logger;             ///< Application logger
    Metadata _metadata;             ///< Application metadata

  public:
    /**
     * @brief Default constructor
     */
    Application(void) {
        _eventHandler.setEventCallback(
            [this](Event &event) { this->_logger.info("Event received."); });
    }

    /**
     * @brief Default destructor
     */
    virtual ~Application(void) = default;

    /**
     * @brief Set the application metadata.
     * @param metadata The metadata to set.
     */
    void setMetadata(const Metadata &metadata) { _metadata = metadata; }

    /**
     * @brief Get the application metadata.
     * @return Reference to the application metadata.
     */
    Metadata &getMetadata(void) { return _metadata; }

    /**
     * @brief Add a window to the application.
     * @param name The name of the window.
     */
    void addWindow(const std::string &name) {
        _windows[name] = std::make_unique<WindowType>();
    }

    /**
     * @brief Get a window by name.
     * @param name The name of the window.
     * @return Reference to the window.
     */
    WindowType &getWindow(const std::string &name) {
        return *_windows.at(name);
    }

    /**
     * @brief Get a const window by name.
     * @param name The name of the window.
     * @return Const reference to the window.
     */
    const std::unique_ptr<WindowType> &
    getWindow(const std::string &name) const {
        return _windows.at(name);
    }

    /**
     * @brief Remove a window by name.
     * @param name The name of the window.
     */
    void removeWindow(const std::string &name) { _windows.erase(name); }

    /**
     * @brief Main application routine (single frame render).
     */
    void routine(void) {}

    /**
     * @brief Check if the application should quit.
     * @return True if the application should quit, false otherwise.
     */
    bool shouldQuit(void) const { return _eventHandler.shouldQuit(); }

    /**
     * @brief Check if any window is still open.
     * @return True if at least one window is still open, false otherwise.
     */
    bool hasOpenWindows(void) const {
        for (const auto &[name, window] : _windows) {
            if (!window->shouldClose()) {
                return true;
            }
        }
        return false;
    }
};

} // namespace guillaume