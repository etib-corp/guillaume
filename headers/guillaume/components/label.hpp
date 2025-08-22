/**
 * @file label.hpp
 * @brief Label component implementation for the Guillaume GUI framework
 *
 * This file contains the Label class which provides a simple text display component
 * with customizable appearance and text alignment options.
 */

#pragma once

#include "guillaume/component.hpp"

#include "guillaume/color.hpp"
#include "guillaume/font.hpp"
#include "guillaume/logger.hpp"
#include "guillaume/rectangle.hpp"
#include "guillaume/vector.hpp"

#include "guillaume/primitives/text_primitive.hpp"

namespace guigui {

/**
 * @class Label
 * @brief A text display component for showing static or dynamic text
 *
 * The Label class extends Component to provide a simple text display
 * with customizable font, color, and alignment options.
 *
 * @details Features:
 * - Customizable text color and font
 * - Text alignment options (left, center, right)
 * - Word wrapping support
 * - Auto-sizing to fit content
 * - Transparent background by default
 *
 * @note The Label uses text primitives for rendering.
 */
class Label : public Component {
public:
    /**
     * @enum TextAlignment
     * @brief Defines the text alignment options
     */
    enum class TextAlignment {
        LEFT, ///< Align text to the left
        CENTER, ///< Center the text
        RIGHT ///< Align text to the right
    };

private:
    std::string _text; ///< Text content to display
    Color _text_color; ///< Color of the text
    Font _font; ///< Font used for the text
    TextAlignment _alignment; ///< Text alignment
    bool _word_wrap; ///< Whether to wrap text to fit bounds
    bool _auto_size; ///< Whether to auto-size to fit content

protected:
    /**
     * @brief Updates the visual appearance of the label
     *
     * This method updates the label's text primitive to reflect current
     * text content, color, and alignment settings.
     */
    void _update_visual_state();

    /**
     * @brief Calculates the text position based on alignment
     *
     * @return Vector Position where text should be rendered
     */
    Vector _calculate_text_position() const;

    /**
     * @brief Calculates the required size for the text content
     *
     * @return Rectangle Required bounds to contain the text
     */
    Rectangle _calculate_text_bounds() const;

public:
    /**
     * @brief Constructs a new Label with specified properties
     *
     * @param text Text content to display
     * @param text_color Color of the text
     * @param font Font to use for the text
     * @param rectangle Bounds of the label
     * @param alignment Text alignment (default: LEFT)
     * @param word_wrap Whether to enable word wrapping (default: false)
     * @param auto_size Whether to auto-size to fit content (default: false)
     */
    Label(const std::string& text, const Color& text_color, const Font& font,
        const Rectangle& rectangle, TextAlignment alignment = TextAlignment::LEFT,
        bool word_wrap = false, bool auto_size = false);

    /**
     * @brief Sets the text content
     *
     * @param text New text to display
     */
    void set_text(const std::string& text);

    /**
     * @brief Gets the current text content
     *
     * @return const std::string& Current text content
     */
    const std::string& get_text() const;

    /**
     * @brief Sets the text color
     *
     * @param color New text color
     */
    void set_text_color(const Color& color);

    /**
     * @brief Gets the current text color
     *
     * @return const Color& Current text color
     */
    const Color& get_text_color() const;

    /**
     * @brief Sets the font
     *
     * @param font New font for the text
     */
    void set_font(const Font& font);

    /**
     * @brief Gets the current font
     *
     * @return const Font& Current font
     */
    const Font& get_font() const;

    /**
     * @brief Sets the text alignment
     *
     * @param alignment New text alignment
     */
    void set_alignment(TextAlignment alignment);

    /**
     * @brief Gets the current text alignment
     *
     * @return TextAlignment Current text alignment
     */
    TextAlignment get_alignment() const;

    /**
     * @brief Sets word wrapping
     *
     * @param word_wrap Whether to enable word wrapping
     */
    void set_word_wrap(bool word_wrap);

    /**
     * @brief Gets word wrapping setting
     *
     * @return bool Current word wrapping setting
     */
    bool get_word_wrap() const;

    /**
     * @brief Sets auto-sizing
     *
     * @param auto_size Whether to enable auto-sizing
     */
    void set_auto_size(bool auto_size);

    /**
     * @brief Gets auto-sizing setting
     *
     * @return bool Current auto-sizing setting
     */
    bool get_auto_size() const;

    /**
     * @brief Sets the renderer for this label
     *
     * @param renderer Shared pointer to the renderer instance
     *
     * Overrides the base Component method to initialize the label's
     * visual primitives.
     */
    void set_renderer(std::shared_ptr<Renderer> renderer) override;

    /**
     * @brief Draws the label
     *
     * Executes the label's rendering primitives to draw the text.
     */
    void draw() override;
};

}
