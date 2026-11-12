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

#include "guillaume/components/checkbox.hpp"

#include <utility>

namespace guillaume::components
{
	Checkbox::State Checkbox::getState(void) const noexcept
	{
		return _state;
	}

	Checkbox &Checkbox::setState(State state)
	{
		if (_state != state) {
			_state = state;
			setHasChanged(true);
		}
		return *this;
	}

	Checkbox::Variant Checkbox::getVariant(void) const noexcept
	{
		return _variant;
	}

	Checkbox &Checkbox::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Checkbox::getLabel(void) const noexcept
	{
		return _label;
	}

	Checkbox &Checkbox::setLabel(const std::string &label)
	{
		if (_label != label) {
			_label = label;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Checkbox::getCheckIcon(void) const noexcept
	{
		return _checkIcon;
	}

	Checkbox &Checkbox::setCheckIcon(ecs::Entity::Identifier identifier)
	{
		if (_checkIcon != identifier) {
			_checkIcon = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Checkbox::getLabelEntity(void) const noexcept
	{
		return _labelEntity;
	}

	Checkbox &Checkbox::setLabelEntity(ecs::Entity::Identifier identifier)
	{
		if (_labelEntity != identifier) {
			_labelEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	const std::optional<utility::graphic::Color32Bit> &
		Checkbox::getCheckedColor(void) const noexcept
	{
		return _checkedColor;
	}

	Checkbox &Checkbox::setCheckedColor(
		std::optional<utility::graphic::Color32Bit> color)
	{
		if (_checkedColor != color) {
			_checkedColor = color;
			setHasChanged(true);
		}
		return *this;
	}

	const std::optional<utility::graphic::Color32Bit> &
		Checkbox::getUncheckedColor(void) const noexcept
	{
		return _uncheckedColor;
	}

	Checkbox &Checkbox::setUncheckedColor(
		std::optional<utility::graphic::Color32Bit> color)
	{
		if (_uncheckedColor != color) {
			_uncheckedColor = color;
			setHasChanged(true);
		}
		return *this;
	}

	const Checkbox::ValueChangeHandler &
		Checkbox::getOnValueChange(void) const noexcept
	{
		return _onValueChange;
	}

	Checkbox &Checkbox::setOnValueChange(ValueChangeHandler handler)
	{
		_onValueChange = std::move(handler);
		return *this;
	}
}	 // namespace guillaume::components
