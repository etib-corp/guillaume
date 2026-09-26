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

#include "guillaume/components/layout.hpp"

namespace guillaume::components
{
	Layout::Axis Layout::getAxis(void) const
	{
		return _axis;
	}

	Layout &Layout::setAxis(Axis axis)
	{
		if (_axis == axis) {
			return *this;
		}
		_axis = axis;
		setHasChanged(true);
		return *this;
	}

	Layout::MainAxisAlignment Layout::getMainAxisAlignment(void) const
	{
		return _mainAxisAlignment;
	}

	Layout &Layout::setMainAxisAlignment(MainAxisAlignment alignment)
	{
		if (_mainAxisAlignment == alignment) {
			return *this;
		}
		_mainAxisAlignment = alignment;
		setHasChanged(true);
		return *this;
	}

	Layout::CrossAxisAlignment Layout::getCrossAxisAlignment(void) const
	{
		return _crossAxisAlignment;
	}

	Layout &Layout::setCrossAxisAlignment(CrossAxisAlignment alignment)
	{
		if (_crossAxisAlignment == alignment) {
			return *this;
		}
		_crossAxisAlignment = alignment;
		setHasChanged(true);
		return *this;
	}

	float Layout::getSpacing(void) const
	{
		return _spacing;
	}

	Layout &Layout::setSpacing(float spacing)
	{
		if (_spacing == spacing) {
			return *this;
		}
		_spacing = spacing;
		setHasChanged(true);
		return *this;
	}

	float Layout::getPadding(void) const
	{
		return _padding;
	}

	Layout &Layout::setPadding(float padding)
	{
		if (_padding == padding) {
			return *this;
		}
		_padding = padding;
		setHasChanged(true);
		return *this;
	}

	bool Layout::hasFixedWidth(void) const
	{
		return _hasFixedWidth;
	}

	float Layout::getFixedWidth(void) const
	{
		return _fixedWidth;
	}

	Layout &Layout::setFixedWidth(float width)
	{
		if (_hasFixedWidth && _fixedWidth == width) {
			return *this;
		}
		_hasFixedWidth = true;
		_fixedWidth	   = width;
		setHasChanged(true);
		return *this;
	}

	Layout &Layout::clearFixedWidth(void)
	{
		if (!_hasFixedWidth) {
			return *this;
		}
		_hasFixedWidth = false;
		_fixedWidth	   = 0.0f;
		setHasChanged(true);
		return *this;
	}

	bool Layout::hasFixedHeight(void) const
	{
		return _hasFixedHeight;
	}

	float Layout::getFixedHeight(void) const
	{
		return _fixedHeight;
	}

	Layout &Layout::setFixedHeight(float height)
	{
		if (_hasFixedHeight && _fixedHeight == height) {
			return *this;
		}
		_hasFixedHeight = true;
		_fixedHeight	= height;
		setHasChanged(true);
		return *this;
	}

	Layout &Layout::clearFixedHeight(void)
	{
		if (!_hasFixedHeight) {
			return *this;
		}
		_hasFixedHeight = false;
		_fixedHeight	= 0.0f;
		setHasChanged(true);
		return *this;
	}
}	 // namespace guillaume::components
