#pragma once

#include <guillaume/application.hpp>

#include "event_handler.hpp"
#include "renderer.hpp"
#include "window.hpp"

namespace simple_application {

class Application
    : public guillaume::Application<Window, Renderer, EventHandler> {
  public:
    Application(int argc, char *argv[]);

    ~Application(void);
};

} // namespace simple_application
