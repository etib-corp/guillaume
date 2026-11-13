/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#pragma once

#include <string>

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/component.hpp"
#include "guillaume/theme/typescale.hpp"

namespace guillaume::components
{

	/**
	 * @brief A run of text: its content, its font metrics and the position
	 * of the editing cursor.
	 *
	 * Readers: systems::MeasureText (Measure phase) writes the size of the
	 * text into the entity's components::Bound, and systems::TextRender
	 * (Render phase) draws it at the entity's components::Transform in the
	 * entity's components::Color.
	 *
	 * Writers: entities::Text and the widget systems set the content;
	 * systems::Typography writes the four font metrics every frame on an
	 * entity that also has a components::Typography; systems::TextInput
	 * appends typed text to the content and systems::KeyboardControl moves
	 * the cursor and deletes around it.
	 *
	 * @code
	 * auto &text = registry.getComponent<components::Text>(id);
	 * text.setContent("Hello").setTypeToken(
	 * 	getTypeToken(theme::TypeRole::TitleMedium));
	 * @endcode
	 *
	 * @note The content is UTF-8 and the cursor position is a byte offset
	 * into it, not a character count.
	 * @note On an entity with a components::Typography, set the role or
	 * the size there: the metrics written here are replaced on the next
	 * frame.
	 * @note No system of the library reads the letter spacing.
	 *
	 * @see systems::MeasureText, systems::TextRender, entities::Text
	 */
	class Text: public ecs::Component
	{
		private:
		std::string _content {};	///< Content, UTF-8
		float _fontSize { 24 };		///< Font size, in dp
		theme::FontWeight _fontWeight {
			theme::FontWeight::Regular
		};	  ///< Font weight; selects the font file

		float _lineHeight { 0.0f };		  ///< Minimum height, in dp (0: none)
		float _letterSpacing { 0.0f };	  ///< Letter spacing, in dp
		std::size_t _cursorPosition {
			0
		};	  ///< Cursor, as a byte offset in the content

		public:
		/**
		 * @brief Default constructor for the Text component.
		 */
		Text(void) = default;

		/**
		 * @brief Default destructor for the Text component.
		 */
		~Text(void) = default;

		/**
		 * @brief Get the text.
		 * @return The content, UTF-8; empty by default.
		 */
		const std::string &getContent(void) const;

		/**
		 * @brief Replace the text.
		 *
		 * A cursor beyond the end of the new content is moved to its end.
		 * Marks the component changed only when the content differs.
		 *
		 * @param content The new content, UTF-8.
		 * @return This component, for chaining.
		 */
		Text &setContent(const std::string &content);

		/**
		 * @brief Get the font size.
		 * @return The font size, in dp (24 by default).
		 */
		float getFontSize(void) const;

		/**
		 * @brief Set the font size.
		 *
		 * Always marks the component changed, even when the size is the
		 * same.
		 *
		 * @param fontSize The font size, in dp.
		 * @return This component, for chaining.
		 */
		Text &setFontSize(float fontSize);

		/**
		 * @brief Get the font weight.
		 * @return The weight (Regular by default).
		 */
		theme::FontWeight getFontWeight(void) const;

		/**
		 * @brief Set the font weight.
		 *
		 * The weight selects the font file the text is measured and drawn
		 * with (see theme::fontPath). Marks the component changed only
		 * when the weight differs.
		 *
		 * @param fontWeight The weight of the font.
		 * @return This component, for chaining.
		 */
		Text &setFontWeight(theme::FontWeight fontWeight);

		/**
		 * @brief Get the minimum height of the text's bound.
		 * @return The line height, in dp; 0 (the default) when the
		 * measured height is used as is.
		 */
		float getLineHeight(void) const;

		/**
		 * @brief Set the minimum height of the text's bound.
		 *
		 * systems::MeasureText writes the larger of the measured height
		 * and this value into the Bound, so short text still takes a full
		 * line. Marks the component changed only when the value differs.
		 *
		 * @param lineHeight The line height, in dp; must not be negative.
		 * @return This component, for chaining.
		 * @throws std::invalid_argument If @p lineHeight is negative.
		 */
		Text &setLineHeight(float lineHeight);

		/**
		 * @brief Get the extra space between letters.
		 * @return The letter spacing, in dp (0 by default).
		 */
		float getLetterSpacing(void) const;

		/**
		 * @brief Set the extra space between letters.
		 *
		 * The value is stored; the measure and render systems do not apply
		 * it. Marks the component changed only when the value differs.
		 *
		 * @param letterSpacing The letter spacing, in dp.
		 * @return This component, for chaining.
		 */
		Text &setLetterSpacing(float letterSpacing);

		/**
		 * @brief Apply the four metrics of a typescale token at once: font
		 * size, weight, line height and letter spacing.
		 *
		 * Marks the component changed only when at least one metric
		 * differs, so systems::Typography can call it every frame.
		 *
		 * @param token The token, usually from `getTypeToken(role)`.
		 * @return This component, for chaining.
		 * @throws std::invalid_argument If the token's line height is
		 * negative.
		 */
		Text &setTypeToken(const theme::TypeToken &token);

		/**
		 * @brief Get the position of the editing cursor.
		 * @return A byte offset into the content, from 0 (before the first
		 * character) to the content size (after the last one).
		 */
		std::size_t getCursorPosition(void) const;

		/**
		 * @brief Move the editing cursor.
		 *
		 * Always marks the component changed.
		 *
		 * @param position A byte offset into the content; a value beyond
		 * the end is clamped to the content size. It is not checked
		 * against UTF-8 character boundaries.
		 * @return This component, for chaining.
		 */
		Text &setCursorPosition(std::size_t position);

		/**
		 * @brief Move the cursor to the start of the previous UTF-8
		 * character. Marks the component changed when it moves.
		 * @return True when the cursor moved, false when it was already
		 * at the start.
		 */
		bool moveCursorLeft(void);

		/**
		 * @brief Move the cursor past the next UTF-8 character. Marks the
		 * component changed when it moves.
		 * @return True when the cursor moved, false when it was already
		 * at the end.
		 */
		bool moveCursorRight(void);

		/**
		 * @brief Insert one byte at the cursor and move the cursor after
		 * it. Marks the component changed.
		 * @param ch The byte to insert; use insertString() for a
		 * character that takes several bytes in UTF-8.
		 * @return This component, for chaining.
		 */
		Text &insertCharacter(char ch);

		/**
		 * @brief Insert a string at the cursor and move the cursor after
		 * it. Always marks the component changed, even for an empty
		 * string.
		 * @param str The UTF-8 text to insert.
		 * @return This component, for chaining.
		 */
		Text &insertString(const std::string &str);

		/**
		 * @brief Delete the UTF-8 character before the cursor (backspace)
		 * and move the cursor back to where it started. Marks the
		 * component changed when a character is deleted.
		 * @return True when a character was deleted, false when the cursor
		 * is at the start.
		 */
		bool deleteCharacterBefore(void);

		/**
		 * @brief Delete the UTF-8 character after the cursor (delete key);
		 * the cursor does not move. Marks the component changed when a
		 * character is deleted.
		 * @return True when a character was deleted, false when the cursor
		 * is at the end.
		 */
		bool deleteCharacterAfter(void);
	};

}	 // namespace guillaume::components
