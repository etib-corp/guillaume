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

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/layout.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving the rules of a View on bare entity identifiers,
	 * without any entity object.
	 *
	 * A View has no component and no system of its own: its state is the
	 * Layout, Transform, Bound, Color and Borders components and its rules
	 * are those of systems::Layout.
	 */
	class TestViewSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;		  ///< Component storage.
		ecs::Entity::Identifier _view { 1 };	  ///< View under test.
		ecs::Entity::Identifier _first { 2 };	  ///< Its first child.
		ecs::Entity::Identifier _second { 3 };	  ///< Its second child.
		guillaume::systems::Layout _system;		  ///< System under test.

		void SetUp(void) override
		{
			_registry.addComponent<components::Layout>(_view)
				.setSpacing(8.0f)
				.setPadding(16.0f)
				.setChildren({ _first, _second });
			_registry.addComponent<components::Transform>(_view).setPose(
				utility::graphic::PoseF(
					utility::graphic::PositionF(100.0f, 200.0f, -300.0f),
					utility::graphic::OrientationF()));
			_registry.addComponent<components::Bound>(_view);
			_registry.addComponent<components::Color>(_view);
			_registry.addComponent<components::Borders>(_view);
			_registry.addComponent<components::Hierarchy>(_view);
			_registry.addComponent<components::Transform>(_first);
			_registry.addComponent<components::Bound>(_first)
				.setWidth(50.0f)
				.setHeight(30.0f);
			_registry.addComponent<components::Transform>(_second);
			_registry.addComponent<components::Bound>(_second)
				.setWidth(20.0f)
				.setHeight(60.0f);

			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Get the layout component of the view.
		 * @return The layout component.
		 */
		components::Layout &layout(void)
		{
			return _registry.getComponent<components::Layout>(_view);
		}

		/**
		 * @brief Get the bound component of the view.
		 * @return The bound component.
		 */
		components::Bound &bound(void)
		{
			return _registry.getComponent<components::Bound>(_view);
		}

		/**
		 * @brief Get the position of an entity.
		 * @param identifier The entity identifier.
		 * @return Its position.
		 */
		utility::graphic::PositionF position(ecs::Entity::Identifier identifier)
		{
			return _registry.getComponent<components::Transform>(identifier)
				.getPose()
				.getPosition();
		}

		/**
		 * @brief Horizontal offset of a child from the view origin.
		 * @param child The child identifier.
		 * @return The offset along x.
		 */
		float offsetX(ecs::Entity::Identifier child)
		{
			return position(child).getX() - position(_view).getX();
		}

		/**
		 * @brief Vertical offset of a child from the view origin.
		 * @param child The child identifier.
		 * @return The offset along y.
		 */
		float offsetY(ecs::Entity::Identifier child)
		{
			return position(child).getY() - position(_view).getY();
		}

		/**
		 * @brief Depth offset of a child from the view origin.
		 * @param child The child identifier.
		 * @return The offset along z.
		 */
		float offsetZ(ecs::Entity::Identifier child)
		{
			return position(child).getZ() - position(_view).getZ();
		}
	};

}	 // namespace guillaume::systems::tests
