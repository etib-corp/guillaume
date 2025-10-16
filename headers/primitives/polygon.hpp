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
#include <vector>

#include "primitive.hpp"
#include "vertex.hpp"
#include "point.hpp"

/**
 * @class Polygon
 * @brief Represents a polygon primitive for 3D rendering.
 *
 * The Polygon class is a derived class of Primitive that represents a polygon
 * shape defined by a series of 3D vertices. It provides methods to manage the
 * vertices for support 3D rendering.
 */
class Polygon : public Primitive {
private:
  std::vector<Vertex> _vertices; ///< Vector of vertices defining the polygon
  Point _rotation;    ///< Euler angles (radians) for 3D rotation (use Point for x/y/z accessors)
  Point _translation; ///< Translation vector for 3D position

public:
  /**
   * @brief Default constructor - initializes an empty polygon
   */
  Polygon(void) : Primitive(), _vertices(), _rotation(), _translation() {}

  /**
   * @brief Constructor from a list of vertices
   *
   * Initializes the polygon with the specified vertices.
   *
   * @param vertices A vector of Vertex objects defining the polygon vertices in
   * 3D space
   */
  Polygon(const std::vector<Vertex> &vertices,
          const Point &rotation = Point())
      : Primitive(), _vertices(vertices), _rotation(rotation), _translation() {}

  /**
   * @brief Construct polygon from a list of Points (converts to vertices with default color)
   */
  Polygon(const std::vector<Point> &points, const Point &rotation = Point())
      : Primitive(), _rotation(rotation), _translation() {
    _vertices.clear();
    for (const auto &p : points) {
      _vertices.emplace_back(p, Color(1.0f, 1.0f, 1.0f));
    }
  }

  /**
   * @brief Destroy the Polygon object
   */
  ~Polygon(void) override = default;

  /**
   * @brief Adds a vertex to the polygon
   *
   * @param vertex The Vertex object to add to the polygon
   *
   * @return Reference to the current Polygon object to allow method chaining
   */
  Polygon &addVertex(const Vertex &vertex) {
    _vertices.push_back(vertex);
    return *this;
  }

  /**
   * @brief Gets the vertices defining the polygon
   *
   * @return A const reference to the vector of Vertex objects
   */
  const std::vector<Vertex> &getVertices(void) const { return _vertices; }

  /**
   * @brief Gets the rotation of the polygon in Euler angles (radians)
   *
   * @return A const reference to the Vector<float, 3> representing rotation
   */
  const Point &getRotation(void) const { return _rotation; }

  /**
   * @brief Gets the translation of the polygon in 3D space
   *
   * @return A const reference to the Vector<float, 3> representing translation
   */
  const Point &getTranslation(void) const { return _translation; }

  /**
   * @brief Sets the rotation of the polygon in Euler angles (radians)
   *
   * @param rotation A Vector<float, 3> representing the new rotation
   *
   * @return Reference to the current Polygon object to allow method chaining
   */
  Polygon &setRotation(const Point &rotation) {
    _rotation = rotation;
    return *this;
  }

  /**
   * @brief Sets the translation of the polygon in 3D space
   *
   * @param translation A Vector<float, 3> representing the new translation
   *
   * @return Reference to the current Polygon object to allow method chaining
   */
  Polygon &setTranslation(const Point &translation) {
    _translation = translation;
    return *this;
  }

  /**
   * @brief Replace the internal vertex list
   */
  void setVertices(const std::vector<Vertex> &vertices) { _vertices = vertices; }

  /**
   * @brief Return the polygon corner positions as Points
   */
  std::vector<Point> getPoints(void) const {
    std::vector<Point> pts;
    pts.reserve(_vertices.size());
    for (const auto &v : _vertices) {
      pts.push_back(v.position);
    }
    return pts;
  }

  // /**
  //  * @brief Calculates the surface normal for this polygon (assuming planar
  //  * polygon)
  //  *
  //  * Uses the first three points to calculate the normal vector using cross
  //  * product. This is useful for 3D rendering, lighting calculations, and
  //  * back-face culling.
  //  *
  //  * @return Point representing the normalized surface normal (0,0,0) if less
  //  * than 3 points
  //  */
  // Point calculateNormal(void) const {
  //   if (_points.size() < 3) {
  //     return Point(0, 0, 0);
  //   }

  //   // Get vectors from first point to second and third points
  //   Point v1(_points[1].x() - _points[0].x(), _points[1].y() -
  //   _points[0].y(),
  //            _points[1].z() - _points[0].z());

  //   Point v2(_points[2].x() - _points[0].x(), _points[2].y() -
  //   _points[0].y(),
  //            _points[2].z() - _points[0].z());

  //   // Calculate cross product to get normal
  //   float nx = v1.y() * v2.z() - v1.z() * v2.y();
  //   float ny = v1.z() * v2.x() - v1.x() * v2.z();
  //   float nz = v1.x() * v2.y() - v1.y() * v2.x();

  //   // Normalize the vector
  //   float length = std::sqrt(nx * nx + ny * ny + nz * nz);
  //   if (length > 0.0001f) { // Avoid division by zero
  //     return Point(nx / length, ny / length, nz / length);
  //   }

  //   return Point(0, 0, 1); // Default to Z-up if calculation fails
  // }

  // /**
  //  * @brief Calculates the centroid (geometric center) of the polygon
  //  *
  //  * @return Point representing the centroid of all vertices
  //  */
  // Point calculateCentroid(void) const {
  //   if (_points.empty()) {
  //     return Point(0, 0, 0);
  //   }

  //   float x = 0, y = 0, z = 0;
  //   for (const auto &point : _points) {
  //     x += point.x();
  //     y += point.y();
  //     z += point.z();
  //   }

  //   float count = static_cast<float>(_points.size());
  //   return Point(x / count, y / count, z / count);
  // }
};
