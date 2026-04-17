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

#include "guillaume/components/bound.hpp"

namespace guillaume::components
{
	Bound &Bound::setWidth(std::size_t width)
	{
		if (_width == width) {
			return *this;
		}
		_width = width;
		setHasChanged(true);
		return *this;
	}

	std::size_t Bound::getWidth(void) const
	{
		return _width;
	}

	Bound &Bound::setHeight(std::size_t height)
	{
		if (_height == height) {
			return *this;
		}
		_height = height;
		setHasChanged(true);
		return *this;
	}

	std::size_t Bound::getHeight(void) const
	{
		return _height;
	}
}	 // namespace guillaume::components