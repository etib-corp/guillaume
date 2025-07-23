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
    // Parse command line arguments first
    auto& config = guigui::Config::get_instance();
    if (!config.parse_command_line(argc, argv)) {
        return EXIT_FAILURE;
    }

    // Initialize logger early for debugging throughout the application lifecycle
    auto& logger = guigui::Logger::get_instance();
    logger.set_log_level(guigui::LogLevel::DEBUG_LEVEL);      // Enable debug output
    logger.set_console_logging(true);                   // Log to console
    logger.set_file_logging(true, "guillaume.log");     // Log to file

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

    // Set up the user interface components with 1000 dynamically generated buttons
    LOG_INFO("Setting up UI components - generating 1000 buttons with different colors");
    auto root_container = std::make_unique<guigui::Container>();
    
    // Generate 1000 buttons in a loop with different colors and positions
    constexpr int num_buttons = 1000;
    constexpr int grid_cols = 40;  // Arrange buttons in a 40x25 grid
    constexpr int grid_rows = 25;
    constexpr int button_width = 30;
    constexpr int button_height = 24;
    constexpr int button_spacing = 2;
    
    for (int i = 0; i < num_buttons; ++i) {
        // Calculate grid position
        int col = i % grid_cols;
        int row = i / grid_cols;
        int x = col * (button_width + button_spacing);
        int y = row * (button_height + button_spacing);
        
        // Generate different colors based on position and index
        // Use HSV-like color generation for variety
        float hue = (float(i) / float(num_buttons)) * 360.0f;  // Full color spectrum
        float saturation = 0.7f + (float(col) / float(grid_cols)) * 0.3f;  // Vary saturation
        float value = 0.5f + (float(row) / float(grid_rows)) * 0.5f;       // Vary brightness
        
        // Convert HSV to RGB (simplified)
        int h_i = int(hue / 60) % 6;
        float f = hue / 60.0f - h_i;
        float p = value * (1 - saturation);
        float q = value * (1 - f * saturation);
        float t = value * (1 - (1 - f) * saturation);
        
        float r, g, b;
        switch (h_i) {
            case 0: r = value; g = t; b = p; break;
            case 1: r = q; g = value; b = p; break;
            case 2: r = p; g = value; b = t; break;
            case 3: r = p; g = q; b = value; break;
            case 4: r = t; g = p; b = value; break;
            case 5: r = value; g = p; b = q; break;
            default: r = g = b = 0; break;
        }
        
        // Convert to 0-255 range
        uint8_t red = static_cast<uint8_t>(r * 255);
        uint8_t green = static_cast<uint8_t>(g * 255);
        uint8_t blue = static_cast<uint8_t>(b * 255);
        
        // Create button with unique properties
        auto button = std::make_unique<guigui::Button>(
            std::to_string(i),                           // Button label (number)
            guigui::Color(red, green, blue, 255),        // Background color (generated)
            guigui::Color(255, 255, 255, 255),           // Text color (white for contrast)
            guigui::Font("Roboto", "assets/Roboto.ttf", 12), // Smaller font for grid
            guigui::Rectangle(x, y, button_width, button_height), // Position and size
            [i](guigui::Button& button) {                // Click callback with captured index
                LOG_INFO_F("Button {} clicked: {}", i, button.get_identifier());
            });
        
        button->set_z_index(i % 10); // Vary z-index (0-9 cycling)
        
        // Configure hover colors (slightly brighter)
        button->set_hovered_colors(
            guigui::Color(
                std::min(255, red + 30), 
                std::min(255, green + 30), 
                std::min(255, blue + 30), 
                255
            ),
            guigui::Color(255, 255, 255, 255)
        );
        
        // Configure pressed colors (slightly darker)
        button->set_pressed_colors(
            guigui::Color(
                std::max(0, red - 30), 
                std::max(0, green - 30), 
                std::max(0, blue - 30), 
                255
            ),
            guigui::Color(220, 220, 220, 255)
        );
        
        // Set hover callback for debugging (only for first 10 to avoid spam)
        if (i < 10) {
            button->set_hover_callback([i](guigui::Component& comp, bool entering) {
                LOG_INFO_F("Button {} hover: {} ({})", 
                           i, comp.get_identifier(), 
                           entering ? "ENTERED" : "EXITED");
            });
        }
        
        // Add button to container
        root_container->add_child(std::move(button));
    }
    
    LOG_INFO_F("Generated {} buttons with varied colors and positions", num_buttons);
    
    context->set_root_component(std::move(root_container));
    LOG_INFO("UI components setup complete with 1000 dynamically generated buttons");

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