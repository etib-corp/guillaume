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

#include "guillaume/components/switch.hpp"

#include <utility>

namespace guillaume::components
{
	Switch::Variant Switch::getVariant(void) const noexcept
	{
		return _variant;
	}

	Switch &Switch::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Switch::getIconGlyph(void) const noexcept
	{
		return _iconGlyph;
	}

	Switch &Switch::setIconGlyph(const std::string &glyph)
	{
		if (_iconGlyph != glyph) {
			_iconGlyph = glyph;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Switch::getLabel(void) const noexcept
	{
		return _label;
	}

	Switch &Switch::setLabel(const std::string &label)
	{
		if (_label != label) {
			_label = label;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Switch::getThumb(void) const noexcept
	{
		return _thumb;
	}

	Switch &Switch::setThumb(ecs::Entity::Identifier identifier)
	{
		if (_thumb != identifier) {
			_thumb = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Switch::getIcon(void) const noexcept
	{
		return _icon;
	}

	Switch &Switch::setIcon(ecs::Entity::Identifier identifier)
	{
		if (_icon != identifier) {
			_icon = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Switch::getLabelEntity(void) const noexcept
	{
		return _labelEntity;
	}

	Switch &Switch::setLabelEntity(ecs::Entity::Identifier identifier)
	{
		if (_labelEntity != identifier) {
			_labelEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	const Switch::ValueChangeHandler &
		Switch::getOnValueChange(void) const noexcept
	{
		return _onValueChange;
	}

	Switch &Switch::setOnValueChange(ValueChangeHandler handler)
	{
		_onValueChange = std::move(handler);
		return *this;
	}
}	 // namespace guillaume::components
