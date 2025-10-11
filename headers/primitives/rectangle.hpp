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
private:
  Point _center;
  float _width;
  float _height;
  Point _rotation; // Euler angles (radians) for 3D rotation

public:
  /**
   * @brief Default constructor - initializes an empty rectangle
   */
  Rectangle(void) : Polygon(), _center(0,0,0), _width(0), _height(0), _rotation(0,0,0) {}

  /**
   * @brief Constructor from top-left and bottom-right points (legacy/compatibility)
   *
   * Computes center, width, height, and uses zero rotation.
   */
  Rectangle(const Point& topLeft, const Point& bottomRight)
    : Rectangle(
        Point((topLeft.x() + bottomRight.x()) * 0.5f,
              (topLeft.y() + bottomRight.y()) * 0.5f,
              (topLeft.z() + bottomRight.z()) * 0.5f),
        std::abs(bottomRight.x() - topLeft.x()),
        std::abs(bottomRight.y() - topLeft.y()),
        Point(0, 0, 0)) {}

  /**
   * @brief Constructor using center, width, height, and rotation (Euler angles)
   *
   * @param center The center of the rectangle in 3D space
   * @param width The width of the rectangle
   * @param height The height of the rectangle
   * @param rotation The rotation (Euler angles in radians: x=pitch, y=yaw, z=roll)
   */
  Rectangle(const Point& center, float width, float height, const Point& rotation)
      : Polygon(computeCorners(center, width, height, rotation)),
        _center(center), _width(width), _height(height), _rotation(rotation) {}

  /**
   * @brief Constructor for 3D rectangle with all four corner points
   *
   * Allows full control over all four corners of the rectangle in 3D space.
   * This constructor enables creating rectangles that are not axis-aligned
   * or have different z-coordinates at different corners.
   *
   * @param topLeft The top-left corner point
   * @param topRight The top-right corner point
   * @param bottomRight The bottom-right corner point
   * @param bottomLeft The bottom-left corner point
   */
  Rectangle(Point topLeft, Point topRight, Point bottomRight, Point bottomLeft)
      : Polygon({topLeft, topRight, bottomRight, bottomLeft}),
        _center(0,0,0), _width(0), _height(0), _rotation(0,0,0) {}

  /**
   * @brief Accessors
   */
  const Point& getCenter() const { return _center; }
  float getWidth() const { return _width; }
  float getHeight() const { return _height; }
  const Point& getRotation() const { return _rotation; }

  /**
   * @brief Destructor
   */
  ~Rectangle(void) override = default;

private:
  // Helper to compute corners from center, width, height, and rotation
  static std::vector<Point> computeCorners(const Point& center, float width, float height, const Point& rotation) {
    // Rectangle in local space (centered at origin, z=0)
    float hw = width * 0.5f;
    float hh = height * 0.5f;
    std::vector<Point> local = {
      Point(-hw, -hh, 0), // top-left
      Point(hw, -hh, 0),  // top-right
      Point(hw, hh, 0),   // bottom-right
      Point(-hw, hh, 0)   // bottom-left
    };

    // Build rotation matrix from Euler angles (rotation order: ZYX)
    float cx = std::cos(rotation.x());
    float sx = std::sin(rotation.x());
    float cy = std::cos(rotation.y());
    float sy = std::sin(rotation.y());
    float cz = std::cos(rotation.z());
    float sz = std::sin(rotation.z());

    // Rotation matrix (R = Rz * Ry * Rx)
    float m00 = cy * cz;
    float m01 = cz * sx * sy - cx * sz;
    float m02 = cx * cz * sy + sx * sz;
    float m10 = cy * sz;
    float m11 = cx * cz + sx * sy * sz;
    float m12 = -cz * sx + cx * sy * sz;
    float m20 = -sy;
    float m21 = cy * sx;
    float m22 = cx * cy;

    std::vector<Point> result;
    for (const auto& p : local) {
      // Rotate
      float x = m00 * p.x() + m01 * p.y() + m02 * p.z();
      float y = m10 * p.x() + m11 * p.y() + m12 * p.z();
      float z = m20 * p.x() + m21 * p.y() + m22 * p.z();
      // Translate
      result.emplace_back(center.x() + x, center.y() + y, center.z() + z);
    }
    return result;
  }
};
