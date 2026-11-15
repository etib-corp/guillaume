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

#include <guillaume/components/animation.hpp>
#include <guillaume/components/backdrop.hpp>
#include <guillaume/components/navigation_drawer.hpp>
#include <guillaume/components/overlay.hpp>
#include <guillaume/components/scrim.hpp>
#include <guillaume/systems/navigation_drawer.hpp>

#include "systems/test_navigation_drawer_destination.hpp"

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::NavigationDrawer on bare entity
	 * identifiers: a drawer with a section header, three destinations (the
	 * last one disabled), a divider and a backdrop, without any entity
	 * object.
	 */
	class TestNavigationDrawerSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;		  ///< Component storage.
		ecs::Entity::Identifier _entity { 1 };	  ///< Drawer under test.
		std::vector<ecs::Entity::Identifier> _destinations {
			10, 20, 30
		};	  ///< Its destinations.
		ecs::Entity::Identifier _section { 40 };	 ///< Its section header.
		ecs::Entity::Identifier _divider { 50 };	 ///< Its divider.
		ecs::Entity::Identifier _backdrop { 60 };	 ///< Its backdrop.
		guillaume::systems::NavigationDrawer _system;	 ///< System under test.
		guillaume::systems::NavigationDrawerDestination
			_destinationSystem;	   ///< System of the destinations.

		void SetUp(void) override
		{
			using Entry		= components::NavigationDrawer::Entry;
			using EntryKind = components::NavigationDrawer::EntryKind;

			_registry.addComponent<components::Layout>(_entity);
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity);
			_registry.addComponent<components::Color>(_entity);
			_registry.addComponent<components::Borders>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::Overlay>(_entity);
			_registry.addComponent<components::SelectionGroup>(_entity);
			_registry.addComponent<components::NavigationDrawer>(_entity)
				.setEntries({
					Entry { EntryKind::Section, "", "Mail", "", false },
					Entry { EntryKind::Destination, "inbox", "Inbox", "24",
							false },
					Entry { EntryKind::Destination, "send", "Sent", "", false },
					Entry { EntryKind::Divider, "", "", "", false },
					Entry { EntryKind::Destination, "delete", "Trash", "",
							true },
				})
				.setEntryEntities({ _section, _destinations[0],
									_destinations[1], _divider,
									_destinations[2] })
				.setDestinationEntities(_destinations)
				.setSectionEntities({ _section })
				.setDividerEntities({ _divider })
				.setBackdrop(_backdrop);

			std::size_t index = 0;
			for (const auto &entry:
				 _registry.getComponent<components::NavigationDrawer>(_entity)
					 .getEntries()) {
				if (entry.kind != EntryKind::Destination) {
					continue;
				}

				const auto destination = _destinations[index++];
				TestNavigationDrawerDestinationSystem::addDestination(
					_registry, destination, destination + 1, destination + 2,
					destination + 3,
					entry.right.empty() ? ecs::Entity::InvalidIdentifier
										: destination + 4);
				_registry
					.getComponent<components::NavigationDrawerDestination>(
						destination)
					.setIcon(entry.icon)
					.setLabel(entry.label)
					.setRight(entry.right)
					.setDisabled(entry.disabled);
			}

			TestNavigationDrawerDestinationSystem::addContent(
				_registry, _section, 60.0f, 20.0f);
			_registry.addComponent<components::Transform>(_divider);
			_registry.addComponent<components::Bound>(_divider)
				.setWidth(296.0f)
				.setHeight(1.0f);

			_registry.addComponent<components::Transform>(_backdrop);
			_registry.addComponent<components::Bound>(_backdrop);
			_registry.addComponent<components::Scrim>(_backdrop);
			_registry.addComponent<components::Animation>(_backdrop);
			_registry.addComponent<components::Backdrop>(_backdrop);

			guillaume::systems::NavigationDrawer::install(_registry, _entity);
			_system.bindComponentRegistry(_registry);
			_destinationSystem.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_destinationSystem.unbindComponentRegistry();
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Get the configuration of the drawer under test.
		 * @return The drawer component.
		 */
		components::NavigationDrawer &drawer(void)
		{
			return _registry.getComponent<components::NavigationDrawer>(
				_entity);
		}

		/**
		 * @brief Get the index of the selected destination.
		 * @return The index.
		 */
		std::size_t selected(void)
		{
			return guillaume::systems::NavigationDrawer::selectedIndex(
				_registry, _entity);
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
		 * @brief Get the vertical position written on an entity.
		 * @param identifier The entity.
		 * @return Its Y coordinate.
		 */
		float yOf(ecs::Entity::Identifier identifier)
		{
			return _registry.getComponent<components::Transform>(identifier)
				.getPose()
				.getPosition()
				.getY();
		}
	};
}	 // namespace guillaume::systems::tests
