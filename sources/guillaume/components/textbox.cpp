/**
 * @file textbox.cpp
 * @brief Implementation of the TextBox component for the Guillaume GUI framework
 */

#include "guillaume/components/textbox.hpp"
#include "guillaume/primitives/rectangle_primitive.hpp"
#include "guillaume/primitives/text_primitive.hpp"
#include <algorithm>

namespace guigui {

TextBox::TextBox(const std::string& placeholder, const Color& background_color, 
                 const Color& border_color, const Color& text_color, const Font& font,
                 const Rectangle& rectangle)
    : Component(rectangle), _text(""), _placeholder(placeholder),
      _background_color(background_color), _text_color(text_color),
      _border_color(border_color), _focused_border_color(border_color), _font(font), 
      _placeholder_color(Color(150, 150, 150, 255)), _cursor_color(Color(0, 0, 0, 255)),
      _is_focused(false), _is_password(false), _password_char('*'), _cursor_position(0),
      _selection_start(0), _selection_end(0), _max_length(0), _border_width(1),
      _validation_callback(nullptr), _text_change_callback(nullptr) {
    
    LOG_DEBUG_F("TextBox created with placeholder: '{}' at ({}, {}, {}, {})", 
                placeholder, rectangle.get_x(), rectangle.get_y(), 
                rectangle.get_width(), rectangle.get_height());
}

void TextBox::set_renderer(std::shared_ptr<Renderer> renderer) {
    Component::set_renderer(renderer);
    _update_visual_state();
}

void TextBox::_update_visual_state() {
    if (!_renderer) {
        LOG_WARN("TextBox::_update_visual_state called without renderer");
        return;
    }

    Rectangle bounds = get_bounds();
    
    // Background
    _add_primitive("background", createRectanglePrimitive(_renderer, bounds, _background_color));
    
    // Border (use focused border color if focused)
    Color border_color = _is_focused ? _focused_border_color : _border_color;
    
    // Top border
    Rectangle top_border(bounds.get_x(), bounds.get_y(), bounds.get_width(), _border_width);
    _add_primitive("border_top", createRectanglePrimitive(_renderer, top_border, border_color));
    
    // Bottom border
    Rectangle bottom_border(bounds.get_x(), bounds.get_y() + bounds.get_height() - _border_width,
                           bounds.get_width(), _border_width);
    _add_primitive("border_bottom", createRectanglePrimitive(_renderer, bottom_border, border_color));
    
    // Left border
    Rectangle left_border(bounds.get_x(), bounds.get_y(), _border_width, bounds.get_height());
    _add_primitive("border_left", createRectanglePrimitive(_renderer, left_border, border_color));
    
    // Right border
    Rectangle right_border(bounds.get_x() + bounds.get_width() - _border_width, bounds.get_y(),
                          _border_width, bounds.get_height());
    _add_primitive("border_right", createRectanglePrimitive(_renderer, right_border, border_color));
    
    // Text content
    std::string display_text = _get_display_text();
    
    if (!display_text.empty()) {
        Vector text_position = _calculate_text_position();
        _add_primitive("text", createTextPrimitive(_renderer, _font, text_position, _text_color, display_text));
    } else if (!_placeholder.empty() && !_is_focused) {
        // Show placeholder when empty and not focused
        Vector text_position = _calculate_text_position();
        _add_primitive("text", createTextPrimitive(_renderer, _font, text_position, _placeholder_color, _placeholder));
    }
    
    // Cursor (only when focused)
    if (_is_focused && is_enabled()) {
        Vector cursor_position = _calculate_cursor_position();
        Rectangle cursor_rect(cursor_position.get_x(), cursor_position.get_y(), 1, 
                             _renderer->get_text_size(_font, "Ag").get_y());
        _add_primitive("cursor", createRectanglePrimitive(_renderer, cursor_rect, _cursor_color));
    }

    LOG_DEBUG_F("TextBox visual state updated for '{}'", _identifier);
}

void TextBox::draw() {
    if (_is_visible && _renderer) {
        _execute_primitives();
    }
}

void TextBox::set_text(const std::string& text) {
    if (_text != text) {
        if (_max_length > 0 && text.length() > _max_length) {
            _text = text.substr(0, _max_length);
        } else {
            _text = text;
        }
        
        _cursor_position = std::min(_cursor_position, _text.length());
        _selection_start = _selection_end = _cursor_position;
        
        _update_visual_state();
        
        if (_text_change_callback) {
            _text_change_callback(*this, _text, _text); // Changed signature
        }
        
        LOG_DEBUG_F("TextBox '{}' text changed to '{}'", _identifier, _text);
    }
}

const std::string& TextBox::get_text() const {
    return _text;
}

void TextBox::set_placeholder(const std::string& placeholder) {
    if (_placeholder != placeholder) {
        _placeholder = placeholder;
        _update_visual_state();
        LOG_DEBUG_F("TextBox '{}' placeholder changed to '{}'", _identifier, placeholder);
    }
}

const std::string& TextBox::get_placeholder() const {
    return _placeholder;
}

void TextBox::set_password(bool is_password, char password_char) {
    if (_is_password != is_password || _password_char != password_char) {
        _is_password = is_password;
        _password_char = password_char;
        _update_visual_state();
        LOG_DEBUG_F("TextBox '{}' password mode {} with char '{}'", _identifier, is_password ? "enabled" : "disabled", password_char);
    }
}

bool TextBox::is_password() const {
    return _is_password;
}

void TextBox::set_max_length(size_t max_length) {
    _max_length = max_length;
    
    if (max_length > 0 && _text.length() > max_length) {
        set_text(_text.substr(0, max_length));
    }
    
    LOG_DEBUG_F("TextBox '{}' max length set to {}", _identifier, max_length);
}

size_t TextBox::get_max_length() const {
    return _max_length;
}

void TextBox::set_validation_callback(ValidationCallback callback) {
    _validation_callback = callback;
    LOG_DEBUG_F("TextBox '{}' validation callback {}", 
               _identifier, callback ? "set" : "cleared");
}

void TextBox::set_text_change_callback(TextChangeCallback callback) {
    _text_change_callback = callback;
    LOG_DEBUG_F("TextBox '{}' text change callback {}", 
               _identifier, callback ? "set" : "cleared");
}

void TextBox::clear() {
    set_text("");
}

void TextBox::select_all() {
    _selection_start = 0;
    _selection_end = _text.length();
    _cursor_position = _selection_end;
    _update_visual_state();
    LOG_DEBUG_F("TextBox '{}' all text selected", _identifier);
}

std::string TextBox::get_selected_text() const {
    if (_selection_start == _selection_end) {
        return "";
    }
    
    size_t start = std::min(_selection_start, _selection_end);
    size_t end = std::max(_selection_start, _selection_end);
    return _text.substr(start, end - start);
}

void TextBox::set_focused(bool focused) {
    if (_is_focused != focused) {
        _is_focused = focused;
        _update_visual_state();
        LOG_DEBUG_F("TextBox '{}' focus {}", _identifier, focused ? "gained" : "lost");
    }
}

bool TextBox::is_focused() const {
    return _is_focused;
}

void TextBox::_on_interaction_state_changed(Component::InteractionState old_state, Component::InteractionState new_state) {
    Component::_on_interaction_state_changed(old_state, new_state);
    _update_visual_state();
}

void TextBox::_on_click_detected() {
    if (is_enabled()) {
        set_focused(true);
    }
}

Vector TextBox::_calculate_text_position() const {
    Rectangle bounds = get_bounds();
    int padding = _border_width + 4; // Add some inner padding
    
    Vector text_size = Vector(0, 0);
    if (_renderer) {
        text_size = _renderer->get_text_size(_font, "Ag"); // Use sample text for height
    }
    
    int x = bounds.get_x() + padding;
    int y = bounds.get_y() + (bounds.get_height() - text_size.get_y()) / 2;
    
    return Vector(x, y);
}

Vector TextBox::_calculate_cursor_position() const {
    Vector text_pos = _calculate_text_position();
    
    if (_renderer && _cursor_position > 0) {
        std::string text_before_cursor = _get_display_text().substr(0, _cursor_position);
        Vector text_width = _renderer->get_text_size(_font, text_before_cursor);
        return Vector(text_pos.get_x() + text_width.get_x(), text_pos.get_y());
    }
    
    return text_pos;
}

std::string TextBox::_get_display_text() const {
    if (_is_password && !_text.empty()) {
        return std::string(_text.length(), _password_char);
    }
    return _text;
}

} // namespace guigui
