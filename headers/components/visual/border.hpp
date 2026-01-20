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

#include "component.hpp"

namespace guillaume::components::visual {

/**
 * @brief Border component defining border style properties.
 */
class Border : public Component {
  private:
    std::size_t _thickness_{0};
    std::uint32_t _color_{0xFF000000};
    std::size_t _radius_{0};

  public:
    /**
     * @brief Default constructor.
     */
    Border(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Border(void) override = default;

    /**
     * @brief Set the border thickness.
     * @param thickness The thickness value.
     */
    void setThickness(std::size_t thickness) { _thickness_ = thickness; }

    /**
     * @brief Get the border thickness.
     * @return The thickness value.
     */
    std::size_t getThickness(void) const { return _thickness_; }

    /**
     * @brief Set the border color.
     * @param color The color value (RGBA).
     */
    void setColor(std::uint32_t color) { _color_ = color; }

    /**
     * @brief Get the border color.
     * @return The color value (RGBA).
     */
    std::uint32_t getColor(void) const { return _color_; }

    /**
     * @brief Set the border radius.
     * @param radius The radius value.
     */
    void setRadius(std::size_t radius) { _radius_ = radius; }

    /**
     * @brief Get the border radius.
     * @return The radius value.
     */
    std::size_t getRadius(void) const { return _radius_; }
};

} // namespace guillaume::components::visual
