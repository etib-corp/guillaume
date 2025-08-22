/**
 * @file label.cpp
 * @brief Implementation of the Label component for the Guillaume GUI framework
 */

#include "guillaume/components/label.hpp"
#include "guillaume/primitives/text_primitive.hpp"
#include <algorithm>

namespace guigui {

Label::Label(const std::string& text, const Color& text_color, const Font& font,
             const Rectangle& rectangle, TextAlignment alignment, bool word_wrap, bool auto_size)
    : Component(rectangle), _text(text), _text_color(text_color), _font(font),
      _alignment(alignment), _word_wrap(word_wrap), _auto_size(auto_size) {
    
    LOG_DEBUG_F("Label created: '{}' at ({}, {}, {}, {})", 
                text, rectangle.get_x(), rectangle.get_y(), 
                rectangle.get_width(), rectangle.get_height());
}

void Label::set_renderer(std::shared_ptr<Renderer> renderer) {
    Component::set_renderer(renderer);
    _update_visual_state();
}

void Label::_update_visual_state() {
    if (!_renderer) {
        LOG_WARN("Label::_update_visual_state called without renderer");
        return;
    }

    if (!_text.empty()) {
        Vector text_position = _calculate_text_position();
        _add_primitive("text", createTextPrimitive(_renderer, _font, text_position, _text_color, _text));
    }

    LOG_DEBUG_F("Label visual state updated for '{}'", _identifier);
}

void Label::draw() {
    if (_is_visible && _renderer) {
        _execute_primitives();
    }
}

void Label::set_text(const std::string& text) {
    if (_text != text) {
        _text = text;
        _update_visual_state();
        LOG_DEBUG_F("Label '{}' text changed to '{}'", _identifier, text);
    }
}

const std::string& Label::get_text() const {
    return _text;
}

void Label::set_text_color(const Color& color) {
    if (_text_color != color) {
        _text_color = color;
        _update_visual_state();
        LOG_DEBUG_F("Label '{}' text color changed", _identifier);
    }
}

const Color& Label::get_text_color() const {
    return _text_color;
}

void Label::set_font(const Font& font) {
    _font = font;
    _update_visual_state();
    LOG_DEBUG_F("Label '{}' font changed", _identifier);
}

const Font& Label::get_font() const {
    return _font;
}

void Label::set_alignment(TextAlignment alignment) {
    if (_alignment != alignment) {
        _alignment = alignment;
        _update_visual_state();
        LOG_DEBUG_F("Label '{}' alignment changed", _identifier);
    }
}

Label::TextAlignment Label::get_alignment() const {
    return _alignment;
}

void Label::set_word_wrap(bool word_wrap) {
    if (_word_wrap != word_wrap) {
        _word_wrap = word_wrap;
        _update_visual_state();
        LOG_DEBUG_F("Label '{}' word wrap {}d", _identifier, word_wrap ? "enable" : "disable");
    }
}

bool Label::get_word_wrap() const {
    return _word_wrap;
}

void Label::set_auto_size(bool auto_size) {
    if (_auto_size != auto_size) {
        _auto_size = auto_size;
        LOG_DEBUG_F("Label '{}' auto-size {}d", _identifier, auto_size ? "enable" : "disable");
    }
}

bool Label::get_auto_size() const {
    return _auto_size;
}

Vector Label::_calculate_text_position() const {
    Rectangle bounds = get_bounds();
    
    Vector text_size = Vector(0, 0);
    if (_renderer && !_text.empty()) {
        text_size = _renderer->get_text_size(_font, _text);
    }
    
    int x, y;
    
    // Calculate horizontal position based on alignment
    switch (_alignment) {
        case TextAlignment::LEFT:
            x = bounds.get_x();
            break;
        case TextAlignment::CENTER:
            x = bounds.get_x() + (bounds.get_width() - text_size.get_x()) / 2;
            break;
        case TextAlignment::RIGHT:
            x = bounds.get_x() + bounds.get_width() - text_size.get_x();
            break;
        default:
            x = bounds.get_x();
            break;
    }
    
    // Center vertically
    y = bounds.get_y() + (bounds.get_height() - text_size.get_y()) / 2;
    
    return Vector(x, y);
}

Rectangle Label::_calculate_text_bounds() const {
    if (_renderer && !_text.empty()) {
        Vector text_size = _renderer->get_text_size(_font, _text);
        Vector text_pos = _calculate_text_position();
        return Rectangle(text_pos.get_x(), text_pos.get_y(), 
                        text_size.get_x(), text_size.get_y());
    }
    return Rectangle(0, 0, 0, 0);
}

} // namespace guigui
