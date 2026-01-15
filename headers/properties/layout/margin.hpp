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
 * @brief Margin layout property class.
 */
class Margin : public guillaume::properties::LayoutProperty {
  private:
    std::size_t _left;
    std::size_t _right;
    std::size_t _top;
    std::size_t _bottom;

  public:
    /**
     * @brief Default constructor with 10px margins.
     */
    Margin() : _left(10), _right(10), _top(10), _bottom(10) {}

    /**
     * @brief Parameterized constructor.
     * @param left Left margin.
     * @param right Right margin.
     * @param top Top margin.
     * @param bottom Bottom margin.
     */
    Margin(std::size_t left, std::size_t right, std::size_t top,
           std::size_t bottom)
        : _left(left), _right(right), _top(top), _bottom(bottom) {}

    /**
     * @brief Set the left margin.
     * @param value The left margin value.
     */
    void setLeft(std::size_t value) { _left = value; }

    /**
     * @brief Get the left margin.
     * @return The left margin value.
     */
    std::size_t getLeft(void) const { return _left; }

    /**
     * @brief Set the right margin.
     * @param value The right margin value.
     */
    void setRight(std::size_t value) { _right = value; }

    /**
     * @brief Get the right margin.
     * @return The right margin value.
     */
    std::size_t getRight(void) const { return _right; }

    /**
     * @brief Set the top margin.
     * @param value The top margin value.
     */
    void setTop(std::size_t value) { _top = value; }

    /**
     * @brief Get the top margin.
     * @return The top margin value.
     */
    std::size_t getTop(void) const { return _top; }

    /**
     * @brief Set the bottom margin.
     * @param value The bottom margin value.
     */
    void setBottom(std::size_t value) { _bottom = value; }

    /**
     * @brief Get the bottom margin.
     * @return The bottom margin value.
     */
    std::size_t getBottom(void) const { return _bottom; }

    /**
     * @brief Apply the property.
     * @param component The component to apply the margin to.
     */
    void apply(Component &component) override;
};

} // namespace guillaume::properties::layout
