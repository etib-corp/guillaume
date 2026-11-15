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
#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/button.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/elevation.hpp>
#include <guillaume/components/glyph.hpp>
#include <guillaume/components/hand_button_interaction.hpp>
#include <guillaume/components/hand_hover_interaction.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/mouse_button_interaction.hpp>
#include <guillaume/components/mouse_hover_interaction.hpp>
#include <guillaume/components/style_state.hpp>
#include <guillaume/components/text.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/components/typography.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/button.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture building a button out of bare identifiers: the button,
	 * an icon child and a label child, without any entity object.
	 *
	 * Tests add the variant component they need and call that variant
	 * system's install().
	 */
	class TestButtonSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;		  ///< Component storage.
		ecs::Entity::Identifier _entity { 1 };	  ///< Button under test.
		ecs::Entity::Identifier _icon { 2 };	  ///< Its icon child.
		ecs::Entity::Identifier _label { 3 };	  ///< Its label child.
		guillaume::systems::Button _system;		  ///< System under test.

		void SetUp(void) override
		{
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity);
			_registry.addComponent<components::Color>(_entity);
			_registry.addComponent<components::Borders>(_entity);
			_registry.addComponent<components::HandButtonInteraction>(_entity);
			_registry.addComponent<components::HandHoverInteraction>(_entity);
			_registry.addComponent<components::MouseHoverInteraction>(_entity);
			_registry.addComponent<components::MouseButtonInteraction>(_entity);
			_registry.addComponent<components::StyleState>(_entity);
			_registry.addComponent<components::Accessibility>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::Button>(_entity)
				.setIconGlyphName("add")
				.setLabelContent("Save")
				.setIcon(_icon)
				.setLabel(_label);

			_registry.addComponent<components::Transform>(_icon);
			_registry.addComponent<components::Bound>(_icon)
				.setWidth(18.0f)
				.setHeight(18.0f);
			_registry.addComponent<components::Color>(_icon);
			_registry.addComponent<components::Glyph>(_icon);

			_registry.addComponent<components::Transform>(_label);
			_registry.addComponent<components::Bound>(_label)
				.setWidth(40.0f)
				.setHeight(20.0f);
			_registry.addComponent<components::Color>(_label);
			_registry.addComponent<components::Text>(_label);
			_registry.addComponent<components::Typography>(_label);

			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Access the button state.
		 * @return The components::Button under test.
		 */
		components::Button &button(void)
		{
			return _registry.getComponent<components::Button>(_entity);
		}

		/**
		 * @brief Horizontal offset of a child from the button origin.
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
