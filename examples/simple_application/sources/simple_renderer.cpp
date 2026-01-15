#include "simple_renderer.hpp"

#include <sstream>
#include <string>

namespace {

std::string formatColor(const utility::Color<uint8_t> &color) {
    std::ostringstream stream;
    stream << "rgba(" << static_cast<int>(color.red()) << ", "
           << static_cast<int>(color.green()) << ", "
           << static_cast<int>(color.blue()) << ", "
           << static_cast<int>(color.alpha()) << ")";
    return stream.str();
}

template <typename Type>
std::string formatVector(const utility::Vector<Type, 2> &vector) {
    std::ostringstream stream;
    stream << "(" << vector[0] << ", " << vector[1] << ")";
    return stream.str();
}

} // namespace

namespace guillaume::simple_application {

SimpleRenderer::SimpleRenderer()
    : _logger(), _drawColor(0, 0, 0, 255), _scale({1.0F, 1.0F}),
      _sdlRenderer(nullptr) {
    _logger.info("SimpleRenderer initialized (no SDL renderer yet)");
}

SimpleRenderer::SimpleRenderer(SDL_Renderer *sdlRenderer)
    : _logger(), _drawColor(0, 0, 0, 255), _scale({1.0F, 1.0F}),
      _sdlRenderer(sdlRenderer) {
    if (_sdlRenderer) {
        // Get initial draw color from SDL
        Uint8 r, g, b, a;
        if (SDL_GetRenderDrawColor(_sdlRenderer, &r, &g, &b, &a)) {
            _drawColor = utility::Color<uint8_t>(r, g, b, a);
        }

        // Get initial scale from SDL
        float scaleX, scaleY;
        if (SDL_GetRenderScale(_sdlRenderer, &scaleX, &scaleY)) {
            _scale = {scaleX, scaleY};
        }

        _logger.info("SimpleRenderer initialized with SDL3 renderer");
    }
}

SimpleRenderer::~SimpleRenderer() {
    // SDL_Renderer is owned and destroyed by the window
    _sdlRenderer = nullptr;
}

void SimpleRenderer::setSDLRenderer(SDL_Renderer *renderer) {
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

void SimpleRenderer::clear(void) {
    if (_sdlRenderer) {
        if (!SDL_RenderClear(_sdlRenderer)) {
            _logger.error(std::string("SDL_RenderClear failed: ") +
                          SDL_GetError());
        }
    } else {
        _logger.debug("Renderer clear (no SDL)");
    }
}

void SimpleRenderer::present(void) {
    if (_sdlRenderer) {
        if (!SDL_RenderPresent(_sdlRenderer)) {
            _logger.error(std::string("SDL_RenderPresent failed: ") +
                          SDL_GetError());
        }
    } else {
        _logger.debug("Renderer present (no SDL)");
    }
}

void SimpleRenderer::setDrawColor(utility::Color<uint8_t> color) {
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

utility::Color<uint8_t> SimpleRenderer::getDrawColor(void) const {
    return _drawColor;
}

void SimpleRenderer::drawPoint(utility::Vector<std::size_t, 2> point) {
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

void SimpleRenderer::drawLine(utility::Vector<std::size_t, 2> start,
                              utility::Vector<std::size_t, 2> end) {
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

void SimpleRenderer::drawRect(utility::Rectangle<std::size_t> rectangle) {
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

void SimpleRenderer::fillRect(utility::Rectangle<std::size_t> rectangle) {
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

void SimpleRenderer::setScale(utility::Vector<float, 2> scale) {
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

utility::Vector<float, 2> SimpleRenderer::getScale(void) const {
    return _scale;
}

bool SimpleRenderer::flush(void) {
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

} // namespace guillaume::simple_application
