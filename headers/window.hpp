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

#include <optional>
#include <string>
#include <type_traits>

#include <logger.hpp>
#include <rectangle.hpp>
#include <standard_logger.hpp>
#include <vector.hpp>

#include "components/container.hpp"
#include "renderer.hpp"

namespace guillaume {

/**
 * @brief Window base class.
 * @tparam RendererType The type of the renderer associated with the window.
 * @tparam LoggerType The type of the logger used by the window.
 */
template <typename RendererType, typename LoggerType = utility::StandardLogger>
    requires std::is_base_of_v<Renderer, RendererType> &&
             std::is_base_of_v<utility::Logger, LoggerType>
class Window : public component::Container {
  protected:
    RendererType _renderer; ///< Window renderer
    LoggerType _logger;     ///< Window logger

  public:
    /**
     * @brief Default destructor
     */
    virtual ~Window(void) = default;

    /**
     * @brief Get the window title.
     * @return The window title.
     */
    virtual std::string getTitle(void) const = 0;

    /**
     * @brief Set the window title.
     * @param title The new window title.
     */
    virtual void setTitle(const std::string &title) = 0;

    /**
     * @brief Set the minimum window size.
     * @param size The minimum window size as a 2D integer vector.
     */
    virtual void setMinimumSize(utility::Vector<std::size_t, 2> size) = 0;

    /**
     * @brief Get the minimum window size.
     * @return The minimum window size as a 2D integer vector.
     */
    virtual utility::Vector<std::size_t, 2> getMinimumSize() const = 0;

    /**
     * @brief Set the maximum window size.
     * @param size The maximum window size as a 2D integer vector.
     */
    virtual void setMaximumSize(utility::Vector<std::size_t, 2> size) = 0;

    /**
     * @brief Get the maximum window size.
     * @return The maximum window size as a 2D integer vector.
     */
    virtual utility::Vector<std::size_t, 2> getMaximumSize(void) const = 0;

    /**
     * @brief Show the window.
     */
    virtual void show(void) = 0;

    /**
     * @brief Hide the window.
     */
    virtual void hide(void) = 0;

    /**
     * @brief Check if the window is visible.
     * @return True if the window is visible, false otherwise.
     */
    virtual bool isVisible(void) const = 0;

    /**
     * @brief Maximize the window.
     */
    virtual void maximize(void) = 0;

    /**
     * @brief Minimize the window.
     */
    virtual void minimize(void) = 0;

    /**
     * @brief Restore the window to its normal size.
     */
    virtual void restore(void) = 0;

    /**
     * @brief Set the window opacity.
     * @param opacity The opacity value (0.0 to 1.0).
     */
    virtual void setOpacity(float opacity) = 0;

    /**
     * @brief Get the window opacity.
     * @return The current opacity value (0.0 to 1.0).
     */
    virtual float getOpacity(void) const = 0;

    /**
     * @brief Request the window to close.
     */
    virtual void requestClose(void) = 0;

    /**
     * @brief Set the window aspect ratio.
     * @param minimumAspectRatio The minimum aspect ratio (width/height), or
     * 0.0f for no limit.
     * @param maximumAspectRatio The maximum aspect ratio (width/height), or
     * 0.0f for no limit.
     */
    virtual void setAspectRatio(float minimumAspectRatio,
                                float maximumAspectRatio) = 0;

    /**
     * @brief Get the window aspect ratio.
     * @param minimumAspectRatio Reference to store minimum aspect ratio.
     * @param maximumAspectRatio Reference to store maximum aspect ratio.
     */
    virtual void getAspectRatio(float &minimumAspectRatio,
                                float &maximumAspectRatio) const = 0;

    /**
     * @brief Set the resizable state of the window.
     * @param resizable True to allow resizing, false to disallow.
     */
    virtual void setResizable(bool resizable) = 0;

    /**
     * @brief Block until any pending window state is finalized.
     * @return True on success, false if timeout.
     */
    virtual bool sync(void) = 0;

    /**
     * @brief Get the renderer associated with the window.
     * @return Reference to the unique pointer of the renderer.
     */
    virtual RendererType &getRenderer(void) { return _renderer; }
};

} // namespace guillaume
