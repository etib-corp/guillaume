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

#include "guillaume/components/elevation.hpp"

namespace guillaume::components
{
	float Elevation::getLevel(void) const
	{
		return _level;
	}

	Elevation &Elevation::setLevel(float level)
	{
		if (_level == level) {
			return *this;
		}
		_level = level;
		setHasChanged(true);
		return *this;
	}

	int Elevation::getLayers(void) const
	{
		return _layers;
	}

	Elevation &Elevation::setLayers(int layers)
	{
		if (_layers == layers) {
			return *this;
		}
		_layers = layers;
		setHasChanged(true);
		return *this;
	}

	float Elevation::getSpread(void) const
	{
		return _spread;
	}

	Elevation &Elevation::setSpread(float spread)
	{
		if (_spread == spread) {
			return *this;
		}
		_spread = spread;
		setHasChanged(true);
		return *this;
	}

	utility::graphic::Color32Bit Elevation::getColor(void) const
	{
		return _color;
	}

	Elevation &Elevation::setColor(const utility::graphic::Color32Bit &color)
	{
		if (_color == color) {
			return *this;
		}
		_color = color;
		setHasChanged(true);
		return *this;
	}
}	 // namespace guillaume::components
