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

#include "guillaume/components/typography.hpp"

namespace guillaume::components
{
	theme::TypeRole Typography::getRole(void) const
	{
		return _role;
	}

	Typography &Typography::setRole(theme::TypeRole role)
	{
		if (_role == role) {
			return *this;
		}
		_role = role;
		setHasChanged(true);
		return *this;
	}

	std::optional<float> Typography::getFontSize(void) const
	{
		return _fontSize;
	}

	Typography &Typography::setFontSize(std::optional<float> fontSize)
	{
		if (_fontSize == fontSize) {
			return *this;
		}
		_fontSize = fontSize;
		setHasChanged(true);
		return *this;
	}
}	 // namespace guillaume::components
