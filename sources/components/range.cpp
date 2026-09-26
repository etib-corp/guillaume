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

#include "guillaume/components/range.hpp"

#include <algorithm>
#include <cmath>

namespace guillaume::components
{
	float Range::getMin(void) const
	{
		return _min;
	}

	Range &Range::setMin(float min)
	{
		if (_min == min) {
			return *this;
		}
		_min = min;
		setHasChanged(true);
		apply(_low, _high);
		return *this;
	}

	float Range::getMax(void) const
	{
		return _max;
	}

	Range &Range::setMax(float max)
	{
		if (_max == max) {
			return *this;
		}
		_max = max;
		setHasChanged(true);
		apply(_low, _high);
		return *this;
	}

	float Range::getLow(void) const
	{
		return _low;
	}

	Range &Range::setLow(float low)
	{
		apply(low, _high);
		return *this;
	}

	float Range::getHigh(void) const
	{
		return _high;
	}

	Range &Range::setHigh(float high)
	{
		apply(_low, high);
		return *this;
	}

	float Range::getStep(void) const
	{
		return _step;
	}

	Range &Range::setStep(float step)
	{
		if (_step == step) {
			return *this;
		}
		_step = step;
		setHasChanged(true);
		return *this;
	}

	Range &Range::setOnChangedHandler(const Handler &handler)
	{
		_onChanged = handler;
		return *this;
	}

	Range::Handler Range::getOnChangedHandler(void) const
	{
		return _onChanged;
	}

	float Range::sanitize(float value) const
	{
		float result = std::clamp(value, _min, _max);
		if (_step > 0.0f) {
			const float steps = std::round((result - _min) / _step);
			result			  = std::clamp(_min + steps * _step, _min, _max);
		}
		return result;
	}

	void Range::apply(float low, float high)
	{
		float newLow  = sanitize(low);
		float newHigh = sanitize(high);

		if (newLow > newHigh) {
			std::swap(newLow, newHigh);
		}

		if (newLow == _low && newHigh == _high) {
			return;
		}

		_low  = newLow;
		_high = newHigh;
		setHasChanged(true);

		if (_onChanged) {
			_onChanged(_low, _high);
		}
	}
}	 // namespace guillaume::components
