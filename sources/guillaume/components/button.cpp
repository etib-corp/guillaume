/**
 * @file button.cpp
 * @brief Implementation of the Button component for the Guillaume GUI framework
 */

#include "guillaume/components/button.hpp"

namespace guigui {

Button::Button(const std::string& label, const Color& normal_background_color, const Color& normal_text_color,
    const Font& font, const Rectangle& rectangle, std::function<void(Button&)> on_click)
    : Component(rectangle) // Initialize base Component with bounds
    , _label(label)
    , _normal_background_color(normal_background_color)
    , _hovered_background_color(normal_background_color) // Default to same as normal
    , _pressed_background_color(normal_background_color) // Default to same as normal
    , _normal_text_color(normal_text_color)
    , _hovered_text_color(normal_text_color) // Default to same as normal
    , _pressed_text_color(normal_text_color) // Default to same as normal
    , _font(font)
    , _on_button_click(on_click)
{
    // Log button creation for debugging
    LOG_INFO_F("Button created: '{}' [{}] at bounds ({},{},{}x{})", 
               _label, get_identifier(),
               rectangle.get_x(), rectangle.get_y(), 
               rectangle.get_width(), rectangle.get_height());
    
    if (_on_button_click) {
        LOG_DEBUG_F("Button '{}' [{}] initialized with click callback", _label, get_identifier());
    }
    
    // Primitives will be added when renderer is set
}

void Button::set_renderer(std::shared_ptr<Renderer> renderer)
{
    // Call base class method to set renderer
    Component::set_renderer(renderer);
    
    // Create and add the visual primitives for the button
    Rectangle bounds = get_bounds();
    // Background rectangle primitive for the button background
    _add_primitive("background", createRectanglePrimitive(_renderer, bounds, _get_current_background_color()));
    // Text primitive for the button label
    _add_primitive("text", createTextPrimitive(_renderer, _font, Vector(bounds.get_x(), bounds.get_y()), _get_current_text_color(), _label));
}

void Button::draw()
{
    // Only draw if the button is visible
    if (is_visible()) {
        _execute_primitives(); // Execute all registered primitives
    }
}

Button::ButtonState Button::get_button_state() const
{
    // Map Component::InteractionState to Button::ButtonState for backward compatibility
    switch (get_interaction_state()) {
        case Component::InteractionState::HOVERED:
            return ButtonState::HOVERED;
        case Component::InteractionState::PRESSED:
            return ButtonState::PRESSED;
        default:
            return ButtonState::NORMAL;
    }
}

void Button::set_normal_colors(const Color& background_color, const Color& text_color)
{
    // Log color changes for debugging
    LOG_DEBUG_F("Button '{}' [{}] setting normal colors: bg({},{},{},{}), text({},{},{},{})", 
               _label, get_identifier(),
               background_color.get_red(), background_color.get_green(), 
               background_color.get_blue(), background_color.get_alpha(),
               text_color.get_red(), text_color.get_green(), 
               text_color.get_blue(), text_color.get_alpha());
               
    _normal_background_color = background_color;
    _normal_text_color = text_color;
    // Update visual state immediately if button is currently in normal state
    if (get_button_state() == ButtonState::NORMAL) {
        _update_visual_state();
    }
}

void Button::set_hovered_colors(const Color& background_color, const Color& text_color)
{
    // Log color changes for debugging
    LOG_DEBUG_F("Button '{}' [{}] setting hovered colors: bg({},{},{},{}), text({},{},{},{})", 
               _label, get_identifier(),
               background_color.get_red(), background_color.get_green(), 
               background_color.get_blue(), background_color.get_alpha(),
               text_color.get_red(), text_color.get_green(), 
               text_color.get_blue(), text_color.get_alpha());
               
    _hovered_background_color = background_color;
    _hovered_text_color = text_color;
    // Update visual state immediately if button is currently hovered
    if (get_button_state() == ButtonState::HOVERED) {
        _update_visual_state();
    }
}

void Button::set_pressed_colors(const Color& background_color, const Color& text_color)
{
    // Log color changes for debugging
    LOG_DEBUG_F("Button '{}' [{}] setting pressed colors: bg({},{},{},{}), text({},{},{},{})", 
               _label, get_identifier(),
               background_color.get_red(), background_color.get_green(), 
               background_color.get_blue(), background_color.get_alpha(),
               text_color.get_red(), text_color.get_green(), 
               text_color.get_blue(), text_color.get_alpha());
               
    _pressed_background_color = background_color;
    _pressed_text_color = text_color;
    // Update visual state immediately if button is currently pressed
    if (get_button_state() == ButtonState::PRESSED) {
        _update_visual_state();
    }
}

void Button::set_button_click_callback(std::function<void(Button&)> callback)
{
    // Log callback assignment for debugging
    LOG_DEBUG_F("Button '{}' [{}] setting click callback: {}", 
               _label, get_identifier(), 
               callback ? "provided" : "cleared");
    _on_button_click = callback;
}

void Button::_on_interaction_state_changed(Component::InteractionState old_state, Component::InteractionState new_state)
{
    // Log button-specific state changes with readable state names
    const char* state_names[] = {"NORMAL", "HOVERED", "PRESSED"};
    LOG_INFO_F("Button '{}' [{}] state change: {} to {}", 
               _label, 
               get_identifier(),
               state_names[static_cast<int>(old_state)], 
               state_names[static_cast<int>(new_state)]);
               
    // Update visual state when interaction state changes
    _update_visual_state();
}

void Button::_on_click_detected()
{
    // Log click events for debugging and user feedback
    LOG_INFO_F("Button '{}' [{}] CLICKED!", _label, get_identifier());
    
    // Call button-specific click callback if provided
    if (_on_button_click) {
        LOG_DEBUG_F("Executing button-specific callback for '{}'", _label);
        _on_button_click(*this);
    }
}

void Button::_update_visual_state()
{
    // Update the visual primitives to reflect the current state
    if (_renderer) {
        Rectangle bounds = get_bounds();
        // Update background rectangle with current state color
        _update_primitive("background", createRectanglePrimitive(_renderer, bounds, _get_current_background_color()));
        // Update text with current state color
        _update_primitive("text", createTextPrimitive(_renderer, _font, Vector(bounds.get_x(), bounds.get_y()), _get_current_text_color(), _label));
    }
}

Color Button::_get_current_background_color() const
{
    // Return the appropriate background color based on current state
    switch (get_button_state()) {
        case ButtonState::HOVERED:
            return _hovered_background_color;
        case ButtonState::PRESSED:
            return _pressed_background_color;
        default:
            return _normal_background_color;
    }
}

Color Button::_get_current_text_color() const
{
    // Return the appropriate text color based on current state
    switch (get_button_state()) {
        case ButtonState::HOVERED:
            return _hovered_text_color;
        case ButtonState::PRESSED:
            return _pressed_text_color;
        default:
            return _normal_text_color;
    }
}

void Button::set_background_color(const Color& color)
{
    // Legacy method - only sets normal background color
    _normal_background_color = color;
    // Update visual state if currently in normal state
    if (get_button_state() == ButtonState::NORMAL) {
        _update_visual_state();
    }
}

void Button::set_label(const std::string& label)
{
    // Log label changes for debugging
    LOG_DEBUG_F("Button [{}] label changed: '{}'  '{}'", get_identifier(), _label, label);
    _label = label;
    // Update the text primitive with the new label
    if (_renderer) {
        Rectangle bounds = get_bounds();
        _update_primitive("text", createTextPrimitive(_renderer, _font, Vector(bounds.get_x(), bounds.get_y()), _get_current_text_color(), _label));
    }
}

} // namespace guigui
