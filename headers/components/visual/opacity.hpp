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

#include <cstdint>

#include "ecs/component.hpp"

namespace guillaume::components::visual {

/**
 * @brief Opacity component defining transparency level.
 */
class Opacity : public ecs::Component {
  private:
    std::uint8_t _alpha_{255};

  public:
    /**
     * @brief Default constructor.
     */
    Opacity(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Opacity(void) override = default;

    /**
     * @brief Set the alpha value (transparency).
     * @param alpha The alpha value (0-255, where 0 is transparent and 255 is
     * opaque).
     */
    void setAlpha(std::uint8_t alpha) { _alpha_ = alpha; }

    /**
     * @brief Get the alpha value.
     * @return The alpha value (0-255).
     */
    std::uint8_t getAlpha(void) const { return _alpha_; }
};

} // namespace guillaume::components::visual
