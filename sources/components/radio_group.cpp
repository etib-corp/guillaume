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

#include "guillaume/components/radio_group.hpp"

#include <utility>

namespace guillaume::components
{
	const std::vector<std::string> &RadioGroup::getOptions(void) const noexcept
	{
		return _options;
	}

	RadioGroup &RadioGroup::setOptions(const std::vector<std::string> &options)
	{
		if (_options != options) {
			_options = options;
			setHasChanged(true);
		}
		return *this;
	}

	std::optional<std::size_t> RadioGroup::getValue(void) const noexcept
	{
		return _value;
	}

	RadioGroup &RadioGroup::setValue(std::optional<std::size_t> value)
	{
		if (_value != value) {
			_value = value;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<ecs::Entity::Identifier> &
		RadioGroup::getButtons(void) const noexcept
	{
		return _buttons;
	}

	RadioGroup &RadioGroup::setButtons(
		const std::vector<ecs::Entity::Identifier> &buttons)
	{
		if (_buttons != buttons) {
			_buttons = buttons;
			setHasChanged(true);
		}
		return *this;
	}

	const RadioGroup::ValueChangeHandler &
		RadioGroup::getOnValueChange(void) const noexcept
	{
		return _onValueChange;
	}

	RadioGroup &RadioGroup::setOnValueChange(ValueChangeHandler handler)
	{
		_onValueChange = std::move(handler);
		return *this;
	}
}	 // namespace guillaume::components
