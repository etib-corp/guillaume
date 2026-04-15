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
		setHasChanged(true);
		return *this;
	}

	std::size_t Text::getFontSize(void) const
	{
		return _fontSize;
	}

	Text &Text::setFontSize(std::size_t fontSize)
	{
		const std::size_t resolvedFontSize = (fontSize == 0) ? 24 : fontSize;
		if (_fontSize == resolvedFontSize) {
			return *this;
		}
		_fontSize = resolvedFontSize;
		setHasChanged(true);
		return *this;
	}
}	 // namespace guillaume::components