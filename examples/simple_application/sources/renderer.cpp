#include "renderer.hpp"

namespace simple_application {

Renderer::Renderer(void) : guillaume::Renderer(), _renderer(nullptr) {
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        getLogger().error("Failed to initialize SDL: " +
                          std::string(SDL_GetError()));
    }

    if (!TTF_Init()) {
        getLogger().error("Failed to initialize SDL_ttf: " +
                          std::string(SDL_GetError()));
    }

    SDL_DisplayID primaryDisplayID = SDL_GetPrimaryDisplay();
    if (primaryDisplayID == 0) {
        getLogger().warning("Failed to get primary display ID: " +
                            std::string(SDL_GetError()));
        throw PrimaryDisplayUnavailableException(SDL_GetError());
    }

    SDL_Rect primaryDisplayBounds{};
    if (!SDL_GetDisplayBounds(primaryDisplayID, &primaryDisplayBounds)) {
        getLogger().warning("Failed to get primary display bounds: " +
                            std::string(SDL_GetError()));
        throw DisplayBoundsUnavailableException(SDL_GetError());
    }

    _window = SDL_CreateWindow("Simple Application", primaryDisplayBounds.w,
                               primaryDisplayBounds.h, SDL_WINDOW_FULLSCREEN);
    if (!_window) {
        getLogger().error("Failed to create SDL window: " +
                          std::string(SDL_GetError()));
    }

    _renderer = SDL_CreateRenderer(_window, nullptr);
    if (!_renderer) {
        getLogger().error("Failed to create SDL renderer: " +
                          std::string(SDL_GetError()));
    }
}

Renderer::~Renderer(void) {
    for (auto &pair : _fontCache) {
        if (pair.second) {
            TTF_CloseFont(pair.second);
        }
    }
    _fontCache.clear();

    if (_renderer) {
        SDL_DestroyRenderer(_renderer);
    }
    if (_window) {
        SDL_DestroyWindow(_window);
    }
    TTF_Quit();
    SDL_Quit();
}

void Renderer::clear(void) {
    getLogger().debug("Clearing the screen");
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}

void Renderer::present(void) {
    getLogger().debug("Presenting the back buffer to the screen");
    SDL_RenderPresent(_renderer);
}

void Renderer::drawTriangle(const guillaume::shapes::Triangle &triangle) {
    getLogger().debug("Drawing a triangle shape");

    auto position = triangle.getPosition();
    auto color = triangle.getColor();
    auto scale = triangle.getScale();

    SDL_SetRenderDrawColor(_renderer, color.red(), color.green(), color.blue(), color.alpha());

    float baseSize = 50.0f;
    SDL_FPoint vertices[4] = {
        {position[0], position[1] - baseSize * scale[1]},
        {position[0] - baseSize * scale[0], position[1] + baseSize * scale[1]},
        {position[0] + baseSize * scale[0], position[1] + baseSize * scale[1]},
        {position[0], position[1] - baseSize * scale[1]}
    };

    SDL_RenderLines(_renderer, vertices, 4);
}

void Renderer::drawRectangle(const guillaume::shapes::Rectangle &rectangle) {
    getLogger().debug("Drawing a rectangle shape");

    auto position = rectangle.getPosition();
    auto size = rectangle.getSize();
    auto color = rectangle.getColor();
    auto scale = rectangle.getScale();

    SDL_SetRenderDrawColor(_renderer, color.red(), color.green(), color.blue(), color.alpha());

    SDL_FRect rect = {position[0], position[1], size[0] * scale[0], size[1] * scale[1]};

    SDL_RenderFillRect(_renderer, &rect);
}

void Renderer::drawCircle(const guillaume::shapes::Circle &circle) {
    getLogger().debug("Drawing a circle shape");

    auto position = circle.getPosition();
    auto color = circle.getColor();
    auto scale = circle.getScale();

    SDL_SetRenderDrawColor(_renderer, color.red(), color.green(), color.blue(), color.alpha());

    float radius = 50.0f * scale[0];
    int32_t centerX = static_cast<int32_t>(position[0]);
    int32_t centerY = static_cast<int32_t>(position[1]);

    int32_t x = static_cast<int32_t>(radius);
    int32_t y = 0;
    int32_t radiusError = 1 - x;

    while (x >= y) {
        SDL_RenderPoint(_renderer, centerX + x, centerY + y);
        SDL_RenderPoint(_renderer, centerX + y, centerY + x);
        SDL_RenderPoint(_renderer, centerX - x, centerY + y);
        SDL_RenderPoint(_renderer, centerX - y, centerY + x);
        SDL_RenderPoint(_renderer, centerX - x, centerY - y);
        SDL_RenderPoint(_renderer, centerX - y, centerY - x);
        SDL_RenderPoint(_renderer, centerX + x, centerY - y);
        SDL_RenderPoint(_renderer, centerX + y, centerY - x);

        y++;
        if (radiusError < 0) {
            radiusError += 2 * y + 1;
        } else {
            x--;
            radiusError += 2 * (y - x + 1);
        }
    }
}

utility::math::Vector<std::float_t, 2>
Renderer::mesureText(const guillaume::Text &text,
                     const guillaume::Font &font) {
    getLogger().debug("Measuring text: " + text.getContent() +
                      " with font: " + font.getFontPath());

    TTF_Font *ttfFont = getOrLoadFont(font);
    if (!ttfFont) {
        getLogger().error("Failed to load font for measurement");
        return {0.0f, 0.0f};
    }

    int width = 0, height = 0;
    if (!TTF_GetStringSize(ttfFont, text.getContent().c_str(), 0, &width, &height)) {
        getLogger().error("Failed to measure text: " +
                          std::string(SDL_GetError()));
        return {0.0f, 0.0f};
    }

    return {static_cast<float>(width), static_cast<float>(height)};
}

void Renderer::drawText(const guillaume::Text &text,
                        const guillaume::Font &font) {
    getLogger().debug("Drawing text: " + text.getContent() +
                      " with font: " + font.getFontPath());

    TTF_Font *ttfFont = getOrLoadFont(font);
    if (!ttfFont) {
        getLogger().error("Failed to load font for rendering");
        return;
    }

    auto color = text.getColor();
    SDL_Color sdlColor = {color.red(), color.green(), color.blue(), color.alpha()};

    // Render text to surface
    SDL_Surface *surface =
        TTF_RenderText_Blended(ttfFont, text.getContent().c_str(), 0, sdlColor);
    if (!surface) {
        getLogger().error("Failed to render text to surface: " +
                          std::string(SDL_GetError()));
        return;
    }

    // Create texture from surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);
    if (!texture) {
        getLogger().error("Failed to create texture from surface: " +
                          std::string(SDL_GetError()));
        SDL_DestroySurface(surface);
        return;
    }

    // Set destination rectangle
    auto position = text.getPosition();
    SDL_FRect destRect = {static_cast<float>(position[0]),
                          static_cast<float>(position[1]),
                          static_cast<float>(surface->w),
                          static_cast<float>(surface->h)};

    // Render texture
    SDL_RenderTexture(_renderer, texture, nullptr, &destRect);

    // Clean up
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

TTF_Font *Renderer::getOrLoadFont(const guillaume::Font &font) {
    std::string fontKey = font.getFontPath() + ":" +
                          std::to_string(font.getFontSize());

    // Check if font is already in cache
    auto it = _fontCache.find(fontKey);
    if (it != _fontCache.end()) {
        return it->second;
    }

    // Load font
    TTF_Font *ttfFont = TTF_OpenFont(font.getFontPath().c_str(),
                                      font.getFontSize());
    if (!ttfFont) {
        getLogger().error("Failed to load font: " +
                          std::string(SDL_GetError()));
        return nullptr;
    }

    // Cache the font
    _fontCache[fontKey] = ttfFont;
    return ttfFont;
}

} // namespace simple_application