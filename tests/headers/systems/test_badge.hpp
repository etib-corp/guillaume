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

#include <guillaume/components/animation.hpp>
#include <guillaume/components/badge.hpp>
#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/text.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/components/typography.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/badge.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::Badge on bare entity identifiers,
	 * without any entity object.
	 */
	class TestBadgeSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;		  ///< Component storage.
		ecs::Entity::Identifier _entity { 1 };	  ///< Widget under test.
		ecs::Entity::Identifier _label { 2 };	  ///< Its label child.
		guillaume::systems::Badge _system;		  ///< System under test.

		void SetUp(void) override
		{
			_registry.addComponent<components::Layout>(_entity)
				.setMainAxisAlignment(
					components::Layout::MainAxisAlignment::Center)
				.setChildren({ _label });
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity);
			_registry.addComponent<components::Color>(_entity);
			_registry.addComponent<components::Borders>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::Animation>(_entity);
			_registry.addComponent<components::Badge>(_entity)
				.setLabel("9")
				.setLabelEntity(_label);
			_registry.addComponent<components::Transform>(_label);
			_registry.addComponent<components::Bound>(_label)
				.setWidth(8.0f)
				.setHeight(10.0f);
			_registry.addComponent<components::Color>(_label);
			_registry.addComponent<components::Text>(_label);
			_registry.addComponent<components::Typography>(_label);

			guillaume::systems::Badge::install(_registry, _entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Get the configuration of the badge under test.
		 * @return The badge component.
		 */
		components::Badge &badge(void)
		{
			return _registry.getComponent<components::Badge>(_entity);
		}

		/**
		 * @brief Get the resolved size of the badge under test.
		 * @return The bound component.
		 */
		const components::Bound &bound(void)
		{
			return _registry.getComponent<components::Bound>(_entity);
		}

		/**
		 * @brief Get the alpha of the color of an entity.
		 * @param identifier The entity.
		 * @return The alpha channel.
		 */
		int alphaOf(ecs::Entity::Identifier identifier)
		{
			return _registry.getComponent<components::Color>(identifier)
				.getColor()
				.getAlpha();
		}
	};

}	 // namespace guillaume::systems::tests
