/**
 * @file slider.cpp
 * @brief Implementation of the Slider component for the Guillaume GUI framework
 */

#include "guillaume/components/slider.hpp"
#include "guillaume/primitives/rectangle_primitive.hpp"
#include "guillaume/primitives/text_primitive.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>

namespace guigui {

Slider::Slider(double min_value, double max_value, double initial_value,
    Orientation orientation, const Color& track_color, const Color& thumb_color,
    const Color& filled_track_color, const Rectangle& rectangle,
    double step_size)
    : Component(rectangle)
    , _min_value(min_value)
    , _max_value(max_value)
    , _value(initial_value < min_value ? min_value : (initial_value > max_value ? max_value : initial_value))
    , _track_color(track_color)
    , _filled_track_color(filled_track_color)
    , _thumb_color(thumb_color)
    , _orientation(orientation)
    , _step_size(step_size)
    , _thumb_size(20)
    , _track_thickness(4)
    , _show_value(false)
    , _value_text_color(Color(0, 0, 0, 255))
    , _disabled_track_color(Color(200, 200, 200, 255))
    , _disabled_thumb_color(Color(180, 180, 180, 255))
    , _hovered_thumb_color(Color(100, 100, 255, 255))
    , _pressed_thumb_color(Color(80, 80, 200, 255))
    , _is_dragging(false)
    , _drag_offset(Vector(0, 0))
    , _value_change_callback(nullptr)
{

    LOG_DEBUG_F("Slider created: range[{}, {}], value={}, orientation={} at ({}, {}, {}, {})",
        min_value, max_value, _value,
        orientation == Orientation::HORIZONTAL ? "HORIZONTAL" : "VERTICAL",
        rectangle.get_x(), rectangle.get_y(),
        rectangle.get_width(), rectangle.get_height());
}

void Slider::set_renderer(std::shared_ptr<Renderer> renderer)
{
    LOG_DEBUG_F("Slider::set_renderer called for '{}'", _identifier);
    Component::set_renderer(renderer);

    // Create initial primitives when renderer is set
    if (_renderer) {
        LOG_DEBUG_F("Slider '{}' creating primitives", _identifier);

        // Create simple track background
        Rectangle bounds = get_bounds();
        _add_primitive("track", createRectanglePrimitive(_renderer, bounds, _track_color));

        // Create filled track
        Rectangle filled_bounds = Rectangle(bounds.get_x(), bounds.get_y(), bounds.get_width() / 2, bounds.get_height());
        _add_primitive("filled_track", createRectanglePrimitive(_renderer, filled_bounds, _filled_track_color));

        // Create thumb
        Rectangle thumb_bounds = Rectangle(bounds.get_x() + bounds.get_width() / 2 - 10, bounds.get_y(), 20, bounds.get_height());
        _add_primitive("thumb", createRectanglePrimitive(_renderer, thumb_bounds, _thumb_color));

        // Create empty text primitive
        Font default_font("Roboto", "assets/Roboto.ttf", 12);
        _add_primitive("value_text", createTextPrimitive(_renderer, default_font, Vector(0, 0), Color(0, 0, 0, 0), ""));

        LOG_DEBUG_F("Slider '{}' primitives created successfully", _identifier);
    }
}

void Slider::_update_visual_state()
{
    LOG_DEBUG_F("Slider::_update_visual_state called for '{}', renderer: {}", _identifier, _renderer ? "present" : "null");
    if (!_renderer) {
        return;
    }

    // Keep it simple - just update with fixed positions
    Rectangle bounds = get_bounds();
    _add_primitive("track", createRectanglePrimitive(_renderer, bounds, _track_color));

    Rectangle filled_bounds = Rectangle(bounds.get_x(), bounds.get_y(), bounds.get_width() / 2, bounds.get_height());
    _add_primitive("filled_track", createRectanglePrimitive(_renderer, filled_bounds, _filled_track_color));

    Rectangle thumb_bounds = Rectangle(bounds.get_x() + bounds.get_width() / 2 - 10, bounds.get_y(), 20, bounds.get_height());
    _add_primitive("thumb", createRectanglePrimitive(_renderer, thumb_bounds, _thumb_color));

    Font default_font("Roboto", "assets/Roboto.ttf", 12);
    _add_primitive("value_text", createTextPrimitive(_renderer, default_font, Vector(0, 0), Color(0, 0, 0, 0), ""));

    LOG_DEBUG_F("Slider visual state updated for '{}'", _identifier);
}

void Slider::draw()
{
    if (_is_visible && _renderer) {
        _execute_primitives();
    }
}

// Getter methods
double Slider::get_value() const
{
    return _value;
}

double Slider::get_min_value() const
{
    return _min_value;
}

double Slider::get_max_value() const
{
    return _max_value;
}

Slider::Orientation Slider::get_orientation() const
{
    return _orientation;
}

bool Slider::is_value_display_enabled() const
{
    return _show_value;
}

// Setter methods
void Slider::set_value(double value)
{
    double new_value = value < _min_value ? _min_value : (value > _max_value ? _max_value : value);
    if (new_value != _value) {
        _value = new_value;
        if (_renderer) {
            _update_visual_state();
        }
        _on_value_changed(new_value);
    }
}

void Slider::set_range(double min_value, double max_value)
{
    _min_value = min_value;
    _max_value = max_value;
    set_value(_value); // Re-validate current value
}

void Slider::set_step_size(double step_size)
{
    _step_size = step_size;
}

void Slider::set_orientation(Orientation orientation)
{
    if (_orientation != orientation) {
        _orientation = orientation;
        if (_renderer) {
            _update_visual_state();
        }
    }
}

void Slider::set_colors(const Color& track_color, const Color& thumb_color, const Color& filled_track_color,
    const Color& hovered_thumb_color, const Color& pressed_thumb_color,
    const Color& disabled_track_color, const Color& disabled_thumb_color)
{
    _track_color = track_color;
    _thumb_color = thumb_color;
    _filled_track_color = filled_track_color;
    _hovered_thumb_color = hovered_thumb_color;
    _pressed_thumb_color = pressed_thumb_color;
    _disabled_track_color = disabled_track_color;
    _disabled_thumb_color = disabled_thumb_color;
    if (_renderer) {
        _update_visual_state();
    }
}

void Slider::set_value_display(bool show_value)
{
    if (_show_value != show_value) {
        _show_value = show_value;
        if (_renderer) {
            _update_visual_state();
        }
    }
}

void Slider::set_value_display(bool show_value, const Font& font, const Color& text_color)
{
    _show_value = show_value;
    // _value_font = font;  // Field not in header, commenting out
    _value_text_color = text_color;
    if (_renderer) {
        _update_visual_state();
    }
}

void Slider::set_value_change_callback(ValueChangeCallback callback)
{
    _value_change_callback = std::move(callback);
}

// Protected helper methods (simple implementations)
Rectangle Slider::_calculate_track_bounds() const
{
    return get_bounds();
}

Rectangle Slider::_calculate_filled_track_bounds() const
{
    Rectangle bounds = get_bounds();
    return Rectangle(bounds.get_x(), bounds.get_y(), bounds.get_width() / 2, bounds.get_height());
}

Rectangle Slider::_calculate_thumb_bounds() const
{
    Rectangle bounds = get_bounds();
    return Rectangle(bounds.get_x() + bounds.get_width() / 2 - 10, bounds.get_y(), 20, bounds.get_height());
}

Vector Slider::_calculate_value_text_position() const
{
    Rectangle bounds = get_bounds();
    return Vector(bounds.get_x(), bounds.get_y() - 20);
}

Color Slider::_get_current_thumb_color() const
{
    return is_enabled() ? _thumb_color : _disabled_thumb_color;
}

Vector Slider::_value_to_position(double value) const
{
    Rectangle bounds = get_bounds();
    return Vector(bounds.get_x() + bounds.get_width() / 2, bounds.get_y() + bounds.get_height() / 2);
}

double Slider::_position_to_value(const Vector& position) const
{
    return _value;
}

double Slider::_clamp_value(double value) const
{
    return value < _min_value ? _min_value : (value > _max_value ? _max_value : value);
}

double Slider::_apply_step(double value) const
{
    return value; // Simple implementation - no stepping
}

std::string Slider::_format_value() const
{
    std::stringstream ss;
    ss << static_cast<int>(_value);
    return ss.str();
}

void Slider::_on_value_changed(double new_value)
{
    if (_value_change_callback) {
        _value_change_callback(*this, new_value);
    }
}

bool Slider::_is_point_in_thumb(const Vector& point) const
{
    return false; // Simple implementation
}

// Event handlers
void Slider::_on_interaction_state_changed(Component::InteractionState old_state, Component::InteractionState new_state)
{
    Component::_on_interaction_state_changed(old_state, new_state);
    // Only update visual state if renderer is present
    if (_renderer) {
        _update_visual_state();
    }
}

void Slider::_on_click_detected()
{
    // Basic click handling for slider could be implemented here
}

} // namespace guigui
