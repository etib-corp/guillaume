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

#include "guillaume/components/value.hpp"

#include <algorithm>
#include <cmath>

namespace guillaume::components
{
	float Value::getMin(void) const
	{
		return _min;
	}

	Value &Value::setMin(float min)
	{
		if (_min == min) {
			return *this;
		}
		_min = min;
		setHasChanged(true);
		setValue(_value);
		return *this;
	}

	float Value::getMax(void) const
	{
		return _max;
	}

	Value &Value::setMax(float max)
	{
		if (_max == max) {
			return *this;
		}
		_max = max;
		setHasChanged(true);
		setValue(_value);
		return *this;
	}

	float Value::getValue(void) const
	{
		return _value;
	}

	Value &Value::setValue(float value)
	{
		const float sanitized = sanitize(value);
		if (sanitized == _value) {
			return *this;
		}
		_value = sanitized;
		setHasChanged(true);
		if (_onChanged) {
			_onChanged(_value);
		}
		return *this;
	}

	float Value::getStep(void) const
	{
		return _step;
	}

	Value &Value::setStep(float step)
	{
		if (_step == step) {
			return *this;
		}
		_step = step;
		setHasChanged(true);
		return *this;
	}

	float Value::getNormalizedValue(void) const
	{
		const float range = _max - _min;
		if (range <= 0.0f) {
			return 0.0f;
		}
		return (_value - _min) / range;
	}

	Value &Value::setNormalizedValue(float normalized)
	{
		const float clamped = std::clamp(normalized, 0.0f, 1.0f);
		return setValue(_min + clamped * (_max - _min));
	}

	Value &Value::setOnChangedHandler(const Handler &handler)
	{
		_onChanged = handler;
		return *this;
	}

	Value::Handler Value::getOnChangedHandler(void) const
	{
		return _onChanged;
	}

	float Value::sanitize(float value) const
	{
		float result = std::clamp(value, _min, _max);
		if (_step > 0.0f) {
			const float steps = std::round((result - _min) / _step);
			result			  = std::clamp(_min + steps * _step, _min, _max);
		}
		return result;
	}
}	 // namespace guillaume::components
