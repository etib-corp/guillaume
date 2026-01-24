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

#include "ecs/component.hpp"

namespace guillaume::components::interaction {

/**
 * @brief Draggable component allowing an entity to be dragged.
 */
class Draggable : public ecs::Component {
  private:
    bool _is_dragging_{false};
    utility::Vector<int, 2> _drag_offset_{0, 0};

  public:
    /**
     * @brief Default constructor.
     */
    Draggable(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Draggable(void) override = default;

    /**
     * @brief Set the dragging state.
     * @param is_dragging True if currently being dragged.
     */
    void setIsDragging(bool is_dragging) { _is_dragging_ = is_dragging; }

    /**
     * @brief Check if currently being dragged.
     * @return True if being dragged.
     */
    bool isDragging(void) const { return _is_dragging_; }

    /**
     * @brief Set the drag offset.
     * @param offset The offset vector.
     */
    void setDragOffset(const utility::Vector<int, 2> &offset) {
        _drag_offset_ = offset;
    }

    /**
     * @brief Get the drag offset.
     * @return Reference to the drag offset vector.
     */
    utility::Vector<int, 2> &getDragOffset(void) { return _drag_offset_; }

    /**
     * @brief Get the drag offset (const).
     * @return Const reference to the drag offset vector.
     */
    const utility::Vector<int, 2> &getDragOffset(void) const {
        return _drag_offset_;
    }
};

} // namespace guillaume::components::interaction
