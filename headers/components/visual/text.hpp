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

#include <color.hpp>

#include "ecs/component.hpp"

namespace guillaume::components::visual {

/**
 * @brief Text alignment options.
 */
enum class TextAlignment { Left, Center, Right };

/**
 * @brief Text component representing styled text content.
 */
class Text : public ecs::Component {
  private:
    std::string _content_{""};
    std::string _font_{"Arial"};
    float _size_{16.0f};
    utility::Color<std::uint8_t> _color_{255, 255, 255, 255};
    TextAlignment _alignment_{TextAlignment::Left};

  public:
    /**
     * @brief Default constructor.
     */
    Text(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Text(void) override = default;

    /**
     * @brief Set the text content.
     * @param content The new text content.
     */
    void setContent(const std::string &content) { _content_ = content; }

    /**
     * @brief Get the text content.
     * @return Reference to the text content.
     */
    std::string &getContent(void) { return _content_; }

    /**
     * @brief Set the font name.
     * @param font The new font name.
     */
    void setFont(const std::string &font) { _font_ = font; }

    /**
     * @brief Get the font name.
     * @return Reference to the font name.
     */
    std::string &getFont(void) { return _font_; }

    /**
     * @brief Set the font size.
     * @param size The new font size.
     */
    void setSize(float size) { _size_ = size; }

    /**
     * @brief Get the font size.
     * @return The font size.
     */
    float getSize(void) const { return _size_; }

    /**
     * @brief Set the text color.
     * @param color The new text color.
     */
    void setColor(const utility::Color<std::uint8_t> &color) {
        _color_ = color;
    }

    /**
     * @brief Get the text color.
     * @return Reference to the text color.
     */
    utility::Color<std::uint8_t> &getColor(void) { return _color_; }

    /**
     * @brief Set the text alignment.
     * @param alignment The new text alignment.
     */
    void setAlignment(TextAlignment alignment) { _alignment_ = alignment; }

    /**
     * @brief Get the text alignment.
     * @return The text alignment.
     */
    TextAlignment getAlignment(void) const { return _alignment_; }
};

} // namespace guillaume::components::visual
