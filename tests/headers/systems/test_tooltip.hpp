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
#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/elevation.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/mouse_hover_interaction.hpp>
#include <guillaume/components/overlay.hpp>
#include <guillaume/components/text.hpp>
#include <guillaume/components/tooltip.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/tooltip.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::Tooltip on bare entity identifiers,
	 * without any entity object.
	 */
	class TestTooltipSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;		  ///< Component storage.
		ecs::Entity::Identifier _entity { 1 };	  ///< Tooltip under test.
		ecs::Entity::Identifier _title { 2 };	  ///< Its title child.
		ecs::Entity::Identifier _text { 3 };	  ///< Its body child.
		ecs::Entity::Identifier _anchor { 4 };	  ///< Its trigger entity.
		guillaume::systems::Tooltip _system;	  ///< System under test.

		void SetUp(void) override
		{
			_registry.addComponent<components::Layout>(_entity);
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity);
			_registry.addComponent<components::Color>(_entity);
			_registry.addComponent<components::Borders>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::Overlay>(_entity);
			_registry.addComponent<components::Elevation>(_entity);
			_registry.addComponent<components::Animation>(_entity);
			_registry.addComponent<components::Accessibility>(_entity);
			_registry.addComponent<components::Tooltip>(_entity)
				.setText("Body")
				.setTitle("Title")
				.setTitleEntity(_title)
				.setTextEntity(_text);

			_registry.addComponent<components::Transform>(_title);
			_registry.addComponent<components::Bound>(_title)
				.setWidth(30.0f)
				.setHeight(12.0f);
			_registry.addComponent<components::Color>(_title);
			_registry.addComponent<components::Transform>(_text);
			_registry.addComponent<components::Bound>(_text)
				.setWidth(50.0f)
				.setHeight(10.0f);
			_registry.addComponent<components::Color>(_text);
			_registry.addComponent<components::Text>(_text);

			_registry.addComponent<components::Transform>(_anchor).setPose(
				utility::graphic::PoseF(
					utility::graphic::PositionF(100.0f, 50.0f, 0.0f),
					utility::graphic::OrientationF()));
			_registry.addComponent<components::Bound>(_anchor)
				.setWidth(40.0f)
				.setHeight(20.0f);
			_registry.addComponent<components::MouseHoverInteraction>(_anchor);

			guillaume::systems::Tooltip::install(_registry, _entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Get the tooltip configuration.
		 * @return The component under test.
		 */
		components::Tooltip &tooltip(void)
		{
			return _registry.getComponent<components::Tooltip>(_entity);
		}

		/**
		 * @brief Hover or leave the anchor.
		 * @param hovered Whether the pointer is over the anchor.
		 */
		void hoverAnchor(bool hovered)
		{
			_registry.getComponent<components::MouseHoverInteraction>(_anchor)
				.setHovered(hovered);
		}

		/**
		 * @brief Get the position of an entity.
		 * @param identifier The entity.
		 * @return Its Transform position.
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
