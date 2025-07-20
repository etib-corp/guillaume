/**
 * @file button.hpp
 * @brief Button component implementation for the Guillaume GUI framework
 * 
 * This file contains the Button class which provides a clickable button component
 * with customizable appearance and interaction states.
 */

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

/**
 * @class Button
 * @brief A clickable button component with text label and customizable appearance
 * 
 * The Button class extends Component to provide a standard clickable button
 * with a text label. It supports different visual states (normal, hovered, pressed)
 * with customizable colors for each state.
 * 
 * @details Features:
 * - Customizable background and text colors for each interaction state
 * - Text label with configurable font
 * - Click callback functionality
 * - Automatic visual feedback based on interaction state
 * - Built on the primitive rendering system
 * 
 * @note The Button uses both rectangle and text primitives for rendering.
 */
class Button : public Component {
public:
    /**
     * @enum ButtonState
     * @brief Defines the visual states of a button
     * @deprecated Use Component::InteractionState instead
     */
    enum class ButtonState {
        NORMAL,   ///< Default state
        HOVERED,  ///< Mouse is hovering over the button
        PRESSED   ///< Button is being pressed
    };

private:
    std::string _label;                             ///< Text label displayed on the button
    Color _normal_background_color;                 ///< Background color in normal state
    Color _hovered_background_color;                ///< Background color when hovered
    Color _pressed_background_color;                ///< Background color when pressed
    Color _normal_text_color;                       ///< Text color in normal state
    Color _hovered_text_color;                      ///< Text color when hovered
    Color _pressed_text_color;                      ///< Text color when pressed
    Font _font;                                     ///< Font used for the button text
    std::function<void(Button&)> _on_button_click;  ///< Button-specific click callback

protected:
    /**
     * @brief Updates the visual appearance based on current interaction state
     * 
     * This method updates the button's primitives (background rectangle and text)
     * to reflect the current interaction state and colors.
     */
    void _update_visual_state();
    
    /**
     * @brief Gets the appropriate background color for the current state
     * 
     * @return Color The background color that should be used for rendering
     */
    Color _get_current_background_color() const;
    
    /**
     * @brief Gets the appropriate text color for the current state
     * 
     * @return Color The text color that should be used for rendering
     */
    Color _get_current_text_color() const;
    
    /**
     * @brief Creates a hover state background color from the normal color
     * 
     * @param normal_color Base color to derive the hover color from
     * @return Color Lighter/brighter version for hover state
     */
    Color _create_hover_color(const Color& normal_color) const;
    
    /**
     * @brief Creates a pressed state background color from the normal color
     * 
     * @param normal_color Base color to derive the pressed color from
     * @return Color Darker version for pressed state
     */
    Color _create_pressed_color(const Color& normal_color) const;
    
    /**
     * @brief Creates a hover state text color that contrasts well with hover background
     * 
     * @param normal_text_color Base text color
     * @param normal_bg_color Base background color
     * @return Color Text color optimized for hover state
     */
    Color _create_hover_text_color(const Color& normal_text_color, const Color& normal_bg_color) const;
    
    /**
     * @brief Creates a pressed state text color that contrasts well with pressed background
     * 
     * @param normal_text_color Base text color
     * @param normal_bg_color Base background color
     * @return Color Text color optimized for pressed state
     */
    Color _create_pressed_text_color(const Color& normal_text_color, const Color& normal_bg_color) const;
    
    /**
     * @brief Calculates the centered position for text within the button bounds
     * 
     * @return Vector Centered position where text should be rendered
     * 
     * This method uses the renderer to get the text size and calculates
     * the position needed to center the text within the button's bounds.
     */
    Vector _calculate_centered_text_position() const;
    
    /**
     * @brief Handles interaction state changes
     * 
     * @param old_state Previous interaction state
     * @param new_state New interaction state
     * 
     * Overrides the base Component method to update visual state when
     * interaction state changes.
     */
    void _on_interaction_state_changed(Component::InteractionState old_state, Component::InteractionState new_state) override;
    
    /**
     * @brief Handles click detection
     * 
     * Overrides the base Component method to trigger button-specific
     * click callback in addition to the base component click handling.
     */
    void _on_click_detected() override;

public:
    /**
     * @brief Constructs a new Button with specified properties
     * 
     * @param label Text to display on the button
     * @param normal_background_color Background color in normal state
     * @param normal_text_color Text color in normal state
     * @param font Font to use for the button text
     * @param rectangle Bounds of the button
     * @param on_click Optional click callback function
     * 
     * @details Creates a button with the specified appearance and behavior.
     * The button will use the normal colors for both hovered and pressed states
     * unless explicitly set using the color setter methods.
     */
    Button(const std::string& label, const Color& normal_background_color, const Color& normal_text_color,
        const Font& font, const Rectangle& rectangle, std::function<void(Button&)> on_click = nullptr);

    /**
     * @brief Gets the current button state
     * 
     * @return ButtonState The current visual state of the button
     * @deprecated Use get_interaction_state() from the base Component class instead
     */
    ButtonState get_button_state() const;
    
    /**
     * @brief Sets the colors for the normal state
     * 
     * @param background_color Background color for normal state
     * @param text_color Text color for normal state
     */
    void set_normal_colors(const Color& background_color, const Color& text_color);
    
    /**
     * @brief Sets the colors for the hovered state
     * 
     * @param background_color Background color when mouse is hovering
     * @param text_color Text color when mouse is hovering
     */
    void set_hovered_colors(const Color& background_color, const Color& text_color);
    
    /**
     * @brief Sets the colors for the pressed state
     * 
     * @param background_color Background color when button is pressed
     * @param text_color Text color when button is pressed
     */
    void set_pressed_colors(const Color& background_color, const Color& text_color);
    
    /**
     * @brief Sets the button-specific click callback
     * 
     * @param callback Function to call when the button is clicked
     * 
     * @details This callback is called in addition to the base component's
     * click callback. It receives a reference to the Button object.
     */
    void set_button_click_callback(std::function<void(Button&)> callback);

    /**
     * @brief Sets the renderer for this button
     * 
     * @param renderer Shared pointer to the renderer instance
     * 
     * Overrides the base Component method to also initialize the button's
     * visual primitives.
     */
    void set_renderer(std::shared_ptr<Renderer> renderer) override;
    
    /**
     * @brief Draws the button
     * 
     * Executes the button's rendering primitives to draw the background
     * rectangle and text label.
     */
    void draw() override;
    
    /**
     * @brief Sets the background color
     * 
     * @param color New background color
     * @deprecated Use set_normal_colors() instead. This method only sets the normal state color.
     */
    void set_background_color(const Color& color);
    
    /**
     * @brief Sets the button's text label
     * 
     * @param label New text to display on the button
     * 
     * Updates the button's text and refreshes the visual state.
     */
    void set_label(const std::string& label);
};

}
