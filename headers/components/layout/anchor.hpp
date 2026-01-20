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

#include "component.hpp"

namespace guillaume::components::layout {

/**
 * @brief Anchor component to anchor an entity to a parent or screen edge.
 */
class Anchor : public Component {
  public:
    /**
     * @brief Anchor point positions for element alignment.
     */
    enum class AnchorPoint {
        TOP_LEFT,     /**< Top-left corner */
        TOP_CENTER,   /**< Top-center position */
        TOP_RIGHT,    /**< Top-right corner */
        CENTER_LEFT,  /**< Center-left position */
        CENTER,       /**< Center position */
        CENTER_RIGHT, /**< Center-right position */
        BOTTOM_LEFT,  /**< Bottom-left corner */
        BOTTOM_CENTER,/**< Bottom-center position */
        BOTTOM_RIGHT  /**< Bottom-right corner */
    };

  private:
    AnchorPoint _anchor_point_{AnchorPoint::TOP_LEFT};

  public:
    /**
     * @brief Default constructor.
     */
    Anchor(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Anchor(void) override = default;

    /**
     * @brief Set the anchor point.
     * @param point The anchor point.
     */
    void setAnchorPoint(AnchorPoint point) { _anchor_point_ = point; }

    /**
     * @brief Get the anchor point.
     * @return The anchor point.
     */
    AnchorPoint getAnchorPoint(void) const { return _anchor_point_; }
};

} // namespace guillaume::components::layout
