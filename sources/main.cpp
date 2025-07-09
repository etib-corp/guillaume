#include <memory>
#include <stdexcept>

#include "guillaume/context.hpp"
#include "guillaume/renderer.hpp"

#include "sdl3_renderer/sdl3_renderer.hpp"

int main(int argc, char* const argv[], char* const envp[])
{
    std::shared_ptr<guigui::Renderer> renderer = nullptr;
    std::unique_ptr<guigui::Context> context = nullptr;

    try {
        renderer = std::make_shared<SDL3Renderer>();
    } catch (const std::runtime_error& exception) {
        fprintf(stderr, "Error initializing renderer: %s\n", exception.what());
        return EXIT_FAILURE;
    }

    try {
        context = std::make_unique<guigui::Context>(std::move(renderer));
    } catch (const std::runtime_error& exception) {
        fprintf(stderr, "Error initializing context: %s\n", exception.what());
        return EXIT_FAILURE;
    }

    // Replace guigui::Container with a concrete subclass, e.g., guigui::ConcreteContainer
    auto root_container = std::make_unique<guigui::Container>();
    context->set_root_component(std::move(root_container));

    try {
        context->run();
    } catch (const std::runtime_error& exception) {
        fprintf(stderr, "Error running context: %s\n", exception.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}