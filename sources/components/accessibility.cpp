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

#include "guillaume/components/accessibility.hpp"

#include <stdexcept>

namespace guillaume::components
{
	const std::string &Accessibility::getLabel(void) const
	{
		return _label;
	}

	Accessibility &Accessibility::setLabel(const std::string &label)
	{
		if (_label != label) {
			_label = label;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Accessibility::getHint(void) const
	{
		return _hint;
	}

	Accessibility &Accessibility::setHint(const std::string &hint)
	{
		if (_hint != hint) {
			_hint = hint;
			setHasChanged(true);
		}
		return *this;
	}

	AccessibilityRole Accessibility::getRole(void) const
	{
		return _role;
	}

	Accessibility &Accessibility::setRole(AccessibilityRole role)
	{
		if (_role != role) {
			_role = role;
			setHasChanged(true);
		}
		return *this;
	}

	bool Accessibility::isSelected(void) const
	{
		return _selected;
	}

	Accessibility &Accessibility::setSelected(bool selected)
	{
		if (_selected != selected) {
			_selected = selected;
			setHasChanged(true);
		}
		return *this;
	}

	std::optional<CheckedState> Accessibility::getChecked(void) const
	{
		return _checked;
	}

	Accessibility &
		Accessibility::setChecked(std::optional<CheckedState> checked)
	{
		if (_checked != checked) {
			_checked = checked;
			setHasChanged(true);
		}
		return *this;
	}

	std::optional<bool> Accessibility::getExpanded(void) const
	{
		return _expanded;
	}

	Accessibility &Accessibility::setExpanded(std::optional<bool> expanded)
	{
		if (_expanded != expanded) {
			_expanded = expanded;
			setHasChanged(true);
		}
		return *this;
	}

	const std::optional<AccessibilityValue> &Accessibility::getValue(void) const
	{
		return _value;
	}

	Accessibility &
		Accessibility::setValue(std::optional<AccessibilityValue> value)
	{
		if (value.has_value()
			&& (value->min > value->max || value->now < value->min
				|| value->now > value->max)) {
			throw std::invalid_argument(
				"Accessibility value must satisfy min <= now <= max");
		}
		_value = value;
		setHasChanged(true);
		return *this;
	}

	LiveRegion Accessibility::getLiveRegion(void) const
	{
		return _liveRegion;
	}

	Accessibility &Accessibility::setLiveRegion(LiveRegion liveRegion)
	{
		if (_liveRegion != liveRegion) {
			_liveRegion = liveRegion;
			setHasChanged(true);
		}
		return *this;
	}

	int Accessibility::getFocusOrder(void) const
	{
		return _focusOrder;
	}

	Accessibility &Accessibility::setFocusOrder(int focusOrder)
	{
		if (focusOrder < 0) {
			throw std::invalid_argument("Focus order must be non-negative");
		}
		if (_focusOrder != focusOrder) {
			_focusOrder = focusOrder;
			setHasChanged(true);
		}
		return *this;
	}

	bool Accessibility::isFocusable(void) const
	{
		return _focusable;
	}

	Accessibility &Accessibility::setFocusable(bool focusable)
	{
		if (_focusable != focusable) {
			_focusable = focusable;
			setHasChanged(true);
		}
		return *this;
	}

}	 // namespace guillaume::components
