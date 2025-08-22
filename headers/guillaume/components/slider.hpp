/**
 * @file slider.hpp
 * @brief Slider component implementation for the Guillaume GUI framework
 *
 * This file contains the Slider class which provides a slider component
 * for selecting values within a specified range.
 */

#pragma once

#include "guillaume/component.hpp"

#include "guillaume/color.hpp"
#include "guillaume/font.hpp"
#include "guillaume/logger.hpp"
#include "guillaume/rectangle.hpp"
#include "guillaume/vector.hpp"

#include "guillaume/primitives/rectangle_primitive.hpp"
#include "guillaume/primitives/text_primitive.hpp"

#include <functional>
#include <optional>
#include <string>

namespace guigui {

/**
 * @class Slider
 * @brief A slider component for selecting values within a range
 *
 * The Slider class extends Component to provide a slider that allows
 * users to select a value from a continuous or discrete range.
 *
 * @details Features:
 * - Horizontal and vertical orientation support
 * - Customizable value range (min/max)
 * - Step size for discrete values
 * - Value change callbacks
 * - Optional value display labels
 * - Customizable track and thumb colors
 * - Keyboard navigation support
 * - Precise mouse control
 *
 * @note The Slider uses rectangle primitives for rendering.
 */
class Slider : public Component {
public:
    /**
     * @enum Orientation
     * @brief Defines the orientation of the slider
     */
    enum class Orientation {
        HORIZONTAL, ///< Horizontal slider (left to right)
        VERTICAL ///< Vertical slider (bottom to top)
    };

    /**
     * @brief Type definition for value change callback
     * @param slider Reference to the slider that changed
     * @param value New value
     */
    using ValueChangeCallback = std::function<void(Slider&, double)>;

private:
    double _value; ///< Current value
    double _min_value; ///< Minimum value
    double _max_value; ///< Maximum value
    double _step_size; ///< Step size for discrete values (0 = continuous)
    Orientation _orientation; ///< Slider orientation
    Color _track_color; ///< Color of the slider track
    Color _thumb_color; ///< Color of the slider thumb
    Color _filled_track_color; ///< Color of the filled portion of track
    Color _hovered_thumb_color; ///< Color of thumb when hovered
    Color _pressed_thumb_color; ///< Color of thumb when pressed
    Color _disabled_track_color; ///< Color of track when disabled
    Color _disabled_thumb_color; ///< Color of thumb when disabled
    int _track_thickness; ///< Thickness of the track
    int _thumb_size; ///< Size of the thumb
    bool _show_value; ///< Whether to display the current value
    std::optional<Font> _value_font; ///< Font for value display (optional)
    Color _value_text_color; ///< Color for value text
    bool _is_dragging; ///< Whether the thumb is being dragged
    Vector _drag_offset; ///< Offset for dragging
    ValueChangeCallback _value_change_callback; ///< Value change callback

protected:
    /**
     * @brief Updates the visual appearance based on current state
     */
    void _update_visual_state();

    /**
     * @brief Calculates the bounds of the slider track
     *
     * @return Rectangle Bounds of the track
     */
    Rectangle _calculate_track_bounds() const;

    /**
     * @brief Calculates the bounds of the filled track portion
     *
     * @return Rectangle Bounds of the filled track
     */
    Rectangle _calculate_filled_track_bounds() const;

    /**
     * @brief Calculates the bounds of the slider thumb
     *
     * @return Rectangle Bounds of the thumb
     */
    Rectangle _calculate_thumb_bounds() const;

    /**
     * @brief Calculates the position for value text display
     *
     * @return Vector Position for value text
     */
    Vector _calculate_value_text_position() const;

    /**
     * @brief Gets the appropriate thumb color for the current state
     *
     * @return Color Thumb color for current state
     */
    Color _get_current_thumb_color() const;

    /**
     * @brief Converts a position to a value
     *
     * @param position Mouse or thumb position
     * @return double Corresponding value
     */
    double _position_to_value(const Vector& position) const;

    /**
     * @brief Converts a value to a position
     *
     * @param value Value to convert
     * @return Vector Corresponding position
     */
    Vector _value_to_position(double value) const;

    /**
     * @brief Clamps a value to the valid range
     *
     * @param value Value to clamp
     * @return double Clamped value
     */
    double _clamp_value(double value) const;

    /**
     * @brief Applies step size to a value
     *
     * @param value Value to step
     * @return double Stepped value
     */
    double _apply_step(double value) const;

    /**
     * @brief Formats the value for display
     *
     * @return std::string Formatted value string
     */
    std::string _format_value() const;

    /**
     * @brief Handles value changes
     *
     * @param new_value New value
     */
    void _on_value_changed(double new_value);

    /**
     * @brief Checks if a point is inside the thumb
     *
     * @param point Point to check
     * @return bool True if point is inside thumb
     */
    bool _is_point_in_thumb(const Vector& point) const;

    // Override Component event handlers
    void _on_interaction_state_changed(Component::InteractionState old_state, Component::InteractionState new_state) override;
    void _on_click_detected() override;

public:
    /**
     * @brief Constructs a new Slider with specified properties
     *
     * @param min_value Minimum value
     * @param max_value Maximum value
     * @param initial_value Initial value
     * @param orientation Slider orientation
     * @param track_color Color of the track
     * @param thumb_color Color of the thumb
     * @param filled_track_color Color of the filled track portion
     * @param rectangle Bounds of the slider
     * @param step_size Step size (0 = continuous)
     */
    Slider(double min_value, double max_value, double initial_value,
        Orientation orientation, const Color& track_color, const Color& thumb_color,
        const Color& filled_track_color, const Rectangle& rectangle,
        double step_size = 0.0);

    /**
     * @brief Sets the current value
     *
     * @param value New value
     */
    void set_value(double value);

    /**
     * @brief Gets the current value
     *
     * @return double Current value
     */
    double get_value() const;

    /**
     * @brief Sets the value range
     *
     * @param min_value Minimum value
     * @param max_value Maximum value
     */
    void set_range(double min_value, double max_value);

    /**
     * @brief Gets the minimum value
     *
     * @return double Minimum value
     */
    double get_min_value() const;

    /**
     * @brief Gets the maximum value
     *
     * @return double Maximum value
     */
    double get_max_value() const;

    /**
     * @brief Sets the step size
     *
     * @param step_size Step size (0 = continuous)
     */
    void set_step_size(double step_size);

    /**
     * @brief Gets the step size
     *
     * @return double Current step size
     */
    double get_step_size() const;

    /**
     * @brief Sets the orientation
     *
     * @param orientation New orientation
     */
    void set_orientation(Orientation orientation);

    /**
     * @brief Gets the orientation
     *
     * @return Orientation Current orientation
     */
    Orientation get_orientation() const;

    /**
     * @brief Sets the track thickness
     *
     * @param thickness Thickness in pixels
     */
    void set_track_thickness(int thickness);

    /**
     * @brief Gets the track thickness
     *
     * @return int Current track thickness
     */
    int get_track_thickness() const;

    /**
     * @brief Sets the thumb size
     *
     * @param size Size in pixels
     */
    void set_thumb_size(int size);

    /**
     * @brief Gets the thumb size
     *
     * @return int Current thumb size
     */
    int get_thumb_size() const;

    /**
     * @brief Sets the colors for different states
     *
     * @param track_color Track color
     * @param thumb_color Thumb color
     * @param filled_track_color Filled track color
     * @param hovered_thumb_color Hovered thumb color
     * @param pressed_thumb_color Pressed thumb color
     * @param disabled_track_color Disabled track color
     * @param disabled_thumb_color Disabled thumb color
     */
    void set_colors(const Color& track_color, const Color& thumb_color,
        const Color& filled_track_color, const Color& hovered_thumb_color,
        const Color& pressed_thumb_color, const Color& disabled_track_color,
        const Color& disabled_thumb_color);

    /**
     * @brief Sets whether to display the current value (without font)
     *
     * @param show_value True to show value
     */
    void set_value_display(bool show_value);

    /**
     * @brief Sets whether to display the current value (with custom font)
     *
     * @param show_value True to show value
     * @param font Font for value display
     * @param text_color Color for value text
     */
    void set_value_display(bool show_value, const Font& font,
        const Color& text_color = Color(0, 0, 0, 255));

    /**
     * @brief Gets whether value display is enabled
     *
     * @return bool True if value display is enabled
     */
    bool is_value_display_enabled() const;

    /**
     * @brief Sets the value change callback
     *
     * @param callback Function to call when value changes
     */
    void set_value_change_callback(ValueChangeCallback callback);

    /**
     * @brief Increments the value by one step
     */
    void increment();

    /**
     * @brief Decrements the value by one step
     */
    void decrement();

    /**
     * @brief Sets the value as a percentage (0.0 to 1.0)
     *
     * @param percentage Percentage value
     */
    void set_percentage(double percentage);

    /**
     * @brief Gets the value as a percentage (0.0 to 1.0)
     *
     * @return double Percentage value
     */
    double get_percentage() const;

    /**
     * @brief Sets the renderer for this slider
     *
     * @param renderer Shared pointer to the renderer instance
     */
    void set_renderer(std::shared_ptr<Renderer> renderer) override;

    /**
     * @brief Draws the slider
     */
    void draw() override;

    /**
     * @brief Handles mouse drag events
     *
     * @param mouse_pos Current mouse position
     * @return bool True if event was handled
     */
    bool handle_mouse_drag(const Vector& mouse_pos);

    /**
     * @brief Handles mouse press events
     *
     * @param mouse_pos Mouse position
     * @return bool True if event was handled
     */
    bool handle_mouse_press(const Vector& mouse_pos);

    /**
     * @brief Handles mouse release events
     *
     * @return bool True if event was handled
     */
    bool handle_mouse_release();
};

}
