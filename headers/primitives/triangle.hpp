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
  Triangle(void) : Polygon() {}

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
      : Polygon({p1, p2, p3}) {}

  /**
   * @brief Calculates the area of the triangle in 3D space
   *
   * Uses the cross product formula to calculate the area regardless
   * of the triangle's orientation in 3D space.
   *
   * @return float The area of the triangle
   */
  float calculateArea(void) const {
    const auto& points = getPoints();
    if (points.size() < 3) {
      return 0.0f;
    }

    // Get vectors from first point to second and third points
    Point v1(points[1].x() - points[0].x(),
             points[1].y() - points[0].y(),
             points[1].z() - points[0].z());

    Point v2(points[2].x() - points[0].x(),
             points[2].y() - points[0].y(),
             points[2].z() - points[0].z());

    // Calculate cross product magnitude
    float nx = v1.y() * v2.z() - v1.z() * v2.y();
    float ny = v1.z() * v2.x() - v1.x() * v2.z();
    float nz = v1.x() * v2.y() - v1.y() * v2.x();

    // Area is half the magnitude of the cross product
    return 0.5f * std::sqrt(nx*nx + ny*ny + nz*nz);
  }

  /**
   * @brief Destructor
   *
   * Cleans up the triangle resources.
   */
  ~Triangle(void) override = default;
};
