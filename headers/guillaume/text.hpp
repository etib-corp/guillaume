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

#include <utility/math/color.hpp>
#include <utility/math/rectangle.hpp>
#include <utility/math/vector.hpp>

namespace guillaume {

/**
 * @brief Text alignment options.
 */
enum class TextAlignment { Left, Center, Right };

/**
 * @brief Represents styled text content for rendering.
 *
 * The Text class encapsulates a string of text along with its visual
 * properties such as color and position. It provides a high-level
 * interface for text rendering without directly dealing with low-level
 * rendering APIs.
 * @see Renderer
 * @see Font
 */
class Text {
  private:
    std::string _content;
    utility::math::Color<uint8_t> _color;
    utility::math::Vector<float, 3> _position;
    utility::math::Vector<float, 3> _rotation;
    TextAlignment _alignment;
    bool _isDirty;
    utility::math::Rectangle<std::size_t> _bounds;

  public:
    /**
     * @brief Construct a new Text object.
     * @param content The text content to display.
     * @param color The color of the text.
     * @param position The position where the text should be rendered.
     * @param rotation The rotation of the text in degrees.
     */
    Text(const std::string &content,
         utility::math::Color<uint8_t> color =
             utility::math::Color<uint8_t>(255, 255, 255, 255),
         utility::math::Vector<float, 3> position =
             utility::math::Vector<float, 3>{0.0f, 0.0f, 0.0f},
         utility::math::Vector<float, 3> rotation =
             utility::math::Vector<float, 3>{0.0f, 0.0f, 0.0f});

    /**
     * @brief Default destructor.
     */
    ~Text() = default;

    /**
     * @brief Get the text content.
     * @return The text content string.
     */
    const std::string &getContent() const;

    /**
     * @brief Set the text content.
     * @param content The new text content.
     */
    void setContent(const std::string &content);

    /**
     * @brief Get the text color.
     * @return The current text color.
     */
    const utility::math::Color<uint8_t> &getColor() const;

    /**
     * @brief Set the text color.
     * @param color The new text color.
     */
    void setColor(const utility::math::Color<uint8_t> &color);

    /**
     * @brief Get the text position.
     * @return The current position as a 3D vector.
     */
    const utility::math::Vector<float, 3> &getPosition() const;

    /**
     * @brief Set the text position.
     * @param position The new position as a 3D vector.
     */
    void setPosition(const utility::math::Vector<float, 3> &position);

    /**
     * @brief Get the text rotation.
     * @return The current rotation as a 3D vector (in degrees).
     */
    const utility::math::Vector<float, 3> &getRotation() const;

    /**
     * @brief Set the text rotation.
     * @param rotation The new rotation as a 3D vector (in degrees).
     */
    void setRotation(const utility::math::Vector<float, 3> &rotation);

    /**
     * @brief Get the text alignment.
     * @return The current text alignment.
     */
    TextAlignment getAlignment() const;

    /**
     * @brief Set the text alignment.
     * @param alignment The new text alignment.
     */
    void setAlignment(TextAlignment alignment);

    /**
     * @brief Check if the text properties have changed since last render.
     * @return True if the text is dirty and needs re-rendering.
     */
    bool isDirty() const;

    /**
     * @brief Mark the text as clean (up-to-date with rendering).
     */
    void markClean();

    /**
     * @brief Mark the text as dirty (needs re-rendering).
     */
    void markDirty();

    /**
     * @brief Get the bounding rectangle of the text.
     * @return The bounding rectangle in pixels.
     */
    const utility::math::Rectangle<std::size_t> &getBounds() const;

    /**
     * @brief Set the bounding rectangle of the text.
     * @param bounds The bounding rectangle.
     */
    void setBounds(const utility::math::Rectangle<std::size_t> &bounds);

    /**
     * @brief Check if the text is empty.
     * @return True if the content is empty.
     */
    bool isEmpty() const;

    /**
     * @brief Get the length of the text content.
     * @return The number of characters in the text.
     */
    std::size_t length() const;
};

/**
 * @brief Shared pointer type for Text objects.
 */
using TextPtr = std::shared_ptr<Text>;

} // namespace guillaume
