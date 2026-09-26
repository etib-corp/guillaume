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

#include "guillaume/components/arc.hpp"

namespace guillaume::components
{
	Arc::Arc(void)
		: _thickness(8.0f)
		, _startAngle(0.0f)
		, _sweepAngle(3.14159265358979323846f)
		, _segments(48)
	{
	}

	float Arc::getThickness(void) const
	{
		return _thickness;
	}

	Arc &Arc::setThickness(float thickness)
	{
		if (_thickness == thickness) {
			return *this;
		}
		_thickness = thickness;
		setHasChanged(true);
		return *this;
	}

	float Arc::getStartAngle(void) const
	{
		return _startAngle;
	}

	Arc &Arc::setStartAngle(float startAngle)
	{
		if (_startAngle == startAngle) {
			return *this;
		}
		_startAngle = startAngle;
		setHasChanged(true);
		return *this;
	}

	float Arc::getSweepAngle(void) const
	{
		return _sweepAngle;
	}

	Arc &Arc::setSweepAngle(float sweepAngle)
	{
		if (_sweepAngle == sweepAngle) {
			return *this;
		}
		_sweepAngle = sweepAngle;
		setHasChanged(true);
		return *this;
	}

	Arc::SegmentCount Arc::getSegments(void) const
	{
		return _segments;
	}

	Arc &Arc::setSegments(SegmentCount segments)
	{
		if (_segments == segments) {
			return *this;
		}
		_segments = segments;
		setHasChanged(true);
		return *this;
	}
}	 // namespace guillaume::components
