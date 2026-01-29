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

#include "guillaume/ecs/component.hpp"

namespace guillaume::components {

/**
 * @brief Component representing a focusable entity.
 */
class Focus : public ecs::Component {
  public:
    using Handler = std::function<void(void)>; ///< Focus event handler type

  private:
    Handler _onFocus; ///< Focus event handler

  public:
    /**
     * @brief Default constructor for the Focus component.
     */
    Focus(void) = default;

    /**
     * @brief Default destructor for the Focus component.
     */
    ~Focus(void) = default;

    /**
     * @brief Set the onFocus event handler.
     * @param handler The function to call on focus events.
     */
    void setOnFocusHandler(const Handler &handler) { _onFocus = handler; }

    /**
     * @brief Get the onFocus event handler.
     * @return The onFocus event handler.
     */
    Handler getOnFocusHandler(void) const { return _onFocus; }
};

} // namespace guillaume::components