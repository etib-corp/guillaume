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

namespace guillaume::components::state {

/**
 * @brief Tooltip component adding tooltip functionality.
 */
class Tooltip : public Component {
  private:
    std::string _tooltip_text_{};
    std::size_t _delay_{500};

  public:
    /**
     * @brief Default constructor.
     */
    Tooltip(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Tooltip(void) override = default;

    /**
     * @brief Set the tooltip text.
     * @param text The tooltip text.
     */
    void setTooltipText(const std::string &text) { _tooltip_text_ = text; }

    /**
     * @brief Get the tooltip text.
     * @return The tooltip text.
     */
    const std::string &getTooltipText(void) const { return _tooltip_text_; }

    /**
     * @brief Set the delay before showing tooltip (in milliseconds).
     * @param delay The delay value.
     */
    void setDelay(std::size_t delay) { _delay_ = delay; }

    /**
     * @brief Get the delay before showing tooltip.
     * @return The delay value (in milliseconds).
     */
    std::size_t getDelay(void) const { return _delay_; }
};

} // namespace guillaume::components::state
