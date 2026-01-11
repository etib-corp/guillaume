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

#include "metadata.hpp"
#include "renderer.hpp"
#include "window.hpp"

namespace guillaume {

/**
 * @brief Application base class.
 *
 * @tparam WindowType The type of the window used by the application.
 * @tparam RendererType The type of the renderer used by the windows.
 * @tparam LoggerType The type of the logger used by the application.
 */
template <typename WindowType, typename RendererType,
          typename LoggerType = utility::StandardLogger>
  requires std::is_base_of_v<Window<RendererType>, WindowType> &&
           std::is_base_of_v<Renderer, RendererType> &&
           std::is_base_of_v<utility::Logger, LoggerType>
class Application {
private:
  std::map<std::string, std::unique_ptr<WindowType>>
      _windows; ///< Application windows

protected:
  LoggerType _logger; ///< Application logger
  Metadata _metadata; ///< Application metadata

public:
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
  WindowType &getWindow(const std::string &name) { return *_windows.at(name); }

  /**
   * @brief Get a const window by name.
   * @param name The name of the window.
   * @return Const reference to the window.
   */
  const WindowType &getWindow(const std::string &name) const {
    return *_windows.at(name);
  }

  /**
   * @brief Remove a window by name.
   * @param name The name of the window.
   */
  void removeWindow(const std::string &name) { _windows.erase(name); }

  /**
   * @brief Main application routine.
   */
  void routine(void) {
    for (auto &[name, window] : _windows) {
      _logger.info("Processing properties for window: " + name);
      window->processProperties();
      _logger.info("Rendering window: " + name);
      window->render(window->getRenderer());
      _logger.info("Syncing window: " + name);
      window->sync();
    }
  }
};

} // namespace guillaume