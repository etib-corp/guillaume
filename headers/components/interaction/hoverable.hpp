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

#include <color.hpp>

#include "ecs/component.hpp"

namespace guillaume::components::interaction {

/**
 * @brief Hoverable component defining hover effects.
 */
class Hoverable : public ecs::Component {
  private:
    utility::Color<std::uint8_t> _hover_color_{255, 255, 255, 255};
    std::function<void()> _on_hover_{};
    bool _is_hovered_{false};

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
    void setHoverColor(const utility::Color<std::uint8_t> &color) {
        _hover_color_ = color;
    }

    /**
     * @brief Get the hover color.
     * @return Reference to the hover color value (RGBA).
     */
    utility::Color<std::uint8_t> &getHoverColor(void) { return _hover_color_; }

    /**
     * @brief Set the hovered state.
     * @param is_hovered True if currently hovered.
     */
    void setIsHovered(bool is_hovered) { _is_hovered_ = is_hovered; }

    /**
     * @brief Check if currently hovered.
     * @return True if hovered.
     */
    bool isHovered(void) const { return _is_hovered_; }

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
