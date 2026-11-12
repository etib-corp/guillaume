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

#include "guillaume/components/common_button.hpp"

#include <utility>

namespace guillaume::components
{
	CommonButton::Color CommonButton::getColorStyle(void) const noexcept
	{
		return _colorStyle;
	}

	CommonButton &CommonButton::setColorStyle(Color style)
	{
		if (_colorStyle != style) {
			_colorStyle = style;
			setHasChanged(true);
		}
		return *this;
	}

	CommonButton::Shape CommonButton::getShape(void) const noexcept
	{
		return _shape;
	}

	CommonButton &CommonButton::setShape(Shape shape)
	{
		if (_shape != shape) {
			_shape = shape;
			setHasChanged(true);
		}
		return *this;
	}

	CommonButton::Size CommonButton::getSize(void) const noexcept
	{
		return _size;
	}

	CommonButton &CommonButton::setSize(Size size)
	{
		if (_size != size) {
			_size = size;
			setHasChanged(true);
		}
		return *this;
	}

	bool CommonButton::isCompact(void) const noexcept
	{
		return _compact;
	}

	CommonButton &CommonButton::setCompact(bool compact)
	{
		if (_compact != compact) {
			_compact = compact;
			setHasChanged(true);
		}
		return *this;
	}

	float CommonButton::getMinWidth(void) const noexcept
	{
		return _minWidth;
	}

	CommonButton &CommonButton::setMinWidth(float width)
	{
		if (_minWidth != width) {
			_minWidth = width;
			setHasChanged(true);
		}
		return *this;
	}

	bool CommonButton::isUppercase(void) const noexcept
	{
		return _uppercase;
	}

	CommonButton &CommonButton::setUppercase(bool uppercase)
	{
		if (_uppercase != uppercase) {
			_uppercase = uppercase;
			setHasChanged(true);
		}
		return *this;
	}
}	 // namespace guillaume::components
