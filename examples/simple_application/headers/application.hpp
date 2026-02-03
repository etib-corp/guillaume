#pragma once

#include <guillaume/application.hpp>

#include "event_handler.hpp"
#include "renderer.hpp"

namespace simple_application {

class Application : public guillaume::Application<Renderer, EventHandler> {
  public:
    Application(int argc, char *argv[]);
    ~Application(void);
};

} // namespace simple_application
