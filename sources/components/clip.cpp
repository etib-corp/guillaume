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

#include "guillaume/components/clip.hpp"

namespace guillaume::components
{
	float Clip::getMargin(void) const
	{
		return _margin;
	}

	Clip &Clip::setMargin(float margin)
	{
		if (_margin == margin) {
			return *this;
		}
		_margin = margin;
		setHasChanged(true);
		return *this;
	}

	bool Clip::isEnabled(void) const
	{
		return _enabled;
	}

	Clip &Clip::setEnabled(bool enabled)
	{
		if (_enabled == enabled) {
			return *this;
		}
		_enabled = enabled;
		setHasChanged(true);
		return *this;
	}

	const Clip::Rect &Clip::getRect(void) const
	{
		return _rect;
	}

	Clip &Clip::setRect(const Rect &rect)
	{
		if (_rect == rect) {
			return *this;
		}
		_rect = rect;
		setHasChanged(true);
		return *this;
	}
}	 // namespace guillaume::components
