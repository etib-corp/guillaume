#include <algorithm>
#include <sstream>
#include <string>

#include "window.hpp"

namespace {

template <typename Type>
std::string formatVector(const utility::math::Vector<Type, 2> &vector) {
    std::ostringstream stream;
    stream << "(" << vector[0] << ", " << vector[1] << ")";
    return stream.str();
}

} // namespace

namespace simple_application {

std::string Window::getTitle(void) const { return _title; }

void Window::setTitle(const std::string &title) {
    _title = title;
    if (_sdlWindow) {
        if (!SDL_SetWindowTitle(_sdlWindow, title.c_str())) {
            _logger.error(std::string("SDL_SetWindowTitle failed: ") +
                          SDL_GetError());
        } else {
            _logger.debug("SDL window title updated to '" + _title + "'");
        }
    } else {
        _logger.debug("SDL window unavailable; caching title '" + _title + "'");
    }
    _logger.info("Window title set to '" + _title + "'");
}

void Window::setMinimumSize(utility::math::Vector<std::size_t, 2> size) {
    _minimumSize = size;
    if (_sdlWindow) {
        if (!SDL_SetWindowMinimumSize(_sdlWindow, static_cast<int>(size[0]),
                                      static_cast<int>(size[1]))) {
            _logger.error(std::string("SDL_SetWindowMinimumSize failed: ") +
                          SDL_GetError());
        } else {
            _logger.debug("SDL minimum size applied: " + formatVector(size));
        }
    } else {
        _logger.debug("SDL window unavailable; caching minimum size " +
                      formatVector(size));
    }
    _logger.debug("Window minimum size set to " + formatVector(size));
}

utility::math::Vector<std::size_t, 2> Window::getMinimumSize() const {
    return _minimumSize;
}

void Window::setMaximumSize(utility::math::Vector<std::size_t, 2> size) {
    _maximumSize = size;
    if (_sdlWindow) {
        if (!SDL_SetWindowMaximumSize(_sdlWindow, static_cast<int>(size[0]),
                                      static_cast<int>(size[1]))) {
            _logger.error(std::string("SDL_SetWindowMaximumSize failed: ") +
                          SDL_GetError());
        } else {
            _logger.debug("SDL maximum size applied: " + formatVector(size));
        }
    } else {
        _logger.debug("SDL window unavailable; caching maximum size " +
                      formatVector(size));
    }
    _logger.debug("Window maximum size set to " + formatVector(size));
}

utility::math::Vector<std::size_t, 2> Window::getMaximumSize(void) const {
    return _maximumSize;
}

void Window::show(void) {
    _visible = true;
    if (_sdlWindow) {
        if (!SDL_ShowWindow(_sdlWindow)) {
            _logger.error(std::string("SDL_ShowWindow failed: ") +
                          SDL_GetError());
        } else {
            _logger.debug("SDL window show request accepted");
        }
    } else {
        _logger.debug("SDL window unavailable; marking as visible only");
    }
    _logger.info("Window shown");
}

void Window::hide(void) {
    _visible = false;
    if (_sdlWindow) {
        if (!SDL_HideWindow(_sdlWindow)) {
            _logger.error(std::string("SDL_HideWindow failed: ") +
                          SDL_GetError());
        } else {
            _logger.debug("SDL window hide request accepted");
        }
    } else {
        _logger.debug("SDL window unavailable; marking as hidden only");
    }
    _logger.info("Window hidden");
}

bool Window::isVisible(void) const {
    if (_sdlWindow) {
        return (SDL_GetWindowFlags(_sdlWindow) & SDL_WINDOW_HIDDEN) == 0;
    }
    return _visible;
}

void Window::maximize(void) {
    _state = WindowState::Maximized;
    if (_sdlWindow) {
        if (!SDL_MaximizeWindow(_sdlWindow)) {
            _logger.error(std::string("SDL_MaximizeWindow failed: ") +
                          SDL_GetError());
        } else {
            _logger.debug("SDL window maximized");
        }
    } else {
        _logger.debug("SDL window unavailable; tracking maximized state");
    }
    _logger.info("Window maximized");
}

void Window::minimize(void) {
    _state = WindowState::Minimized;
    if (_sdlWindow) {
        if (!SDL_MinimizeWindow(_sdlWindow)) {
            _logger.error(std::string("SDL_MinimizeWindow failed: ") +
                          SDL_GetError());
        } else {
            _logger.debug("SDL window minimized");
        }
    } else {
        _logger.debug("SDL window unavailable; tracking minimized state");
    }
    _logger.info("Window minimized");
}

void Window::restore(void) {
    _state = WindowState::Normal;
    if (_sdlWindow) {
        if (!SDL_RestoreWindow(_sdlWindow)) {
            _logger.error(std::string("SDL_RestoreWindow failed: ") +
                          SDL_GetError());
        } else {
            _logger.debug("SDL window restored");
        }
    } else {
        _logger.debug("SDL window unavailable; tracking restored state");
    }
    _logger.info("Window restored");
}

void Window::setOpacity(float opacity) {
    _opacity = std::clamp(opacity, 0.0F, 1.0F);
    if (_sdlWindow) {
        if (!SDL_SetWindowOpacity(_sdlWindow, _opacity)) {
            _logger.error(std::string("SDL_SetWindowOpacity failed: ") +
                          SDL_GetError());
        } else {
            _logger.debug("SDL opacity applied: " + std::to_string(_opacity));
        }
    } else {
        _logger.debug("SDL window unavailable; caching opacity " +
                      std::to_string(_opacity));
    }
    _logger.debug("Window opacity set to " + std::to_string(_opacity));
}

float Window::getOpacity(void) const {
    if (_sdlWindow) {
        return SDL_GetWindowOpacity(_sdlWindow);
    }
    return _opacity;
}

void Window::requestClose(void) {
    _shouldClose = true;
    _visible = false;
    _logger.info("Window close requested");
}

void Window::setAspectRatio(float minimumAspectRatio, float) {
    _minimumAspectRatio = minimumAspectRatio;
    _maximumAspectRatio = minimumAspectRatio;

    if (_sdlWindow && minimumAspectRatio > 0.0F) {
        if (!SDL_SetWindowAspectRatio(_sdlWindow, minimumAspectRatio,
                                      minimumAspectRatio)) {
            _logger.error(std::string("SDL_SetWindowAspectRatio failed: ") +
                          SDL_GetError());
        } else {
            _logger.debug("SDL aspect ratio applied: " +
                          std::to_string(minimumAspectRatio));
        }
    } else if (!_sdlWindow) {
        _logger.debug("SDL window unavailable; caching aspect ratio " +
                      std::to_string(minimumAspectRatio));
    }
    _logger.debug(
        "Window aspect ratio set min=" + std::to_string(_minimumAspectRatio) +
        " max=" + std::to_string(_maximumAspectRatio));
}

void Window::getAspectRatio(float &minimumAspectRatio,
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

void Window::setResizable(bool resizable) {
    _resizable = resizable;
    if (_sdlWindow) {
        if (!SDL_SetWindowResizable(_sdlWindow, resizable)) {
            _logger.error(std::string("SDL_SetWindowResizable failed: ") +
                          SDL_GetError());
        } else {
            _logger.debug(std::string("SDL resizable flag applied: ") +
                          (_resizable ? "true" : "false"));
        }
    } else {
        _logger.debug(
            std::string("SDL window unavailable; caching resizable to ") +
            (_resizable ? "true" : "false"));
    }
    _logger.info(std::string("Window resizable set to ") +
                 (_resizable ? "true" : "false"));
}

bool Window::sync(void) {
    if (_sdlWindow) {
        if (!SDL_SyncWindow(_sdlWindow)) {
            _logger.error(std::string("SDL_SyncWindow failed: ") +
                          SDL_GetError());
            return false;
        }
        _logger.debug("SDL window sync completed");
    } else {
        _logger.debug("SDL window unavailable; sync is a no-op");
    }
    _logger.debug("Window sync complete");
    return true;
}

} // namespace simple_application
