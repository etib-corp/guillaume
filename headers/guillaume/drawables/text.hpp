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

#include "guillaume/drawable.hpp"

namespace guillaume::drawables {

/**
 * @brief Class representing a text drawable shape.
 * @see Drawable
 */
class Text : public Drawable {
  private:
    std::string _content;  ///< Text content
    std::size_t _fontSize; ///< Font size of the text

  public:
    /**
     * @brief Default constructor
     */
    Text(void);

    /**
     * @brief Parameterized constructor
     * @param position Initial position of the text.
     * @param rotation Initial rotation of the text (in degrees).
     * @param scale Initial scale of the text.
     * @param color Initial color of the text (RGBA).
     * @param content The text content.
     * @param fontSize Font size of the text.
     */
    Text(Position position, Rotation rotation, Scale scale, Color color,
         const std::string &content, std::size_t fontSize);

    /**
     * @brief Destructor
     */
    ~Text(void);

    /**
     * @brief Set the text content.
     * @param content The new text content.
     * @return The Text drawable for chaining.
     */
    Text &setContent(const std::string &content) {
        _content = content;
        return *this;
    }

    /**
     * @brief Get the text content.
     * @return The text content.
     */
    const std::string &getContent(void) const { return _content; }

    /**
     * @brief Set the font size of the text.
     * @param fontSize The new font size.
     * @return The text for chaining.
     */
    Text &setFontSize(std::size_t fontSize) {
        _fontSize = fontSize;
        return *this;
    }

    /**
     * @brief Get the font size of the text.
     * @return The font size.
     */
    std::size_t getFontSize(void) const { return _fontSize; }
};

} // namespace guillaume::drawables