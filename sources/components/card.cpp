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

#include "guillaume/components/card.hpp"

#include <utility>

namespace guillaume::components
{
	Card::Variant Card::getVariant(void) const noexcept
	{
		return _variant;
	}

	Card &Card::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	bool Card::isInteractive(void) const noexcept
	{
		return _interactive;
	}

	Card &Card::setInteractive(bool interactive)
	{
		if (_interactive != interactive) {
			_interactive = interactive;
			setHasChanged(true);
		}
		return *this;
	}

	bool Card::isPressed(void) const noexcept
	{
		return _pressed;
	}

	Card &Card::setPressed(bool pressed)
	{
		if (_pressed != pressed) {
			_pressed = pressed;
			setHasChanged(true);
		}
		return *this;
	}

	float Card::getPressProgress(void) const noexcept
	{
		return _pressProgress;
	}

	Card &Card::setPressProgress(float pressProgress)
	{
		if (_pressProgress != pressProgress) {
			_pressProgress = pressProgress;
			setHasChanged(true);
		}
		return *this;
	}

	float Card::getPressFrom(void) const noexcept
	{
		return _pressFrom;
	}

	Card &Card::setPressFrom(float pressFrom)
	{
		if (_pressFrom != pressFrom) {
			_pressFrom = pressFrom;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Card::getCoverPath(void) const noexcept
	{
		return _coverPath;
	}

	Card &Card::setCoverPath(const std::string &coverPath)
	{
		if (_coverPath != coverPath) {
			_coverPath = coverPath;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Card::getTitle(void) const noexcept
	{
		return _title;
	}

	Card &Card::setTitle(const std::string &title)
	{
		if (_title != title) {
			_title = title;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Card::getSubtitle(void) const noexcept
	{
		return _subtitle;
	}

	Card &Card::setSubtitle(const std::string &subtitle)
	{
		if (_subtitle != subtitle) {
			_subtitle = subtitle;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Card::getContentText(void) const noexcept
	{
		return _contentText;
	}

	Card &Card::setContentText(const std::string &contentText)
	{
		if (_contentText != contentText) {
			_contentText = contentText;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Card::getCover(void) const noexcept
	{
		return _cover;
	}

	Card &Card::setCover(ecs::Entity::Identifier identifier)
	{
		if (_cover != identifier) {
			_cover = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Card::getTitleSection(void) const noexcept
	{
		return _titleSection;
	}

	Card &Card::setTitleSection(ecs::Entity::Identifier identifier)
	{
		if (_titleSection != identifier) {
			_titleSection = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Card::getTitleText(void) const noexcept
	{
		return _titleText;
	}

	Card &Card::setTitleText(ecs::Entity::Identifier identifier)
	{
		if (_titleText != identifier) {
			_titleText = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Card::getSubtitleText(void) const noexcept
	{
		return _subtitleText;
	}

	Card &Card::setSubtitleText(ecs::Entity::Identifier identifier)
	{
		if (_subtitleText != identifier) {
			_subtitleText = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Card::getContentEntity(void) const noexcept
	{
		return _contentEntity;
	}

	Card &Card::setContentEntity(ecs::Entity::Identifier identifier)
	{
		if (_contentEntity != identifier) {
			_contentEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Card::getActionsSection(void) const noexcept
	{
		return _actionsSection;
	}

	Card &Card::setActionsSection(ecs::Entity::Identifier identifier)
	{
		if (_actionsSection != identifier) {
			_actionsSection = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<Card::Action> &Card::getActions(void) const noexcept
	{
		return _actions;
	}

	Card &Card::addAction(Action action)
	{
		_actions.push_back(std::move(action));
		setHasChanged(true);
		return *this;
	}

	const Card::Identifiers &Card::getActionButtons(void) const noexcept
	{
		return _actionButtons;
	}

	Card &Card::setActionButtons(const Identifiers &identifiers)
	{
		if (_actionButtons != identifiers) {
			_actionButtons = identifiers;
			setHasChanged(true);
		}
		return *this;
	}

	const Card::Handler &Card::getOnPress(void) const noexcept
	{
		return _onPress;
	}

	Card &Card::setOnPress(Handler handler)
	{
		_onPress = std::move(handler);
		return *this;
	}
}	 // namespace guillaume::components
