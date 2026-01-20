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

#include <cstddef>

#include "component.hpp"

namespace guillaume::components::visual {

/**
 * @brief Rectangle component defining a rectangular area.
 */
class Rectangle : public Component {
  private:
    std::size_t _width_{0};
    std::size_t _height_{0};

  public:
    /**
     * @brief Default constructor.
     */
    Rectangle(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Rectangle(void) override = default;

    /**
     * @brief Set the width.
     * @param width The width value.
     */
    void setWidth(std::size_t width) { _width_ = width; }

    /**
     * @brief Get the width.
     * @return The width value.
     */
    std::size_t getWidth(void) const { return _width_; }

    /**
     * @brief Set the height.
     * @param height The height value.
     */
    void setHeight(std::size_t height) { _height_ = height; }

    /**
     * @brief Get the height.
     * @return The height value.
     */
    std::size_t getHeight(void) const { return _height_; }
};

} // namespace guillaume::components::visual
