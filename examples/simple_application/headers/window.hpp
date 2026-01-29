#pragma once

#include <string>

#include <SDL3/SDL.h>

#include <utility/math/rectangle.hpp>
#include <utility/logging/standard_logger.hpp>
#include <utility/math/vector.hpp>

#include <guillaume/window.hpp>

#include "renderer.hpp"

namespace simple_application {

class Window : public guillaume::Window<Renderer> {
  private:
    enum class WindowState { Normal, Minimized, Maximized };

    Renderer _renderer;
    utility::logging::StandardLogger _logger;
    std::string _title;
    utility::math::Vector<std::size_t, 2> _minimumSize;
    utility::math::Vector<std::size_t, 2> _maximumSize;
    float _opacity;
    bool _visible;
    bool _resizable;
    float _minimumAspectRatio;
    float _maximumAspectRatio;
    WindowState _state;

    SDL_Window *_sdlWindow;
    bool _shouldClose;

  public:
    Window(void)
        : guillaume::Window<Renderer>(), _title("Guillaume Simple Window"),
          _minimumSize({100, 100}), _maximumSize({3840, 2160}), _opacity(1.0F),
          _visible(false), _resizable(true), _minimumAspectRatio(0.0F),
          _maximumAspectRatio(0.0F), _state(WindowState::Normal),
          _sdlWindow(nullptr), _shouldClose(false) {

        // Initialize SDL3
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            _logger.error(std::string("SDL_Init failed: ") + SDL_GetError());
            throw std::runtime_error("Failed to initialize SDL3");
        }

        // Create SDL window (800x600, resizable by default)
        _sdlWindow =
            SDL_CreateWindow(_title.c_str(), 800, 600, SDL_WINDOW_RESIZABLE);
        if (!_sdlWindow) {
            _logger.error(std::string("SDL_CreateWindow failed: ") +
                          SDL_GetError());
            SDL_Quit();
            throw std::runtime_error("Failed to create SDL3 window");
        }

        // Create SDL renderer
        SDL_Renderer *sdlRenderer = SDL_CreateRenderer(_sdlWindow, nullptr);
        if (!sdlRenderer) {
            _logger.error(std::string("SDL_CreateRenderer failed: ") +
                          SDL_GetError());
            SDL_DestroyWindow(_sdlWindow);
            SDL_Quit();
            throw std::runtime_error("Failed to create SDL3 renderer");
        }

        // Initialize renderer with SDL_Renderer
        _renderer.setSDLRenderer(sdlRenderer);

        _logger.info("Window created with SDL3 ('" + _title + "')");
    }

    ~Window(void) {
        if (_renderer.getSDLRenderer()) {
            SDL_DestroyRenderer(_renderer.getSDLRenderer());
            _logger.debug("SDL renderer destroyed");
        }
        if (_sdlWindow) {
            SDL_DestroyWindow(_sdlWindow);
            _logger.debug("SDL window destroyed");
        }
        SDL_Quit();
        _logger.info("SDL3 quit");
    }

    std::string getTitle(void) const override;
    void setTitle(const std::string &title) override;
    void setMinimumSize(utility::math::Vector<std::size_t, 2> size) override;
    utility::math::Vector<std::size_t, 2> getMinimumSize() const override;
    void setMaximumSize(utility::math::Vector<std::size_t, 2> size) override;
    utility::math::Vector<std::size_t, 2> getMaximumSize(void) const override;
    void show(void) override;
    void hide(void) override;
    bool isVisible(void) const override;
    void maximize(void) override;
    void minimize(void) override;
    void restore(void) override;
    void setOpacity(float opacity) override;
    float getOpacity(void) const override;
    void requestClose(void) override;
    void setAspectRatio(float minimumAspectRatio,
                        float maximumAspectRatio) override;
    void getAspectRatio(float &minimumAspectRatio,
                        float &maximumAspectRatio) const override;
    void setResizable(bool resizable) override;
    bool sync(void) override;

    SDL_Window *getSDLWindow() const { return _sdlWindow; }
    bool shouldClose() const { return _shouldClose; }
};

} // namespace simple_application
