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

#include <utility/math/vector.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components {

/**
 * @brief Component representing a transform in 3D space.
 */
class Transform : public ecs::Component {
  public:
    using Position = utility::math::Vector<float, 3>; ///< 3D position type
    using Rotation = utility::math::Vector<float, 3>; ///< 3D rotation type
    using Scale = utility::math::Vector<float, 3>;    ///< 3D scale type

  private:
    Position _position; ///< Position of the entity
    Rotation _rotation; ///< Rotation of the entity
    Scale _scale;       ///< Scale of the entity

  public:
    /**
     * @brief Default constructor for the Transform component.
     */
    Transform(void) = default;

    /**
     * @brief Default destructor for the Transform component.
     */
    ~Transform(void) = default;

    /**
     * @brief Get the position of the entity.
     * @return The position vector.
     */
    Position getPosition(void) const { return _position; }

    /**
     * @brief Set the position of the entity.
     * @param position The new position vector.
     */
    void setPosition(const Position &position) { _position = position; }

    /**
     * @brief Get the rotation of the entity.
     * @return The rotation vector.
     */
    Rotation getRotation(void) const { return _rotation; }

    /**
     * @brief Set the rotation of the entity.
     * @param rotation The new rotation vector.
     */
    void setRotation(const Rotation &rotation) { _rotation = rotation; }

    /**
     * @brief Get the scale of the entity.
     * @return The scale vector.
     */
    Scale getScale(void) const { return _scale; }

    /**
     * @brief Set the scale of the entity.
     * @param scale The new scale vector.
     */
    void setScale(const Scale &scale) { _scale = scale; }
};

} // namespace guillaume::components