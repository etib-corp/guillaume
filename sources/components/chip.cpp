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

#include "guillaume/components/chip.hpp"

#include <utility>

namespace guillaume::components
{
	Chip::Variant Chip::getVariant(void) const noexcept
	{
		return _variant;
	}

	Chip &Chip::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	Chip::Mode Chip::getMode(void) const noexcept
	{
		return _mode;
	}

	Chip &Chip::setMode(Mode mode)
	{
		if (_mode != mode) {
			_mode = mode;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Chip::getLabel(void) const noexcept
	{
		return _label;
	}

	Chip &Chip::setLabel(const std::string &label)
	{
		if (_label != label) {
			_label = label;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Chip::getIconGlyph(void) const noexcept
	{
		return _iconGlyph;
	}

	Chip &Chip::setIconGlyph(const std::string &iconGlyph)
	{
		if (_iconGlyph != iconGlyph) {
			_iconGlyph = iconGlyph;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Chip::getLeadingIcon(void) const noexcept
	{
		return _leadingIcon;
	}

	Chip &Chip::setLeadingIcon(ecs::Entity::Identifier identifier)
	{
		if (_leadingIcon != identifier) {
			_leadingIcon = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Chip::getTrailingIcon(void) const noexcept
	{
		return _trailingIcon;
	}

	Chip &Chip::setTrailingIcon(ecs::Entity::Identifier identifier)
	{
		if (_trailingIcon != identifier) {
			_trailingIcon = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Chip::getLabelEntity(void) const noexcept
	{
		return _labelEntity;
	}

	Chip &Chip::setLabelEntity(ecs::Entity::Identifier identifier)
	{
		if (_labelEntity != identifier) {
			_labelEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Chip::getCheckIcon(void) const noexcept
	{
		return _checkIcon;
	}

	Chip &Chip::setCheckIcon(ecs::Entity::Identifier identifier)
	{
		if (_checkIcon != identifier) {
			_checkIcon = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	bool Chip::isCompact(void) const noexcept
	{
		return _compact;
	}

	Chip &Chip::setCompact(bool compact)
	{
		if (_compact != compact) {
			_compact = compact;
			setHasChanged(true);
		}
		return *this;
	}

	bool Chip::isElevated(void) const noexcept
	{
		return _elevated;
	}

	Chip &Chip::setElevated(bool elevated)
	{
		if (_elevated != elevated) {
			_elevated = elevated;
			setHasChanged(true);
		}
		return *this;
	}

	bool Chip::isShowSelectedOverlay(void) const noexcept
	{
		return _showSelectedOverlay;
	}

	Chip &Chip::setShowSelectedOverlay(bool show)
	{
		if (_showSelectedOverlay != show) {
			_showSelectedOverlay = show;
			setHasChanged(true);
		}
		return *this;
	}

	bool Chip::isShowSelectedCheck(void) const noexcept
	{
		return _showSelectedCheck;
	}

	Chip &Chip::setShowSelectedCheck(bool show)
	{
		if (_showSelectedCheck != show) {
			_showSelectedCheck = show;
			setHasChanged(true);
		}
		return *this;
	}

	const Chip::Handler &Chip::getOnPress(void) const noexcept
	{
		return _onPress;
	}

	Chip &Chip::setOnPress(Handler handler)
	{
		_onPress = std::move(handler);
		return *this;
	}

	const Chip::Handler &Chip::getOnClose(void) const noexcept
	{
		return _onClose;
	}

	Chip &Chip::setOnClose(Handler handler)
	{
		_onClose = std::move(handler);
		return *this;
	}
}	 // namespace guillaume::components
