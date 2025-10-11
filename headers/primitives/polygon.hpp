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

#include <vector>
#include <cmath>

#include "point.hpp"
#include "primitive.hpp"

/**
 * @class Polygon
 * @brief Represents a polygon primitive for 3D rendering.
 *
 * The Polygon class is a derived class of Primitive that represents a polygon
 * shape defined by a series of 3D points (vertices). It provides methods to
 * manage the points, calculate surface normals, and support 3D rendering.
 */
class Polygon : public Primitive {
private:
  std::vector<Point> _points; ///< Vector of 3D points defining the polygon vertices

public:
  /**
   * @brief Default constructor - initializes an empty polygon
   */
  Polygon(void) : Primitive(), _points() {}

  /**
   * @brief Constructor from a list of 3D points
   *
   * Initializes the polygon with the specified 3D points.
   *
   * @param points A vector of Point objects defining the polygon vertices in 3D space
   */
  Polygon(const std::vector<Point> &points) : Primitive(), _points(points) {}

  /**
   * @brief Destroy the Polygon object
   */
  ~Polygon(void) override = default;

  /**
   * @brief Adds a 3D point to the polygon
   *
   * @param point The Point object to add to the polygon in 3D space
   */
  void addPoint(const Point &point) { _points.push_back(point); }

  /**
   * @brief Gets the 3D points defining the polygon
   *
   * @return A const reference to the vector of Point objects
   */
  const std::vector<Point> &getPoints(void) const { return _points; }

  /**
   * @brief Calculates the surface normal for this polygon (assuming planar polygon)
   *
   * Uses the first three points to calculate the normal vector using cross product.
   * This is useful for 3D rendering, lighting calculations, and back-face culling.
   *
   * @return Point representing the normalized surface normal (0,0,0) if less than 3 points
   */
  Point calculateNormal(void) const {
    if (_points.size() < 3) {
      return Point(0, 0, 0);
    }

    // Get vectors from first point to second and third points
    Point v1(_points[1].x() - _points[0].x(),
             _points[1].y() - _points[0].y(),
             _points[1].z() - _points[0].z());

    Point v2(_points[2].x() - _points[0].x(),
             _points[2].y() - _points[0].y(),
             _points[2].z() - _points[0].z());

    // Calculate cross product to get normal
    float nx = v1.y() * v2.z() - v1.z() * v2.y();
    float ny = v1.z() * v2.x() - v1.x() * v2.z();
    float nz = v1.x() * v2.y() - v1.y() * v2.x();

    // Normalize the vector
    float length = std::sqrt(nx*nx + ny*ny + nz*nz);
    if (length > 0.0001f) { // Avoid division by zero
      return Point(nx/length, ny/length, nz/length);
    }

    return Point(0, 0, 1); // Default to Z-up if calculation fails
  }

  /**
   * @brief Calculates the centroid (geometric center) of the polygon
   *
   * @return Point representing the centroid of all vertices
   */
  Point calculateCentroid(void) const {
    if (_points.empty()) {
      return Point(0, 0, 0);
    }

    float x = 0, y = 0, z = 0;
    for (const auto& point : _points) {
      x += point.x();
      y += point.y();
      z += point.z();
    }

    float count = static_cast<float>(_points.size());
    return Point(x/count, y/count, z/count);
  }
};
