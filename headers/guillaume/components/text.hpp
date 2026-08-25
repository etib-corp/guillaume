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

namespace guillaume::components
{

	/**
	 * @brief Component representing a text element.
	 * @see systems::KeyboardControl
	 * @see systems::TextInput
	 * @see guillaume::Text
	 */
	class Text: public ecs::Component
	{
		private:
		std::string _content {};	///< Text content
		float _fontSize { 24 };		///< Font size of the text
		std::size_t _cursorPosition {
			0
		};	  ///< Cursor position for text editing

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
		 * @brief Get the text content.
		 * @return The text content.
		 */
		const std::string &getContent(void) const;

		/**
		 * @brief Set the text content.
		 * @param content The new text content.
		 * @return The Text component for chaining.
		 */
		Text &setContent(const std::string &content);

		/**
		 * @brief Get the font size of the text.
		 * @return The font size.
		 */
		float getFontSize(void) const;

		/**
		 * @brief Set the font size of the text.
		 * @param fontSize The new font size.
		 * @return The Text component for chaining.
		 */
		Text &setFontSize(float fontSize);

		/**
		 * @brief Get the cursor position.
		 * @return The cursor position in the text.
		 */
		std::size_t getCursorPosition(void) const;

		/**
		 * @brief Set the cursor position.
		 * @param position The cursor position to set.
		 * @return The Text component for chaining.
		 */
		Text &setCursorPosition(std::size_t position);

		/**
		 * @brief Move the cursor left by one character.
		 * @return True if the cursor was moved, false if already at start.
		 */
		bool moveCursorLeft(void);

		/**
		 * @brief Move the cursor right by one character.
		 * @return True if the cursor was moved, false if already at end.
		 */
		bool moveCursorRight(void);

		/**
		 * @brief Insert a character at the cursor position.
		 * @param ch The character to insert.
		 * @return The Text component for chaining.
		 */
		Text &insertCharacter(char ch);

		/**
		 * @brief Insert a UTF-8 string at the cursor position.
		 * @param str The string to insert.
		 * @return The Text component for chaining.
		 */
		Text &insertString(const std::string &str);

		/**
		 * @brief Delete the character before the cursor (backspace).
		 * @return True if a character was deleted, false otherwise.
		 */
		bool deleteCharacterBefore(void);

		/**
		 * @brief Delete the character at the cursor (delete key).
		 * @return True if a character was deleted, false otherwise.
		 */
		bool deleteCharacterAfter(void);
	};

}	 // namespace guillaume::components
