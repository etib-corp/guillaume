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
#include <guillaume/components/radio_button.hpp>
#include <guillaume/components/ring.hpp>
#include <guillaume/components/selection.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/radio_button.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::RadioButton on bare entity identifiers,
	 * without any entity object.
	 */
	class TestRadioButtonSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;			///< Component storage.
		ecs::Entity::Identifier _entity { 1 };		///< Widget under test.
		ecs::Entity::Identifier _label { 2 };		///< Its label child.
		ecs::Entity::Identifier _mark { 3 };		///< Its glyph/dot/thumb.
		guillaume::systems::RadioButton _system;	///< System under test.

		void SetUp(void) override
		{
			_registry.addComponent<components::Layout>(_entity)
				.setFixedWidth(20.0f)
				.setFixedHeight(20.0f);
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity)
				.setWidth(20.0f)
				.setHeight(20.0f);
			_registry.addComponent<components::Color>(_entity);
			_registry.addComponent<components::Borders>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::Selectable>(_entity);
			_registry.addComponent<components::Ring>(_entity);
			_registry.addComponent<components::RadioButton>(_entity)
				.setDot(_mark)
				.setLabelEntity(_label);
			_registry.addComponent<components::Transform>(_label);
			_registry.addComponent<components::Bound>(_label)
				.setWidth(50.0f)
				.setHeight(10.0f);
			_registry.addComponent<components::Color>(_label);
			_registry.addComponent<components::Transform>(_mark);
			_registry.addComponent<components::Bound>(_mark)
				.setWidth(10.0f)
				.setHeight(10.0f);
			_registry.addComponent<components::Color>(_mark);

			guillaume::systems::RadioButton::install(_registry, _entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Horizontal offset of a child from the widget origin.
		 * @param child The child identifier.
		 * @return The offset along x.
		 */
		float offsetX(ecs::Entity::Identifier child)
		{
			return _registry.getComponent<components::Transform>(child)
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
