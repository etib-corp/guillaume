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

#include "guillaume/components/extended_floating_action_button.hpp"

#include <utility>

namespace guillaume::components
{
	ExtendedFloatingActionButton::Variant
		ExtendedFloatingActionButton::getVariant(void) const noexcept
	{
		return _variant;
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	ExtendedFloatingActionButton::Size
		ExtendedFloatingActionButton::getSize(void) const noexcept
	{
		return _size;
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setSize(Size size)
	{
		if (_size != size) {
			_size = size;
			setHasChanged(true);
		}
		return *this;
	}

	ExtendedFloatingActionButton::Mode
		ExtendedFloatingActionButton::getMode(void) const noexcept
	{
		return _mode;
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setMode(Mode mode)
	{
		if (_mode != mode) {
			_mode = mode;
			setHasChanged(true);
		}
		return *this;
	}

	const std::optional<float> &
		ExtendedFloatingActionButton::getCustomSize(void) const noexcept
	{
		return _customSize;
	}

	ExtendedFloatingActionButton &ExtendedFloatingActionButton::setCustomSize(
		const std::optional<float> &size)
	{
		if (_customSize != size) {
			_customSize = size;
			setHasChanged(true);
		}
		return *this;
	}

	bool ExtendedFloatingActionButton::isExtended(void) const noexcept
	{
		return _extended;
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setExtended(bool extended)
	{
		if (_extended != extended) {
			_extended = extended;
			setHasChanged(true);
		}
		return *this;
	}

	bool ExtendedFloatingActionButton::isLowered(void) const noexcept
	{
		return _lowered;
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setLowered(bool lowered)
	{
		if (_lowered != lowered) {
			_lowered = lowered;
			setHasChanged(true);
		}
		return *this;
	}
}	 // namespace guillaume::components
