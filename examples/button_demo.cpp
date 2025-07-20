/**
 * @file button_demo.cpp
 * @brief Button interaction example
 * 
 * This example demonstrates how to create and use button components
 * with various event handlers and visual states.
 */

#include "guillaume.h"
#include <iostream>
#include <string>

class ButtonDemo {
private:
    std::shared_ptr<guigui::Context> context;
    int click_count = 0;
    
public:
    ButtonDemo() {
        // Create main window
        context = guigui::Context::create("Button Demo", 800, 600);
        context->set_background_color(guigui::Color::from_rgb(250, 250, 250));
        
        setup_buttons();
    }
    
    void setup_buttons() {
        // Create a simple click button
        auto click_button = guigui::Button::create("Click Me!");
        click_button->set_position({50, 50});
        click_button->set_size({150, 40});
        
        click_button->on_click([this]() {
            click_count++;
            std::cout << "Button clicked " << click_count << " times!" << std::endl;
        });
        
        context->add_component(click_button);
        
        // Create a toggle button
        auto toggle_button = guigui::Button::create("Toggle");
        toggle_button->set_position({220, 50});
        toggle_button->set_size({150, 40});
        
        bool is_toggled = false;
        toggle_button->on_click([&is_toggled, toggle_button]() {
            is_toggled = !is_toggled;
            toggle_button->set_text(is_toggled ? "ON" : "OFF");
            std::cout << "Toggle state: " << (is_toggled ? "ON" : "OFF") << std::endl;
        });
        
        context->add_component(toggle_button);
        
        // Create a button with hover effects
        auto hover_button = guigui::Button::create("Hover Me");
        hover_button->set_position({390, 50});
        hover_button->set_size({150, 40});
        
        hover_button->on_hover_enter([]() {
            std::cout << "Mouse entered button" << std::endl;
        });
        
        hover_button->on_hover_exit([]() {
            std::cout << "Mouse left button" << std::endl;
        });
        
        hover_button->on_click([]() {
            std::cout << "Hover button clicked!" << std::endl;
        });
        
        context->add_component(hover_button);
        
        // Create an exit button
        auto exit_button = guigui::Button::create("Exit");
        exit_button->set_position({350, 500});
        exit_button->set_size({100, 40});
        exit_button->set_background_color(guigui::Color::from_rgb(255, 100, 100));
        
        exit_button->on_click([this]() {
            std::cout << "Exit button clicked - closing application" << std::endl;
            context->stop();
        });
        
        context->add_component(exit_button);
        
        // Create disabled button to show different states
        auto disabled_button = guigui::Button::create("Disabled");
        disabled_button->set_position({50, 120});
        disabled_button->set_size({150, 40});
        disabled_button->set_enabled(false);
        
        context->add_component(disabled_button);
        
        // Create a button that enables the disabled button
        auto enable_button = guigui::Button::create("Enable");
        enable_button->set_position({220, 120});
        enable_button->set_size({150, 40});
        
        enable_button->on_click([disabled_button, enable_button]() {
            bool is_enabled = disabled_button->is_enabled();
            disabled_button->set_enabled(!is_enabled);
            enable_button->set_text(is_enabled ? "Enable" : "Disable");
            std::cout << "Button " << (is_enabled ? "disabled" : "enabled") << std::endl;
        });
        
        context->add_component(enable_button);
        
        // Create a counter display (using Label if available)
        create_info_labels();
    }
    
    void create_info_labels() {
        // Info label showing instructions
        auto info_label = guigui::Label::create(
            "Click buttons to see interactions.\n"
            "Hover over 'Hover Me' button.\n"
            "Toggle button changes text.\n"
            "Enable/Disable button controls state."
        );
        info_label->set_position({50, 200});
        info_label->set_size({500, 100});
        info_label->set_text_color(guigui::Color::from_rgb(64, 64, 64));
        
        context->add_component(info_label);
        
        // Click counter label
        auto counter_label = guigui::Label::create("Clicks: 0");
        counter_label->set_position({50, 320});
        counter_label->set_size({200, 30});
        counter_label->set_text_color(guigui::Color::from_rgb(0, 100, 200));
        
        // Update counter periodically (simplified approach)
        // In a real app, you'd use a proper update mechanism
        
        context->add_component(counter_label);
    }
    
    void run() {
        try {
            std::cout << "Starting Button Demo..." << std::endl;
            std::cout << "Try clicking the different buttons!" << std::endl;
            
            context->run();
            
            std::cout << "Button Demo finished with " << click_count << " total clicks" << std::endl;
            
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
};

int main() {
    ButtonDemo demo;
    demo.run();
    return 0;
}
