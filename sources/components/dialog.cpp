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

#include "guillaume/components/dialog.hpp"

#include <algorithm>
#include <utility>

namespace guillaume::components
{
	Dialog::Variant Dialog::getVariant(void) const noexcept
	{
		return _variant;
	}

	Dialog &Dialog::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Dialog::getTitle(void) const noexcept
	{
		return _title;
	}

	Dialog &Dialog::setTitle(const std::string &title)
	{
		if (_title != title) {
			_title = title;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Dialog::getMessage(void) const noexcept
	{
		return _message;
	}

	Dialog &Dialog::setMessage(const std::string &message)
	{
		if (_message != message) {
			_message = message;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<Dialog::Action> &Dialog::getActions(void) const noexcept
	{
		return _actions;
	}

	Dialog &Dialog::setActions(std::vector<Action> actions)
	{
		const bool same = std::equal(
			_actions.begin(), _actions.end(), actions.begin(), actions.end(),
			[](const Action &left, const Action &right) {
				return left.label == right.label;
			});

		_actions = std::move(actions);
		if (!same) {
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Dialog::getIcon(void) const noexcept
	{
		return _icon;
	}

	Dialog &Dialog::setIcon(const std::string &icon)
	{
		if (_icon != icon) {
			_icon = icon;
			setHasChanged(true);
		}
		return *this;
	}

	std::string Dialog::getEffectiveIcon(void) const
	{
		if (!_icon.empty()) {
			return _icon;
		}
		return _variant == Variant::Confirmation ? std::string("info")
												 : std::string();
	}

	ecs::Entity::Identifier Dialog::getIconEntity(void) const noexcept
	{
		return _iconEntity;
	}

	Dialog &Dialog::setIconEntity(ecs::Entity::Identifier identifier)
	{
		if (_iconEntity != identifier) {
			_iconEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Dialog::getTitleEntity(void) const noexcept
	{
		return _titleEntity;
	}

	Dialog &Dialog::setTitleEntity(ecs::Entity::Identifier identifier)
	{
		if (_titleEntity != identifier) {
			_titleEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Dialog::getMessageEntity(void) const noexcept
	{
		return _messageEntity;
	}

	Dialog &Dialog::setMessageEntity(ecs::Entity::Identifier identifier)
	{
		if (_messageEntity != identifier) {
			_messageEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<ecs::Entity::Identifier> &
		Dialog::getActionEntities(void) const noexcept
	{
		return _actionEntities;
	}

	Dialog &Dialog::setActionEntities(
		const std::vector<ecs::Entity::Identifier> &identifiers)
	{
		if (_actionEntities != identifiers) {
			_actionEntities = identifiers;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Dialog::getActionRow(void) const noexcept
	{
		return _actionRow;
	}

	Dialog &Dialog::setActionRow(ecs::Entity::Identifier identifier)
	{
		if (_actionRow != identifier) {
			_actionRow = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Dialog::getBackdrop(void) const noexcept
	{
		return _backdrop;
	}

	Dialog &Dialog::setBackdrop(ecs::Entity::Identifier identifier)
	{
		if (_backdrop != identifier) {
			_backdrop = identifier;
			setHasChanged(true);
		}
		return *this;
	}
}	 // namespace guillaume::components
