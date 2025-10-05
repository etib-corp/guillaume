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

#include "point.hpp"
#include "primitive.hpp"

/**
 * @class Polygon
 * @brief Represents a polygon primitive.
 *
 * The Polygon class is a derived class of Primitive that represents a polygon
 * shape defined by a series of points (vertices). It provides methods to
 * manage the points and render the polygon.
 */
class Polygon : public Primitive {
private:
  std::vector<Point> _points; ///< Vector of points defining the polygon

public:
  /**
   * @brief Default constructor - initializes an empty polygon
   */
  Polygon(void) : Primitive(), _points() {}

  /**
   * @brief Constructor from a list of points
   *
   * Initializes the polygon with the specified points.
   *
   * @param points A vector of Point objects defining the polygon vertices
   */
  Polygon(const std::vector<Point> &points) : Primitive(), _points(points) {}

  /**
   * @brief Destroy the Polygon object
   */
  ~Polygon(void) override = default;

  /**
   * @brief Adds a point to the polygon
   *
   * @param point The Point object to add to the polygon
   */
  void addPoint(const Point &point) { _points.push_back(point); }

  /**
   * @brief Gets the points defining the polygon
   *
   * @return A const reference to the vector of Point objects
   */
  const std::vector<Point> &getPoints(void) const { return _points; }
};
