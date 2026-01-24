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
#include <string>
#include <vector>

#include "ecs/component.hpp"

namespace guillaume::components::functional {

/**
 * @brief Dropdown component defining a dropdown menu.
 */
class Dropdown : public ecs::Component {
  private:
    std::vector<std::string> _options_{};
    std::size_t _selected_index_{0};
    std::function<void(std::size_t)> _on_select_{};

  public:
    /**
     * @brief Default constructor.
     */
    Dropdown(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Dropdown(void) override = default;

    /**
     * @brief Set the options list.
     * @param options The list of options.
     */
    void setOptions(const std::vector<std::string> &options) {
        _options_ = options;
    }

    /**
     * @brief Get the options list.
     * @return The list of options.
     */
    const std::vector<std::string> &getOptions(void) const { return _options_; }

    /**
     * @brief Set the selected index.
     * @param index The selected index.
     */
    void setSelectedIndex(std::size_t index) { _selected_index_ = index; }

    /**
     * @brief Get the selected index.
     * @return The selected index.
     */
    std::size_t getSelectedIndex(void) const { return _selected_index_; }

    /**
     * @brief Get the selected option text.
     * @return The selected option text, or empty string if invalid index.
     */
    std::string getSelectedOption(void) const {
        if (_selected_index_ < _options_.size()) {
            return _options_[_selected_index_];
        }
        return "";
    }

    /**
     * @brief Set the select callback.
     * @param callback The function to call when an option is selected.
     */
    void setOnSelect(std::function<void(std::size_t)> callback) {
        _on_select_ = callback;
    }

    /**
     * @brief Get the select callback.
     * @return The select callback function.
     */
    const std::function<void(std::size_t)> &getOnSelect(void) const {
        return _on_select_;
    }

    /**
     * @brief Trigger the select callback.
     * @param index The index that was selected.
     */
    void select(std::size_t index) {
        _selected_index_ = index;
        if (_on_select_) {
            _on_select_(index);
        }
    }
};

} // namespace guillaume::components::functional
