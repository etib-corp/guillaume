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

#include "primitives/polygon.hpp"

/**
 * @class Triangle
 * @brief Represents a triangle primitive.
 *
 * The Triangle class is a derived class of Polygon that represents a triangle
 * shape defined by three points (vertices). It provides methods to manage the
 * points and render the triangle.
 */
class Triangle : public Polygon {
public:
  /**
   * @brief Default constructor - initializes an empty triangle
   */
  Triangle(void) : Polygon() {}

  /**
   * @brief Constructor from three points
   *
   * Initializes the triangle with the specified vertices.
   *
   * @param p1 The first vertex of the triangle
   * @param p2 The second vertex of the triangle
   * @param p3 The third vertex of the triangle
   */
  Triangle(const Point &p1, const Point &p2, const Point &p3)
      : Polygon({p1, p2, p3}) {}

  /**
   * @brief Destructor
   *
   * Cleans up the triangle resources.
   */
  ~Triangle(void) override = default;
};
