/**
 * @file checkbox.cpp
 * @brief Implementation of the Checkbox component for the Guillaume GUI framework
 */

#include "guillaume/components/checkbox.hpp"
#include "guillaume/primitives/rectangle_primitive.hpp"
#include "guillaume/primitives/text_primitive.hpp"
#include <algorithm>

namespace guigui {

Checkbox::Checkbox(const std::string& label, bool is_checked, const Color& box_color,
    const Color& check_color, const Color& text_color, const Font& font,
    const Rectangle& rectangle, LabelPosition label_position)
    : Component(rectangle)
    , _is_checked(is_checked)
    , _label(label)
    , _box_color(box_color)
    , _check_color(check_color)
    , _text_color(text_color)
    , _font(font)
    , _label_position(label_position)
    , _border_width(1)
    , _border_color(Color(0, 0, 0, 255))
    , _disabled_box_color(Color(200, 200, 200, 255))
    , _disabled_text_color(Color(128, 128, 128, 255))
    , _state_change_callback(nullptr)
{

    LOG_DEBUG_F("Checkbox created: '{}' (checked: {}) at ({}, {}, {}, {})",
        label, is_checked, rectangle.get_x(), rectangle.get_y(),
        rectangle.get_width(), rectangle.get_height());
}

void Checkbox::set_renderer(std::shared_ptr<Renderer> renderer)
{
    Component::set_renderer(renderer);
    _update_visual_state();
}

void Checkbox::_update_visual_state()
{
    if (!_renderer) {
        LOG_WARN("Checkbox::_update_visual_state called without renderer");
        return;
    }

    Rectangle box_bounds = _calculate_box_bounds();
    Color current_box_color = _get_current_box_color();

    // Create or update checkbox box background
    _add_primitive("box_background", createRectanglePrimitive(_renderer, box_bounds, current_box_color));

    // Create or update checkbox borders
    Rectangle top_border(box_bounds.get_x(), box_bounds.get_y(),
        box_bounds.get_width(), _border_width);
    _add_primitive("border_top", createRectanglePrimitive(_renderer, top_border, _border_color));

    Rectangle bottom_border(box_bounds.get_x(),
        box_bounds.get_y() + box_bounds.get_height() - _border_width,
        box_bounds.get_width(), _border_width);
    _add_primitive("border_bottom", createRectanglePrimitive(_renderer, bottom_border, _border_color));

    Rectangle left_border(box_bounds.get_x(), box_bounds.get_y(),
        _border_width, box_bounds.get_height());
    _add_primitive("border_left", createRectanglePrimitive(_renderer, left_border, _border_color));

    Rectangle right_border(box_bounds.get_x() + box_bounds.get_width() - _border_width,
        box_bounds.get_y(), _border_width, box_bounds.get_height());
    _add_primitive("border_right", createRectanglePrimitive(_renderer, right_border, _border_color));

    // Create or update check mark if checked, make transparent if unchecked
    if (_is_checked) {
        Rectangle check_bounds = _calculate_check_bounds();
        Color current_check_color = _check_color; // Use the normal check color
        _add_primitive("check_mark", createRectanglePrimitive(_renderer, check_bounds, current_check_color));
    } else {
        // Create a transparent rectangle to "hide" the check mark when unchecked
        Rectangle check_bounds = _calculate_check_bounds();
        Color transparent_color = Color(0, 0, 0, 0); // Transparent
        _add_primitive("check_mark", createRectanglePrimitive(_renderer, check_bounds, transparent_color));
    }

    // Create or update label text
    if (!_label.empty()) {
        Vector text_position = _calculate_label_position();
        Color current_text_color = _get_current_text_color();
        _add_primitive("label", createTextPrimitive(_renderer, _font, text_position, current_text_color, _label));
    }

    LOG_DEBUG_F("Checkbox visual state updated for '{}'", _identifier);
}

void Checkbox::draw()
{
    if (_is_visible && _renderer) {
        _execute_primitives();
    }
}

void Checkbox::set_checked(bool checked)
{
    if (_is_checked != checked) {
        _is_checked = checked;
        _update_visual_state();

        if (_state_change_callback) {
            _state_change_callback(*this, _is_checked);
        }

        LOG_DEBUG_F("Checkbox '{}' state changed to {}", _identifier, checked ? "checked" : "unchecked");
    }
}

bool Checkbox::is_checked() const
{
    return _is_checked;
}

void Checkbox::toggle()
{
    set_checked(!_is_checked);
}

void Checkbox::set_label(const std::string& label)
{
    if (_label != label) {
        _label = label;
        _update_visual_state();
        LOG_DEBUG_F("Checkbox '{}' label changed to '{}'", _identifier, label);
    }
}

const std::string& Checkbox::get_label() const
{
    return _label;
}

void Checkbox::set_label_position(LabelPosition position)
{
    if (_label_position != position) {
        _label_position = position;
        _update_visual_state();
        LOG_DEBUG_F("Checkbox '{}' label position changed", _identifier);
    }
}

Checkbox::LabelPosition Checkbox::get_label_position() const
{
    return _label_position;
}

void Checkbox::set_state_change_callback(StateChangeCallback callback)
{
    _state_change_callback = callback;
    LOG_DEBUG_F("Checkbox '{}' state change callback {}",
        _identifier, callback ? "set" : "cleared");
}

void Checkbox::_on_interaction_state_changed(Component::InteractionState old_state, Component::InteractionState new_state)
{
    Component::_on_interaction_state_changed(old_state, new_state);
    _update_visual_state();
}

void Checkbox::_on_click_detected()
{
    if (is_enabled()) {
        toggle();
    }
}

Rectangle Checkbox::_calculate_box_bounds() const
{
    Rectangle bounds = get_bounds();
    int box_size = std::min(bounds.get_width(), bounds.get_height());

    // Adjust for label position
    switch (_label_position) {
    case LabelPosition::RIGHT:
        return Rectangle(bounds.get_x(), bounds.get_y(), box_size, box_size);
    case LabelPosition::LEFT:
        return Rectangle(bounds.get_x() + bounds.get_width() - box_size,
            bounds.get_y(), box_size, box_size);
    case LabelPosition::TOP:
        return Rectangle(bounds.get_x(), bounds.get_y() + bounds.get_height() - box_size,
            box_size, box_size);
    case LabelPosition::BOTTOM:
        return Rectangle(bounds.get_x(), bounds.get_y(), box_size, box_size);
    default:
        return Rectangle(bounds.get_x(), bounds.get_y(), box_size, box_size);
    }
}

Rectangle Checkbox::_calculate_check_bounds() const
{
    Rectangle box_bounds = _calculate_box_bounds();
    int padding = 3;
    return Rectangle(box_bounds.get_x() + padding, box_bounds.get_y() + padding,
        box_bounds.get_width() - 2 * padding,
        box_bounds.get_height() - 2 * padding);
}

Vector Checkbox::_calculate_label_position() const
{
    Rectangle bounds = get_bounds();
    Rectangle box_bounds = _calculate_box_bounds();

    Vector text_size = Vector(0, 0);
    if (_renderer && !_label.empty()) {
        text_size = _renderer->get_text_size(_font, _label);
    }

    int spacing = 8;

    switch (_label_position) {
    case LabelPosition::RIGHT:
        return Vector(box_bounds.get_x() + box_bounds.get_width() + spacing,
            box_bounds.get_y() + (box_bounds.get_height() - text_size.get_y()) / 2);
    case LabelPosition::LEFT:
        return Vector(bounds.get_x(),
            box_bounds.get_y() + (box_bounds.get_height() - text_size.get_y()) / 2);
    case LabelPosition::TOP:
        return Vector(bounds.get_x() + (bounds.get_width() - text_size.get_x()) / 2,
            bounds.get_y());
    case LabelPosition::BOTTOM:
        return Vector(bounds.get_x() + (bounds.get_width() - text_size.get_x()) / 2,
            box_bounds.get_y() + box_bounds.get_height() + spacing);
    default:
        return Vector(box_bounds.get_x() + box_bounds.get_width() + spacing,
            box_bounds.get_y() + (box_bounds.get_height() - text_size.get_y()) / 2);
    }
}

Color Checkbox::_get_current_box_color() const
{
    return is_enabled() ? _box_color : _disabled_box_color;
}

Color Checkbox::_get_current_text_color() const
{
    return is_enabled() ? _text_color : _disabled_text_color;
}

} // namespace guigui
