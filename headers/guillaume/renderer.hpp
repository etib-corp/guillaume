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

#include <functional>
#include <optional>
#include <string>
#include <vector>

#include <utility/logging/loggable.hpp>
#include <utility/math/color.hpp>
#include <utility/math/rectangle.hpp>
#include <utility/math/vector.hpp>

#include "guillaume/font.hpp"
#include "guillaume/text.hpp"

namespace guillaume {

/**
 * @brief Renderer interface.
 */
class Renderer : public utility::logging::Loggable {
  public:
    /**
     * @brief Default constructor
     */
    Renderer(void) {}

    /**
     * @brief Default destructor
     */
    virtual ~Renderer(void) = default;

    /**
     * @brief Clear the current rendering target with the drawing color.
     */
    virtual void clear(void) = 0;

    /**
     * @brief Present the composed back buffer to the screen.
     */
    virtual void present(void) = 0;

    /**
     * @brief Set the color used for drawing operations.
     * @param color The color to use for drawing.
     */
    virtual void setDrawColor(utility::math::Color<uint8_t> color) = 0;

    /**
     * @brief Get the color used for drawing operations.
     * @return The current draw color.
     */
    virtual utility::math::Color<uint8_t> getDrawColor(void) const = 0;

    /**
     * @brief Draw a point at the specified position.
     * @param point The position of the point to draw as a 2D integer vector.
     */
    virtual void drawPoint(utility::math::Vector<std::size_t, 2> point) = 0;

    /**
     * @brief Draw a line between two points.
     * @param start The starting point of the line as a 2D integer vector.
     * @param end The ending point of the line as a 2D integer vector.
     */
    virtual void drawLine(utility::math::Vector<std::size_t, 2> start,
                          utility::math::Vector<std::size_t, 2> end) = 0;

    /**
     * @brief Draw a rectangle outline.
     * @param rectangle The rectangle to draw.
     */
    virtual void drawRect(utility::math::Rectangle<std::size_t> rectangle) = 0;

    /**
     * @brief Draw a filled rectangle.
     * @param rectangle The rectangle to fill.
     */
    virtual void fillRect(utility::math::Rectangle<std::size_t> rectangle) = 0;

    /**
     * @brief Set the drawing scale for rendering operations.
     * @param scale The scaling factors as a 2D std::size_t vector (scale_x,
     * scale_y).
     */
    virtual void setScale(utility::math::Vector<float, 2> scale) = 0;

    /**
     * @brief Get the current drawing scale.
     * @return The current scaling factors as a 2D std::size_t vector.
     */
    virtual utility::math::Vector<float, 2> getScale(void) const = 0;

    /**
     * @brief Force the rendering context to flush pending commands.
     * @return True on success, false on failure.
     */
    virtual bool flush(void) = 0;

    /**
     * @brief Draw a Text object.
     * @param text The Text object containing content, styling, and position
     * information.
     * @return True on success, false on failure.
     */
    virtual bool drawText(const Text &text) = 0;

    /**
     * @brief Load a font for use in rendering.
     * @param fontPath Path to the font file.
     * @param fontSize Size of the font in points.
     * @return Shared pointer to the loaded Font, or nullptr on failure.
     */
    virtual std::shared_ptr<Font> loadFont(const std::string &fontPath,
                                           int fontSize) = 0;

    /**
     * @brief Measure the bounds of text when rendered with a specific font.
     * @param text The text string to measure.
     * @param font The font to use for measurement.
     * @return The bounding rectangle of the rendered text.
     */
    virtual utility::math::Rectangle<std::size_t>
    measureText(const std::string &text, std::shared_ptr<Font> font) = 0;
};

} // namespace guillaume
