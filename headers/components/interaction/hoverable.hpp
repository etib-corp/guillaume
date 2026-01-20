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

#include <cstdint>
#include <functional>

#include "component.hpp"

namespace guillaume::components::interaction {

/**
 * @brief Hoverable component defining hover effects.
 */
class Hoverable : public Component {
  private:
    std::uint32_t _hover_color_{0xFFFFFFFF};
    std::function<void()> _on_hover_{};

  public:
    /**
     * @brief Default constructor.
     */
    Hoverable(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Hoverable(void) override = default;

    /**
     * @brief Set the hover color.
     * @param color The color value (RGBA).
     */
    void setHoverColor(std::uint32_t color) { _hover_color_ = color; }

    /**
     * @brief Get the hover color.
     * @return The hover color value (RGBA).
     */
    std::uint32_t getHoverColor(void) const { return _hover_color_; }

    /**
     * @brief Set the hover callback.
     * @param callback The function to call when hovered.
     */
    void setOnHover(std::function<void()> callback) { _on_hover_ = callback; }

    /**
     * @brief Get the hover callback.
     * @return The hover callback function.
     */
    const std::function<void()> &getOnHover(void) const { return _on_hover_; }

    /**
     * @brief Trigger the hover callback.
     */
    void hover(void) const {
        if (_on_hover_) {
            _on_hover_();
        }
    }
};

} // namespace guillaume::components::interaction
