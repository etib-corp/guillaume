#pragma once

#include "guillaume/component.hpp"

#include "guillaume/color.hpp"
#include "guillaume/font.hpp"
#include "guillaume/rectangle.hpp"
#include "guillaume/vector.hpp"
#include "guillaume/events/mouse_event.hpp"
#include "guillaume/logger.hpp"

#include "guillaume/primitives/rectangle_primitive.hpp"
#include "guillaume/primitives/text_primitive.hpp"

namespace guigui {

class Button : public Component {
public:
    enum class ButtonState {
        NORMAL,
        HOVERED,
        PRESSED
    };

private:
    std::string _label;
    Color _normal_background_color;
    Color _hovered_background_color;
    Color _pressed_background_color;
    Color _normal_text_color;
    Color _hovered_text_color;
    Color _pressed_text_color;
    Font _font;
    std::function<void(Button&)> _on_button_click; // Renamed to avoid conflict with base class

protected:
    void _update_visual_state();
    Color _get_current_background_color() const;
    Color _get_current_text_color() const;
    
    // Override base Component interaction methods
    void _on_interaction_state_changed(Component::InteractionState old_state, Component::InteractionState new_state) override;
    void _on_click_detected() override;

public:
    Button(const std::string& label, const Color& normal_background_color, const Color& normal_text_color,
        const Font& font, const Rectangle& rectangle, std::function<void(Button&)> on_click = nullptr);

    // State management (now uses base Component's InteractionState)
    ButtonState get_button_state() const;
    
    // Color setters for different states
    void set_normal_colors(const Color& background_color, const Color& text_color);
    void set_hovered_colors(const Color& background_color, const Color& text_color);
    void set_pressed_colors(const Color& background_color, const Color& text_color);
    
    // Button-specific click callback
    void set_button_click_callback(std::function<void(Button&)> callback);

    void set_renderer(std::shared_ptr<Renderer> renderer) override;
    void draw() override;
    void set_background_color(const Color& color); // Legacy method - sets normal background color
    void set_label(const std::string& label);
};

}
