#include "renderer.hpp"

namespace simple_application {

Renderer::Renderer(void) : guillaume::Renderer() {
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        getLogger().error("Failed to initialize SDL: " +
                          std::string(SDL_GetError()));
    }

    SDL_DisplayID primaryDisplayID = SDL_GetPrimaryDisplay();
    if (primaryDisplayID == 0) {
        getLogger().warning("Failed to get primary display ID: " +
                            std::string(SDL_GetError()));
        throw std::runtime_error("SDL_GetPrimaryDisplay failed");
    }

    SDL_Rect primaryDisplayBounds{};
    if (!SDL_GetDisplayBounds(primaryDisplayID, &primaryDisplayBounds)) {
        getLogger().warning("Failed to get primary display bounds: " +
                            std::string(SDL_GetError()));
        throw std::runtime_error("SDL_GetDisplayBounds failed");
    }

    // Create SDL window
    _window = SDL_CreateWindow("Simple Application", primaryDisplayBounds.w,
                               primaryDisplayBounds.h, SDL_WINDOW_FULLSCREEN);
    if (!_window) {
        getLogger().error("Failed to create SDL window: " +
                          std::string(SDL_GetError()));
    }
}

Renderer::~Renderer(void) {
    if (_window) {
        SDL_DestroyWindow(_window);
    }
    SDL_Quit();
}

void Renderer::clear(void) { getLogger().debug("Clearing the screen"); }

void Renderer::present(void) {
    getLogger().debug("Presenting the back buffer to the screen");
}

void Renderer::drawTriangle(const guillaume::shapes::Triangle &triangle) {
    getLogger().debug("Drawing a triangle shape");
}

void Renderer::drawRectangle(const guillaume::shapes::Rectangle &rectangle) {
    getLogger().debug("Drawing a rectangle shape");
}

void Renderer::drawCircle(const guillaume::shapes::Circle &circle) {
    getLogger().debug("Drawing a circle shape");
}

utility::math::Vector<std::float_t, 2>
Renderer::mesureText(const guillaume::Text &text, const guillaume::Font &font) {
    getLogger().debug("Measuring text: " + text.getContent() +
                      " with font: " + font.getFontPath());
    return {0.0f, 0.0f};
}

void Renderer::drawText(const guillaume::Text &text,
                        const guillaume::Font &font) {
    getLogger().debug("Drawing text: " + text.getContent() +
                      " with font: " + font.getFontPath());
}

} // namespace simple_application