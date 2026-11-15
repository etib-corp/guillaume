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

#include "guillaume/components/floating_action_button_menu.hpp"

#include <utility>

namespace guillaume::components
{
	const std::string &
		FloatingActionButtonMenu::getIconGlyphName(void) const noexcept
	{
		return _iconGlyphName;
	}

	FloatingActionButtonMenu &
		FloatingActionButtonMenu::setIconGlyphName(const std::string &name)
	{
		if (_iconGlyphName != name) {
			_iconGlyphName = name;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<FloatingActionButtonMenu::Action> &
		FloatingActionButtonMenu::getActions(void) const noexcept
	{
		return _actions;
	}

	FloatingActionButtonMenu &
		FloatingActionButtonMenu::setActions(std::vector<Action> actions)
	{
		// Actions carry callbacks, which cannot be compared.
		_actions = std::move(actions);
		return *this;
	}

	const std::string &
		FloatingActionButtonMenu::getAccessibilityLabel(void) const noexcept
	{
		return _accessibilityLabel;
	}

	FloatingActionButtonMenu &FloatingActionButtonMenu::setAccessibilityLabel(
		const std::string &label)
	{
		if (_accessibilityLabel != label) {
			_accessibilityLabel = label;
			setHasChanged(true);
		}
		return *this;
	}

	bool FloatingActionButtonMenu::isOpen(void) const noexcept
	{
		return _open;
	}

	FloatingActionButtonMenu &FloatingActionButtonMenu::setOpen(bool open)
	{
		if (_open != open) {
			_open = open;
			setHasChanged(true);
		}
		return *this;
	}

	const FloatingActionButtonMenu::StateChangeHandler &
		FloatingActionButtonMenu::getOnStateChange(void) const noexcept
	{
		return _onStateChange;
	}

	FloatingActionButtonMenu &
		FloatingActionButtonMenu::setOnStateChange(StateChangeHandler handler)
	{
		_onStateChange = std::move(handler);
		return *this;
	}

	ecs::Entity::Identifier
		FloatingActionButtonMenu::getTrigger(void) const noexcept
	{
		return _trigger;
	}

	FloatingActionButtonMenu &
		FloatingActionButtonMenu::setTrigger(ecs::Entity::Identifier identifier)
	{
		if (_trigger != identifier) {
			_trigger = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<ecs::Entity::Identifier> &
		FloatingActionButtonMenu::getActionButtons(void) const noexcept
	{
		return _actionButtons;
	}

	FloatingActionButtonMenu &FloatingActionButtonMenu::setActionButtons(
		const std::vector<ecs::Entity::Identifier> &buttons)
	{
		if (_actionButtons != buttons) {
			_actionButtons = buttons;
			setHasChanged(true);
		}
		return *this;
	}
}	 // namespace guillaume::components
