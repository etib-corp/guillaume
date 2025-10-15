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
#include <cmath>

/**
 * @class Rectangle
 * @brief Represents a rectangle primitive.
 * The Rectangle class is a derived class of Polygon that represents a rectangle
 * shape defined by its width and height. It provides methods to manage the
 * points and render the rectangle.
 */

class Rectangle : public Polygon {
private:
  float _width;  ///< Width of the rectangle
  float _height; ///< Height of the rectangle

public:
  /**
   * @brief Default constructor - initializes an empty rectangle
   */
  Rectangle(void)
      : Polygon({
            Vertex(Point(-0.5f, -0.5f, 0.0f),
                   Color(1.0f, 0.0f, 0.0f, 1.0f)), // Top-left
            Vertex(Point(0.5f, -0.5f, 0.0f),
                   Color(1.0f, 0.0f, 0.0f, 1.0f)), // Top-right
            Vertex(Point(0.5f, 0.5f, 0.0f),
                   Color(1.0f, 0.0f, 0.0f, 1.0f)), // Bottom-right
            Vertex(Point(-0.5f, 0.5f, 0.0f),
                   Color(1.0f, 0.0f, 0.0f, 1.0f)), // Bottom-left
        }),
        _width(1), _height(1) {}

  /**
   * @brief Destructor
   */
  ~Rectangle(void) override = default;

  /**
   * @brief Accessors
   */
  float getWidth() const { return _width; }

  /**
   * @brief Accessors
   */
  float getHeight() const { return _height; }

  /**
   * @brief Sets the dimensions of the rectangle.
   *
   * This method sets the width and height of the rectangle.
   *
   * @param width The width of the rectangle.
   * @param height The height of the rectangle.
   *
   * @return Reference to the current Rectangle object to allow method chaining
   */
  Rectangle &setDimensions(float width, float height) {
    _width = width;
    _height = height;

    return *this;
  }

  /**
   * @brief Sets the height of the rectangle.
   *
   * This method sets the height of the rectangle.
   *
   * @param height The height of the rectangle.
   *
   * @return Reference to the current Rectangle object to allow method chaining
   */
  Rectangle &setHeight(float height) {
    _height = height;
    return *this;
  }

  /**
   * @brief Sets the width of the rectangle.
   *
   * This method sets the width of the rectangle.
   *
   * @param width The width of the rectangle.
   *
   * @return Reference to the current Rectangle object to allow method chaining
   */
  Rectangle &setWidth(float width) {
    _width = width;
    return *this;
  }
};
