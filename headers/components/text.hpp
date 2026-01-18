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

#include <memory>
#include <string>

#include <color.hpp>
#include <font.hpp>
#include <text.hpp>
#include <vector.hpp>

#include "component.hpp"

namespace guillaume::component {

/**
 * @brief A text component for rendering styled text within the UI system.
 *
 * The Text component integrates the graphics Text class into the Guillaume
 * component system, allowing text to be styled with properties and rendered
 * as part of a component hierarchy.
 */
class Text : public Component {
  private:
    std::shared_ptr<guillaume::Text> _text; ///< The graphics Text object
    std::shared_ptr<Font> _font;            ///< Associated font

  public:
    /**
     * @brief Construct a Text component with content and font.
     * @param content The text content to display.
     * @param font The font to use for rendering.
     */
    Text(const std::string &content, std::shared_ptr<Font> font);

    /**
     * @brief Construct a Text component with custom styling.
     * @param content The text content to display.
     * @param font The font to use for rendering.
     * @param color The text color.
     * @param position The text position.
     */
    Text(const std::string &content, std::shared_ptr<Font> font,
         utility::Color<uint8_t> color,
         utility::Vector<std::size_t, 2> position);

    /**
     * @brief Default destructor
     */
    virtual ~Text(void) = default;

    /**
     * @brief Get the text content.
     * @return The current text content.
     */
    const std::string &getContent() const;

    /**
     * @brief Set the text content.
     * @param content The new text content.
     */
    void setContent(const std::string &content);

    /**
     * @brief Get the font used by this component.
     * @return Shared pointer to the font.
     */
    std::shared_ptr<Font> getFont() const;

    /**
     * @brief Set the font to use for rendering.
     * @param font The new font.
     */
    void setFont(std::shared_ptr<Font> font);

    /**
     * @brief Get the text color.
     * @return The current text color.
     */
    utility::Color<uint8_t> getColor() const;

    /**
     * @brief Set the text color.
     * @param color The new text color.
     */
    void setColor(const utility::Color<uint8_t> &color);

    /**
     * @brief Get the text position.
     * @return The current position as a 2D vector.
     */
    utility::Vector<std::size_t, 2> getPosition() const;

    /**
     * @brief Set the text position.
     * @param position The new position.
     */
    void setPosition(const utility::Vector<std::size_t, 2> &position);

    /**
     * @brief Get the underlying graphics Text object.
     * @return Shared pointer to the graphics Text object.
     */
    std::shared_ptr<guillaume::Text> getText() const;

    /**
     * @brief Render the text component.
     * @param renderer The renderer to use for rendering.
     */
    void render(Renderer &renderer) override;
};

} // namespace guillaume::component
