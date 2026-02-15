/*
 Copyright (c) 2026 ETIB Corporation

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

#include <utility/math/color.hpp>
#include <utility/math/vector.hpp>

namespace guillaume {

/**
 * @brief Abstract base class for drawable shapes.
 * @see shapes::Rectangle
 * @see shapes::Circle
 * @see shapes::Triangle
 * @see Renderer
 */
class Shape {
  private:
    utility::math::Vector<std::float_t, 3> _position; ///< Position in 3D space
    utility::math::Vector<std::float_t, 3> _rotation; ///< Rotation angles
    utility::math::Vector<std::float_t, 3> _scale;    ///< Scaling factors
    utility::math::Color<uint8_t> _color;             ///< Shape color

  public:
    /**
     * @brief Default constructor
     */
    Shape(void);

    /**
     * @brief Default destructor
     */
    virtual ~Shape(void) = default;

    /**
     * @brief Set the position of the shape.
     * @param position The new position vector.
     */
    void setPosition(const utility::math::Vector<std::float_t, 3> &position);

    /**
     * @brief Get the position of the shape.
     * @return The position vector.
     */
    utility::math::Vector<std::float_t, 3> getPosition(void) const {
        return _position;
    }

    /**
     * @brief Set the rotation of the shape.
     * @param rotation The new rotation vector (in degrees).
     */
    void setRotation(const utility::math::Vector<std::float_t, 3> &rotation);

    /**
     * @brief Get the rotation of the shape.
     * @return The rotation vector (in degrees).
     */
    utility::math::Vector<std::float_t, 3> getRotation(void) const {
        return _rotation;
    }

    /**
     * @brief Set the scale of the shape.
     * @param scale The new scale vector.
     */
    void setScale(const utility::math::Vector<std::float_t, 3> &scale);

    /**
     * @brief Get the scale of the shape.
     * @return The scale vector.
     */
    utility::math::Vector<std::float_t, 3> getScale(void) const {
        return _scale;
    }

    /**
     * @brief Set the color of the shape.
     * @param color The new color.
     */
    void setColor(const utility::math::Color<uint8_t> &color);

    /**
     * @brief Get the color of the shape.
     * @return The color.
     */
    utility::math::Color<uint8_t> getColor(void) const { return _color; }
};

} // namespace guillaume