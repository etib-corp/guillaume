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

#include <guillaume/components/navigation_bar.hpp>
#include <guillaume/systems/navigation_bar.hpp>

#include "systems/test_navigation_bar_destination.hpp"

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::NavigationBar on bare entity
	 * identifiers: a bar and three destinations with their children,
	 * without any entity object.
	 */
	class TestNavigationBarSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;		  ///< Component storage.
		ecs::Entity::Identifier _entity { 1 };	  ///< Bar under test.
		std::vector<ecs::Entity::Identifier> _destinations {
			10, 20, 30
		};	  ///< Its destinations.
		ecs::Entity::Identifier _badge { 24 };	  ///< Badge of the second one.
		guillaume::systems::NavigationBar _system;	  ///< System under test.
		guillaume::systems::NavigationBarDestination
			_destinationSystem;	   ///< System of the destinations.

		void SetUp(void) override
		{
			_registry.addComponent<components::Layout>(_entity)
				.setAxis(components::Layout::Axis::Horizontal)
				.setMainAxisAlignment(
					components::Layout::MainAxisAlignment::SpaceBetween)
				.setPadding(8.0f)
				.setFixedWidth(360.0f)
				.setFixedHeight(components::NavigationBar::Height);
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity);
			_registry.addComponent<components::Color>(_entity);
			_registry.addComponent<components::Borders>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::SelectionGroup>(_entity);
			_registry.addComponent<components::NavigationBar>(_entity)
				.setDestinations({
					{ "home_outline", "Home", "home", "" },
					{ "mail_outline", "Mail", "mail", "3" },
					{ "person", "Me", "", "" },
				})
				.setDestinationEntities(_destinations);

			const auto &specs =
				_registry.getComponent<components::NavigationBar>(_entity)
					.getDestinations();
			for (std::size_t index = 0; index < _destinations.size(); ++index) {
				const auto destination = _destinations[index];
				TestNavigationBarDestinationSystem::addDestination(
					_registry, destination, indicator(index), icon(index),
					label(index),
					specs[index].badge.empty() ? ecs::Entity::InvalidIdentifier
											   : destination + 4);
				_registry
					.getComponent<components::NavigationBarDestination>(
						destination)
					.setIcon(specs[index].icon)
					.setFocusedIcon(specs[index].focusedIcon)
					.setLabel(specs[index].label)
					.setBadge(specs[index].badge);
			}

			guillaume::systems::NavigationBar::install(_registry, _entity);
			_system.bindComponentRegistry(_registry);
			_destinationSystem.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_destinationSystem.unbindComponentRegistry();
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Get the active indicator of a destination.
		 * @param index The destination index.
		 * @return Its identifier.
		 */
		ecs::Entity::Identifier indicator(std::size_t index) const
		{
			return _destinations[index] + 1;
		}

		/**
		 * @brief Get the icon of a destination.
		 * @param index The destination index.
		 * @return Its identifier.
		 */
		ecs::Entity::Identifier icon(std::size_t index) const
		{
			return _destinations[index] + 2;
		}

		/**
		 * @brief Get the label of a destination.
		 * @param index The destination index.
		 * @return Its identifier.
		 */
		ecs::Entity::Identifier label(std::size_t index) const
		{
			return _destinations[index] + 3;
		}

		/**
		 * @brief Whether a destination is selected.
		 * @param index The destination index.
		 * @return True when its Selectable is selected.
		 */
		bool isSelected(std::size_t index)
		{
			return _registry
				.getComponent<components::Selectable>(_destinations[index])
				.isSelected();
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
		 * @brief Get the horizontal position written on an entity.
		 * @param identifier The entity.
		 * @return Its X coordinate.
		 */
		float xOf(ecs::Entity::Identifier identifier)
		{
			return _registry.getComponent<components::Transform>(identifier)
				.getPose()
				.getPosition()
				.getX();
		}
	};
}	 // namespace guillaume::systems::tests
