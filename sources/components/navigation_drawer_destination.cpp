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

#include "guillaume/components/navigation_drawer_destination.hpp"

#include <utility>

namespace guillaume::components
{
	const std::string &NavigationDrawerDestination::getIcon(void) const noexcept
	{
		return _icon;
	}

	NavigationDrawerDestination &
		NavigationDrawerDestination::setIcon(const std::string &icon)
	{
		if (_icon != icon) {
			_icon = icon;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &
		NavigationDrawerDestination::getLabel(void) const noexcept
	{
		return _label;
	}

	NavigationDrawerDestination &
		NavigationDrawerDestination::setLabel(const std::string &label)
	{
		if (_label != label) {
			_label = label;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &
		NavigationDrawerDestination::getRight(void) const noexcept
	{
		return _right;
	}

	NavigationDrawerDestination &
		NavigationDrawerDestination::setRight(const std::string &right)
	{
		if (_right != right) {
			_right = right;
			setHasChanged(true);
		}
		return *this;
	}

	bool NavigationDrawerDestination::isDisabled(void) const noexcept
	{
		return _disabled;
	}

	NavigationDrawerDestination &
		NavigationDrawerDestination::setDisabled(bool disabled)
	{
		if (_disabled != disabled) {
			_disabled = disabled;
			setHasChanged(true);
		}
		return *this;
	}

	bool NavigationDrawerDestination::isCollapsed(void) const noexcept
	{
		return _collapsed;
	}

	NavigationDrawerDestination &
		NavigationDrawerDestination::setCollapsed(bool collapsed)
	{
		if (_collapsed != collapsed) {
			_collapsed = collapsed;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier
		NavigationDrawerDestination::getLead(void) const noexcept
	{
		return _lead;
	}

	NavigationDrawerDestination &
		NavigationDrawerDestination::setLead(ecs::Entity::Identifier identifier)
	{
		if (_lead != identifier) {
			_lead = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier
		NavigationDrawerDestination::getIconEntity(void) const noexcept
	{
		return _iconEntity;
	}

	NavigationDrawerDestination &NavigationDrawerDestination::setIconEntity(
		ecs::Entity::Identifier identifier)
	{
		if (_iconEntity != identifier) {
			_iconEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier
		NavigationDrawerDestination::getLabelEntity(void) const noexcept
	{
		return _labelEntity;
	}

	NavigationDrawerDestination &NavigationDrawerDestination::setLabelEntity(
		ecs::Entity::Identifier identifier)
	{
		if (_labelEntity != identifier) {
			_labelEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier
		NavigationDrawerDestination::getRightEntity(void) const noexcept
	{
		return _rightEntity;
	}

	NavigationDrawerDestination &NavigationDrawerDestination::setRightEntity(
		ecs::Entity::Identifier identifier)
	{
		if (_rightEntity != identifier) {
			_rightEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	const NavigationDrawerDestination::PressHandler &
		NavigationDrawerDestination::getOnPress(void) const noexcept
	{
		return _onPress;
	}

	NavigationDrawerDestination &
		NavigationDrawerDestination::setOnPress(PressHandler handler)
	{
		_onPress = std::move(handler);
		return *this;
	}
}	 // namespace guillaume::components
