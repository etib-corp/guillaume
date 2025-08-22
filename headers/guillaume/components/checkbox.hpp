/**
 * @file checkbox.hpp
 * @brief Checkbox component implementation for the Guillaume GUI framework
 *
 * This file contains the Checkbox class which provides a toggleable checkbox component
 * with customizable appearance and state change callbacks.
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
#include <string>

namespace guigui {

/**
 * @class Checkbox
 * @brief A toggleable checkbox component with optional label
 *
 * The Checkbox class extends Component to provide a checkbox that can be
 * toggled between checked and unchecked states.
 *
 * @details Features:
 * - Toggleable checked/unchecked state
 * - Optional text label
 * - Customizable colors for different states
 * - State change callbacks
 * - Visual feedback for interaction states
 * - Support for disabled state
 * - Configurable checkbox size and label position
 *
 * @note The Checkbox uses rectangle and text primitives for rendering.
 */
class Checkbox : public Component {
public:
    /**
     * @enum LabelPosition
     * @brief Defines the position of the label relative to the checkbox
     */
    enum class LabelPosition {
        LEFT, ///< Label appears to the left of the checkbox
        RIGHT, ///< Label appears to the right of the checkbox
        TOP, ///< Label appears above the checkbox
        BOTTOM ///< Label appears below the checkbox
    };

    /**
     * @brief Type definition for state change callback
     * @param checkbox Reference to the checkbox that changed
     * @param is_checked New checked state
     */
    using StateChangeCallback = std::function<void(Checkbox&, bool)>;

private:
    bool _is_checked; ///< Current checked state
    std::string _label; ///< Label text
    Color _box_color; ///< Color of the checkbox box
    Color _check_color; ///< Color of the check mark
    Color _border_color; ///< Color of the checkbox border
    Color _hovered_box_color; ///< Color when hovered
    Color _pressed_box_color; ///< Color when pressed
    Color _disabled_box_color; ///< Color when disabled
    Color _text_color; ///< Color of the label text
    Color _disabled_text_color; ///< Color of label text when disabled
    Font _font; ///< Font for the label
    LabelPosition _label_position; ///< Position of the label
    int _box_size; ///< Size of the checkbox box
    int _border_width; ///< Width of the border
    int _label_spacing; ///< Spacing between box and label
    StateChangeCallback _state_change_callback; ///< State change callback

protected:
    /**
     * @brief Updates the visual appearance based on current state
     */
    void _update_visual_state();

    /**
     * @brief Calculates the position and size of the checkbox box
     *
     * @return Rectangle Bounds of the checkbox box
     */
    Rectangle _calculate_box_bounds() const;

    /**
     * @brief Calculates the position of the label text
     *
     * @return Vector Position where label should be rendered
     */
    Vector _calculate_label_position() const;

    /**
     * @brief Calculates the position and size of the check mark
     *
     * @return Rectangle Bounds of the check mark
     */
    Rectangle _calculate_check_bounds() const;

    /**
     * @brief Gets the appropriate box color for the current state
     *
     * @return Color Box color for current state
     */
    Color _get_current_box_color() const;

    /**
     * @brief Gets the appropriate text color for the current state
     *
     * @return Color Text color for current state
     */
    Color _get_current_text_color() const;

    /**
     * @brief Handles state changes
     *
     * @param new_checked New checked state
     */
    void _on_state_changed(bool new_checked);

    /**
     * @brief Creates check mark primitives
     */
    void _create_check_mark();

    // Override Component event handlers
    void _on_interaction_state_changed(Component::InteractionState old_state, Component::InteractionState new_state) override;
    void _on_click_detected() override;

public:
    /**
     * @brief Constructs a new Checkbox with specified properties
     *
     * @param label Label text (can be empty)
     * @param is_checked Initial checked state
     * @param box_color Color of the checkbox box
     * @param check_color Color of the check mark
     * @param text_color Color of the label text
     * @param font Font for the label
     * @param rectangle Bounds of the entire checkbox component
     * @param label_position Position of the label (default: RIGHT)
     */
    Checkbox(const std::string& label, bool is_checked, const Color& box_color,
        const Color& check_color, const Color& text_color, const Font& font,
        const Rectangle& rectangle, LabelPosition label_position = LabelPosition::RIGHT);

    /**
     * @brief Sets the checked state
     *
     * @param checked New checked state
     */
    void set_checked(bool checked);

    /**
     * @brief Gets the checked state
     *
     * @return bool Current checked state
     */
    bool is_checked() const;

    /**
     * @brief Toggles the checked state
     */
    void toggle();

    /**
     * @brief Sets the label text
     *
     * @param label New label text
     */
    void set_label(const std::string& label);

    /**
     * @brief Gets the label text
     *
     * @return const std::string& Current label text
     */
    const std::string& get_label() const;

    /**
     * @brief Sets the label position
     *
     * @param position New label position
     */
    void set_label_position(LabelPosition position);

    /**
     * @brief Gets the label position
     *
     * @return LabelPosition Current label position
     */
    LabelPosition get_label_position() const;

    /**
     * @brief Sets the checkbox box size
     *
     * @param size New box size in pixels
     */
    void set_box_size(int size);

    /**
     * @brief Gets the checkbox box size
     *
     * @return int Current box size
     */
    int get_box_size() const;

    /**
     * @brief Sets the colors for different states
     *
     * @param box_color Normal box color
     * @param check_color Check mark color
     * @param border_color Border color
     * @param hovered_box_color Hovered box color
     * @param pressed_box_color Pressed box color
     * @param disabled_box_color Disabled box color
     */
    void set_box_colors(const Color& box_color, const Color& check_color,
        const Color& border_color, const Color& hovered_box_color,
        const Color& pressed_box_color, const Color& disabled_box_color);

    /**
     * @brief Sets the text colors
     *
     * @param text_color Normal text color
     * @param disabled_text_color Disabled text color
     */
    void set_text_colors(const Color& text_color, const Color& disabled_text_color);

    /**
     * @brief Sets the state change callback
     *
     * @param callback Function to call when state changes
     */
    void set_state_change_callback(StateChangeCallback callback);

    /**
     * @brief Sets the font for the label
     *
     * @param font New font
     */
    void set_font(const Font& font);

    /**
     * @brief Gets the font for the label
     *
     * @return const Font& Current font
     */
    const Font& get_font() const;

    /**
     * @brief Sets the spacing between box and label
     *
     * @param spacing Spacing in pixels
     */
    void set_label_spacing(int spacing);

    /**
     * @brief Gets the spacing between box and label
     *
     * @return int Current spacing
     */
    int get_label_spacing() const;

    /**
     * @brief Sets the renderer for this checkbox
     *
     * @param renderer Shared pointer to the renderer instance
     */
    void set_renderer(std::shared_ptr<Renderer> renderer) override;

    /**
     * @brief Draws the checkbox
     */
    void draw() override;
};

}
