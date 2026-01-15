#include "simple_window.hpp"

#include <algorithm>
#include <sstream>
#include <string>

namespace {

template <typename Type>
std::string formatVector(const utility::Vector<Type, 2> &vector) {
    std::ostringstream stream;
    stream << "(" << vector[0] << ", " << vector[1] << ")";
    return stream.str();
}

} // namespace

namespace guillaume::simple_application {

SimpleWindow::SimpleWindow()
    : _logger(), _title("Guillaume Simple Window"), _minimumSize({100, 100}),
      _maximumSize({3840, 2160}), _opacity(1.0F), _visible(false),
      _resizable(true), _minimumAspectRatio(0.0F), _maximumAspectRatio(0.0F),
      _state(WindowState::Normal), _sdlWindow(nullptr), _shouldClose(false) {

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

    _logger.info("SimpleWindow created with SDL3 ('" + _title + "')");
}

SimpleWindow::~SimpleWindow() {
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

SimpleRenderer &SimpleWindow::getRenderer(void) { return _renderer; }

std::string SimpleWindow::getTitle(void) const { return _title; }

void SimpleWindow::setTitle(const std::string &title) {
    _title = title;
    if (_sdlWindow) {
        if (!SDL_SetWindowTitle(_sdlWindow, title.c_str())) {
            _logger.error(std::string("SDL_SetWindowTitle failed: ") +
                          SDL_GetError());
        }
    }
    _logger.info("Window title set to '" + _title + "'");
}

void SimpleWindow::setMinimumSize(utility::Vector<std::size_t, 2> size) {
    _minimumSize = size;
    if (_sdlWindow) {
        if (!SDL_SetWindowMinimumSize(_sdlWindow, static_cast<int>(size[0]),
                                      static_cast<int>(size[1]))) {
            _logger.error(std::string("SDL_SetWindowMinimumSize failed: ") +
                          SDL_GetError());
        }
    }
    _logger.debug("Window minimum size set to " + formatVector(size));
}

utility::Vector<std::size_t, 2> SimpleWindow::getMinimumSize() const {
    return _minimumSize;
}

void SimpleWindow::setMaximumSize(utility::Vector<std::size_t, 2> size) {
    _maximumSize = size;
    if (_sdlWindow) {
        if (!SDL_SetWindowMaximumSize(_sdlWindow, static_cast<int>(size[0]),
                                      static_cast<int>(size[1]))) {
            _logger.error(std::string("SDL_SetWindowMaximumSize failed: ") +
                          SDL_GetError());
        }
    }
    _logger.debug("Window maximum size set to " + formatVector(size));
}

utility::Vector<std::size_t, 2> SimpleWindow::getMaximumSize(void) const {
    return _maximumSize;
}

void SimpleWindow::show(void) {
    _visible = true;
    if (_sdlWindow) {
        if (!SDL_ShowWindow(_sdlWindow)) {
            _logger.error(std::string("SDL_ShowWindow failed: ") +
                          SDL_GetError());
        }
    }
    _logger.info("Window shown");
}

void SimpleWindow::hide(void) {
    _visible = false;
    if (_sdlWindow) {
        if (!SDL_HideWindow(_sdlWindow)) {
            _logger.error(std::string("SDL_HideWindow failed: ") +
                          SDL_GetError());
        }
    }
    _logger.info("Window hidden");
}

bool SimpleWindow::isVisible(void) const {
    if (_sdlWindow) {
        return (SDL_GetWindowFlags(_sdlWindow) & SDL_WINDOW_HIDDEN) == 0;
    }
    return _visible;
}

void SimpleWindow::maximize(void) {
    _state = WindowState::Maximized;
    if (_sdlWindow) {
        if (!SDL_MaximizeWindow(_sdlWindow)) {
            _logger.error(std::string("SDL_MaximizeWindow failed: ") +
                          SDL_GetError());
        }
    }
    _logger.info("Window maximized");
}

void SimpleWindow::minimize(void) {
    _state = WindowState::Minimized;
    if (_sdlWindow) {
        if (!SDL_MinimizeWindow(_sdlWindow)) {
            _logger.error(std::string("SDL_MinimizeWindow failed: ") +
                          SDL_GetError());
        }
    }
    _logger.info("Window minimized");
}

void SimpleWindow::restore(void) {
    _state = WindowState::Normal;
    if (_sdlWindow) {
        if (!SDL_RestoreWindow(_sdlWindow)) {
            _logger.error(std::string("SDL_RestoreWindow failed: ") +
                          SDL_GetError());
        }
    }
    _logger.info("Window restored");
}

void SimpleWindow::setOpacity(float opacity) {
    _opacity = std::clamp(opacity, 0.0F, 1.0F);
    if (_sdlWindow) {
        if (!SDL_SetWindowOpacity(_sdlWindow, _opacity)) {
            _logger.error(std::string("SDL_SetWindowOpacity failed: ") +
                          SDL_GetError());
        }
    }
    _logger.debug("Window opacity set to " + std::to_string(_opacity));
}

float SimpleWindow::getOpacity(void) const {
    if (_sdlWindow) {
        return SDL_GetWindowOpacity(_sdlWindow);
    }
    return _opacity;
}

void SimpleWindow::requestClose(void) {
    _shouldClose = true;
    _visible = false;
    _logger.info("Window close requested");
}

void SimpleWindow::setAspectRatio(float minimumAspectRatio, float) {
    _minimumAspectRatio = minimumAspectRatio;
    _maximumAspectRatio = minimumAspectRatio;

    if (_sdlWindow && minimumAspectRatio > 0.0F) {
        if (!SDL_SetWindowAspectRatio(_sdlWindow, minimumAspectRatio,
                                      minimumAspectRatio)) {
            _logger.error(std::string("SDL_SetWindowAspectRatio failed: ") +
                          SDL_GetError());
        }
    }
    _logger.debug(
        "Window aspect ratio set min=" + std::to_string(_minimumAspectRatio) +
        " max=" + std::to_string(_maximumAspectRatio));
}

void SimpleWindow::getAspectRatio(float &minimumAspectRatio,
                                  float &maximumAspectRatio) const {
    if (_sdlWindow) {
        float minAspect, maxAspect;
        if (SDL_GetWindowAspectRatio(_sdlWindow, &minAspect, &maxAspect)) {
            minimumAspectRatio = minAspect;
            maximumAspectRatio = maxAspect;
            return;
        }
    }
    minimumAspectRatio = _minimumAspectRatio;
    maximumAspectRatio = _maximumAspectRatio;
}

void SimpleWindow::setResizable(bool resizable) {
    _resizable = resizable;
    if (_sdlWindow) {
        if (!SDL_SetWindowResizable(_sdlWindow, resizable)) {
            _logger.error(std::string("SDL_SetWindowResizable failed: ") +
                          SDL_GetError());
        }
    }
    _logger.info(std::string("Window resizable set to ") +
                 (_resizable ? "true" : "false"));
}

bool SimpleWindow::sync(void) {
    if (_sdlWindow) {
        if (!SDL_SyncWindow(_sdlWindow)) {
            _logger.error(std::string("SDL_SyncWindow failed: ") +
                          SDL_GetError());
            return false;
        }
    }
    _logger.debug("Window sync complete");
    return true;
}

} // namespace guillaume::simple_application
