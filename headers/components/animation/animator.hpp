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

#include "component.hpp"

namespace guillaume::components::animation {

/**
 * @brief Animator component handling animation data.
 */
class Animator : public Component {
  private:
    std::size_t _current_frame_{0};
    double _animation_speed_{1.0};

  public:
    /**
     * @brief Default constructor.
     */
    Animator(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Animator(void) override = default;

    /**
     * @brief Set the current frame.
     * @param frame The current frame number.
     */
    void setCurrentFrame(std::size_t frame) { _current_frame_ = frame; }

    /**
     * @brief Get the current frame.
     * @return The current frame number.
     */
    std::size_t getCurrentFrame(void) const { return _current_frame_; }

    /**
     * @brief Set the animation speed.
     * @param speed The animation speed multiplier.
     */
    void setAnimationSpeed(double speed) { _animation_speed_ = speed; }

    /**
     * @brief Get the animation speed.
     * @return The animation speed multiplier.
     */
    double getAnimationSpeed(void) const { return _animation_speed_; }
};

} // namespace guillaume::components::animation
