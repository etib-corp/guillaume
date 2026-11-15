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
#include <guillaume/components/color.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/mouse_button_interaction.hpp>
#include <guillaume/components/navigation_drawer_destination.hpp>
#include <guillaume/components/selection.hpp>
#include <guillaume/components/style_state.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/navigation_drawer_destination.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::NavigationDrawerDestination on bare
	 * entity identifiers: a destination with its lead group, icon, label
	 * and trailing text children, without any entity object.
	 */
	class TestNavigationDrawerDestinationSystem: public ::testing::Test
	{
		public:
		/**
		 * @brief Give identifiers the components of a navigation drawer
		 * destination and of its children, and install the destination.
		 *
		 * The lead group holds the icon and the label, as in a regular
		 * (not collapsed) item.
		 * @param registry The registry receiving the components.
		 * @param destination The destination identifier.
		 * @param lead The identifier of its lead group.
		 * @param icon The identifier of its icon.
		 * @param label The identifier of its label.
		 * @param right The identifier of its trailing text, or
		 * InvalidIdentifier for none.
		 */
		static void addDestination(ecs::ComponentRegistry &registry,
								   ecs::Entity::Identifier destination,
								   ecs::Entity::Identifier lead,
								   ecs::Entity::Identifier icon,
								   ecs::Entity::Identifier label,
								   ecs::Entity::Identifier right)
		{
			registry.addComponent<components::Layout>(destination);
			registry.addComponent<components::Transform>(destination);
			registry.addComponent<components::Bound>(destination);
			registry.addComponent<components::Color>(destination);
			registry.addComponent<components::Borders>(destination);
			registry.addComponent<components::Hierarchy>(destination);
			registry.addComponent<components::Selectable>(destination);
			registry
				.addComponent<components::NavigationDrawerDestination>(
					destination)
				.setLead(lead)
				.setIconEntity(icon)
				.setLabelEntity(label)
				.setRightEntity(right);

			registry.addComponent<components::Transform>(lead);
			registry.addComponent<components::Bound>(lead);
			registry.addComponent<components::Color>(lead);
			registry.addComponent<components::Hierarchy>(lead);
			registry.addComponent<components::Layout>(lead)
				.setSpacing(12.0f)
				.setChildren({ icon, label });

			addContent(registry, icon, 24.0f, 24.0f);
			addContent(registry, label, 40.0f, 20.0f);
			if (right != ecs::Entity::InvalidIdentifier) {
				addContent(registry, right, 16.0f, 20.0f);
			}

			guillaume::systems::NavigationDrawerDestination::install(
				registry, destination);
		}

		/**
		 * @brief Give an identifier the components of a measured leaf child.
		 * @param registry The registry receiving the components.
		 * @param identifier The child identifier.
		 * @param width Its measured width.
		 * @param height Its measured height.
		 */
		static void addContent(ecs::ComponentRegistry &registry,
							   ecs::Entity::Identifier identifier, float width,
							   float height)
		{
			registry.addComponent<components::Transform>(identifier);
			registry.addComponent<components::Bound>(identifier)
				.setWidth(width)
				.setHeight(height);
			registry.addComponent<components::Color>(identifier);
		}

		/**
		 * @brief Release the left mouse button on an entity.
		 * @param registry The registry holding the components.
		 * @param identifier The entity.
		 */
		static void click(ecs::ComponentRegistry &registry,
						  ecs::Entity::Identifier identifier)
		{
			registry
				.getComponent<components::MouseButtonInteraction>(identifier)
				.getOnButtonReleaseHandler(
					utility::event::MouseButtonEvent::Button::Left)();
		}

		protected:
		ecs::ComponentRegistry _registry;		  ///< Component storage.
		ecs::Entity::Identifier _entity { 1 };	  ///< Destination under test.
		ecs::Entity::Identifier _lead { 2 };	  ///< Its lead group.
		ecs::Entity::Identifier _icon { 3 };	  ///< Its icon child.
		ecs::Entity::Identifier _label { 4 };	  ///< Its label child.
		ecs::Entity::Identifier _right { 5 };	  ///< Its trailing text.
		guillaume::systems::NavigationDrawerDestination
			_system;	///< System under test.

		void SetUp(void) override
		{
			addDestination(_registry, _entity, _lead, _icon, _label, _right);
			_registry
				.getComponent<components::NavigationDrawerDestination>(_entity)
				.setIcon("inbox")
				.setLabel("Inbox")
				.setRight("24");
			_registry.getComponent<components::Layout>(_entity).setFixedWidth(
				296.0f);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Get the color written on an entity.
		 * @param identifier The entity.
		 * @return Its color.
		 */
		utility::graphic::Color32Bit colorOf(ecs::Entity::Identifier identifier)
		{
			return _registry.getComponent<components::Color>(identifier)
				.getColor();
		}

		/**
		 * @brief Get the position written on an entity.
		 * @param identifier The entity.
		 * @return Its position.
		 */
		utility::graphic::PositionF
			positionOf(ecs::Entity::Identifier identifier)
		{
			return _registry.getComponent<components::Transform>(identifier)
				.getPose()
				.getPosition();
		}
	};
}	 // namespace guillaume::systems::tests
