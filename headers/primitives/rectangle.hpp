/*
 Copyright (c) 2025 ETIB Corporation

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

#include "point.hpp"
#include "primitives/polygon.hpp"

/**
 * @class Rectangle
 * @brief Represents a rectangle primitive.
 * The Rectangle class is a derived class of Polygon that represents a rectangle
 * shape defined by its width and height. It provides methods to manage the
 * points and render the rectangle.
 */
class Rectangle : public Polygon {
public:
  /**
   * @brief Default constructor - initializes an empty rectangle
   */
  Rectangle(void) : Polygon() {}

  /**
   * @brief Constructor from width and height
   *
   * Initializes the rectangle with the specified width and height.
   * The rectangle is defined by four points in a clockwise manner starting
   * from the top-left corner.
   *
   * @param width The width of the rectangle
   * @param height The height of the rectangle
   */
  Rectangle(Point topLeft, Point bottomRight)
      : Polygon({topLeft, Point(bottomRight.x(), topLeft.y(), 0.0f),
                 bottomRight, Point(topLeft.x(), bottomRight.y(), 0.0f)}){}
  /**
   * @brief Destructor
   */
  ~Rectangle(void) override = default;
};
