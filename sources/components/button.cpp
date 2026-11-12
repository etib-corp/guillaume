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

#include "guillaume/components/button.hpp"

#include <utility>

namespace guillaume::components
{
	const std::string &Button::getIconGlyphName(void) const noexcept
	{
		return _iconGlyphName;
	}

	Button &Button::setIconGlyphName(const std::string &name)
	{
		if (_iconGlyphName != name) {
			_iconGlyphName = name;
			setHasChanged(true);
		}
		return *this;
	}

	Glyph::Style Button::getIconStyle(void) const noexcept
	{
		return _iconStyle;
	}

	Button &Button::setIconStyle(Glyph::Style style)
	{
		if (_iconStyle != style) {
			_iconStyle = style;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Button::getSelectedIconGlyphName(void) const noexcept
	{
		return _selectedIconGlyphName;
	}

	Button &Button::setSelectedIconGlyphName(const std::string &name)
	{
		if (_selectedIconGlyphName != name) {
			_selectedIconGlyphName = name;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Button::getLabelContent(void) const noexcept
	{
		return _labelContent;
	}

	Button &Button::setLabelContent(const std::string &content)
	{
		if (_labelContent != content) {
			_labelContent = content;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Button::getIcon(void) const noexcept
	{
		return _icon;
	}

	Button &Button::setIcon(ecs::Entity::Identifier identifier)
	{
		if (_icon != identifier) {
			_icon = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Button::getLabel(void) const noexcept
	{
		return _label;
	}

	Button &Button::setLabel(ecs::Entity::Identifier identifier)
	{
		if (_label != identifier) {
			_label = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	bool Button::isToggle(void) const noexcept
	{
		return _toggle;
	}

	Button &Button::setToggle(bool toggle)
	{
		if (_toggle != toggle) {
			_toggle = toggle;
			setHasChanged(true);
		}
		return *this;
	}

	bool Button::isSelected(void) const noexcept
	{
		return _selected;
	}

	Button &Button::setSelected(bool selected)
	{
		if (_selected != selected) {
			_selected = selected;
			setHasChanged(true);
		}
		return *this;
	}

	bool Button::isMorph(void) const noexcept
	{
		return _morph;
	}

	Button &Button::setMorph(bool morph)
	{
		if (_morph != morph) {
			_morph = morph;
			setHasChanged(true);
		}
		return *this;
	}

	bool Button::isTrailingIcon(void) const noexcept
	{
		return _trailingIcon;
	}

	Button &Button::setTrailingIcon(bool trailing)
	{
		if (_trailingIcon != trailing) {
			_trailingIcon = trailing;
			setHasChanged(true);
		}
		return *this;
	}

	bool Button::isLoading(void) const noexcept
	{
		return _loading;
	}

	Button &Button::setLoading(bool loading)
	{
		if (_loading != loading) {
			_loading = loading;
			setHasChanged(true);
		}
		return *this;
	}

	bool Button::isVisible(void) const noexcept
	{
		return _visible;
	}

	Button &Button::setVisible(bool visible)
	{
		if (_visible != visible) {
			_visible = visible;
			setHasChanged(true);
		}
		return *this;
	}

	const Button::Handler &Button::getOnClick(void) const noexcept
	{
		return _onClick;
	}

	Button &Button::setOnClick(Handler handler)
	{
		_onClick = std::move(handler);
		return *this;
	}

	const Button::Handler &Button::getOnLongPress(void) const noexcept
	{
		return _onLongPress;
	}

	Button &Button::setOnLongPress(Handler handler)
	{
		_onLongPress = std::move(handler);
		return *this;
	}

	Button::TimePoint Button::getPressStart(void) const noexcept
	{
		return _pressStart;
	}

	Button &Button::setPressStart(TimePoint time)
	{
		if (_pressStart != time) {
			_pressStart = time;
			setHasChanged(true);
		}
		return *this;
	}

	const Button::OptionalColor &
		Button::getContainerColorOverride(void) const noexcept
	{
		return _containerColorOverride;
	}

	Button &Button::setContainerColorOverride(const OptionalColor &color)
	{
		if (_containerColorOverride != color) {
			_containerColorOverride = color;
			setHasChanged(true);
		}
		return *this;
	}

	const Button::OptionalColor &
		Button::getContentColorOverride(void) const noexcept
	{
		return _contentColorOverride;
	}

	Button &Button::setContentColorOverride(const OptionalColor &color)
	{
		if (_contentColorOverride != color) {
			_contentColorOverride = color;
			setHasChanged(true);
		}
		return *this;
	}

	Button::RestyleFunction Button::getRestyle(void) const noexcept
	{
		return _restyle;
	}

	Button &Button::setRestyle(RestyleFunction function)
	{
		if (_restyle != function) {
			_restyle = function;
			setHasChanged(true);
		}
		return *this;
	}
}	 // namespace guillaume::components
