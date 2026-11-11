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

#include <gtest/gtest.h>

#include <guillaume/components/accessibility.hpp>
#include <guillaume/components/badge.hpp>
#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/glyph.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/mouse_button_interaction.hpp>
#include <guillaume/components/navigation_bar_destination.hpp>
#include <guillaume/components/selection.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/navigation_bar_destination.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::NavigationBarDestination on bare
	 * entity identifiers: a destination with its indicator, icon, label and
	 * badge children, without any entity object.
	 */
	class TestNavigationBarDestinationSystem: public ::testing::Test
	{
		public:
		/**
		 * @brief Give identifiers the components of a navigation bar
		 * destination and of its children, and install the destination.
		 * @param registry The registry receiving the components.
		 * @param destination The destination identifier.
		 * @param indicator The identifier of its active indicator.
		 * @param icon The identifier of its icon.
		 * @param label The identifier of its label.
		 * @param badge The identifier of its badge, or InvalidIdentifier for
		 * none.
		 */
		static void addDestination(ecs::ComponentRegistry &registry,
								   ecs::Entity::Identifier destination,
								   ecs::Entity::Identifier indicator,
								   ecs::Entity::Identifier icon,
								   ecs::Entity::Identifier label,
								   ecs::Entity::Identifier badge)
		{
			registry.addComponent<components::Layout>(destination);
			registry.addComponent<components::Transform>(destination);
			registry.addComponent<components::Bound>(destination);
			registry.addComponent<components::Color>(destination);
			registry.addComponent<components::Borders>(destination);
			registry.addComponent<components::Hierarchy>(destination);
			registry.addComponent<components::Selectable>(destination);
			registry
				.addComponent<components::NavigationBarDestination>(destination)
				.setIndicator(indicator)
				.setIconEntity(icon)
				.setLabelEntity(label)
				.setBadgeEntity(badge);

			registry.addComponent<components::Transform>(indicator);
			registry.addComponent<components::Bound>(indicator);
			registry.addComponent<components::Color>(indicator);
			registry.addComponent<components::Hierarchy>(indicator);
			registry.addComponent<components::Layout>(indicator)
				.setMainAxisAlignment(
					components::Layout::MainAxisAlignment::Center)
				.setFixedWidth(
					components::NavigationBarDestination::IndicatorWidth)
				.setFixedHeight(
					components::NavigationBarDestination::IndicatorHeight)
				.setChildren({ icon });

			registry.addComponent<components::Transform>(icon);
			registry.addComponent<components::Bound>(icon)
				.setWidth(24.0f)
				.setHeight(24.0f);
			registry.addComponent<components::Color>(icon);
			registry.addComponent<components::Glyph>(icon);

			registry.addComponent<components::Transform>(label);
			registry.addComponent<components::Bound>(label)
				.setWidth(30.0f)
				.setHeight(16.0f);
			registry.addComponent<components::Color>(label);

			if (badge != ecs::Entity::InvalidIdentifier) {
				registry.addComponent<components::Transform>(badge);
				registry.addComponent<components::Bound>(badge);
				registry.addComponent<components::Color>(badge);
				registry.addComponent<components::Borders>(badge);
				registry.addComponent<components::Layout>(badge);
				registry.addComponent<components::Hierarchy>(badge);
				registry.addComponent<components::Badge>(badge);
			}

			guillaume::systems::NavigationBarDestination::install(registry,
																  destination);
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
		ecs::Entity::Identifier _indicator { 2 };	 ///< Its active indicator.
		ecs::Entity::Identifier _icon { 3 };		 ///< Its icon child.
		ecs::Entity::Identifier _label { 4 };		 ///< Its label child.
		ecs::Entity::Identifier _badge { 5 };		 ///< Its badge child.
		guillaume::systems::NavigationBarDestination
			_system;	///< System under test.

		void SetUp(void) override
		{
			addDestination(_registry, _entity, _indicator, _icon, _label,
						   _badge);
			_registry
				.getComponent<components::NavigationBarDestination>(_entity)
				.setIcon("mail_outline")
				.setFocusedIcon("mail")
				.setLabel("Mail")
				.setBadge("3");
			_registry.getComponent<components::Layout>(_entity)
				.setFixedWidth(100.0f)
				.setFixedHeight(64.0f);
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
