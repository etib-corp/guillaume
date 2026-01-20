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

#include "component.hpp"

namespace guillaume::components::functional {

/**
 * @brief InputField component for text input.
 */
class InputField : public Component {
  private:
    std::string _placeholder_{};
    std::size_t _max_length_{0};
    std::function<void(const std::string &)> _on_submit_{};
    std::string _text_{};

  public:
    /**
     * @brief Default constructor.
     */
    InputField(void) = default;

    /**
     * @brief Default destructor.
     */
    ~InputField(void) override = default;

    /**
     * @brief Set the placeholder text.
     * @param placeholder The placeholder text.
     */
    void setPlaceholder(const std::string &placeholder) {
        _placeholder_ = placeholder;
    }

    /**
     * @brief Get the placeholder text.
     * @return The placeholder text.
     */
    const std::string &getPlaceholder(void) const { return _placeholder_; }

    /**
     * @brief Set the maximum length.
     * @param max_length The maximum length (0 for unlimited).
     */
    void setMaxLength(std::size_t max_length) { _max_length_ = max_length; }

    /**
     * @brief Get the maximum length.
     * @return The maximum length (0 for unlimited).
     */
    std::size_t getMaxLength(void) const { return _max_length_; }

    /**
     * @brief Set the submit callback.
     * @param callback The function to call when submitted.
     */
    void setOnSubmit(std::function<void(const std::string &)> callback) {
        _on_submit_ = callback;
    }

    /**
     * @brief Get the submit callback.
     * @return The submit callback function.
     */
    const std::function<void(const std::string &)> &getOnSubmit(void) const {
        return _on_submit_;
    }

    /**
     * @brief Set the current text.
     * @param text The current text.
     */
    void setText(const std::string &text) { _text_ = text; }

    /**
     * @brief Get the current text.
     * @return The current text.
     */
    const std::string &getText(void) const { return _text_; }

    /**
     * @brief Trigger the submit callback.
     */
    void submit(void) const {
        if (_on_submit_) {
            _on_submit_(_text_);
        }
    }
};

} // namespace guillaume::components::functional
