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

#include <cmath>

#include "primitives/polygon.hpp"

/**
 * @class Triangle
 * @brief Represents a triangle primitive for 3D rendering.
 *
 * The Triangle class is a derived class of Polygon that represents a triangle
 * shape defined by three 3D points (vertices). It inherits all 3D capabilities
 * from Polygon including normal calculation and centroid computation.
 */
class Triangle : public Polygon {
public:
  /**
   * @brief Default constructor - initializes an empty triangle
   */
  Triangle(void)
      : Polygon({
            Vertex(Point(0.0f, 0.5f, 0.0f),
                   Color(1.0f, 0.0f, 0.0f)), // Top vertex
            Vertex(Point(-0.5f, -0.5f, 0.0f),
                   Color(1.0f, 0.0f, 0.0f)), // Bottom-left vertex
            Vertex(Point(0.5f, -0.5f, 0.0f),
                   Color(1.0f, 0.0f, 0.0f)), // Bottom-right vertex
        }) {}

  /**
   * @brief Constructor from three 3D points
   *
   * Initializes the triangle with the specified 3D vertices.
   *
   * @param p1 The first vertex of the triangle in 3D space
   * @param p2 The second vertex of the triangle in 3D space
   * @param p3 The third vertex of the triangle in 3D space
   */
  Triangle(const Point &p1, const Point &p2, const Point &p3)
      : Polygon({
            Vertex(p1, Color(1.0f, 1.0f, 1.0f)),
            Vertex(p2, Color(1.0f, 1.0f, 1.0f)),
            Vertex(p3, Color(1.0f, 1.0f, 1.0f)),
        }) {}

  /**
   * @brief Destructor
   *
   * Cleans up the triangle resources.
   */
  ~Triangle(void) override = default;
};
