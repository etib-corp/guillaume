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
#include <guillaume/components/chip.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/elevation.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/mouse_button_interaction.hpp>
#include <guillaume/components/selection.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/chip.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::Chip on bare entity identifiers,
	 * without any entity object.
	 */
	class TestChipSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;			///< Component storage.
		ecs::Entity::Identifier _entity { 1 };		///< Widget under test.
		ecs::Entity::Identifier _leading { 2 };		///< Its leading icon.
		ecs::Entity::Identifier _check { 3 };		///< Its selected check.
		ecs::Entity::Identifier _label { 4 };		///< Its label child.
		ecs::Entity::Identifier _trailing { 5 };	///< Its close icon.
		guillaume::systems::Chip _system;			///< System under test.

		void SetUp(void) override
		{
			_registry.addComponent<components::Layout>(_entity)
				.setSpacing(8.0f)
				.setFixedHeight(32.0f);
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity);
			_registry.addComponent<components::Color>(_entity);
			_registry.addComponent<components::Borders>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::Selectable>(_entity);
			_registry.addComponent<components::Elevation>(_entity);
			_registry.addComponent<components::Chip>(_entity)
				.setVariant(components::Chip::Variant::Filter)
				.setLabel("Tag")
				.setLeadingIcon(_leading)
				.setCheckIcon(_check)
				.setLabelEntity(_label)
				.setTrailingIcon(_trailing);
			for (const auto child: { _leading, _check, _label, _trailing }) {
				_registry.addComponent<components::Transform>(child);
				_registry.addComponent<components::Bound>(child)
					.setWidth(18.0f)
					.setHeight(18.0f);
				_registry.addComponent<components::Color>(child);
			}

			guillaume::systems::Chip::install(_registry, _entity);
			guillaume::systems::Chip::installCloseIcon(_registry, _entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Get the configuration of the chip under test.
		 * @return The chip component.
		 */
		components::Chip &chip(void)
		{
			return _registry.getComponent<components::Chip>(_entity);
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
		 * @brief Get the children the Layout system arranges.
		 * @return The layout children of the chip, in order.
		 */
		std::vector<ecs::Entity::Identifier> laidOut(void)
		{
			return _registry.getComponent<components::Layout>(_entity)
				.getChildren();
		}

		/**
		 * @brief Release the left mouse button on an entity.
		 * @param identifier The entity.
		 */
		void click(ecs::Entity::Identifier identifier)
		{
			_registry
				.getComponent<components::MouseButtonInteraction>(identifier)
				.getOnButtonReleaseHandler(
					utility::event::MouseButtonEvent::Button::Left)();
		}
	};

}	 // namespace guillaume::systems::tests
