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

#include "guillaume/ecs/component.hpp"
#include "utility/math/color.hpp"

namespace guillaume::components {

/**
 * @brief Component representing a color.
 */
class Color : public ecs::Component {

  private:
    utility::math::Color<std::uint8_t> _color; ///< The color value

  public:
    /**
     * @brief Constructor for the Color component.
     */
    explicit Color(void) : _color() {}

    /**
     * @brief Default destructor for the Color component.
     */
    ~Color(void) = default;

    /**
     * @brief Get the color value.
     * @return The color value.
     */
    const utility::math::Color<std::uint8_t> &getColor() const {
        return _color;
    }

    /**
     * @brief Set the color value.
     * @param color The new color value to set.
     * @return Reference to this Color component for chaining.
     */
    Color &setColor(const utility::math::Color<std::uint8_t> &color) {
        _color = color;
        return *this;
    }
};

} // namespace guillaume::components