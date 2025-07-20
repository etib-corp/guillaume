/**
 * @file basic_window.cpp
 * @brief Basic Guillaume window example
 * 
 * This example demonstrates how to create a simple Guillaume window
 * with basic setup and event handling.
 */

#include "guillaume.h"
#include <iostream>

int main() {
    try {
        // Create a context with window title and size
        auto context = guigui::Context::create("Basic Guillaume Window", 800, 600);
        
        // Set a light gray background
        context->set_background_color(guigui::Color::from_rgb(240, 240, 240));
        
        // Optional: Set up event handling
        context->on_event([](const guigui::Event& event) {
            if (event.get_type() == guigui::EventType::KeyPress) {
                auto& key_event = static_cast<const guigui::KeyboardEvent&>(event);
                if (key_event.get_key() == guigui::Key::Escape) {
                    std::cout << "Escape key pressed - exiting" << std::endl;
                    // Note: In a real application, you'd call context->stop() here
                }
            }
        });
        
        // Run the application
        std::cout << "Starting Guillaume application..." << std::endl;
        context->run();
        
        std::cout << "Application finished" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
