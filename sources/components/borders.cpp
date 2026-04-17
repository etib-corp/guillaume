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

#include "guillaume/components/borders.hpp"

namespace guillaume::components
{
	Borders::Borders(void)
		: _topLeftRadius(0.0f)
		, _topRightRadius(0.0f)
		, _bottomRightRadius(0.0f)
		, _bottomLeftRadius(0.0f)
	{
	}

	Borders::BorderRadius Borders::getTopLeftRadius(void) const
	{
		return _topLeftRadius;
	}

	Borders &Borders::setTopLeftRadius(BorderRadius radius)
	{
		if (_topLeftRadius == radius) {
			return *this;
		}
		_topLeftRadius = radius;
		setHasChanged(true);
		return *this;
	}

	Borders::BorderRadius Borders::getTopRightRadius(void) const
	{
		return _topRightRadius;
	}

	Borders &Borders::setTopRightRadius(BorderRadius radius)
	{
		if (_topRightRadius == radius) {
			return *this;
		}
		_topRightRadius = radius;
		setHasChanged(true);
		return *this;
	}

	Borders::BorderRadius Borders::getBottomRightRadius(void) const
	{
		return _bottomRightRadius;
	}

	Borders &Borders::setBottomRightRadius(BorderRadius radius)
	{
		if (_bottomRightRadius == radius) {
			return *this;
		}
		_bottomRightRadius = radius;
		setHasChanged(true);
		return *this;
	}

	Borders::BorderRadius Borders::getBottomLeftRadius(void) const
	{
		return _bottomLeftRadius;
	}

	Borders &Borders::setBottomLeftRadius(BorderRadius radius)
	{
		if (_bottomLeftRadius == radius) {
			return *this;
		}
		_bottomLeftRadius = radius;
		setHasChanged(true);
		return *this;
	}

	Borders &Borders::setBorderRadius(BorderRadius radius)
	{
		return setTopLeftRadius(radius)
			.setTopRightRadius(radius)
			.setBottomRightRadius(radius)
			.setBottomLeftRadius(radius);
	}
}	 // namespace guillaume::components