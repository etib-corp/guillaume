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

#include "properties/style_property.hpp"

namespace guillaume::properties::style {

/**
 * @brief Opacity style property class.
 */
class Opacity : public guillaume::properties::StyleProperty {
  private:
    float _alpha; ///< Opacity value (0.0 to 1.0)

  public:
    /**
     * @brief Default constructor with full opacity.
     */
    Opacity(void) : _alpha(1.0f) {}

    /**
     * @brief Parameterized constructor.
     * @param alpha Opacity value (0.0 = fully transparent, 1.0 = fully opaque).
     */
    explicit Opacity(float alpha) : _alpha(alpha) {
        if (_alpha < 0.0f)
            _alpha = 0.0f;
        if (_alpha > 1.0f)
            _alpha = 1.0f;
    }

    /**
     * @brief Set the opacity.
     * @param alpha Opacity value (0.0 to 1.0).
     */
    void setAlpha(float alpha) {
        _alpha = (alpha < 0.0f) ? 0.0f : (alpha > 1.0f) ? 1.0f : alpha;
    }

    /**
     * @brief Get the opacity.
     * @return The opacity value (0.0 to 1.0).
     */
    float getAlpha(void) const { return _alpha; }

    /**
     * @brief Apply the opacity property to a component.
     * @param component The component to apply the opacity to.
     */
    void apply(Component &component) override;
};

} // namespace guillaume::properties::style
