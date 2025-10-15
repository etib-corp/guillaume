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

#include <string>

#include "point.hpp"
#include "primitive.hpp"

/**
 * @class Text
 * @brief Represents a text primitive for rendering in 3D space.
 *
 * The Text class is a derived class of Primitive that represents text
 * content at a specific 3D position with optional rotation and scaling.
 * This allows components like Labels and Buttons to generate text
 * primitives for 3D rendering.
 */
class Text : public Primitive {
private:
  std::string _content; ///< The text content to display
  Point _position;      ///< 3D position where the text should be rendered
  Point _rotation; ///< Rotation angles (x, y, z) in radians for 3D orientation
  float _scale;    ///< Scale factor for text size

public:
  /**
   * @brief Default constructor - initializes empty text at origin with no
   * rotation and unit scale
   */
  Text(void)
      : Primitive(), _content(""), _position(0, 0, 0), _rotation(0, 0, 0),
        _scale(1.0f) {}

  /**
   * @brief Constructor with text content
   *
   * @param content The text content to display
   */
  Text(const std::string &content)
      : Primitive(), _content(content), _position(0, 0, 0), _rotation(0, 0, 0),
        _scale(1.0f) {}

  /**
   * @brief Constructor with text content and 3D position
   *
   * @param content The text content to display
   * @param position The 3D position where the text should be rendered
   */
  Text(const std::string &content, const Point &position)
      : Primitive(), _content(content), _position(position), _rotation(0, 0, 0),
        _scale(1.0f) {}

  /**
   * @brief Constructor with text content, position, rotation, and scale for
   * full 3D control
   *
   * @param content The text content to display
   * @param position The 3D position where the text should be rendered
   * @param rotation The rotation angles (x, y, z) in radians
   * @param scale The scale factor for text size
   */
  Text(const std::string &content, const Point &position, const Point &rotation,
       float scale)
      : Primitive(), _content(content), _position(position),
        _rotation(rotation), _scale(scale) {}

  /**
   * @brief Destroy the Text object
   */
  ~Text(void) override = default;

  /**
   * @brief Gets the text content
   *
   * @return const std::string& Reference to the text content
   */
  const std::string &getContent(void) const { return _content; }

  /**
   * @brief Sets the text content
   *
   * @param content The new text content
   */
  void setContent(const std::string &content) { _content = content; }

  /**
   * @brief Gets the text position
   *
   * @return const Point& Reference to the text position
   */
  const Point &getPosition(void) const { return _position; }

  /**
   * @brief Sets the text position
   *
   * @param position The new 3D position for the text
   */
  void setPosition(const Point &position) { _position = position; }

  /**
   * @brief Gets the text rotation
   *
   * @return const Point& Reference to the rotation angles (x, y, z) in radians
   */
  const Point &getRotation(void) const { return _rotation; }

  /**
   * @brief Sets the text rotation
   *
   * @param rotation The new rotation angles (x, y, z) in radians
   */
  void setRotation(const Point &rotation) { _rotation = rotation; }

  /**
   * @brief Gets the text scale
   *
   * @return float The scale factor
   */
  float getScale(void) const { return _scale; }

  /**
   * @brief Sets the text scale
   *
   * @param scale The new scale factor
   */
  void setScale(float scale) { _scale = scale; }
};
