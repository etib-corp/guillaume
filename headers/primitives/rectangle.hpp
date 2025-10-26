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
  float _width;                    ///< Width of the rectangle
  float _height;                   ///< Height of the rectangle
  Point _center;                   ///< Center point for 3D constructor
  std::vector<Point> _pointsCache; ///< Cached corner points for convenience

public:
  /**
   * @brief Default constructor - initializes an square centered at origin with
   * width and height of 1 unit.
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
   * @brief Construct from two corners: top-left and bottom-right
   */
  Rectangle(const Point &topLeft, const Point &bottomRight) : Rectangle() {
    // compute four corner points in order: top-left, top-right, bottom-right,
    // bottom-left
    _pointsCache.clear();
    _pointsCache.push_back(topLeft);
    _pointsCache.push_back(Point(bottomRight.x(), topLeft.y(), topLeft.z()));
    _pointsCache.push_back(bottomRight);
    _pointsCache.push_back(
        Point(topLeft.x(), bottomRight.y(), bottomRight.z()));

    // update polygon vertices
    std::vector<Vertex> verts;
    for (const auto &p : _pointsCache) {
      verts.emplace_back(p, Color(1.0f, 0.0f, 0.0f, 1.0f));
    }
    // replace underlying polygon vertices
    setVertices(verts);
  }

  /**
   * @brief Construct from center, width, height and optional rotation (as
   * Point: x,y,z)
   */
  Rectangle(const Point &center, float width, float height,
            const Point &rotation = Point())
      : Rectangle() {
    _center = center;
    _width = width;
    _height = height;
    setRotation(rotation);

    // compute unrotated corners centered at center
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;
    Point tl(center.x() - halfW, center.y() - halfH, center.z());
    Point tr(center.x() + halfW, center.y() - halfH, center.z());
    Point br(center.x() + halfW, center.y() + halfH, center.z());
    Point bl(center.x() - halfW, center.y() + halfH, center.z());

    // apply rotation (Euler angles in rotation: x, y, z)
    auto applyRotation = [&](const Point &p) {
      // Translate to origin (center)
      float x = p.x() - center.x();
      float y = p.y() - center.y();
      float z = p.z() - center.z();

      // Rotation angles
      float rx = rotation.x();
      float ry = rotation.y();
      float rz = rotation.z();

      // Rotate around X axis
      if (rx != 0.0f) {
        float cosy = std::cos(rx);
        float siny = std::sin(rx);
        float ny = y * cosy - z * siny;
        float nz = y * siny + z * cosy;
        y = ny;
        z = nz;
      }

      // Rotate around Y axis
      if (ry != 0.0f) {
        float cosy = std::cos(ry);
        float siny = std::sin(ry);
        float nx = x * cosy + z * siny;
        float nz = -x * siny + z * cosy;
        x = nx;
        z = nz;
      }

      // Rotate around Z axis
      if (rz != 0.0f) {
        float cosz = std::cos(rz);
        float sinz = std::sin(rz);
        float nx = x * cosz - y * sinz;
        float ny = x * sinz + y * cosz;
        x = nx;
        y = ny;
      }

      // Translate back to center
      return Point(x + center.x(), y + center.y(), z + center.z());
    };

    _pointsCache.clear();
    _pointsCache.push_back(applyRotation(tl));
    _pointsCache.push_back(applyRotation(tr));
    _pointsCache.push_back(applyRotation(br));
    _pointsCache.push_back(applyRotation(bl));

    // update polygon vertices
    std::vector<Vertex> verts2;
    for (const auto &p : _pointsCache) {
      verts2.emplace_back(p, Color(1.0f, 0.0f, 0.0f, 1.0f));
    }
    setVertices(verts2);
  }

  /**
   * @brief Construct from four corner points
   */
  Rectangle(const Point &p0, const Point &p1, const Point &p2, const Point &p3)
      : Rectangle() {
    _pointsCache = {p0, p1, p2, p3};
    std::vector<Vertex> verts3;
    for (const auto &p : _pointsCache) {
      verts3.emplace_back(p, Color(1.0f, 0.0f, 0.0f, 1.0f));
    }
    setVertices(verts3);
  }

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

  /**
   * @brief Returns the vector of corner points (in order)
   */
  const std::vector<Point> &getPoints() const { return _pointsCache; }

  /**
   * @brief Returns the polygon vertices as stored in base class
   */
  const std::vector<Vertex> &getVertices() const {
    return Polygon::getVertices();
  }

  /**
   * @brief Returns center point (for center/width/height constructors)
   */
  const Point &getCenter() const { return _center; }
};
