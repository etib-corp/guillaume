#include <sstream>
#include <string>

#include "renderer.hpp"

namespace {

std::string formatColor(const utility::math::Color<uint8_t> &color) {
    std::ostringstream stream;
    stream << "rgba(" << static_cast<int>(color.red()) << ", "
           << static_cast<int>(color.green()) << ", "
           << static_cast<int>(color.blue()) << ", "
           << static_cast<int>(color.alpha()) << ")";
    return stream.str();
}

template <typename Type>
std::string formatVector(const utility::math::Vector<Type, 2> &vector) {
    std::ostringstream stream;
    stream << "(" << vector[0] << ", " << vector[1] << ")";
    return stream.str();
}

} // namespace

namespace simple_application {

Renderer::Renderer()
    : guillaume::Renderer(), _logger(), _drawColor(0, 0, 0, 255),
      _scale({1.0F, 1.0F}), _sdlRenderer(nullptr), _fontCache() {
    if (!TTF_Init()) {
        _logger.error(std::string("TTF_Init failed: ") + SDL_GetError());
    }
    _logger.info("Renderer initialized (no SDL renderer yet)");
}

Renderer::Renderer(SDL_Renderer *sdlRenderer)
    : guillaume::Renderer(), _logger(), _drawColor(0, 0, 0, 255),
      _scale({1.0F, 1.0F}), _sdlRenderer(sdlRenderer), _fontCache() {
    if (!TTF_Init()) {
        _logger.error(std::string("TTF_Init failed: ") + SDL_GetError());
    }

    if (_sdlRenderer) {
        // Get initial draw color from SDL
        Uint8 r, g, b, a;
        if (SDL_GetRenderDrawColor(_sdlRenderer, &r, &g, &b, &a)) {
            _drawColor = utility::math::Color<uint8_t>(r, g, b, a);
        }

        // Get initial scale from SDL
        float scaleX, scaleY;
        if (SDL_GetRenderScale(_sdlRenderer, &scaleX, &scaleY)) {
            _scale = {scaleX, scaleY};
        }

        _logger.info("Renderer initialized with SDL3 renderer");
    }
}

Renderer::~Renderer() {
    // Clean up font cache
    for (auto &pair : _fontCache) {
        if (pair.second) {
            TTF_CloseFont(pair.second);
        }
    }
    _fontCache.clear();

    TTF_Quit();

    // SDL_Renderer is owned and destroyed by the window
    _sdlRenderer = nullptr;
}

void Renderer::setSDLRenderer(SDL_Renderer *renderer) {
    _sdlRenderer = renderer;
    if (_sdlRenderer) {
        _logger.info("SDL3 renderer set");
        // Set initial draw color
        SDL_SetRenderDrawColor(_sdlRenderer, _drawColor.red(),
                               _drawColor.green(), _drawColor.blue(),
                               _drawColor.alpha());
        // Set initial scale
        SDL_SetRenderScale(_sdlRenderer, _scale[0], _scale[1]);
    }
}

void Renderer::clear(void) {
    if (_sdlRenderer) {
        if (!SDL_RenderClear(_sdlRenderer)) {
            _logger.error(std::string("SDL_RenderClear failed: ") +
                          SDL_GetError());
        }
    } else {
        _logger.debug("Renderer clear (no SDL)");
    }
}

void Renderer::present(void) {
    if (_sdlRenderer) {
        if (!SDL_RenderPresent(_sdlRenderer)) {
            _logger.error(std::string("SDL_RenderPresent failed: ") +
                          SDL_GetError());
        }
    } else {
        _logger.debug("Renderer present (no SDL)");
    }
}

void Renderer::setDrawColor(utility::math::Color<uint8_t> color) {
    _drawColor = color;
    if (_sdlRenderer) {
        if (!SDL_SetRenderDrawColor(_sdlRenderer, color.red(), color.green(),
                                    color.blue(), color.alpha())) {
            _logger.error(std::string("SDL_SetRenderDrawColor failed: ") +
                          SDL_GetError());
        }
    } else {
        _logger.debug("Renderer draw color set to " + formatColor(color));
    }
}

utility::math::Color<uint8_t> Renderer::getDrawColor(void) const {
    return _drawColor;
}

void Renderer::drawPoint(utility::math::Vector<std::size_t, 2> point) {
    if (_sdlRenderer) {
        if (!SDL_RenderPoint(_sdlRenderer, static_cast<float>(point[0]),
                             static_cast<float>(point[1]))) {
            _logger.error(std::string("SDL_RenderPoint failed: ") +
                          SDL_GetError());
        }
    } else {
        _logger.debug("Renderer draw point at " + formatVector(point));
    }
}

void Renderer::drawLine(utility::math::Vector<std::size_t, 2> start,
                        utility::math::Vector<std::size_t, 2> end) {
    if (_sdlRenderer) {
        if (!SDL_RenderLine(_sdlRenderer, static_cast<float>(start[0]),
                            static_cast<float>(start[1]),
                            static_cast<float>(end[0]),
                            static_cast<float>(end[1]))) {
            _logger.error(std::string("SDL_RenderLine failed: ") +
                          SDL_GetError());
        }
    } else {
        _logger.debug("Renderer draw line from " + formatVector(start) +
                      " to " + formatVector(end));
    }
}

void Renderer::drawRect(utility::math::Rectangle<std::size_t> rectangle) {
    if (_sdlRenderer) {
        SDL_FRect rect{static_cast<float>(rectangle.x()),
                       static_cast<float>(rectangle.y()),
                       static_cast<float>(rectangle.width()),
                       static_cast<float>(rectangle.height())};
        if (!SDL_RenderRect(_sdlRenderer, &rect)) {
            _logger.error(std::string("SDL_RenderRect failed: ") +
                          SDL_GetError());
        }
    } else {
        _logger.debug(
            "Renderer draw rectangle (x=" + std::to_string(rectangle.x()) +
            ", y=" + std::to_string(rectangle.y()) +
            ", w=" + std::to_string(rectangle.width()) +
            ", h=" + std::to_string(rectangle.height()) + ")");
    }
}

void Renderer::fillRect(utility::math::Rectangle<std::size_t> rectangle) {
    if (_sdlRenderer) {
        SDL_FRect rect{static_cast<float>(rectangle.x()),
                       static_cast<float>(rectangle.y()),
                       static_cast<float>(rectangle.width()),
                       static_cast<float>(rectangle.height())};
        if (!SDL_RenderFillRect(_sdlRenderer, &rect)) {
            _logger.error(std::string("SDL_RenderFillRect failed: ") +
                          SDL_GetError());
        }
    } else {
        _logger.debug(
            "Renderer fill rectangle (x=" + std::to_string(rectangle.x()) +
            ", y=" + std::to_string(rectangle.y()) +
            ", w=" + std::to_string(rectangle.width()) +
            ", h=" + std::to_string(rectangle.height()) + ")");
    }
}

void Renderer::setScale(utility::math::Vector<float, 2> scale) {
    _scale = scale;
    if (_sdlRenderer) {
        if (!SDL_SetRenderScale(_sdlRenderer, scale[0], scale[1])) {
            _logger.error(std::string("SDL_SetRenderScale failed: ") +
                          SDL_GetError());
        }
    } else {
        _logger.debug("Renderer scale set to " + formatVector(scale));
    }
}

utility::math::Vector<float, 2> Renderer::getScale(void) const { return _scale; }
TTF_Font *Renderer::loadTTFFont(const std::string &fontPath, int fontSize) {
    std::string cacheKey = fontPath + ":" + std::to_string(fontSize);

    // Check if font is already in cache
    auto it = _fontCache.find(cacheKey);
    if (it != _fontCache.end()) {
        return it->second;
    }

    // Load the font
    TTF_Font *font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) {
        _logger.error(std::string("TTF_OpenFont failed for ") + fontPath +
                      ": " + SDL_GetError());
        return nullptr;
    }

    // Cache the font
    _fontCache[cacheKey] = font;
    _logger.info("Loaded and cached font: " + fontPath + " at size " +
                 std::to_string(fontSize));
    return font;
}

bool Renderer::flush(void) {
    if (_sdlRenderer) {
        if (!SDL_FlushRenderer(_sdlRenderer)) {
            _logger.error(std::string("SDL_FlushRenderer failed: ") +
                          SDL_GetError());
            return false;
        }
    } else {
        _logger.debug("Renderer flush (no SDL)");
    }
    return true;
}

bool Renderer::drawText(const guillaume::Text &text) {
    if (text.isEmpty() || !text.getFont()) {
        return false;
    }

    if (!_sdlRenderer) {
        _logger.debug("Renderer draw text (no SDL)");
        return false;
    }

    // Save current draw color
    auto originalColor = _drawColor;

    // Set the text color
    setDrawColor(text.getColor());

    // Get the font handle
    TTF_Font *font = static_cast<TTF_Font *>(text.getFont()->getNativeHandle());
    if (!font) {
        _logger.error("Invalid font handle in Text object");
        setDrawColor(originalColor);
        return false;
    }

    // Create surface from text
    SDL_Color textColor = {text.getColor().red(), text.getColor().green(),
                           text.getColor().blue(), text.getColor().alpha()};
    SDL_Surface *textSurface = TTF_RenderText_Blended(
        font, text.getContent().c_str(), text.getContent().length(), textColor);
    if (!textSurface) {
        _logger.error(std::string("TTF_RenderText_Blended failed: ") +
                      SDL_GetError());
        setDrawColor(originalColor);
        return false;
    }

    // Create texture from surface
    SDL_Texture *textTexture =
        SDL_CreateTextureFromSurface(_sdlRenderer, textSurface);
    if (!textTexture) {
        _logger.error(std::string("SDL_CreateTextureFromSurface failed: ") +
                      SDL_GetError());
        SDL_DestroySurface(textSurface);
        setDrawColor(originalColor);
        return false;
    }

    // Get texture dimensions
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_DestroySurface(textSurface);

    // Render texture to screen
    auto position = text.getPosition();
    SDL_FRect destRect{
        static_cast<float>(position[0]), static_cast<float>(position[1]),
        static_cast<float>(textWidth), static_cast<float>(textHeight)};

    if (!SDL_RenderTexture(_sdlRenderer, textTexture, nullptr, &destRect)) {
        _logger.error(std::string("SDL_RenderTexture failed: ") +
                      SDL_GetError());
        SDL_DestroyTexture(textTexture);
        setDrawColor(originalColor);
        return false;
    }

    SDL_DestroyTexture(textTexture);

    // Restore original draw color
    setDrawColor(originalColor);

    return true;
}

std::shared_ptr<guillaume::Font> Renderer::loadFont(const std::string &fontPath,
                                                    int fontSize) {
    TTF_Font *ttfFont = loadTTFFont(fontPath, fontSize);
    if (!ttfFont) {
        return nullptr;
    }

    auto font = std::make_shared<guillaume::Font>(fontPath, fontSize);
    font->setNativeHandle(ttfFont);

    return font;
}

utility::math::Rectangle<std::size_t>
Renderer::measureText(const std::string &text,
                      std::shared_ptr<guillaume::Font> font) {
    if (!font || !font->isValid()) {
        return utility::math::Rectangle<std::size_t>(0, 0, 0, 0);
    }

    TTF_Font *ttfFont = static_cast<TTF_Font *>(font->getNativeHandle());
    if (!ttfFont) {
        return utility::math::Rectangle<std::size_t>(0, 0, 0, 0);
    }

    int width = 0, height = 0;
    if (!TTF_GetStringSize(ttfFont, text.c_str(), text.length(), &width,
                           &height)) {
        _logger.error(std::string("TTF_GetStringSize failed: ") +
                      SDL_GetError());
        return utility::math::Rectangle<std::size_t>(0, 0, 0, 0);
    }

    return utility::math::Rectangle<std::size_t>(0, 0,
                                           static_cast<std::size_t>(width),
                                           static_cast<std::size_t>(height));
}

} // namespace simple_application
