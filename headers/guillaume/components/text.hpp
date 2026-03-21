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

#include <string>

#include <utility/math/color.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components {

/**
 * @brief Component representing a text element.
 * @see systems::KeyboardControl
 * @see systems::TextInput
 * @see guillaume::Text
 */
class Text : public ecs::Component {
  private:
    std::string _content;       ///< Text content
    std::size_t _fontSize = 24; ///< Font size of the text
    utility::math::Color<uint8_t> _color = {255, 255, 255,
                                            255}; ///< Color of the text (RGBA)

  public:
    /**
     * @brief Default constructor for the Text component.
     */
    Text(void) = default;

    /**
     * @brief Default destructor for the Text component.
     */
    ~Text(void) = default;

    /**
     * @brief Get the text content.
     * @return The text content.
     */
    const std::string &getContent(void) const { return _content; }

    /**
     * @brief Set the text content.
     * @param content The new text content.
     * @return The Text component for chaining.
     */
    Text &setContent(const std::string &content) {
        _content = content;
        return *this;
    }

    /**
     * @brief Get the font size of the text.
     * @return The font size.
     */
    std::size_t getFontSize(void) const { return _fontSize; }

    /**
     * @brief Set the font size of the text.
     * @param fontSize The new font size.
     * @return The Text component for chaining.
     */
    Text &setFontSize(std::size_t fontSize) {
        _fontSize = (fontSize == 0) ? 24 : fontSize;
        return *this;
    }

    /**
     * @brief Get the color of the text.
     * @return The text color.
     */
    utility::math::Color<uint8_t> getColor(void) const { return _color; }

    /**
     * @brief Set the color of the text.
     * @param color The new text color.
     * @return The Text component for chaining.
     */
    Text &setColor(utility::math::Color<uint8_t> color) {
        _color = color;
        return *this;
    }
};

} // namespace guillaume::components