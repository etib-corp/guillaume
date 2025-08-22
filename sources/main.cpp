/**
 * @file main.cpp
 * @brief Main application entry point for Guillaume GUI framework demonstration
 *
 * This file demonstrates how to use the Guillaume GUI framework to create
 * a simple application with a button component. It shows the basic setup
 * process including renderer initialization, context creation, and component
 * configuration.
 *
 * @details The application creates:
 * - An SDL3 renderer for graphics operations
 * - A GUI context for managing the application lifecycle
 * - A button component with interactive states and callbacks
 * - Logging configuration for debugging and monitoring
 *
 * @author Guillaume Framework Team
 * @version 2.02
 */

#include <memory>
#include <stdexcept>

#include "guillaume/config.hpp"
#include "guillaume/context.hpp"
#include "guillaume/logger.hpp"
#include "guillaume/renderer.hpp"

#include "sdl3_renderer/sdl3_renderer.hpp"

#include "guillaume/components/button.hpp"
#include "guillaume/components/checkbox.hpp"
#include "guillaume/components/label.hpp"
#include "guillaume/components/slider.hpp"
#include "guillaume/components/textbox.hpp"

/**
 * @brief Main application entry point
 *
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @param envp Array of environment variable strings
 * @return int Exit code (EXIT_SUCCESS on normal termination, EXIT_FAILURE on error)
 *
 */
int main(int argc, char* const argv[], char* const envp[])
{
    // Parse command line arguments first
    auto& config = guigui::Config::get_instance();
    if (!config.parse_command_line(argc, argv)) {
        return EXIT_FAILURE;
    }

    // Initialize logger early for debugging throughout the application lifecycle
    auto& logger = guigui::Logger::get_instance();
    logger.set_log_level(guigui::LogLevel::INFO_LEVEL); // Enable debug output
    logger.set_console_logging(true); // Log to console
    logger.set_file_logging(true, "guillaume.log"); // Log to file

    LOG_INFO("Guillaume application starting");
    LOG_INFO_F("Window mode: {}",
        config.get_window_mode() == guigui::Config::WindowMode::FULLSCREEN ? "Fullscreen" : "Windowed");
    LOG_INFO_F("Debug mouse coordinates: {}", config.is_debug_mouse_coordinates() ? "enabled" : "disabled");

    // Renderer and context pointers for resource management
    std::shared_ptr<guigui::Renderer> renderer = nullptr;
    std::unique_ptr<guigui::Context> context = nullptr;

    // Initialize the SDL3 renderer for graphics operations
    try {
        LOG_INFO("Initializing SDL3 renderer");
        renderer = std::make_shared<SDL3Renderer>();
        LOG_INFO("SDL3 renderer initialized successfully");
    } catch (const std::runtime_error& exception) {
        LOG_FATAL_F("Error initializing renderer: {}", exception.what());
        return EXIT_FAILURE;
    }

    // Create the GUI context that manages the application lifecycle
    try {
        LOG_INFO("Creating GUI context");
        context = std::make_unique<guigui::Context>(std::move(renderer));
        LOG_INFO("GUI context created successfully");
    } catch (const std::runtime_error& exception) {
        LOG_FATAL_F("Error initializing context: {}", exception.what());
        return EXIT_FAILURE;
    }

    // Set up the user interface components - demonstrating new components
    LOG_INFO("Setting up UI components - demonstrating Guillaume framework components");
    auto root_container = std::make_unique<guigui::Container>();

    // Create a simple demo with the new components

    // Title label
    auto title_label = std::make_unique<guigui::Label>(
        "Guillaume Framework - New Components Demo",
        guigui::Color(0, 0, 0, 255), // Black text
        guigui::Font("Roboto", "assets/Roboto.ttf", 18),
        guigui::Rectangle(50, 20, 500, 30),
        guigui::Label::TextAlignment::CENTER);
    root_container->add_child(std::move(title_label));

    // Sample button (existing component)
    auto sample_button = std::make_unique<guigui::Button>(
        "Click Me!",
        guigui::Color(70, 130, 180, 255), // Steel blue
        guigui::Color(255, 255, 255, 255), // White text
        guigui::Font("Roboto", "assets/Roboto.ttf", 14),
        guigui::Rectangle(50, 70, 120, 35),
        [](guigui::Button& button) {
            LOG_INFO_F("Button '{}' was clicked!", button.get_identifier());
        });
    root_container->add_child(std::move(sample_button));

    // Label component (new)
    auto info_label = std::make_unique<guigui::Label>(
        "This is a new Label component",
        guigui::Color(0, 128, 0, 255), // Green text
        guigui::Font("Roboto", "assets/Roboto.ttf", 14),
        guigui::Rectangle(200, 75, 250, 25),
        guigui::Label::TextAlignment::LEFT);
    root_container->add_child(std::move(info_label));

    // TextBox component (new)
    auto text_input = std::make_unique<guigui::TextBox>(
        "Enter your name here...",
        guigui::Color(255, 255, 255, 255), // White background
        guigui::Color(128, 128, 128, 255), // Gray border
        guigui::Color(0, 0, 0, 255), // Black text
        guigui::Font("Roboto", "assets/Roboto.ttf", 14),
        guigui::Rectangle(50, 120, 200, 30));
    text_input->set_text_change_callback([](guigui::TextBox& textbox, const std::string& old_text, const std::string& new_text) {
        LOG_INFO_F("Text changed from '{}' to '{}'", old_text, new_text);
    });
    root_container->add_child(std::move(text_input));

    // Checkbox component (new)
    auto checkbox = std::make_unique<guigui::Checkbox>(
        "Enable notifications",
        false, // Initially unchecked
        guigui::Color(255, 255, 255, 255), // White box
        guigui::Color(0, 128, 0, 255), // Green check
        guigui::Color(0, 0, 0, 255), // Black text
        guigui::Font("Roboto", "assets/Roboto.ttf", 14),
        guigui::Rectangle(270, 120, 150, 30));
    checkbox->set_state_change_callback([](guigui::Checkbox& checkbox, bool checked) {
        LOG_INFO_F("Checkbox '{}' is now: {}", checkbox.get_label(), checked ? "checked" : "unchecked");
    });
    root_container->add_child(std::move(checkbox));

    // Horizontal Slider component (new) - temporarily disabled
    auto h_slider = std::make_unique<guigui::Slider>(
        0.0, 100.0, 50.0, // Min, max, initial value
        guigui::Slider::Orientation::HORIZONTAL,
        guigui::Color(200, 200, 200, 255), // Gray track
        guigui::Color(70, 130, 180, 255), // Blue thumb
        guigui::Color(100, 149, 237, 255), // Light blue filled track
        guigui::Rectangle(50, 170, 200, 30),
        1.0 // Step size
    );
    h_slider->set_value_display(true, guigui::Font("Roboto", "assets/Roboto.ttf", 12), guigui::Color(0, 0, 0, 255));
    h_slider->set_value_change_callback([](guigui::Slider& slider, double value) {
        LOG_INFO_F("Horizontal slider value: {}", value);
    });
    root_container->add_child(std::move(h_slider));

    // Vertical Slider component (new) - temporarily disabled
    auto v_slider = std::make_unique<guigui::Slider>(
        0.0, 1.0, 0.3, // Min, max, initial value
        guigui::Slider::Orientation::VERTICAL,
        guigui::Color(200, 200, 200, 255), // Gray track
        guigui::Color(255, 140, 0, 255), // Orange thumb
        guigui::Color(255, 165, 0, 255), // Orange filled track
        guigui::Rectangle(280, 170, 30, 100),
        0.1 // Step size
    );
    v_slider->set_value_display(true, guigui::Font("Roboto", "assets/Roboto.ttf", 12), guigui::Color(0, 0, 0, 255));
    v_slider->set_value_change_callback([](guigui::Slider& slider, double value) {
        LOG_INFO_F("Vertical slider value: {}", value);
    });
    root_container->add_child(std::move(v_slider));

    LOG_INFO("Generated demo UI with new Guillaume components");

    context->set_root_component(std::move(root_container));
    LOG_INFO("UI components setup complete with new component demonstrations");

    // Run the main application loop
    try {
        LOG_INFO("Starting main application loop");
        context->run(); // This blocks until the application is terminated
        LOG_INFO("Application loop finished normally");
    } catch (const std::runtime_error& exception) {
        LOG_FATAL_F("Error running context: {}", exception.what());
        return EXIT_FAILURE;
    }

    LOG_INFO("Guillaume application exiting successfully");
    return EXIT_SUCCESS;
}