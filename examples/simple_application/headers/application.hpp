#pragma once

#include <guillaume/application.hpp>

#include "event_handler.hpp"
#include "renderer.hpp"
#include "window.hpp"

namespace simple_application {

class Application
    : public guillaume::Application<Window, Application, EventHandler> {
  public:
    Application(int argc, char *argv[])
        : guillaume::Application<Window, Application, EventHandler>() {
        info("Application initialized.");
    }

    ~Application(void) { info("Application terminated."); }
};

} // namespace simple_application
