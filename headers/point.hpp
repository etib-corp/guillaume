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

#include "vector.hpp"

/**
 * @class Point
 * @brief Represents a point in 3D space.
 *
 * The Point class is a simple wrapper around a 3D vector to represent a point
 * in space. It provides constructors for initialization and access to the
 * underlying vector components.
 */
class Point : public Vector<float, 3> {
public:
  /**
   * @brief Default constructor - initializes point at the origin (0,0,0)
   */
  Point(void) : Vector<float, 3>({0.0f, 0.0f, 0.0f}) {}

  /**
   * @brief Constructor from coordinates
   *
   * Initializes the point with the specified x, y, and z coordinates.
   *
   * @param x The x-coordinate of the point
   * @param y The y-coordinate of the point
   * @param z The z-coordinate of the point
   */
  Point(float x, float y, float z) : Vector<float, 3>({x, y, z}) {}

  /**
   * @brief Access the x-coordinate of the point
   *
   * @return The x-coordinate
   */
  float x(void) const { return (*this)[0]; }

  /**
   * @brief Access the y-coordinate of the point
   *
   * @return The y-coordinate
   */
  float y(void) const { return (*this)[1]; }

  /**
   * @brief Access the z-coordinate of the point
   *
   * @return The z-coordinate
   */
  float z(void) const { return (*this)[2]; }
};
