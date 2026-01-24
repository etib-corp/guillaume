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

#include "ecs/component.hpp"

namespace guillaume::components::layout {

/**
 * @brief Scrollable component enabling scrolling for an entity.
 */
class Scrollable : public ecs::Component {
  private:
    std::size_t _scroll_position_{0};
    std::size_t _content_size_{0};

  public:
    /**
     * @brief Default constructor.
     */
    Scrollable(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Scrollable(void) override = default;

    /**
     * @brief Set the scroll position.
     * @param position The scroll position.
     */
    void setScrollPosition(std::size_t position) {
        _scroll_position_ = position;
    }

    /**
     * @brief Get the scroll position.
     * @return The scroll position.
     */
    std::size_t getScrollPosition(void) const { return _scroll_position_; }

    /**
     * @brief Set the content size.
     * @param size The content size.
     */
    void setContentSize(std::size_t size) { _content_size_ = size; }

    /**
     * @brief Get the content size.
     * @return The content size.
     */
    std::size_t getContentSize(void) const { return _content_size_; }
};

} // namespace guillaume::components::layout
