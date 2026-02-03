#pragma once

#include <string>

#include <SDL3/SDL.h>

#include <utility/math/rectangle.hpp>
#include <utility/math/vector.hpp>

#include <guillaume/window.hpp>

#include "renderer.hpp"

namespace simple_application {

class Window : public guillaume::Window {
  public:
    Window(void);

    ~Window(void) override;

    utility::math::Vector<std::size_t, 2> getSize(void) const override;
};

} // namespace simple_application
