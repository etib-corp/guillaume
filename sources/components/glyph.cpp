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

#include "guillaume/components/glyph.hpp"

namespace guillaume::components
{
	uint32_t Glyph::getCode(void) const
	{
		return _code;
	}

	Glyph &Glyph::setCode(uint32_t code)
	{
		if (_code == code) {
			return *this;
		}
		_code = code;
		setHasChanged(true);
		return *this;
	}

	const std::string &Glyph::getName(void) const
	{
		return _name;
	}

	Glyph &Glyph::setName(const std::string &name)
	{
		if (_name == name) {
			return *this;
		}
		_name = name;
		setHasChanged(true);
		return *this;
	}

	const float &Glyph::getFontSize(void) const
	{
		return _fontSize;
	}

	Glyph &Glyph::setFontSize(const float &fontSize)
	{
		if (_fontSize == fontSize) {
			return *this;
		}
		_fontSize = fontSize;
		setHasChanged(true);
		return *this;
	}

	const Glyph::Style &Glyph::getStyle(void) const
	{
		return _style;
	}

	Glyph &Glyph::setStyle(const Glyph::Style &style)
	{
		if (_style == style) {
			return *this;
		}
		_style = style;
		setHasChanged(true);
		return *this;
	}

}	 // namespace guillaume::components
