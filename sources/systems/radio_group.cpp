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

#include "guillaume/systems/radio_group.hpp"

#include <stdexcept>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/radio_button.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/systems/radio_button.hpp"

namespace guillaume::systems
{
	RadioGroup::RadioGroup(void)
		: ecs::SystemFiller<components::RadioGroup>(ecs::Phase::Layout)
	{
	}

	void RadioGroup::install(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		if (!componentRegistry.hasComponent<components::Accessibility>(
				identifier)) {
			componentRegistry.addComponent<components::Accessibility>(
				identifier);
		}
		componentRegistry.getComponent<components::Accessibility>(identifier)
			.setRole(components::AccessibilityRole::RadioGroup)
			.setFocusable(false);

		const auto &buttons =
			componentRegistry.getComponent<components::RadioGroup>(identifier)
				.getButtons();
		for (std::size_t index = 0; index < buttons.size(); ++index) {
			if (!componentRegistry.hasComponent<components::RadioButton>(
					buttons[index])) {
				continue;
			}
			componentRegistry
				.getComponent<components::RadioButton>(buttons[index])
				.setOnPress([registry, identifier, index]() {
					press(*registry, identifier, index);
				});
		}
	}

	void RadioGroup::select(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							std::size_t index)
	{
		auto &group = componentRegistry.getComponent<components::RadioGroup>(
			entityIdentifier);

		if (index >= group.getOptions().size()) {
			throw std::out_of_range("RadioGroup option index out of range");
		}
		group.setValue(index);
		apply(componentRegistry, entityIdentifier);
	}

	void RadioGroup::press(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   std::size_t index)
	{
		auto &group = componentRegistry.getComponent<components::RadioGroup>(
			entityIdentifier);

		if (index >= group.getButtons().size()) {
			return;
		}

		const bool changed =
			!group.getValue().has_value() || *group.getValue() != index;
		group.setValue(index);
		apply(componentRegistry, entityIdentifier);
		// Copy: the callback may replace itself.
		const auto onValueChange = group.getOnValueChange();
		if (changed && onValueChange) {
			onValueChange(index);
		}
	}

	void RadioGroup::apply(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &group =
			componentRegistry.getComponent<components::RadioGroup>(
				entityIdentifier);
		const auto value	= group.getValue();
		const auto &buttons = group.getButtons();

		for (std::size_t index = 0; index < buttons.size(); ++index) {
			if (!componentRegistry.hasComponent<components::Selectable>(
					buttons[index])
				|| !componentRegistry.hasComponent<components::RadioButton>(
					buttons[index])) {
				continue;
			}

			const bool selected = value.has_value() && *value == index;
			if (componentRegistry
					.getComponent<components::Selectable>(buttons[index])
					.isSelected()
				!= selected) {
				RadioButton::select(componentRegistry, buttons[index],
									selected);
			}
		}

		if (!componentRegistry.hasComponent<components::Layout>(
				entityIdentifier)) {
			return;
		}

		// The radio buttons are the children the Layout system stacks.
		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);
		layout.setChildren(buttons);
		Layout::apply(componentRegistry, entityIdentifier, layout.getChildren(),
					  Layout::layerOf(componentRegistry, entityIdentifier));
	}

	void RadioGroup::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::RadioGroup>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
