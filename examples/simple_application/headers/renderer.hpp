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

#include <unordered_map>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <utility/math/color.hpp>
#include <utility/math/rectangle.hpp>
#include <utility/logging/standard_logger.hpp>
#include <utility/math/vector.hpp>

#include <guillaume/font.hpp>
#include <guillaume/renderer.hpp>
#include <guillaume/text.hpp>

namespace simple_application {

class Renderer : public guillaume::Renderer {
  private:
    utility::logging::StandardLogger _logger;
    utility::math::Color<uint8_t> _drawColor;
    utility::math::Vector<float, 2> _scale;
    SDL_Renderer *_sdlRenderer;
    std::unordered_map<std::string, TTF_Font *> _fontCache;

    TTF_Font *loadTTFFont(const std::string &fontPath, int fontSize);

  public:
    Renderer(void);
    Renderer(SDL_Renderer *sdlRenderer);
    ~Renderer(void) override;

    void clear(void) override;
    void present(void) override;
    void setDrawColor(utility::math::Color<uint8_t> color) override;
    utility::math::Color<uint8_t> getDrawColor(void) const override;
    void drawPoint(utility::math::Vector<std::size_t, 2> point) override;
    void drawLine(utility::math::Vector<std::size_t, 2> start,
                  utility::math::Vector<std::size_t, 2> end) override;
    void drawRect(utility::math::Rectangle<std::size_t> rectangle) override;
    bool drawText(const guillaume::Text &text) override;
    std::shared_ptr<guillaume::Font> loadFont(const std::string &fontPath,
                                              int fontSize) override;
    utility::math::Rectangle<std::size_t>
    measureText(const std::string &text,
                std::shared_ptr<guillaume::Font> font) override;
    void fillRect(utility::math::Rectangle<std::size_t> rectangle) override;
    void setScale(utility::math::Vector<float, 2> scale) override;
    utility::math::Vector<float, 2> getScale(void) const override;
    bool flush(void) override;

    SDL_Renderer *getSDLRenderer(void) const { return _sdlRenderer; }
    void setSDLRenderer(SDL_Renderer *renderer);
};

} // namespace simple_application
