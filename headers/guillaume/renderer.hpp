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
#include <utility/logging/standard_logger.hpp>

#include "guillaume/shapes/circle.hpp"
#include "guillaume/shapes/rectangle.hpp"
#include "guillaume/shapes/triangle.hpp"

#include "guillaume/font.hpp"
#include "guillaume/text.hpp"

namespace guillaume {

/**
 * @brief Renderer interface.
 */
class Renderer
    : protected utility::logging::Loggable<Renderer,
                                           utility::logging::StandardLogger> {
  public:
    /**
     * @brief Default constructor
     */
    Renderer(void);

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
     * @brief Draw a triangle shape.
     * @param triangle The triangle to draw.
     */
    virtual void drawTriangle(const shapes::Triangle &triangle) = 0;

    /**
     * @brief Draw a rectangle shape.
     * @param rectangle The rectangle to draw.
     */
    virtual void drawRectangle(const shapes::Rectangle &rectangle) = 0;

    /**
     * @brief Draw a circle shape.
     * @param circle The circle to draw.
     */
    virtual void drawCircle(const shapes::Circle &circle) = 0;

    /**
     * @brief Measure the size of the given text using the specified font.
     * @param text The text to measure.
     * @param font The font to use for measurement.
     * @return The size of the text as a 2D vector (width, height).
     */
    virtual utility::math::Vector<std::float_t, 2>
    mesureText(const Text &text, const Font &font) = 0;

    /**
     * @brief Draw text using a specific font.
     * @param text The text to draw.
     * @param font The font to use.
     */
    virtual void drawText(const Text &text, const Font &font) = 0;
};

} // namespace guillaume
