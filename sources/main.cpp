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

#include "guillaume/context.hpp"
#include "guillaume/logger.hpp"
#include "guillaume/renderer.hpp"

#include "sdl3_renderer/sdl3_renderer.hpp"

#include "guillaume/components/button.hpp"

/**
 * @brief Main application entry point
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @param envp Array of environment variable strings
 * @return int Exit code (EXIT_SUCCESS on normal termination, EXIT_FAILURE on error)
 * 
 * @details This function:
 * 1. Initializes the logging system with DEBUG level and file output
 * 2. Creates an SDL3 renderer for graphics operations
 * 3. Sets up a GUI context with the renderer
 * 4. Creates a button component with hover and click interactions
 * 5. Runs the main application loop until user quits
 * 
 * The application demonstrates the basic Guillaume framework workflow:
 * - Component creation and configuration
 * - Event handling (hover, click)
 * - Visual state changes based on interaction
 * - Proper resource management and error handling
 */
int main(int argc, char* const argv[], char* const envp[])
{
    // Initialize logger early for debugging throughout the application lifecycle
    auto& logger = guigui::Logger::get_instance();
    logger.set_log_level(guigui::LogLevel::DEBUG);      // Enable debug output
    logger.set_console_logging(true);                   // Log to console
    logger.set_file_logging(true, "guillaume.log");     // Log to file

    LOG_INFO("Guillaume application starting");

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

    // Set up the user interface components
    LOG_INFO("Setting up UI components");
    auto root_container = std::make_unique<guigui::Container>();
    
    // Create a button with enhanced interaction logging and visual feedback
    auto button = std::make_unique<guigui::Button>(
        "Click Me",                                    // Button label text
        guigui::Color(0, 128, 255, 255),              // Background color (blue)
        guigui::Color(255, 255, 255, 255),            // Text color (white)
        guigui::Font("Roboto", "assets/Roboto.ttf", 48), // Font specification
        guigui::Rectangle(50, 50, 200, 50),           // Position and size (x, y, width, height)
        [](guigui::Button& button) {                  // Click callback function
            LOG_INFO_F("Button clicked: {}", button.get_identifier());
        });
    
    // Configure visual feedback colors for different interaction states
    // Set hover colors to provide visual feedback when mouse is over the button
    button->set_hovered_colors(
        guigui::Color(30, 144, 255, 255), // Lighter blue background on hover
        guigui::Color(255, 255, 255, 255) // Keep text white on hover
    );
    
    // Set pressed colors to show visual feedback when button is being clicked
    button->set_pressed_colors(
        guigui::Color(0, 100, 200, 255),  // Darker blue background when pressed
        guigui::Color(220, 220, 220, 255) // Slightly gray text when pressed
    );
    
    // Add hover callback for additional logging and interaction tracking
    button->set_hover_callback([](guigui::Component& comp, bool entering) {
        LOG_INFO_F("Button hover: {} ({})", 
                   comp.get_identifier(), 
                   entering ? "ENTERED" : "EXITED");
    });
    
    // Add the button to the root container and set up the UI hierarchy
    root_container->add_child(std::move(button));
    context->set_root_component(std::move(root_container));
    LOG_INFO("UI components setup complete");

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