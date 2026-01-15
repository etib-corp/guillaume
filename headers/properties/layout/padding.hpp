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

#include "properties/layout_property.hpp"

namespace guillaume::properties::layout {

/**
 * @brief Padding layout property class.
 */
class Padding : public guillaume::properties::LayoutProperty {
  private:
    std::size_t _left;
    std::size_t _right;
    std::size_t _top;
    std::size_t _bottom;

  public:
    /**
     * @brief Default constructor with 10px padding.
     */
    Padding(void) : _left(10), _right(10), _top(10), _bottom(10) {}

    /**
     * @brief Parameterized constructor.
     * @param left Left padding.
     * @param right Right padding.
     * @param top Top padding.
     * @param bottom Bottom padding.
     */
    Padding(std::size_t left, std::size_t right, std::size_t top,
            std::size_t bottom)
        : _left(left), _right(right), _top(top), _bottom(bottom) {}

    /**
     * @brief Set the left padding.
     * @param value The left padding value.
     */
    void setLeft(std::size_t value) { _left = value; }

    /**
     * @brief Get the left padding.
     * @return The left padding value.
     */
    std::size_t getLeft(void) const { return _left; }

    /**
     * @brief Set the right padding.
     * @param value The right padding value.
     */
    void setRight(std::size_t value) { _right = value; }

    /**
     * @brief Get the right padding.
     * @return The right padding value.
     */
    std::size_t getRight(void) const { return _right; }

    /**
     * @brief Set the top padding.
     * @param value The top padding value.
     */
    void setTop(std::size_t value) { _top = value; }

    /**
     * @brief Get the top padding.
     * @return The top padding value.
     */
    std::size_t getTop(void) const { return _top; }

    /**
     * @brief Set the bottom padding.
     * @param value The bottom padding value.
     */
    void setBottom(std::size_t value) { _bottom = value; }

    /**
     * @brief Get the bottom padding.
     * @return The bottom padding value.
     */
    std::size_t getBottom(void) const { return _bottom; }

    /**
     * @brief Apply the property.
     * @param component The component to apply the padding to.
     */
    void apply(Component &component) override;
};

} // namespace guillaume::properties::layout
