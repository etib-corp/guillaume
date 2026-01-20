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

#include <vector.hpp>

#include "component.hpp"

namespace guillaume::components::visual {

/**
 * @brief Transform component representing position, rotation, and scale.
 */
class Transform : public Component {
  private:
    utility::Vector<std::size_t, 3> _position_{0, 0, 0};
    utility::Vector<std::size_t, 3> _rotation_{0, 0, 0};
    utility::Vector<std::size_t, 3> _scale_{1, 1, 1};

  public:
    /**
     * @brief Default constructor.
     */
    Transform(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Transform(void) override = default;

    /**
     * @brief Set the position vector.
     * @param position The new position vector.
     */
    void setPosition(const utility::Vector<std::size_t, 3> &position) {
        _position_ = position;
    }

    /**
     * @brief Get the position vector.
     * @return Reference to the position vector.
     */
    utility::Vector<std::size_t, 3> &getPosition(void) { return _position_; }

    /**
     * @brief Set the rotation vector.
     * @param rotation The new rotation vector.
     */
    void setRotation(const utility::Vector<std::size_t, 3> &rotation) {
        _rotation_ = rotation;
    }

    /**
     * @brief Get the rotation vector.
     * @return Reference to the rotation vector.
     */
    utility::Vector<std::size_t, 3> &getRotation(void) { return _rotation_; }

    /**
     * @brief Set the scale vector.
     * @param scale The new scale vector.
     */
    void setScale(const utility::Vector<std::size_t, 3> &scale) {
        _scale_ = scale;
    }

    /**
     * @brief Get the scale vector.
     * @return Reference to the scale vector.
     */
    utility::Vector<std::size_t, 3> &getScale(void) { return _scale_; }
};

} // namespace guillaume::components::visual