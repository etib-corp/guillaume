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
#include <string>

#include "component.hpp"

namespace guillaume::components::layout {

/**
 * @brief Layout component defining layout rules for UI elements.
 */
class Layout : public Component {
  public:
    /**
     * @brief Direction for flex layout arrangement.
     */
    enum class FlexDirection {
        ROW,    /**< Arrange elements horizontally */
        COLUMN  /**< Arrange elements vertically */
    };

  private:
    FlexDirection _flex_direction_{FlexDirection::ROW};
    std::size_t _margin_{0};
    std::size_t _padding_{0};

  public:
    /**
     * @brief Default constructor.
     */
    Layout(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Layout(void) override = default;

    /**
     * @brief Set the flex direction.
     * @param direction The flex direction (ROW or COLUMN).
     */
    void setFlexDirection(FlexDirection direction) {
        _flex_direction_ = direction;
    }

    /**
     * @brief Get the flex direction.
     * @return The flex direction.
     */
    FlexDirection getFlexDirection(void) const { return _flex_direction_; }

    /**
     * @brief Set the margin.
     * @param margin The margin value.
     */
    void setMargin(std::size_t margin) { _margin_ = margin; }

    /**
     * @brief Get the margin.
     * @return The margin value.
     */
    std::size_t getMargin(void) const { return _margin_; }

    /**
     * @brief Set the padding.
     * @param padding The padding value.
     */
    void setPadding(std::size_t padding) { _padding_ = padding; }

    /**
     * @brief Get the padding.
     * @return The padding value.
     */
    std::size_t getPadding(void) const { return _padding_; }
};

} // namespace guillaume::components::layout
