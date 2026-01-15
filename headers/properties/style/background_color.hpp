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

#include <color.hpp>

#include "properties/style_property.hpp"

namespace guillaume {
class Component;
}

namespace guillaume::properties::style {

/**
 * @brief Background color style property class.
 */
class BackgroundColor : public guillaume::properties::StyleProperty {
  private:
    utility::Color<uint8_t> _color; ///< Background color

  public:
    /**
     * @brief Default constructor with black background.
     */
    BackgroundColor(void) : _color() {}

    /**
     * @brief Parameterized constructor.
     * @param color The background color.
     */
    explicit BackgroundColor(const utility::Color<uint8_t> &color)
        : _color(color) {}

    /**
     * @brief Parameterized constructor with RGB values.
     * @param r Red channel (0-255).
     * @param g Green channel (0-255).
     * @param b Blue channel (0-255).
     */
    BackgroundColor(uint8_t r, uint8_t g, uint8_t b) : _color(r, g, b) {}

    /**
     * @brief Set the background color.
     * @param color The new background color.
     */
    void setColor(const utility::Color<uint8_t> &color) { _color = color; }

    /**
     * @brief Get the background color.
     * @return The background color.
     */
    utility::Color<uint8_t> getColor(void) const { return _color; }

    /**
     * @brief Apply the background color property to a component.
     * @param component The component to apply the color to.
     */
    void apply(Component &component) override;
};

} // namespace guillaume::properties::style
