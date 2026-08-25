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

#include "guillaume/components/text.hpp"

namespace guillaume::components
{
	const std::string &Text::getContent(void) const
	{
		return _content;
	}

	Text &Text::setContent(const std::string &content)
	{
		if (_content == content) {
			return *this;
		}
		_content = content;
		// Clamp cursor to valid range
		if (_cursorPosition > _content.size()) {
			_cursorPosition = _content.size();
		}
		setHasChanged(true);
		return *this;
	}

	float Text::getFontSize(void) const
	{
		return _fontSize;
	}

	Text &Text::setFontSize(float fontSize)
	{
		_fontSize = fontSize;
		setHasChanged(true);
		return *this;
	}

	std::size_t Text::getCursorPosition(void) const
	{
		return _cursorPosition;
	}

	Text &Text::setCursorPosition(std::size_t position)
	{
		_cursorPosition = std::min(position, _content.size());
		setHasChanged(true);
		return *this;
	}

	bool Text::moveCursorLeft(void)
	{
		if (_cursorPosition == 0) {
			return false;
		}

		// Move back one byte at a time until we find a UTF-8 sequence start
		do {
			--_cursorPosition;
		} while (_cursorPosition > 0
				 && (_content[_cursorPosition] & 0xC0) == 0x80);

		setHasChanged(true);
		return true;
	}

	bool Text::moveCursorRight(void)
	{
		if (_cursorPosition >= _content.size()) {
			return false;
		}

		// Move forward past the current UTF-8 character
		++_cursorPosition;
		while (_cursorPosition < _content.size()
			   && (_content[_cursorPosition] & 0xC0) == 0x80) {
			++_cursorPosition;
		}

		setHasChanged(true);
		return true;
	}

	Text &Text::insertCharacter(char ch)
	{
		_content.insert(_cursorPosition, 1, ch);
		++_cursorPosition;
		setHasChanged(true);
		return *this;
	}

	Text &Text::insertString(const std::string &str)
	{
		_content.insert(_cursorPosition, str);
		_cursorPosition += str.size();
		setHasChanged(true);
		return *this;
	}

	bool Text::deleteCharacterBefore(void)
	{
		if (_cursorPosition == 0 || _content.empty()) {
			return false;
		}

		// Find the start of the previous UTF-8 character
		std::size_t deleteStart = _cursorPosition - 1;
		while (deleteStart > 0 && (_content[deleteStart] & 0xC0) == 0x80) {
			--deleteStart;
		}

		const std::size_t deleteLength = _cursorPosition - deleteStart;
		_content.erase(deleteStart, deleteLength);
		_cursorPosition = deleteStart;
		setHasChanged(true);
		return true;
	}

	bool Text::deleteCharacterAfter(void)
	{
		if (_cursorPosition >= _content.size()) {
			return false;
		}

		// Find the end of the current UTF-8 character
		std::size_t deleteEnd = _cursorPosition + 1;
		while (deleteEnd < _content.size()
			   && (_content[deleteEnd] & 0xC0) == 0x80) {
			++deleteEnd;
		}

		const std::size_t deleteLength = deleteEnd - _cursorPosition;
		_content.erase(_cursorPosition, deleteLength);
		setHasChanged(true);
		return true;
	}
}	 // namespace guillaume::components
