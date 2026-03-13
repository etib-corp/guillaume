#pragma once

#include <guillaume/application.hpp>

#include "event_handler.hpp"
#include "renderer.hpp"

#include "scenes/main.hpp"
#include "scenes/settings.hpp"

namespace simple_application {

class Application
    : public guillaume::Application<Renderer, EventHandler, scenes::Main,
                                    scenes::Settings> {
  public:
    Application(int argc, char *argv[]);
    ~Application(void);
};

} // namespace simple_application
