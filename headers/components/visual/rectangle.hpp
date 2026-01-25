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

#include <cstddef>
#include <cstdint>

#include <color.hpp>

#include "ecs/component.hpp"

namespace guillaume::components::visual {

/**
 * @brief Rectangle component defining a rectangular area with colors.
 */
class Rectangle : public ecs::Component {
  private:
    utility::Color<std::uint8_t> _fill_color_{255, 255, 255, 255};
    utility::Color<std::uint8_t> _border_color_{0, 0, 0, 255};
    float _border_thickness_{0.0f};

  public:
    /**
     * @brief Default constructor.
     */
    Rectangle(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Rectangle(void) override = default;

    /**
     * @brief Set the fill color.
     * @param color The fill color value (RGBA).
     */
    void setFillColor(const utility::Color<std::uint8_t> &color) {
        _fill_color_ = color;
    }

    /**
     * @brief Get the fill color.
     * @return Reference to the fill color.
     */
    utility::Color<std::uint8_t> &getFillColor(void) { return _fill_color_; }

    /**
     * @brief Set the border color.
     * @param color The border color value (RGBA).
     */
    void setBorderColor(const utility::Color<std::uint8_t> &color) {
        _border_color_ = color;
    }

    /**
     * @brief Get the border color.
     * @return Reference to the border color.
     */
    utility::Color<std::uint8_t> &getBorderColor(void) {
        return _border_color_;
    }

    /**
     * @brief Set the border thickness.
     * @param thickness The border thickness value.
     */
    void setBorderThickness(float thickness) { _border_thickness_ = thickness; }

    /**
     * @brief Get the border thickness.
     * @return The border thickness value.
     */
    float getBorderThickness(void) const { return _border_thickness_; }
};

} // namespace guillaume::components::visual
