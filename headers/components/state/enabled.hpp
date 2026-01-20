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
 * @brief Enabled component tracking whether an entity is interactive.
 */
class Enabled : public Component {
  private:
    bool _is_enabled_{true};

  public:
    /**
     * @brief Default constructor.
     */
    Enabled(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Enabled(void) override = default;

    /**
     * @brief Set the enabled state.
     * @param is_enabled True if enabled.
     */
    void setIsEnabled(bool is_enabled) { _is_enabled_ = is_enabled; }

    /**
     * @brief Check if enabled.
     * @return True if enabled.
     */
    bool isEnabled(void) const { return _is_enabled_; }
};

} // namespace guillaume::components::state
