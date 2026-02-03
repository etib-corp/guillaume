#include <iostream>

#include "application.hpp"

namespace simple_application {

Application::Application(int argc, char *argv[])
    : guillaume::Application<Renderer, EventHandler>() {
    getLogger().info("Application initialized.");
}

Application::~Application(void) { getLogger().info("Application terminated."); }

} // namespace simple_application