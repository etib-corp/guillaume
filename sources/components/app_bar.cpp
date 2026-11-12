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

#include "guillaume/components/app_bar.hpp"

#include <utility>

namespace guillaume::components
{
	AppBar::Variant AppBar::getVariant(void) const noexcept
	{
		return _variant;
	}

	AppBar &AppBar::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &AppBar::getTitle(void) const noexcept
	{
		return _title;
	}

	AppBar &AppBar::setTitle(const std::string &title)
	{
		if (_title != title) {
			_title = title;
			setHasChanged(true);
		}
		return *this;
	}

	const AppBar::Action &AppBar::getNavigation(void) const noexcept
	{
		return _navigation;
	}

	AppBar &AppBar::setNavigation(Action navigation)
	{
		// An action carries a callback, which cannot be compared.
		_navigation = std::move(navigation);
		setHasChanged(true);
		return *this;
	}

	const std::vector<AppBar::Action> &AppBar::getActions(void) const noexcept
	{
		return _actions;
	}

	AppBar &AppBar::setActions(std::vector<Action> actions)
	{
		// An action carries a callback, which cannot be compared.
		_actions = std::move(actions);
		setHasChanged(true);
		return *this;
	}

	bool AppBar::isElevated(void) const noexcept
	{
		return _elevated;
	}

	AppBar &AppBar::setElevated(bool elevated)
	{
		if (_elevated != elevated) {
			_elevated = elevated;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier AppBar::getNavigationButton(void) const noexcept
	{
		return _navigationButton;
	}

	AppBar &AppBar::setNavigationButton(ecs::Entity::Identifier identifier)
	{
		if (_navigationButton != identifier) {
			_navigationButton = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier AppBar::getTitleEntity(void) const noexcept
	{
		return _titleEntity;
	}

	AppBar &AppBar::setTitleEntity(ecs::Entity::Identifier identifier)
	{
		if (_titleEntity != identifier) {
			_titleEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	const AppBar::Identifiers &AppBar::getActionButtons(void) const noexcept
	{
		return _actionButtons;
	}

	AppBar &AppBar::setActionButtons(const Identifiers &identifiers)
	{
		if (_actionButtons != identifiers) {
			_actionButtons = identifiers;
			setHasChanged(true);
		}
		return *this;
	}
}	 // namespace guillaume::components
