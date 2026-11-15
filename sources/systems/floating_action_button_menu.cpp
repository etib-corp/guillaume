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

#include "guillaume/systems/floating_action_button_menu.hpp"

#include <string>
#include <vector>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/button.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/systems/button.hpp"
#include "guillaume/systems/layout.hpp"

namespace guillaume::systems
{
	namespace
	{
		/**
		 * @brief Whether a menu child is a button this system can drive.
		 * @param registry The registry holding the components.
		 * @param button The child identifier (may be invalid).
		 * @return True when it has a components::Button.
		 */
		bool isButton(const ecs::ComponentRegistry &registry,
					  ecs::Entity::Identifier button)
		{
			return registry.hasComponent<components::Button>(button);
		}
	}	 // namespace

	FloatingActionButtonMenu::FloatingActionButtonMenu(void)
		: ecs::SystemFiller<components::FloatingActionButtonMenu>(
			  ecs::Phase::Layout)
	{
	}

	void FloatingActionButtonMenu::install(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		componentRegistry.getComponent<components::Layout>(identifier)
			.setAxis(components::Layout::Axis::Vertical)
			.setMainAxisAlignment(components::Layout::MainAxisAlignment::End)
			.setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Center)
			.setSpacing(components::FloatingActionButtonMenu::Spacing)
			.setPadding(0.0f);

		const auto &menu =
			componentRegistry
				.getComponent<components::FloatingActionButtonMenu>(identifier);
		const auto trigger = menu.getTrigger();

		if (isButton(componentRegistry, trigger)) {
			componentRegistry.getComponent<components::Button>(trigger)
				.setOnClick([registry, identifier]() {
					toggle(*registry, identifier);
				});
		}
		if (componentRegistry.hasComponent<components::Accessibility>(
				trigger)) {
			componentRegistry.getComponent<components::Accessibility>(trigger)
				.setLabel(menu.getAccessibilityLabel());
		}

		const auto &actions = menu.getActions();
		const auto &buttons = menu.getActionButtons();
		for (std::size_t index = 0;
			 index < buttons.size() && index < actions.size(); ++index) {
			if (isButton(componentRegistry, buttons[index])) {
				componentRegistry
					.getComponent<components::Button>(buttons[index])
					.setOnClick(actions[index].onClick);
			}
		}
	}

	void FloatingActionButtonMenu::setOpen(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier, bool open)
	{
		auto &menu = componentRegistry
						 .getComponent<components::FloatingActionButtonMenu>(
							 entityIdentifier);

		if (menu.isOpen() == open) {
			return;
		}
		menu.setOpen(open);
		apply(componentRegistry, entityIdentifier);

		// Copy: the callback may replace itself.
		const auto onStateChange = menu.getOnStateChange();
		if (onStateChange) {
			onStateChange(open);
		}
	}

	void FloatingActionButtonMenu::open(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		setOpen(componentRegistry, entityIdentifier, true);
	}

	void FloatingActionButtonMenu::close(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		setOpen(componentRegistry, entityIdentifier, false);
	}

	void FloatingActionButtonMenu::toggle(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		setOpen(componentRegistry, entityIdentifier,
				!componentRegistry
					 .getComponent<components::FloatingActionButtonMenu>(
						 entityIdentifier)
					 .isOpen());
	}

	void FloatingActionButtonMenu::apply(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &menu =
			componentRegistry
				.getComponent<components::FloatingActionButtonMenu>(
					entityIdentifier);
		const bool open	   = menu.isOpen();
		const auto trigger = menu.getTrigger();
		std::vector<ecs::Entity::Identifier> children;

		// Closed actions are hidden and ignore activation, so they cannot be
		// clicked through while the menu is collapsed.
		for (const auto action: menu.getActionButtons()) {
			if (!isButton(componentRegistry, action)) {
				continue;
			}

			auto &button =
				componentRegistry.getComponent<components::Button>(action);
			if (componentRegistry.hasComponent<components::Accessibility>(
					action)) {
				componentRegistry
					.getComponent<components::Accessibility>(action)
					.setFocusable(open);
			}
			if (button.isVisible() != open) {
				button.setVisible(open);
				Button::restyle(componentRegistry, action);
			}
			if (open) {
				children.push_back(action);
			}
		}

		if (isButton(componentRegistry, trigger)) {
			auto &button =
				componentRegistry.getComponent<components::Button>(trigger);
			const std::string icon =
				open ? std::string(CloseGlyph) : menu.getIconGlyphName();

			componentRegistry.getComponent<components::Accessibility>(trigger)
				.setExpanded(open);
			if (button.getIconGlyphName() != icon) {
				button.setIconGlyphName(icon);
				Button::syncContent(componentRegistry, trigger);
			}
			children.push_back(trigger);
		}

		// The open actions sit above the trigger; closed ones are not laid
		// out at all.
		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);
		layout.setChildren(children);
		Layout::apply(componentRegistry, entityIdentifier, layout.getChildren(),
					  Layout::layerOf(componentRegistry, entityIdentifier));
	}

	void FloatingActionButtonMenu::update(
		const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::FloatingActionButtonMenu>(
				entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
