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

#include <vector>

#include <utility/math/color.hpp>
#include <utility/math/vector.hpp>
#include <utility/math/vertex.hpp>

namespace guillaume {

/**
 * @brief Abstract base class for drawable shapes.
 * @see Renderer
 */
class Drawable {
  public:
    using Position =
        utility::math::Vector<std::float_t,
                              3>; ///< Position of the shape in 3D space
    using Rotation =
        utility::math::Vector<std::float_t,
                              3>; ///< Rotation of the shape (in degrees)
    using Scale =
        utility::math::Vector<std::float_t, 3>;  ///< Scale of the shapeF
    using Color = utility::math::Color<uint8_t>; ///< Color of the shape (RGBA)
    using Vertex =
        utility::math::Vertex<std::float_t,
                              uint8_t>; ///< Vertex type for shape geometry

  private:
    Position _position; ///< Position of the shape in 3D space
    Rotation _rotation; ///< Rotation of the shape (in degrees)
    Scale _scale;       ///< Scale of the shape
    Color _color;       ///< Color of the shape (RGBA)

  public:
    /**
     * @brief Default constructor
     */
    Drawable(void);

    /**
     * @brief Parameterized constructor
     * @param position Initial position of the shape.
     * @param rotation Initial rotation of the shape (in degrees).
     * @param scale Initial scale of the shape.
     * @param color Initial color of the shape (RGBA).
     */
    Drawable(Position position, Rotation rotation, Scale scale, Color color);

    /**
     * @brief Default destructor
     */
    virtual ~Drawable(void);

    /**
     * @brief Set the position of the shape.
     * @param position The new position vector.
     * @return The shape for chaining.
     */
    Drawable &setPosition(const Position &position);

    /**
     * @brief Get the position of the shape.
     * @return The position vector.
     */
    Position getPosition(void) const { return _position; }

    /**
     * @brief Set the rotation of the shape.
     * @param rotation The new rotation vector.
     * @return The shape for chaining.
     */
    Drawable &setRotation(const Rotation &rotation);

    /**
     * @brief Get the rotation of the shape.
     * @return The rotation vector (in degrees).
     */
    Rotation getRotation(void) const { return _rotation; }

    /**
     * @brief Set the scale of the shape.
     * @param scale The new scale vector.
     * @return The shape for chaining.
     */
    Drawable &setScale(const Scale &scale);

    /**
     * @brief Get the scale of the shape.
     * @return The scale vector.
     */
    Scale getScale(void) const { return _scale; }

    /**
     * @brief Set the color of the shape.
     * @param color The new color.
     * @return The shape for chaining.
     */
    Drawable &setColor(const Color &color);

    /**
     * @brief Get the color of the shape.
     * @return The color.
     */
    Color getColor(void) const { return _color; }
};

} // namespace guillaume