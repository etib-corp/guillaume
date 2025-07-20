/**
 * @file custom_component.cpp
 * @brief Example of creating a custom component
 * 
 * This example shows how to create custom components by inheriting
 * from the Component base class and implementing custom rendering
 * and interaction behavior.
 */

#include "guillaume.h"
#include <iostream>
#include <cmath>
#include <memory>

/**
 * @class CircleButton
 * @brief A custom circular button component
 * 
 * This demonstrates creating a custom component with:
 * - Custom shape (circle instead of rectangle)
 * - Custom hit testing
 * - Custom visual states
 * - Animation effects
 */
class CircleButton : public guigui::Component {
private:
    std::string _text;
    float _radius;
    guigui::Color _base_color;
    float _animation_progress = 0.0f;
    bool _is_animating = false;
    
    static int _instance_counter;
    
public:
    CircleButton(const std::string& text, float radius) 
        : Component("circle_button_" + std::to_string(_instance_counter++))
        , _text(text)
        , _radius(radius)
        , _base_color(guigui::Color::from_rgb(100, 150, 255)) {
        
        // Set size based on radius
        set_size({_radius * 2, _radius * 2});
        
        // Set up event handlers
        on_click([this]() {
            std::cout << "Circle button '" << _text << "' clicked!" << std::endl;
            start_click_animation();
        });
        
        on_hover_enter([this]() {
            std::cout << "Hovering over '" << _text << "'" << std::endl;
        });
        
        on_hover_exit([this]() {
            std::cout << "No longer hovering over '" << _text << "'" << std::endl;
        });
        
        // Initial visual setup
        update_visual_state();
    }
    
    // Custom hit testing for circular shape
    bool contains_point(const guigui::Vector2& point) const override {
        auto center = get_position() + guigui::Vector2{_radius, _radius};
        auto distance = std::sqrt(
            std::pow(point.x - center.x, 2) + 
            std::pow(point.y - center.y, 2)
        );
        return distance <= _radius;
    }
    
    void set_text(const std::string& text) {
        if (_text != text) {
            _text = text;
            update_visual_state();
            _mark_dirty();
        }
    }
    
    std::string get_text() const {
        return _text;
    }
    
    void set_base_color(const guigui::Color& color) {
        _base_color = color;
        update_visual_state();
        _mark_dirty();
    }
    
    void set_radius(float radius) {
        _radius = radius;
        set_size({_radius * 2, _radius * 2});
        update_visual_state();
        _mark_dirty();
    }
    
    float get_radius() const {
        return _radius;
    }

protected:
    void update_visual_state() override {
        clear_primitives();
        
        // Calculate colors based on state and animation
        guigui::Color circle_color = calculate_current_color();
        
        // Create circle primitive
        auto center = get_position() + guigui::Vector2{_radius, _radius};
        auto circle = std::make_unique<guigui::CirclePrimitive>(
            center, _radius, circle_color
        );
        add_primitive(std::move(circle));
        
        // Add border if hovered or pressed
        if (get_interaction_state() != guigui::Component::InteractionState::NORMAL) {
            auto border_color = guigui::Color::from_rgb(50, 50, 50);
            auto border = std::make_unique<guigui::CircleOutlinePrimitive>(
                center, _radius, border_color, 2.0f
            );
            add_primitive(std::move(border));
        }
        
        // Add text
        if (!_text.empty()) {
            auto text_color = calculate_text_color(circle_color);
            auto text_pos = center - guigui::Vector2{
                static_cast<float>(_text.length() * 4), // Rough text width estimation
                8 // Half text height
            };
            
            auto text_primitive = std::make_unique<guigui::TextPrimitive>(
                _text, text_pos, text_color
            );
            add_primitive(std::move(text_primitive));
        }
        
        // Continue animation if needed
        if (_is_animating) {
            update_animation();
        }
    }
    
    void on_state_changed(guigui::Component::InteractionState old_state,
                         guigui::Component::InteractionState new_state) override {
        std::cout << "Circle button state changed: " 
                  << static_cast<int>(old_state) << " -> " 
                  << static_cast<int>(new_state) << std::endl;
        
        update_visual_state();
    }

private:
    guigui::Color calculate_current_color() const {
        guigui::Color base = _base_color;
        
        // Apply state-based modifications
        switch (get_interaction_state()) {
            case guigui::Component::InteractionState::HOVERED:
                base = lighten_color(base, 0.2f);
                break;
            case guigui::Component::InteractionState::PRESSED:
                base = darken_color(base, 0.3f);
                break;
            case guigui::Component::InteractionState::DISABLED:
                base = guigui::Color::from_rgb(128, 128, 128);
                break;
            default:
                break;
        }
        
        // Apply animation effect
        if (_is_animating) {
            float intensity = std::sin(_animation_progress * M_PI) * 0.3f;
            base = lighten_color(base, intensity);
        }
        
        return base;
    }
    
    guigui::Color calculate_text_color(const guigui::Color& background) const {
        // Simple contrast calculation
        float brightness = (background.r + background.g + background.b) / 3.0f;
        return brightness > 0.5f ? 
            guigui::Color::black() : 
            guigui::Color::white();
    }
    
    guigui::Color lighten_color(const guigui::Color& color, float amount) const {
        return guigui::Color::from_rgba(
            std::min(1.0f, color.r + amount),
            std::min(1.0f, color.g + amount),
            std::min(1.0f, color.b + amount),
            color.a
        );
    }
    
    guigui::Color darken_color(const guigui::Color& color, float amount) const {
        return guigui::Color::from_rgba(
            std::max(0.0f, color.r - amount),
            std::max(0.0f, color.g - amount),
            std::max(0.0f, color.b - amount),
            color.a
        );
    }
    
    void start_click_animation() {
        _is_animating = true;
        _animation_progress = 0.0f;
        update_visual_state();
        _mark_dirty();
    }
    
    void update_animation() {
        if (!_is_animating) return;
        
        _animation_progress += 0.1f; // Animation speed
        
        if (_animation_progress >= 1.0f) {
            _is_animating = false;
            _animation_progress = 0.0f;
        }
        
        // Request next frame
        _mark_dirty();
    }
};

// Static member definition
int CircleButton::_instance_counter = 0;

/**
 * @class ColorPicker
 * @brief A simple color picker component using multiple circle buttons
 */
class ColorPicker : public guigui::Container {
private:
    guigui::Color _selected_color;
    std::function<void(const guigui::Color&)> _on_color_changed;
    
public:
    ColorPicker() : Container("color_picker") {
        _selected_color = guigui::Color::white();
        setup_color_buttons();
    }
    
    void on_color_changed(std::function<void(const guigui::Color&)> callback) {
        _on_color_changed = callback;
    }
    
    guigui::Color get_selected_color() const {
        return _selected_color;
    }

private:
    void setup_color_buttons() {
        std::vector<guigui::Color> colors = {
            guigui::Color::from_rgb(255, 100, 100), // Red
            guigui::Color::from_rgb(100, 255, 100), // Green  
            guigui::Color::from_rgb(100, 100, 255), // Blue
            guigui::Color::from_rgb(255, 255, 100), // Yellow
            guigui::Color::from_rgb(255, 100, 255), // Magenta
            guigui::Color::from_rgb(100, 255, 255), // Cyan
        };
        
        for (size_t i = 0; i < colors.size(); ++i) {
            auto button = std::make_shared<CircleButton>("", 25);
            button->set_base_color(colors[i]);
            
            // Position in a grid
            int x = (i % 3) * 70 + 25;
            int y = (i / 3) * 70 + 25;
            button->set_position({static_cast<float>(x), static_cast<float>(y)});
            
            // Set up color selection
            button->on_click([this, colors, i]() {
                _selected_color = colors[i];
                std::cout << "Selected color: R=" << colors[i].r 
                         << " G=" << colors[i].g 
                         << " B=" << colors[i].b << std::endl;
                
                if (_on_color_changed) {
                    _on_color_changed(_selected_color);
                }
            });
            
            add_component(button);
        }
    }
};

/**
 * Main application demonstrating custom components
 */
class CustomComponentDemo {
private:
    std::shared_ptr<guigui::Context> context;
    std::shared_ptr<CircleButton> display_button;
    
public:
    CustomComponentDemo() {
        context = guigui::Context::create("Custom Component Demo", 800, 600);
        context->set_background_color(guigui::Color::from_rgb(245, 245, 245));
        
        setup_demo();
    }
    
    void setup_demo() {
        // Create some circle buttons with different properties
        auto small_button = std::make_shared<CircleButton>("Small", 20);
        small_button->set_position({50, 50});
        small_button->set_base_color(guigui::Color::from_rgb(255, 150, 150));
        context->add_component(small_button);
        
        auto medium_button = std::make_shared<CircleButton>("Medium", 35);
        medium_button->set_position({150, 40});
        medium_button->set_base_color(guigui::Color::from_rgb(150, 255, 150));
        context->add_component(medium_button);
        
        auto large_button = std::make_shared<CircleButton>("Large", 50);
        large_button->set_position({270, 25});
        large_button->set_base_color(guigui::Color::from_rgb(150, 150, 255));
        context->add_component(large_button);
        
        // Create a display button that changes based on color picker
        display_button = std::make_shared<CircleButton>("Color", 60);
        display_button->set_position({450, 50});
        context->add_component(display_button);
        
        // Create color picker
        auto color_picker = std::make_shared<ColorPicker>();
        color_picker->set_position({50, 200});
        
        color_picker->on_color_changed([this](const guigui::Color& color) {
            display_button->set_base_color(color);
        });
        
        context->add_component(color_picker);
        
        // Create info label
        auto info_label = guigui::Label::create(
            "Custom Component Demo\n"
            "• Circle buttons with custom hit testing\n" 
            "• Click animation effects\n"
            "• Color picker using container\n"
            "• Custom visual states"
        );
        info_label->set_position({300, 200});
        info_label->set_size({400, 150});
        context->add_component(info_label);
        
        // Exit button
        auto exit_button = guigui::Button::create("Exit");
        exit_button->set_position({350, 500});
        exit_button->set_size({100, 40});
        exit_button->on_click([this]() {
            context->stop();
        });
        context->add_component(exit_button);
    }
    
    void run() {
        try {
            std::cout << "Starting Custom Component Demo..." << std::endl;
            context->run();
            std::cout << "Demo finished" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
};

int main() {
    CustomComponentDemo demo;
    demo.run();
    return 0;
}
