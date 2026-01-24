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

#include <functional>

#include "ecs/component.hpp"

namespace guillaume::components::interaction {

/**
 * @brief Clickable component marking an entity as clickable.
 */
class Clickable : public ecs::Component {
  private:
    std::function<void()> _on_click_{};

  public:
    /**
     * @brief Default constructor.
     */
    Clickable(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Clickable(void) override = default;

    /**
     * @brief Set the click callback.
     * @param callback The function to call when clicked.
     */
    void setOnClick(std::function<void()> callback) { _on_click_ = callback; }

    /**
     * @brief Get the click callback.
     * @return The click callback function.
     */
    const std::function<void()> &getOnClick(void) const { return _on_click_; }

    /**
     * @brief Trigger the click callback.
     */
    void click(void) const {
        if (_on_click_) {
            _on_click_();
        }
    }
};

} // namespace guillaume::components::interaction
