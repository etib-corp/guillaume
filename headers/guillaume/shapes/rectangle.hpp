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

#include <utility/math/vector.hpp>

#include "guillaume/shape.hpp"

namespace guillaume::shapes {

/**
 * @brief Rectangle shape.
 * @see Shape
 */
class Rectangle : public Shape {
  private:
    utility::math::Vector<std::float_t, 2> _size; ///< Size of the rectangle

  public:
    /**
     * @brief Default constructor
     */
    Rectangle(void);

    /**
     * @brief Default destructor
     */
    ~Rectangle(void) = default;

    /**
     * @brief Set the size of the rectangle.
     * @param size The new size vector.
     */
    void setSize(const utility::math::Vector<std::float_t, 2> &size);

    /**
     * @brief Get the size of the rectangle.
     * @return The size vector.
     */
    utility::math::Vector<std::float_t, 2> getSize(void) const { return _size; }
};

} // namespace guillaume::shapes