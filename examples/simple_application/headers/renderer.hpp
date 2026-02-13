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

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <exception>
#include <string>
#include <unordered_map>

#include <guillaume/renderer.hpp>

namespace simple_application {

class PrimaryDisplayUnavailableException : public std::exception {
  private:
    std::string _message;

  public:
    explicit PrimaryDisplayUnavailableException(const std::string &details)
        : _message("SDL_GetPrimaryDisplay failed: " + details) {}

    const char *what(void) const noexcept override { return _message.c_str(); }
};

class DisplayBoundsUnavailableException : public std::exception {
  private:
    std::string _message;

  public:
    explicit DisplayBoundsUnavailableException(const std::string &details)
        : _message("SDL_GetDisplayBounds failed: " + details) {}

    const char *what(void) const noexcept override { return _message.c_str(); }
};

class Renderer : public guillaume::Renderer {
  private:
    SDL_Window *_window;     ///< SDL window pointer
    SDL_Renderer *_renderer; ///< SDL renderer pointer
    std::unordered_map<std::string, TTF_Font *>
        _fontCache; ///< Cache for loaded fonts


    TTF_Font *getOrLoadFont(const guillaume::Font &font);

  public:
    /**
     * @brief Default constructor for the Renderer class.
     * Initializes SDL, creates a window and renderer, and sets up font caching.
     */
    Renderer(void);

    /**
     * @brief Destructor for the Renderer class.
     * Cleans up SDL resources and cached fonts.
     */
    ~Renderer(void) override;

    /**
     * @brief Clears the renderer's drawing surface.
     */
    void clear(void) override;

    /**
     * @brief Presents the rendered content to the display.
     */
    void present(void) override;

    /**
     * @brief Draws a triangle shape using the SDL renderer.
     * @param triangle The triangle shape to draw.
     */
    void drawTriangle(const guillaume::shapes::Triangle &triangle) override;

    /**
     * @brief Draws a rectangle shape using the SDL renderer.
     * @param rectangle The rectangle shape to draw.
     */
    void drawRectangle(const guillaume::shapes::Rectangle &rectangle) override;

    /**
     * @brief Draws a circle shape using the SDL renderer.
     * @param circle The circle shape to draw.
     */
    void drawCircle(const guillaume::shapes::Circle &circle) override;

    /**
     * @brief Measures the pixel dimensions of a given text string when rendered with a specific font.
     * @param text The text to measure.
     * @param font The font to use for measurement.
     * @return A 2D vector containing the width and height of the rendered text in pixels in the form of utility::math::Vector<std::float_t, 2>.
     */
    utility::math::Vector<std::float_t, 2>
    mesureText(const guillaume::Text &text,
               const guillaume::Font &font) override;

    /**
     * @brief Draws text using a specific font.
     * @param text The text to draw.
     * @param font The font to use for rendering the text.
     */
    void drawText(const guillaume::Text &text,
                  const guillaume::Font &font) override;
};

} // namespace simple_application