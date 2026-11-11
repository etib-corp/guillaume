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

#include "guillaume/components/divider.hpp"

namespace guillaume::components
{
	Divider::Variant Divider::getVariant(void) const
	{
		return _variant;
	}

	Divider &Divider::setVariant(Variant value)
	{
		if (_variant == value) {
			return *this;
		}
		_variant = value;
		setHasChanged(true);
		return *this;
	}

	float Divider::getLength(void) const
	{
		return _length;
	}

	Divider &Divider::setLength(float value)
	{
		if (_length == value) {
			return *this;
		}
		_length = value;
		setHasChanged(true);
		return *this;
	}

	float Divider::getThickness(void) const
	{
		return _thickness;
	}

	Divider &Divider::setThickness(float value)
	{
		if (_thickness == value) {
			return *this;
		}
		_thickness = value;
		setHasChanged(true);
		return *this;
	}

	float Divider::getInset(void) const
	{
		return _inset;
	}

	Divider &Divider::setInset(float value)
	{
		if (_inset == value) {
			return *this;
		}
		_inset = value;
		setHasChanged(true);
		return *this;
	}

	const utility::graphic::Color32Bit &Divider::getColor(void) const
	{
		return _color;
	}

	Divider &Divider::setColor(const utility::graphic::Color32Bit &value)
	{
		_color = value;
		setHasChanged(true);
		return *this;
	}

	const utility::graphic::PoseF &Divider::getOriginPose(void) const
	{
		return _originPose;
	}

	Divider &Divider::setOriginPose(const utility::graphic::PoseF &value)
	{
		if (_originPose == value) {
			return *this;
		}
		_originPose = value;
		setHasChanged(true);
		return *this;
	}

	const utility::graphic::PoseF &Divider::getAppliedPose(void) const
	{
		return _appliedPose;
	}

	Divider &Divider::setAppliedPose(const utility::graphic::PoseF &value)
	{
		_appliedPose = value;
		return *this;
	}

	float Divider::getLeadingInset(void) const
	{
		return _variant == Variant::FullWidth ? 0.0f : _inset;
	}

	float Divider::getTrailingInset(void) const
	{
		return _variant == Variant::Middle ? _inset : 0.0f;
	}
}	 // namespace guillaume::components
