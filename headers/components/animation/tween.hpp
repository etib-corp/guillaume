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
#include <functional>

#include "component.hpp"

namespace guillaume::components::animation {

/**
 * @brief Tween component for smooth transitions.
 */
class Tween : public Component {
  public:
    /**
     * @brief Easing function types for smooth value transitions.
     */
    enum class EasingFunction {
        LINEAR,      /**< Linear interpolation (no easing) */
        EASE_IN,     /**< Accelerating from zero velocity */
        EASE_OUT,    /**< Decelerating to zero velocity */
        EASE_IN_OUT  /**< Acceleration until halfway, then deceleration */
    };

  private:
    double _target_value_{0.0};
    std::size_t _duration_{0};
    EasingFunction _easing_function_{EasingFunction::LINEAR};

  public:
    /**
     * @brief Default constructor.
     */
    Tween(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Tween(void) override = default;

    /**
     * @brief Set the target value.
     * @param value The target value to tween towards.
     */
    void setTargetValue(double value) { _target_value_ = value; }

    /**
     * @brief Get the target value.
     * @return The target value.
     */
    double getTargetValue(void) const { return _target_value_; }

    /**
     * @brief Set the duration of the tween (in milliseconds).
     * @param duration The duration value.
     */
    void setDuration(std::size_t duration) { _duration_ = duration; }

    /**
     * @brief Get the duration of the tween.
     * @return The duration value (in milliseconds).
     */
    std::size_t getDuration(void) const { return _duration_; }

    /**
     * @brief Set the easing function.
     * @param function The easing function to use.
     */
    void setEasingFunction(EasingFunction function) {
        _easing_function_ = function;
    }

    /**
     * @brief Get the easing function.
     * @return The easing function.
     */
    EasingFunction getEasingFunction(void) const { return _easing_function_; }
};

} // namespace guillaume::components::animation
