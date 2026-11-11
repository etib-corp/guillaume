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

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/radio_button.hpp>
#include <guillaume/components/radio_group.hpp>
#include <guillaume/components/ring.hpp>
#include <guillaume/components/selection.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/radio_button.hpp>
#include <guillaume/systems/radio_group.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::RadioGroup on bare entity identifiers:
	 * a group and three radio buttons, without any entity object.
	 */
	class TestRadioGroupSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;		  ///< Component storage.
		ecs::Entity::Identifier _entity { 1 };	  ///< Group under test.
		std::vector<ecs::Entity::Identifier> _radios {
			10, 11, 12
		};	  ///< Its radio buttons.
		guillaume::systems::RadioGroup _system;	   ///< System under test.

		void SetUp(void) override
		{
			_registry.addComponent<components::Layout>(_entity)
				.setAxis(components::Layout::Axis::Vertical)
				.setSpacing(components::RadioGroup::RowSpacing);
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::RadioGroup>(_entity)
				.setOptions({ "A", "B", "C" })
				.setButtons(_radios);

			for (const auto radio: _radios) {
				addRadio(radio);
			}

			guillaume::systems::RadioGroup::install(_registry, _entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Give an identifier the components of a radio button.
		 * @param radio The radio button identifier.
		 */
		void addRadio(ecs::Entity::Identifier radio)
		{
			_registry.addComponent<components::Layout>(radio)
				.setFixedWidth(20.0f)
				.setFixedHeight(20.0f);
			_registry.addComponent<components::Transform>(radio);
			_registry.addComponent<components::Bound>(radio)
				.setWidth(20.0f)
				.setHeight(20.0f);
			_registry.addComponent<components::Color>(radio);
			_registry.addComponent<components::Borders>(radio);
			_registry.addComponent<components::Hierarchy>(radio);
			_registry.addComponent<components::Selectable>(radio);
			_registry.addComponent<components::Ring>(radio);
			_registry.addComponent<components::RadioButton>(radio);
			guillaume::systems::RadioButton::install(_registry, radio);
		}

		/**
		 * @brief Access the group state.
		 * @return The components::RadioGroup under test.
		 */
		components::RadioGroup &group(void)
		{
			return _registry.getComponent<components::RadioGroup>(_entity);
		}

		/**
		 * @brief Whether a radio button is selected.
		 * @param index The option index.
		 * @return True when its Selectable is selected.
		 */
		bool isSelected(std::size_t index)
		{
			return _registry
				.getComponent<components::Selectable>(_radios[index])
				.isSelected();
		}

		/**
		 * @brief Vertical position of a radio button.
		 * @param index The option index.
		 * @return Its position along y.
		 */
		float positionY(std::size_t index)
		{
			return _registry.getComponent<components::Transform>(_radios[index])
				.getPose()
				.getPosition()
				.getY();
		}
	};

}	 // namespace guillaume::systems::tests
