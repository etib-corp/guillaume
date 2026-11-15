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
#include <guillaume/components/animation.hpp>
#include <guillaume/components/arc.hpp>
#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/loading_indicator.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/loading_indicator.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::LoadingIndicator on bare entity
	 * identifiers, without any entity object.
	 */
	class TestLoadingIndicatorSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;		  ///< Component storage.
		ecs::Entity::Identifier _entity { 1 };	  ///< Widget under test.
		ecs::Entity::Identifier _child { 2 };	  ///< Its animated child.
		guillaume::systems::LoadingIndicator _system;	 ///< System under test.

		void SetUp(void) override
		{
			_registry.addComponent<components::Layout>(_entity);
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity);
			_registry.addComponent<components::Color>(_entity);
			_registry.addComponent<components::Borders>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::Animation>(_entity);
			_registry.addComponent<components::Accessibility>(_entity);
			_registry.addComponent<components::LoadingIndicator>(_entity)
				.setIndicator(_child);
			// The child carries what both the bar and the arc need, so one
			// fixture covers both variants.
			_registry.addComponent<components::Transform>(_child);
			_registry.addComponent<components::Bound>(_child);
			_registry.addComponent<components::Color>(_child);
			_registry.addComponent<components::Borders>(_child);
			_registry.addComponent<components::Arc>(_child);
			_registry.addComponent<components::Animation>(_child);

			guillaume::systems::LoadingIndicator::install(_registry, _entity);
			guillaume::systems::LoadingIndicator::installChildren(_registry,
																  _entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Get the state of the widget under test.
		 * @return Its components::LoadingIndicator.
		 */
		components::LoadingIndicator &state(void)
		{
			return _registry.getComponent<components::LoadingIndicator>(
				_entity);
		}

		/**
		 * @brief Horizontal offset of the child from the widget origin.
		 * @return The offset along x.
		 */
		float offsetX(void)
		{
			return _registry.getComponent<components::Transform>(_child)
					   .getPose()
					   .getPosition()
					   .getX()
				- _registry.getComponent<components::Transform>(_entity)
					  .getPose()
					  .getPosition()
					  .getX();
		}
	};

}	 // namespace guillaume::systems::tests
