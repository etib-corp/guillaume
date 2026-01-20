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

namespace guillaume::components::state {

/**
 * @brief Visibility component tracking whether an entity is visible.
 */
class Visibility : public Component {
  private:
    bool _is_visible_{true};

  public:
    /**
     * @brief Default constructor.
     */
    Visibility(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Visibility(void) override = default;

    /**
     * @brief Set the visibility state.
     * @param is_visible True if visible.
     */
    void setIsVisible(bool is_visible) { _is_visible_ = is_visible; }

    /**
     * @brief Check if visible.
     * @return True if visible.
     */
    bool isVisible(void) const { return _is_visible_; }
};

} // namespace guillaume::components::state
