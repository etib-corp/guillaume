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

#include <memory>
#include <utility>

#include "guillaume/entities/floating_action_button_menu.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/floating_action_button.hpp"
#include "guillaume/systems/layout.hpp"

namespace guillaume::entities
{
	FloatingActionButtonMenu::Builder::Builder(
		ecs::ComponentRegistry &componentRegistry,
		ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<FloatingActionButtonMenu>(componentRegistry,
													  entityRegistry)
	{
		reset();
	}

	std::shared_ptr<FloatingActionButtonMenu>
		FloatingActionButtonMenu::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<FloatingActionButtonMenu>(
			this->getComponentRegistry(), _iconGlyphName, _actions,
			_accessibilityLabel);
		return entity;
	}

	void FloatingActionButtonMenu::Builder::reset(void)
	{
		_iconGlyphName.clear();
		_actions.clear();
		_accessibilityLabel.clear();
	}

	FloatingActionButtonMenu::Builder &
		FloatingActionButtonMenu::Builder::withIcon(
			const std::string &iconGlyphName)
	{
		_iconGlyphName = iconGlyphName;
		return *this;
	}

	FloatingActionButtonMenu::Builder &
		FloatingActionButtonMenu::Builder::addAction(
			const std::string &iconGlyphName, std::function<void(void)> onClick)
	{
		_actions.push_back(Action { "", iconGlyphName, std::move(onClick) });
		return *this;
	}

	FloatingActionButtonMenu::Builder &
		FloatingActionButtonMenu::Builder::withAccessibilityLabel(
			const std::string &accessibilityLabel)
	{
		_accessibilityLabel = accessibilityLabel;
		return *this;
	}

	std::shared_ptr<FloatingActionButtonMenu>
		FloatingActionButtonMenu::Director::makeFloatingActionButtonMenu(
			Builder &builder, std::shared_ptr<Entity> parent,
			const std::string &iconGlyphName)
	{
		return builder.withIcon(iconGlyphName).registerEntity(parent);
	}

	FloatingActionButtonMenu::FloatingActionButtonMenu(
		ecs::ComponentRegistry &registry, const std::string &iconGlyphName,
		const std::vector<Action> &actions,
		const std::string &accessibilityLabel)
		: ecs::ParentEntityFiller<components::Transform, components::Bound,
								  components::Layout>(registry)
		, _iconGlyphName(iconGlyphName)
		, _actions(actions)
		, _accessibilityLabel(accessibilityLabel)
		, _isOpen(false)
		, _trigger()
		, _actionButtons()
	{
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setAxis(components::Layout::Axis::Vertical)
			.setMainAxisAlignment(components::Layout::MainAxisAlignment::End)
			.setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Center)
			.setSpacing(16.0f)
			.setPadding(0.0f);
	}

	FloatingActionButtonMenu::~FloatingActionButtonMenu(void)
	{
	}

	FloatingActionButtonMenu &FloatingActionButtonMenu::open(void)
	{
		_isOpen = true;
		return *this;
	}

	FloatingActionButtonMenu &FloatingActionButtonMenu::close(void)
	{
		_isOpen = false;
		return *this;
	}

	FloatingActionButtonMenu &FloatingActionButtonMenu::toggle(void)
	{
		_isOpen = !_isOpen;
		return *this;
	}

	bool FloatingActionButtonMenu::isOpen(void) const
	{
		return _isOpen;
	}

	std::size_t FloatingActionButtonMenu::getActionCount(void) const
	{
		return _actionButtons.size();
	}

	ecs::Entity::Identifier
		FloatingActionButtonMenu::getTriggerIdentifier(void) const
	{
		return _trigger != nullptr ? _trigger->getIdentifier()
								   : ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier
		FloatingActionButtonMenu::getActionIdentifier(std::size_t index) const
	{
		if (index >= _actionButtons.size()) {
			return ecs::Entity::InvalidIdentifier;
		}

		return _actionButtons[index]->getIdentifier();
	}

	void FloatingActionButtonMenu::triggerHandler(void)
	{
		toggle();
		layoutMenu();
	}

	void FloatingActionButtonMenu::layoutMenu(void)
	{
		std::vector<ecs::Entity::Identifier> childIdentifiers;

		if (_isOpen) {
			for (const auto &button: _actionButtons) {
				childIdentifiers.push_back(button->getIdentifier());
			}
		}

		if (_trigger != nullptr) {
			childIdentifiers.push_back(_trigger->getIdentifier());
		}

		systems::Layout::apply(getComponentRegistry(), getIdentifier(),
							   childIdentifiers,
							   static_cast<std::uint32_t>(getLayer()));
	}

	void FloatingActionButtonMenu::initialize(void)
	{
		FloatingActionButton::Builder triggerBuilder(getComponentRegistry(),
													 *this);
		FloatingActionButton::Director triggerDirector;

		_trigger = triggerDirector.makeFloatingActionButton(
			triggerBuilder, shared_from_this(), _iconGlyphName,
			std::bind(&FloatingActionButtonMenu::triggerHandler, this),
			FloatingActionButton::Variant::Surface,
			FloatingActionButton::Size::Medium);

		FloatingActionButton::Builder actionBuilder(getComponentRegistry(),
													*this);
		FloatingActionButton::Director actionDirector;

		for (const auto &action: _actions) {
			auto button = actionDirector.makeFloatingActionButton(
				actionBuilder, shared_from_this(), action.iconGlyphName,
				action.onClick, FloatingActionButton::Variant::Surface,
				FloatingActionButton::Size::Small);

			_actionButtons.push_back(button);
		}
	}

	void FloatingActionButtonMenu::update(void)
	{
		_trigger->update();

		for (auto &button: _actionButtons) {
			button->update();
		}

		layoutMenu();
	}
}	 // namespace guillaume::entities
