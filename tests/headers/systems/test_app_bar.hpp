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

#include <guillaume/components/app_bar.hpp>
#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/button.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/elevation.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/text.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/components/typography.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/app_bar.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::AppBar on bare entity identifiers,
	 * without any entity object.
	 */
	class TestAppBarSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;			  ///< Component storage.
		ecs::Entity::Identifier _entity { 1 };		  ///< Bar under test.
		ecs::Entity::Identifier _navigation { 2 };	  ///< Its back button.
		ecs::Entity::Identifier _title { 3 };		  ///< Its title child.
		ecs::Entity::Identifier _search { 4 };		  ///< Its first action.
		ecs::Entity::Identifier _more { 5 };		  ///< Its second action.
		guillaume::systems::AppBar _system;			  ///< System under test.

		void SetUp(void) override
		{
			_registry.addComponent<components::Layout>(_entity).setFixedWidth(
				360.0f);
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity);
			_registry.addComponent<components::Color>(_entity);
			_registry.addComponent<components::Borders>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::Elevation>(_entity);
			_registry.addComponent<components::AppBar>(_entity)
				.setTitle("Inbox")
				.setNavigation({ "menu", {}, "Menu", false })
				.setActions({ { "search", {}, "Search", false },
							  { "more_vert", {}, "More", false } })
				.setNavigationButton(_navigation)
				.setTitleEntity(_title)
				.setActionButtons({ _search, _more });

			for (const auto child: { _navigation, _search, _more }) {
				_registry.addComponent<components::Transform>(child);
				_registry.addComponent<components::Bound>(child)
					.setWidth(48.0f)
					.setHeight(48.0f);
				_registry.addComponent<components::Button>(child);
			}
			_registry.addComponent<components::Transform>(_title);
			_registry.addComponent<components::Bound>(_title)
				.setWidth(60.0f)
				.setHeight(28.0f);
			_registry.addComponent<components::Color>(_title);
			_registry.addComponent<components::Text>(_title);
			_registry.addComponent<components::Typography>(_title);

			guillaume::systems::AppBar::installTitle(_registry, _entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Get the configuration of the bar under test.
		 * @return The app bar component.
		 */
		components::AppBar &appBar(void)
		{
			return _registry.getComponent<components::AppBar>(_entity);
		}

		/**
		 * @brief Get the layout of the bar under test.
		 * @return The layout component.
		 */
		components::Layout &layout(void)
		{
			return _registry.getComponent<components::Layout>(_entity);
		}

		/**
		 * @brief Get the X position written on an entity.
		 * @param identifier The entity.
		 * @return The X coordinate of its transform.
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
