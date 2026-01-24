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

#include "ecs/component.hpp"

namespace guillaume::components::functional {

/**
 * @brief ProgressBar component displaying progress.
 */
class ProgressBar : public ecs::Component {
  private:
    double _min_value_{0.0};
    double _max_value_{100.0};
    double _current_value_{0.0};

  public:
    /**
     * @brief Default constructor.
     */
    ProgressBar(void) = default;

    /**
     * @brief Default destructor.
     */
    ~ProgressBar(void) override = default;

    /**
     * @brief Set the minimum value.
     * @param min_value The minimum value.
     */
    void setMinValue(double min_value) { _min_value_ = min_value; }

    /**
     * @brief Get the minimum value.
     * @return The minimum value.
     */
    double getMinValue(void) const { return _min_value_; }

    /**
     * @brief Set the maximum value.
     * @param max_value The maximum value.
     */
    void setMaxValue(double max_value) { _max_value_ = max_value; }

    /**
     * @brief Get the maximum value.
     * @return The maximum value.
     */
    double getMaxValue(void) const { return _max_value_; }

    /**
     * @brief Set the current value.
     * @param current_value The current value.
     */
    void setCurrentValue(double current_value) {
        _current_value_ = current_value;
    }

    /**
     * @brief Get the current value.
     * @return The current value.
     */
    double getCurrentValue(void) const { return _current_value_; }

    /**
     * @brief Get the progress percentage (0.0 to 1.0).
     * @return The progress as a percentage.
     */
    double getProgress(void) const {
        if (_max_value_ == _min_value_) {
            return 0.0;
        }
        return (_current_value_ - _min_value_) / (_max_value_ - _min_value_);
    }
};

} // namespace guillaume::components::functional
