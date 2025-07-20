#include <memory>
#include <stdexcept>

#include "guillaume/context.hpp"
#include "guillaume/logger.hpp"
#include "guillaume/renderer.hpp"

#include "sdl3_renderer/sdl3_renderer.hpp"

#include "guillaume/components/button.hpp"

int main(int argc, char* const argv[], char* const envp[])
{
    // Initialize logger early
    auto& logger = guigui::Logger::get_instance();
    logger.set_log_level(guigui::LogLevel::DEBUG);
    logger.set_console_logging(true);
    logger.set_file_logging(true, "guillaume.log");

    LOG_INFO("Guillaume application starting...");

    std::shared_ptr<guigui::Renderer> renderer = nullptr;
    std::unique_ptr<guigui::Context> context = nullptr;

    try {
        LOG_INFO("Initializing SDL3 renderer...");
        renderer = std::make_shared<SDL3Renderer>();
        LOG_INFO("SDL3 renderer initialized successfully");
    } catch (const std::runtime_error& exception) {
        LOG_FATAL_F("Error initializing renderer: {}", exception.what());
        return EXIT_FAILURE;
    }

    try {
        LOG_INFO("Creating GUI context...");
        context = std::make_unique<guigui::Context>(std::move(renderer));
        LOG_INFO("GUI context created successfully");
    } catch (const std::runtime_error& exception) {
        LOG_FATAL_F("Error initializing context: {}", exception.what());
        return EXIT_FAILURE;
    }

    LOG_INFO("Setting up UI components...");
    auto root_container = std::make_unique<guigui::Container>();
    
    // Create a button with enhanced interaction logging
    auto button = std::make_unique<guigui::Button>(
        "Click Me",
        guigui::Color(0, 128, 255, 255), // Background color
        guigui::Color(255, 255, 255, 255), // Text color
        guigui::Font("Roboto", "assets/Roboto.ttf", 48),
        guigui::Rectangle(50, 50, 200, 50),
        [](guigui::Button& button) {
            LOG_INFO_F("🎯 Button clicked: {}", button.get_identifier());
        });
    
    // Set hover and pressed colors to see visual feedback
    button->set_hovered_colors(
        guigui::Color(30, 144, 255, 255), // Lighter blue on hover
        guigui::Color(255, 255, 255, 255)
    );
    
    button->set_pressed_colors(
        guigui::Color(0, 100, 200, 255),  // Darker blue when pressed
        guigui::Color(220, 220, 220, 255) // Slightly gray text when pressed
    );
    
    // Add hover callback for additional logging
    button->set_hover_callback([](guigui::Component& comp, bool entering) {
        LOG_INFO_F("🖱️  Button hover: {} ({})", 
                   comp.get_identifier(), 
                   entering ? "ENTERED" : "EXITED");
    });
    
    root_container->add_child(std::move(button));
    context->set_root_component(std::move(root_container));
    LOG_INFO("UI components setup complete");

    try {
        LOG_INFO("Starting main application loop...");
        context->run();
        LOG_INFO("Application loop finished normally");
    } catch (const std::runtime_error& exception) {
        LOG_FATAL_F("Error running context: {}", exception.what());
        return EXIT_FAILURE;
    }

    LOG_INFO("Guillaume application exiting successfully");
    return EXIT_SUCCESS;
}