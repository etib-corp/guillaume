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

#include <utility/math/rectangle.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components {

/**
 * @brief Component representing a bounding rectangle.
 */
class Bound : public ecs::Component {
  private:
    utility::math::Rectangle<float> _rectangle; ///< Bounding rectangle

  public:
    /**
     * @brief Default constructor for the Bound component.
     */
    Bound(void) = default;

    /**
     * @brief Default destructor for the Bound component.
     */
    ~Bound(void) = default;

    /**
     * @brief Get the bounding rectangle.
     * @return The bounding rectangle.
     */
    const utility::math::Rectangle<float> &getRectangle(void) const {
        return _rectangle;
    }

    /**
     * @brief Set the bounding rectangle.
     * @param rectangle The new bounding rectangle.
     */
    void setRectangle(const utility::math::Rectangle<float> &rectangle) {
        _rectangle = rectangle;
    }
};

} // namespace guillaume::components