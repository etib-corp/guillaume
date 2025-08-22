/**
 * @file textbox.hpp
 * @brief TextBox component implementation for the Guillaume GUI framework
 *
 * This file contains the TextBox class which provides an input field for text entry
 * with customizable appearance and input validation.
 */

#pragma once

#include "guillaume/component.hpp"

#include "guillaume/color.hpp"
#include "guillaume/events/keyboard_event.hpp"
#include "guillaume/events/mouse_event.hpp"
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
 * @class TextBox
 * @brief An input field component for text entry
 *
 * The TextBox class extends Component to provide a text input field
 * with cursor support, text selection, and input validation.
 *
 * @details Features:
 * - Text input with cursor positioning
 * - Text selection support
 * - Input validation callbacks
 * - Placeholder text
 * - Customizable border and background colors
 * - Focus state management
 * - Character limits
 *
 * @note The TextBox uses rectangle and text primitives for rendering.
 */
class TextBox : public Component {
public:
    /**
     * @brief Type definition for input validation callback
     * @param input The current input text
     * @return bool True if input is valid, false otherwise
     */
    using ValidationCallback = std::function<bool(const std::string&)>;

    /**
     * @brief Type definition for text change callback
     * @param textbox Reference to the textbox that changed
     * @param old_text Previous text content
     * @param new_text New text content
     */
    using TextChangeCallback = std::function<void(TextBox&, const std::string&, const std::string&)>;

private:
    std::string _text; ///< Current text content
    std::string _placeholder; ///< Placeholder text when empty
    Color _background_color; ///< Background color
    Color _border_color; ///< Border color
    Color _focused_border_color; ///< Border color when focused
    Color _text_color; ///< Text color
    Color _placeholder_color; ///< Placeholder text color
    Color _cursor_color; ///< Cursor color
    Font _font; ///< Font used for text
    bool _is_focused; ///< Whether the textbox has focus
    bool _is_password; ///< Whether to mask input (password field)
    char _password_char; ///< Character to use for password masking
    size_t _cursor_position; ///< Current cursor position
    size_t _selection_start; ///< Selection start position
    size_t _selection_end; ///< Selection end position
    size_t _max_length; ///< Maximum text length (0 = unlimited)
    int _border_width; ///< Width of the border
    ValidationCallback _validation_callback; ///< Input validation callback
    TextChangeCallback _text_change_callback; ///< Text change callback

protected:
    /**
     * @brief Updates the visual appearance based on current state
     */
    void _update_visual_state();

    /**
     * @brief Gets the display text (either actual text or masked for passwords)
     *
     * @return std::string Text to display
     */
    std::string _get_display_text() const;

    /**
     * @brief Calculates the text position within the textbox
     *
     * @return Vector Position where text should be rendered
     */
    Vector _calculate_text_position() const;

    /**
     * @brief Calculates the cursor position for rendering
     *
     * @return Vector Position where cursor should be rendered
     */
    Vector _calculate_cursor_position() const;

    /**
     * @brief Handles character input
     *
     * @param character Character to insert
     */
    void _insert_character(char character);

    /**
     * @brief Handles backspace key
     */
    void _handle_backspace();

    /**
     * @brief Handles delete key
     */
    void _handle_delete();

    /**
     * @brief Moves cursor left
     *
     * @param select Whether to extend selection
     */
    void _move_cursor_left(bool select = false);

    /**
     * @brief Moves cursor right
     *
     * @param select Whether to extend selection
     */
    void _move_cursor_right(bool select = false);

    /**
     * @brief Moves cursor to beginning
     *
     * @param select Whether to extend selection
     */
    void _move_cursor_home(bool select = false);

    /**
     * @brief Moves cursor to end
     *
     * @param select Whether to extend selection
     */
    void _move_cursor_end(bool select = false);

    /**
     * @brief Calculates cursor position from mouse coordinates
     *
     * @param mouse_pos Mouse position
     * @return size_t Cursor position
     */
    size_t _get_cursor_position_from_mouse(const Vector& mouse_pos) const;

    /**
     * @brief Validates input text
     *
     * @param text Text to validate
     * @return bool True if valid
     */
    bool _validate_input(const std::string& text) const;

    /**
     * @brief Handles focus changes
     *
     * @param focused New focus state
     */
    void _on_focus_changed(bool focused);

    /**
     * @brief Handles text changes
     *
     * @param old_text Previous text
     * @param new_text New text
     */
    void _on_text_changed(const std::string& old_text, const std::string& new_text);

    // Override Component event handlers
    void _on_interaction_state_changed(Component::InteractionState old_state, Component::InteractionState new_state) override;
    void _on_click_detected() override;

public:
    /**
     * @brief Constructs a new TextBox with specified properties
     *
     * @param placeholder Placeholder text
     * @param background_color Background color
     * @param border_color Border color
     * @param text_color Text color
     * @param font Font to use for text
     * @param rectangle Bounds of the textbox
     */
    TextBox(const std::string& placeholder, const Color& background_color,
        const Color& border_color, const Color& text_color, const Font& font,
        const Rectangle& rectangle);

    /**
     * @brief Sets the text content
     *
     * @param text New text content
     */
    void set_text(const std::string& text);

    /**
     * @brief Gets the current text content
     *
     * @return const std::string& Current text
     */
    const std::string& get_text() const;

    /**
     * @brief Sets the placeholder text
     *
     * @param placeholder New placeholder text
     */
    void set_placeholder(const std::string& placeholder);

    /**
     * @brief Gets the placeholder text
     *
     * @return const std::string& Current placeholder
     */
    const std::string& get_placeholder() const;

    /**
     * @brief Sets whether this is a password field
     *
     * @param is_password True for password field
     * @param password_char Character to use for masking (default: '*')
     */
    void set_password(bool is_password, char password_char = '*');

    /**
     * @brief Gets whether this is a password field
     *
     * @return bool True if password field
     */
    bool is_password() const;

    /**
     * @brief Sets the maximum text length
     *
     * @param max_length Maximum length (0 = unlimited)
     */
    void set_max_length(size_t max_length);

    /**
     * @brief Gets the maximum text length
     *
     * @return size_t Maximum length
     */
    size_t get_max_length() const;

    /**
     * @brief Sets the cursor position
     *
     * @param position New cursor position
     */
    void set_cursor_position(size_t position);

    /**
     * @brief Gets the cursor position
     *
     * @return size_t Current cursor position
     */
    size_t get_cursor_position() const;

    /**
     * @brief Sets focus state
     *
     * @param focused New focus state
     */
    void set_focused(bool focused);

    /**
     * @brief Gets focus state
     *
     * @return bool Current focus state
     */
    bool is_focused() const;

    /**
     * @brief Sets the input validation callback
     *
     * @param callback Validation function
     */
    void set_validation_callback(ValidationCallback callback);

    /**
     * @brief Sets the text change callback
     *
     * @param callback Text change function
     */
    void set_text_change_callback(TextChangeCallback callback);

    /**
     * @brief Sets the colors for different states
     *
     * @param background Background color
     * @param border Border color
     * @param focused_border Border color when focused
     * @param text Text color
     * @param placeholder Placeholder text color
     * @param cursor Cursor color
     */
    void set_colors(const Color& background, const Color& border,
        const Color& focused_border, const Color& text,
        const Color& placeholder, const Color& cursor);

    /**
     * @brief Selects all text
     */
    void select_all();

    /**
     * @brief Clears the text
     */
    void clear();

    /**
     * @brief Gets selected text
     *
     * @return std::string Selected text
     */
    std::string get_selected_text() const;

    /**
     * @brief Sets the renderer for this textbox
     *
     * @param renderer Shared pointer to the renderer instance
     */
    void set_renderer(std::shared_ptr<Renderer> renderer) override;

    /**
     * @brief Draws the textbox
     */
    void draw() override;

    /**
     * @brief Handles keyboard input events
     *
     * @param event Keyboard event
     * @return bool True if event was handled
     */
    bool handle_keyboard_event(const KeyboardEvent& event);
};

}
