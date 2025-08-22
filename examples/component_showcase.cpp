/**
 * @file component_showcase.cpp
 * @brief Example demonstrating all Guillaume framework components
 * 
 * This example shows how to use all the available components including:
 * - Button (existing)
 * - Label (new)
 * - TextBox (new) 
 * - Checkbox (new)
 * - Slider (new)
 */

#include <memory>
#include <stdexcept>

#include "guillaume/config.hpp"
#include "guillaume/context.hpp"
#include "guillaume/logger.hpp"
#include "guillaume/renderer.hpp"

#include "sdl3_renderer/sdl3_renderer.hpp"

#include "guillaume/components/button.hpp"
#include "guillaume/components/label.hpp"
#include "guillaume/components/textbox.hpp"
#include "guillaume/components/checkbox.hpp"
#include "guillaume/components/slider.hpp"

int main(int argc, char* const argv[], char* const envp[])
{
    // Parse command line arguments
    auto& config = guigui::Config::get_instance();
    if (!config.parse_command_line(argc, argv)) {
        return EXIT_FAILURE;
    }

    // Initialize logger
    auto& logger = guigui::Logger::get_instance();
    logger.set_log_level(guigui::LogLevel::DEBUG_LEVEL);
    logger.set_console_logging(true);
    logger.set_file_logging(true, "guillaume.log");

    LOG_INFO("Guillaume Component Showcase starting");

    // Initialize renderer and context
    std::shared_ptr<guigui::Renderer> renderer = nullptr;
    std::unique_ptr<guigui::Context> context = nullptr;

    try {
        LOG_INFO("Initializing SDL3 renderer");
        renderer = std::make_shared<SDL3Renderer>();
        LOG_INFO("Creating GUI context");
        context = std::make_unique<guigui::Context>(std::move(renderer));
    } catch (const std::runtime_error& exception) {
        LOG_FATAL_F("Error initializing: {}", exception.what());
        return EXIT_FAILURE;
    }

    // Create root container
    auto root_container = std::make_unique<guigui::Container>();
    
    // Define common colors and fonts
    guigui::Color bg_color(240, 240, 240, 255);           // Light gray
    guigui::Color primary_color(70, 130, 180, 255);       // Steel blue
    guigui::Color secondary_color(100, 149, 237, 255);    // Cornflower blue
    guigui::Color text_color(0, 0, 0, 255);               // Black
    guigui::Color white_color(255, 255, 255, 255);        // White
    guigui::Font main_font("Roboto", "assets/Roboto.ttf", 14);
    guigui::Font title_font("Roboto", "assets/Roboto.ttf", 18);

    // Create title label
    auto title_label = std::make_unique<guigui::Label>(
        "Guillaume Framework - Component Showcase",
        text_color,
        title_font,
        guigui::Rectangle(20, 20, 600, 30),
        guigui::Label::TextAlignment::CENTER
    );
    root_container->add_child(std::move(title_label));

    // Create section: Buttons
    auto button_section_label = std::make_unique<guigui::Label>(
        "Buttons:",
        text_color,
        main_font,
        guigui::Rectangle(20, 70, 100, 25),
        guigui::Label::TextAlignment::LEFT
    );
    root_container->add_child(std::move(button_section_label));

    // Sample button
    auto sample_button = std::make_unique<guigui::Button>(
        "Click Me!",
        primary_color,
        white_color,
        main_font,
        guigui::Rectangle(130, 70, 100, 30),
        [](guigui::Button& button) {
            LOG_INFO_F("Button '{}' was clicked!", button.get_identifier());
        }
    );
    root_container->add_child(std::move(sample_button));

    // Another button with different colors
    auto action_button = std::make_unique<guigui::Button>(
        "Action",
        guigui::Color(220, 20, 60, 255), // Crimson
        white_color,
        main_font,
        guigui::Rectangle(240, 70, 80, 30),
        [](guigui::Button& button) {
            LOG_INFO("Action button clicked!");
        }
    );
    root_container->add_child(std::move(action_button));

    // Create section: Labels
    auto label_section_label = std::make_unique<guigui::Label>(
        "Labels:",
        text_color,
        main_font,
        guigui::Rectangle(20, 120, 100, 25)
    );
    root_container->add_child(std::move(label_section_label));

    // Sample labels with different alignments
    auto left_label = std::make_unique<guigui::Label>(
        "Left Aligned",
        text_color,
        main_font,
        guigui::Rectangle(130, 120, 120, 25),
        guigui::Label::TextAlignment::LEFT
    );
    root_container->add_child(std::move(left_label));

    auto center_label = std::make_unique<guigui::Label>(
        "Centered",
        secondary_color,
        main_font,
        guigui::Rectangle(260, 120, 120, 25),
        guigui::Label::TextAlignment::CENTER
    );
    root_container->add_child(std::move(center_label));

    auto right_label = std::make_unique<guigui::Label>(
        "Right Aligned",
        guigui::Color(34, 139, 34, 255), // Forest green
        main_font,
        guigui::Rectangle(390, 120, 120, 25),
        guigui::Label::TextAlignment::RIGHT
    );
    root_container->add_child(std::move(right_label));

    // Create section: TextBoxes
    auto textbox_section_label = std::make_unique<guigui::Label>(
        "Text Input:",
        text_color,
        main_font,
        guigui::Rectangle(20, 170, 100, 25)
    );
    root_container->add_child(std::move(textbox_section_label));

    // Regular text box
    auto text_input = std::make_unique<guigui::TextBox>(
        "Enter your name...",
        white_color,
        guigui::Color(128, 128, 128, 255), // Gray border
        text_color,
        main_font,
        guigui::Rectangle(130, 170, 150, 30)
    );
    text_input->set_text_change_callback([](guigui::TextBox& textbox, const std::string& old_text, const std::string& new_text) {
        LOG_INFO_F("Text changed from '{}' to '{}'", old_text, new_text);
    });
    root_container->add_child(std::move(text_input));

    // Password text box
    auto password_input = std::make_unique<guigui::TextBox>(
        "Password...",
        white_color,
        guigui::Color(128, 128, 128, 255),
        text_color,
        main_font,
        guigui::Rectangle(290, 170, 150, 30)
    );
    password_input->set_password(true);
    root_container->add_child(std::move(password_input));

    // Create section: Checkboxes
    auto checkbox_section_label = std::make_unique<guigui::Label>(
        "Checkboxes:",
        text_color,
        main_font,
        guigui::Rectangle(20, 220, 100, 25)
    );
    root_container->add_child(std::move(checkbox_section_label));

    // Sample checkboxes
    auto checkbox1 = std::make_unique<guigui::Checkbox>(
        "Enable notifications",
        false,
        white_color,
        guigui::Color(0, 128, 0, 255), // Green check
        text_color,
        main_font,
        guigui::Rectangle(130, 220, 150, 25)
    );
    checkbox1->set_state_change_callback([](guigui::Checkbox& checkbox, bool checked) {
        LOG_INFO_F("Checkbox '{}' is now: {}", checkbox.get_label(), checked ? "checked" : "unchecked");
    });
    root_container->add_child(std::move(checkbox1));

    auto checkbox2 = std::make_unique<guigui::Checkbox>(
        "Auto-save",
        true,
        white_color,
        guigui::Color(0, 100, 200, 255), // Blue check
        text_color,
        main_font,
        guigui::Rectangle(290, 220, 120, 25)
    );
    checkbox2->set_state_change_callback([](guigui::Checkbox& checkbox, bool checked) {
        LOG_INFO_F("Auto-save is now: {}", checked ? "enabled" : "disabled");
    });
    root_container->add_child(std::move(checkbox2));

    // Create section: Sliders
    auto slider_section_label = std::make_unique<guigui::Label>(
        "Sliders:",
        text_color,
        main_font,
        guigui::Rectangle(20, 270, 100, 25)
    );
    root_container->add_child(std::move(slider_section_label));

    // Horizontal slider
    auto h_slider = std::make_unique<guigui::Slider>(
        0.0, 100.0, 50.0,
        guigui::Slider::Orientation::HORIZONTAL,
        guigui::Color(200, 200, 200, 255), // Light gray track
        primary_color,                      // Blue thumb
        secondary_color,                    // Blue filled track
        guigui::Rectangle(130, 270, 200, 30),
        1.0 // Step size of 1
    );
    h_slider->set_value_display(true, main_font, text_color);
    h_slider->set_value_change_callback([](guigui::Slider& slider, double value) {
        LOG_INFO_F("Horizontal slider value: {}", value);
    });
    root_container->add_child(std::move(h_slider));

    // Vertical slider
    auto v_slider = std::make_unique<guigui::Slider>(
        0.0, 1.0, 0.3,
        guigui::Slider::Orientation::VERTICAL,
        guigui::Color(200, 200, 200, 255),
        guigui::Color(255, 140, 0, 255),    // Orange thumb
        guigui::Color(255, 165, 0, 255),    // Orange filled track
        guigui::Rectangle(350, 270, 30, 100),
        0.1 // Step size of 0.1
    );
    v_slider->set_value_display(true, main_font, text_color);
    v_slider->set_value_change_callback([](guigui::Slider& slider, double value) {
        LOG_INFO_F("Vertical slider value: {}", value);
    });
    root_container->add_child(std::move(v_slider));

    // Create section: Interactive Demo
    auto demo_section_label = std::make_unique<guigui::Label>(
        "Interactive Demo:",
        text_color,
        main_font,
        guigui::Rectangle(20, 390, 150, 25)
    );
    root_container->add_child(std::move(demo_section_label));

    // Status label that will be updated by other controls
    auto status_label = std::make_unique<guigui::Label>(
        "Status: Ready",
        guigui::Color(0, 128, 0, 255), // Green
        main_font,
        guigui::Rectangle(180, 390, 200, 25)
    );
    // We need to keep a reference to update it later
    // In a real application, you'd manage this with proper component references
    
    root_container->add_child(std::move(status_label));

    // Reset button
    auto reset_button = std::make_unique<guigui::Button>(
        "Reset All",
        guigui::Color(255, 69, 0, 255), // Orange red
        white_color,
        main_font,
        guigui::Rectangle(400, 390, 100, 30),
        [](guigui::Button& button) {
            LOG_INFO("Reset button clicked - would reset all controls to default values");
            // In a real application, you'd reset all the controls here
        }
    );
    root_container->add_child(std::move(reset_button));

    // Additional info labels
    auto info_label1 = std::make_unique<guigui::Label>(
        "• Click buttons to see console output",
        guigui::Color(64, 64, 64, 255), // Dark gray
        guigui::Font("Roboto", "assets/Roboto.ttf", 12),
        guigui::Rectangle(20, 440, 300, 20)
    );
    root_container->add_child(std::move(info_label1));

    auto info_label2 = std::make_unique<guigui::Label>(
        "• Type in text boxes to test input",
        guigui::Color(64, 64, 64, 255),
        guigui::Font("Roboto", "assets/Roboto.ttf", 12),
        guigui::Rectangle(20, 460, 300, 20)
    );
    root_container->add_child(std::move(info_label2));

    auto info_label3 = std::make_unique<guigui::Label>(
        "• Toggle checkboxes and adjust sliders",
        guigui::Color(64, 64, 64, 255),
        guigui::Font("Roboto", "assets/Roboto.ttf", 12),
        guigui::Rectangle(20, 480, 300, 20)
    );
    root_container->add_child(std::move(info_label3));

    // Set the root component
    context->set_root_component(std::move(root_container));
    LOG_INFO("Component showcase UI setup complete");

    // Run the application
    try {
        LOG_INFO("Starting component showcase application loop");
        context->run();
        LOG_INFO("Application loop finished normally");
    } catch (const std::runtime_error& exception) {
        LOG_FATAL_F("Error running context: {}", exception.what());
        return EXIT_FAILURE;
    }

    LOG_INFO("Guillaume Component Showcase exiting successfully");
    return EXIT_SUCCESS;
}
