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

#include "guillaume/components/radio_button.hpp"

#include <utility>

namespace guillaume::components
{
	RadioButton::Variant RadioButton::getVariant(void) const noexcept
	{
		return _variant;
	}

	RadioButton &RadioButton::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &RadioButton::getLabel(void) const noexcept
	{
		return _label;
	}

	RadioButton &RadioButton::setLabel(const std::string &label)
	{
		if (_label != label) {
			_label = label;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier RadioButton::getDot(void) const noexcept
	{
		return _dot;
	}

	RadioButton &RadioButton::setDot(ecs::Entity::Identifier identifier)
	{
		if (_dot != identifier) {
			_dot = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier RadioButton::getLabelEntity(void) const noexcept
	{
		return _labelEntity;
	}

	RadioButton &RadioButton::setLabelEntity(ecs::Entity::Identifier identifier)
	{
		if (_labelEntity != identifier) {
			_labelEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	const RadioButton::PressHandler &
		RadioButton::getOnPress(void) const noexcept
	{
		return _onPress;
	}

	RadioButton &RadioButton::setOnPress(PressHandler handler)
	{
		_onPress = std::move(handler);
		return *this;
	}
}	 // namespace guillaume::components
