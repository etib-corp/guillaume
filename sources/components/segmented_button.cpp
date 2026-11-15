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

#include "guillaume/components/segmented_button.hpp"

#include <utility>

namespace guillaume::components
{
	const std::vector<SegmentedButton::Segment> &
		SegmentedButton::getSegments(void) const noexcept
	{
		return _segments;
	}

	SegmentedButton &
		SegmentedButton::setSegments(const std::vector<Segment> &segments)
	{
		if (_segments != segments) {
			_segments = segments;
			setHasChanged(true);
		}
		return *this;
	}

	SegmentedButton &SegmentedButton::setSegmentDisabled(std::size_t index,
														 bool disabled)
	{
		if (index < _segments.size() && _segments[index].disabled != disabled) {
			_segments[index].disabled = disabled;
			setHasChanged(true);
		}
		return *this;
	}

	SegmentedButton::SelectionMode
		SegmentedButton::getSelectionMode(void) const noexcept
	{
		return _selectionMode;
	}

	SegmentedButton &SegmentedButton::setSelectionMode(SelectionMode mode)
	{
		if (_selectionMode != mode) {
			_selectionMode = mode;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<ecs::Entity::Identifier> &
		SegmentedButton::getButtons(void) const noexcept
	{
		return _buttons;
	}

	SegmentedButton &SegmentedButton::setButtons(
		const std::vector<ecs::Entity::Identifier> &buttons)
	{
		if (_buttons != buttons) {
			_buttons = buttons;
			setHasChanged(true);
		}
		return *this;
	}

	const SegmentedButton::ChangeHandler &
		SegmentedButton::getOnChange(void) const noexcept
	{
		return _onChange;
	}

	SegmentedButton &SegmentedButton::setOnChange(ChangeHandler handler)
	{
		_onChange = std::move(handler);
		return *this;
	}
}	 // namespace guillaume::components
