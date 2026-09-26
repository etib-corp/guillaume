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

#include "guillaume/components/line.hpp"

namespace guillaume::components
{
	Line::Line(void)
		: _thickness(2.0f)
		, _dashed(false)
		, _dashLength(8.0f)
		, _gapLength(8.0f)
	{
	}

	float Line::getThickness(void) const
	{
		return _thickness;
	}

	Line &Line::setThickness(float thickness)
	{
		if (_thickness == thickness) {
			return *this;
		}
		_thickness = thickness;
		setHasChanged(true);
		return *this;
	}

	bool Line::isDashed(void) const
	{
		return _dashed;
	}

	Line &Line::setDashed(bool dashed)
	{
		if (_dashed == dashed) {
			return *this;
		}
		_dashed = dashed;
		setHasChanged(true);
		return *this;
	}

	float Line::getDashLength(void) const
	{
		return _dashLength;
	}

	Line &Line::setDashLength(float dashLength)
	{
		if (_dashLength == dashLength) {
			return *this;
		}
		_dashLength = dashLength;
		setHasChanged(true);
		return *this;
	}

	float Line::getGapLength(void) const
	{
		return _gapLength;
	}

	Line &Line::setGapLength(float gapLength)
	{
		if (_gapLength == gapLength) {
			return *this;
		}
		_gapLength = gapLength;
		setHasChanged(true);
		return *this;
	}
}	 // namespace guillaume::components
