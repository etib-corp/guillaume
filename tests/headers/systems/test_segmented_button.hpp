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

#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <guillaume/components/accessibility.hpp>
#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/button.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/common_button.hpp>
#include <guillaume/components/glyph.hpp>
#include <guillaume/components/hand_button_interaction.hpp>
#include <guillaume/components/hand_hover_interaction.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/mouse_button_interaction.hpp>
#include <guillaume/components/mouse_hover_interaction.hpp>
#include <guillaume/components/segmented_button.hpp>
#include <guillaume/components/style_state.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/button.hpp>
#include <guillaume/systems/common_button.hpp>
#include <guillaume/systems/segmented_button.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::SegmentedButton on bare entity
	 * identifiers: a row and three segment buttons with their icon child,
	 * without any entity object.
	 */
	class TestSegmentedButtonSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;		  ///< Component storage.
		ecs::Entity::Identifier _entity { 1 };	  ///< Row under test.
		std::vector<ecs::Entity::Identifier> _buttons {
			10, 20, 30
		};	  ///< Its segment buttons.
		std::vector<ecs::Entity::Identifier> _icons {
			11, 21, 31
		};	  ///< The icon child of each segment.
		guillaume::systems::SegmentedButton _system;	///< System under test.

		void SetUp(void) override
		{
			_registry.addComponent<components::Layout>(_entity);
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::SegmentedButton>(_entity)
				.setSegments({
					{ "Day", "", false, "" },
					{ "Week", "event", false, "Whole week" },
					{ "Month", "", true, "" },
				})
				.setButtons(_buttons);

			for (std::size_t index = 0; index < _buttons.size(); ++index) {
				addButton(_buttons[index], _icons[index]);
			}

			guillaume::systems::SegmentedButton::install(_registry, _entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Give an identifier the components of a segment button.
		 * @param button The button identifier.
		 * @param icon The identifier of its icon child.
		 */
		void addButton(ecs::Entity::Identifier button,
					   ecs::Entity::Identifier icon)
		{
			_registry.addComponent<components::Transform>(button);
			_registry.addComponent<components::Bound>(button);
			_registry.addComponent<components::Color>(button);
			_registry.addComponent<components::Borders>(button);
			_registry.addComponent<components::HandButtonInteraction>(button);
			_registry.addComponent<components::HandHoverInteraction>(button);
			_registry.addComponent<components::MouseHoverInteraction>(button);
			_registry.addComponent<components::MouseButtonInteraction>(button);
			_registry.addComponent<components::StyleState>(button);
			_registry.addComponent<components::Accessibility>(button);
			_registry.addComponent<components::Hierarchy>(button);
			_registry.addComponent<components::Button>(button).setIcon(icon);

			_registry.addComponent<components::Transform>(icon);
			_registry.addComponent<components::Bound>(icon)
				.setWidth(18.0f)
				.setHeight(18.0f);
			_registry.addComponent<components::Color>(icon);
			_registry.addComponent<components::Glyph>(icon);

			_registry.addComponent<components::CommonButton>(button)
				.setColorStyle(components::CommonButton::Color::Segmented)
				.setShape(components::CommonButton::Shape::Square)
				.setSize(components::CommonButton::Size::Medium);
			guillaume::systems::CommonButton::install(_registry, button);
		}

		/**
		 * @brief Access the row state.
		 * @return The components::SegmentedButton under test.
		 */
		components::SegmentedButton &segmented(void)
		{
			return _registry.getComponent<components::SegmentedButton>(_entity);
		}

		/**
		 * @brief Access the state of a segment button.
		 * @param index The segment index.
		 * @return Its components::Button.
		 */
		components::Button &button(std::size_t index)
		{
			return _registry.getComponent<components::Button>(_buttons[index]);
		}

		/**
		 * @brief Glyph shown by a segment.
		 * @param index The segment index.
		 * @return The glyph name of its icon child.
		 */
		const std::string &glyph(std::size_t index)
		{
			return _registry.getComponent<components::Glyph>(_icons[index])
				.getName();
		}

		/**
		 * @brief Horizontal position of a segment.
		 * @param index The segment index.
		 * @return Its position along x.
		 */
		float positionX(std::size_t index)
		{
			return _registry
				.getComponent<components::Transform>(_buttons[index])
				.getPose()
				.getPosition()
				.getX();
		}
	};

}	 // namespace guillaume::systems::tests
