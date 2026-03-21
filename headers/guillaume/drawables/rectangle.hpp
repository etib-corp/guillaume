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

#include "guillaume/drawable.hpp"

namespace guillaume::drawables {

/**
 * @brief Rectangle drawable shape.
 * @see Drawable
 */
class Rectangle : public Drawable {
  public:
    using Size = utility::math::Vector<std::float_t, 2>; ///< Width and height
                                                         ///< of the rectangle
    using BorderRadius = std::float_t; ///< Border radius for rounded rectangles

  private:
    Size _size;                   ///< Size of the rectangle (width, height)
    BorderRadius _leftTopRadius;  ///< Border radius for the top-left corner
    BorderRadius _rightTopRadius; ///< Border radius for the top-right corner
    BorderRadius
        _rightBottomRadius; ///< Border radius for the bottom-right corner
    BorderRadius
        _leftBottomRadius; ///< Border radius for the bottom-left corner

  public:
    /**
     * @brief Default constructor
     */
    Rectangle(void);

    /**
     * @brief Parameterized constructor
     * @param position Initial position of the rectangle.
     * @param rotation Initial rotation of the rectangle (in degrees).
     * @param scale Initial scale of the rectangle.
     * @param color Initial color of the rectangle (RGBA).
     * @param size Size of the rectangle (width, height).
     * @param borderRadius Border radius for rounded rectangles.
     */
    Rectangle(Position position, Rotation rotation, Scale scale, Color color,
              Size size, BorderRadius borderRadius);

    /**
     * @brief Destructor
     */
    ~Rectangle(void) = default;

    /**
     * @brief Set the size of the rectangle.
     * @param size The new size vector (width, height).
     * @return The rectangle for chaining.
     */
    Rectangle &setSize(const Size &size) {
        _size = size;
        return *this;
    }

    /**
     * @brief Get the size of the rectangle.
     * @return The size vector.
     */
    Size getSize(void) const { return _size; }

    /**
     * @brief Set the border radius for all corners of the rectangle.
     * @param radius The new border radius value.
     * @return The rectangle for chaining.
     */
    Rectangle &setAllBorderRadius(BorderRadius radius) {
        _leftTopRadius = radius;
        _rightTopRadius = radius;
        _rightBottomRadius = radius;
        _leftBottomRadius = radius;
        return *this;
    }

    /**
     * @brief Set the border radius for each corner of the rectangle.
     * @param radius The border radius for the top-left corner.
     * @return The rectangle for chaining.
     */
    Rectangle &setLeftTopBorderRadius(BorderRadius radius) {
        _leftTopRadius = radius;
        return *this;
    }

    /**
     * @brief Set the border radius for the top-right corner of the rectangle.
     * @param radius The new border radius value.
     * @return The rectangle for chaining.
     */
    Rectangle &setRightTopBorderRadius(BorderRadius radius) {
        _rightTopRadius = radius;
        return *this;
    }

    /**
     * @brief Set the border radius for the bottom-right corner of the
     * rectangle.
     * @param radius The new border radius value.
     * @return The rectangle for chaining.
     */
    Rectangle &setRightBottomBorderRadius(BorderRadius radius) {
        _rightBottomRadius = radius;
        return *this;
    }

    /**
     * @brief Set the border radius for the bottom-left corner of the
     * rectangle.
     * @param radius The new border radius value.
     * @return The rectangle for chaining.
     */
    Rectangle &setLeftBottomBorderRadius(BorderRadius radius) {
        _leftBottomRadius = radius;
        return *this;
    }

    /**
     * @brief Get the border radius for the top-left corner of the rectangle.
     * @return The border radius value.
     */
    BorderRadius getLeftTopBorderRadius(void) const { return _leftTopRadius; }

    /**
     * @brief Get the border radius for the top-right corner of the rectangle.
     * @return The border radius value.
     */
    BorderRadius getRightTopBorderRadius(void) const { return _rightTopRadius; }

    /**
     * @brief Get the border radius for the bottom-right corner of the
     * rectangle.
     * @return The border radius value.
     */
    BorderRadius getRightBottomBorderRadius(void) const {
        return _rightBottomRadius;
    }

    /**
     * @brief Get the border radius for the bottom-left corner of the
     * rectangle.
     * @return The border radius value.
     */
    BorderRadius getLeftBottomBorderRadius(void) const {
        return _leftBottomRadius;
    }

    /**
     * @brief Get the vertices defining the rectangle.
     * @return The vector of vertices.
     */
    std::vector<Vertex> getVertices(void);
};

} // namespace guillaume::drawables