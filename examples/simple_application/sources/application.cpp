#include <iostream>

#include "application.hpp"

namespace simple_application {

Application::Application(int argc, char *argv[])
    : guillaume::Application<Window, Renderer, EventHandler>() {
    guillaume::Application<Window, Renderer, EventHandler>::getLogger().info(
        "Application initialized.");
}

Application::~Application(void) {
    guillaume::Application<Window, Renderer, EventHandler>::getLogger().info(
        "Application terminated.");
}

} // namespace simple_application