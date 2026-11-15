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

#include "guillaume/components/navigation_bar_destination.hpp"

#include <utility>

namespace guillaume::components
{
	const std::string &NavigationBarDestination::getIcon(void) const noexcept
	{
		return _icon;
	}

	NavigationBarDestination &
		NavigationBarDestination::setIcon(const std::string &icon)
	{
		if (_icon != icon) {
			_icon = icon;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &
		NavigationBarDestination::getFocusedIcon(void) const noexcept
	{
		return _focusedIcon;
	}

	NavigationBarDestination &
		NavigationBarDestination::setFocusedIcon(const std::string &icon)
	{
		if (_focusedIcon != icon) {
			_focusedIcon = icon;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &NavigationBarDestination::getLabel(void) const noexcept
	{
		return _label;
	}

	NavigationBarDestination &
		NavigationBarDestination::setLabel(const std::string &label)
	{
		if (_label != label) {
			_label = label;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &NavigationBarDestination::getBadge(void) const noexcept
	{
		return _badge;
	}

	NavigationBarDestination &
		NavigationBarDestination::setBadge(const std::string &badge)
	{
		if (_badge != badge) {
			_badge = badge;
			setHasChanged(true);
		}
		return *this;
	}

	bool NavigationBarDestination::isLabelShown(void) const noexcept
	{
		return _showLabel;
	}

	NavigationBarDestination &NavigationBarDestination::setShowLabel(bool show)
	{
		if (_showLabel != show) {
			_showLabel = show;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier
		NavigationBarDestination::getIndicator(void) const noexcept
	{
		return _indicator;
	}

	NavigationBarDestination &NavigationBarDestination::setIndicator(
		ecs::Entity::Identifier identifier)
	{
		if (_indicator != identifier) {
			_indicator = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier
		NavigationBarDestination::getIconEntity(void) const noexcept
	{
		return _iconEntity;
	}

	NavigationBarDestination &NavigationBarDestination::setIconEntity(
		ecs::Entity::Identifier identifier)
	{
		if (_iconEntity != identifier) {
			_iconEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier
		NavigationBarDestination::getLabelEntity(void) const noexcept
	{
		return _labelEntity;
	}

	NavigationBarDestination &NavigationBarDestination::setLabelEntity(
		ecs::Entity::Identifier identifier)
	{
		if (_labelEntity != identifier) {
			_labelEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier
		NavigationBarDestination::getBadgeEntity(void) const noexcept
	{
		return _badgeEntity;
	}

	NavigationBarDestination &NavigationBarDestination::setBadgeEntity(
		ecs::Entity::Identifier identifier)
	{
		if (_badgeEntity != identifier) {
			_badgeEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	const NavigationBarDestination::PressHandler &
		NavigationBarDestination::getOnPress(void) const noexcept
	{
		return _onPress;
	}

	NavigationBarDestination &
		NavigationBarDestination::setOnPress(PressHandler handler)
	{
		_onPress = std::move(handler);
		return *this;
	}
}	 // namespace guillaume::components
