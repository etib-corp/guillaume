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

#pragma once

#include <vector>

#include <gtest/gtest.h>

#include <guillaume/components/accessibility.hpp>
#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/button.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/floating_action_button.hpp>
#include <guillaume/components/floating_action_button_menu.hpp>
#include <guillaume/components/glyph.hpp>
#include <guillaume/components/hand_button_interaction.hpp>
#include <guillaume/components/hand_hover_interaction.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/mouse_button_interaction.hpp>
#include <guillaume/components/mouse_hover_interaction.hpp>
#include <guillaume/components/style_state.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/button.hpp>
#include <guillaume/systems/floating_action_button.hpp>
#include <guillaume/systems/floating_action_button_menu.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::FloatingActionButtonMenu on bare entity
	 * identifiers: a menu, its trigger and two action buttons with their icon
	 * child, without any entity object.
	 */
	class TestFloatingActionButtonMenuSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;				///< Component storage.
		ecs::Entity::Identifier _entity { 1 };			///< Menu under test.
		ecs::Entity::Identifier _trigger { 10 };		///< Its trigger.
		ecs::Entity::Identifier _triggerIcon { 11 };	///< The trigger icon.
		std::vector<ecs::Entity::Identifier> _actions {
			20, 30
		};	  ///< Its action buttons.
		std::vector<int> _clicks { 0, 0 };	  ///< Clicks per action.
		guillaume::systems::FloatingActionButtonMenu
			_system;	///< System under test.

		void SetUp(void) override
		{
			_registry.addComponent<components::Layout>(_entity);
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry
				.addComponent<components::FloatingActionButtonMenu>(_entity)
				.setIconGlyphName("add")
				.setAccessibilityLabel("Create")
				.setActions({
					{ "", "edit",
					  [this]() {
						  ++_clicks[0];
					  } },
					{ "", "share",
					  [this]() {
						  ++_clicks[1];
					  } },
				})
				.setTrigger(_trigger)
				.setActionButtons(_actions);

			addButton(_trigger, _triggerIcon);
			for (const auto action: _actions) {
				addButton(action, action + 1);
			}

			guillaume::systems::FloatingActionButtonMenu::install(_registry,
																  _entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Give an identifier the components of a floating action
		 * button.
		 * @param button The button identifier.
		 * @param icon The identifier of its icon child.
		 */
		void addButton(ecs::Entity::Identifier button,
					   ecs::Entity::Identifier icon)
		{
			_registry.addComponent<components::Transform>(button);
			_registry.addComponent<components::Bound>(button);
			_registry.addComponent<components::Color>(button);
			_registry.addComponent<components::Borders>(button);
			_registry.addComponent<components::HandButtonInteraction>(button);
			_registry.addComponent<components::HandHoverInteraction>(button);
			_registry.addComponent<components::MouseHoverInteraction>(button);
			_registry.addComponent<components::MouseButtonInteraction>(button);
			_registry.addComponent<components::StyleState>(button);
			_registry.addComponent<components::Accessibility>(button);
			_registry.addComponent<components::Hierarchy>(button);
			_registry.addComponent<components::Button>(button).setIcon(icon);

			_registry.addComponent<components::Transform>(icon);
			_registry.addComponent<components::Bound>(icon)
				.setWidth(18.0f)
				.setHeight(18.0f);
			_registry.addComponent<components::Color>(icon);
			_registry.addComponent<components::Glyph>(icon);

			_registry.addComponent<components::FloatingActionButton>(button);
			guillaume::systems::FloatingActionButton::install(_registry,
															  button);
			guillaume::systems::FloatingActionButton::apply(_registry, button);
		}

		/**
		 * @brief Access the menu state.
		 * @return The components::FloatingActionButtonMenu under test.
		 */
		components::FloatingActionButtonMenu &menu(void)
		{
			return _registry.getComponent<components::FloatingActionButtonMenu>(
				_entity);
		}

		/**
		 * @brief Children the Layout system arranges for the menu.
		 * @return The layout child identifiers.
		 */
		const std::vector<ecs::Entity::Identifier> &layoutChildren(void)
		{
			return _registry.getComponent<components::Layout>(_entity)
				.getChildren();
		}

		/**
		 * @brief Vertical position of a child.
		 * @param child The child identifier.
		 * @return Its position along y.
		 */
		float positionY(ecs::Entity::Identifier child)
		{
			return _registry.getComponent<components::Transform>(child)
				.getPose()
				.getPosition()
				.getY();
		}
	};

}	 // namespace guillaume::systems::tests
