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
#include "guillaume/entities/extended_floating_action_button.hpp"
#include "guillaume/entities/floating_action_button.hpp"
#include "guillaume/systems/floating_action_button_menu.hpp"

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
		auto entity = FloatingActionButtonMenu::create(
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
		FloatingActionButtonMenu::Builder::addAction(
			const std::string &iconGlyphName, const std::string &label,
			std::function<void(void)> onClick)
	{
		_actions.push_back(Action { label, iconGlyphName, std::move(onClick) });
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
		FloatingActionButtonMenu::Builder::makeFloatingActionButtonMenu(
			std::shared_ptr<Entity> parent, const std::string &iconGlyphName)
	{
		return withIcon(iconGlyphName).registerEntity(parent);
	}

	FloatingActionButtonMenu::FloatingActionButtonMenu(
		ecs::ComponentRegistry &registry, const std::string &iconGlyphName,
		const std::vector<Action> &actions,
		const std::string &accessibilityLabel)
		: ecs::ParentEntityFiller<components::Transform, components::Bound,
								  components::Layout,
								  components::FloatingActionButtonMenu>(
			  registry)
	{
		state()
			.setIconGlyphName(iconGlyphName)
			.setActions(actions)
			.setAccessibilityLabel(accessibilityLabel);
		systems::FloatingActionButtonMenu::install(getComponentRegistry(),
												   getIdentifier());
	}

	FloatingActionButtonMenu::~FloatingActionButtonMenu(void)
	{
	}

	components::FloatingActionButtonMenu &FloatingActionButtonMenu::state(void)
	{
		return getComponentRegistry()
			.getComponent<components::FloatingActionButtonMenu>(
				getIdentifier());
	}

	const components::FloatingActionButtonMenu &
		FloatingActionButtonMenu::state(void) const
	{
		return getComponentRegistry()
			.getComponent<components::FloatingActionButtonMenu>(
				getIdentifier());
	}

	FloatingActionButtonMenu &FloatingActionButtonMenu::open(void)
	{
		systems::FloatingActionButtonMenu::open(getComponentRegistry(),
												getIdentifier());
		return *this;
	}

	FloatingActionButtonMenu &FloatingActionButtonMenu::close(void)
	{
		systems::FloatingActionButtonMenu::close(getComponentRegistry(),
												 getIdentifier());
		return *this;
	}

	FloatingActionButtonMenu &FloatingActionButtonMenu::toggle(void)
	{
		systems::FloatingActionButtonMenu::toggle(getComponentRegistry(),
												  getIdentifier());
		return *this;
	}

	FloatingActionButtonMenu &FloatingActionButtonMenu::setOnStateChange(
		std::function<void(bool)> onStateChange)
	{
		state().setOnStateChange(std::move(onStateChange));
		return *this;
	}

	bool FloatingActionButtonMenu::isOpen(void) const
	{
		return state().isOpen();
	}

	std::size_t FloatingActionButtonMenu::getActionCount(void) const
	{
		return state().getActionButtons().size();
	}

	ecs::Entity::Identifier
		FloatingActionButtonMenu::getTriggerIdentifier(void) const
	{
		return state().getTrigger();
	}

	ecs::Entity::Identifier
		FloatingActionButtonMenu::getActionIdentifier(std::size_t index) const
	{
		const auto &buttons = state().getActionButtons();

		return index < buttons.size() ? buttons[index]
									  : ecs::Entity::InvalidIdentifier;
	}

	void FloatingActionButtonMenu::build(void)
	{
		auto &registry = getComponentRegistry();
		// Copies: building children may move the component storage.
		const std::string iconGlyphName	  = state().getIconGlyphName();
		const std::vector<Action> actions = state().getActions();

		// systems::FloatingActionButtonMenu::install wires every click.
		FloatingActionButton::Builder triggerBuilder(registry, *this);
		const auto trigger = triggerBuilder.makeFloatingActionButton(
			shared_from_this(), iconGlyphName, std::function<void(void)>(),
			FloatingActionButton::Variant::Primary,
			FloatingActionButton::Size::Medium);

		FloatingActionButton::Builder actionBuilder(registry, *this);
		ExtendedFloatingActionButton::Builder labelledBuilder(registry, *this);
		std::vector<ecs::Entity::Identifier> identifiers;

		for (const auto &action: actions) {
			std::shared_ptr<ButtonBase> button;

			if (action.label.empty()) {
				button = actionBuilder.makeFloatingActionButton(
					shared_from_this(), action.iconGlyphName,
					std::function<void(void)>(),
					FloatingActionButton::Variant::Surface,
					FloatingActionButton::Size::Small);
			} else {
				// A labelled action shows its label beside the icon.
				button =
					labelledBuilder.withIcon(action.iconGlyphName)
						.withLabel(action.label)
						.withVariant(
							ExtendedFloatingActionButton::Variant::Surface)
						.withSize(ExtendedFloatingActionButton::Size::Small)
						.withAccessibilityLabel(action.label)
						.registerEntity(shared_from_this());
			}

			identifiers.push_back(button->getIdentifier());
		}

		state()
			.setTrigger(trigger->getIdentifier())
			.setActionButtons(identifiers);
		systems::FloatingActionButtonMenu::install(registry, getIdentifier());
		systems::FloatingActionButtonMenu::apply(registry, getIdentifier());
	}
}	 // namespace guillaume::entities
