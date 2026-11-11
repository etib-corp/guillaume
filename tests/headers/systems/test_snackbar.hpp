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
#include <guillaume/components/color.hpp>
#include <guillaume/components/elevation.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/overlay.hpp>
#include <guillaume/components/snackbar.hpp>
#include <guillaume/components/text.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/snackbar.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::Snackbar on bare entity identifiers,
	 * without any entity object.
	 */
	class TestSnackbarSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;		   ///< Component storage.
		ecs::Entity::Identifier _entity { 1 };	   ///< Snackbar under test.
		ecs::Entity::Identifier _message { 2 };	   ///< Its message child.
		ecs::Entity::Identifier _action { 3 };	   ///< Its action child.
		guillaume::systems::Snackbar _system;	   ///< System under test.
		int _dismissals { 0 };					   ///< Dismiss callback calls.
		int _actions { 0 };						   ///< Action callback calls.

		void SetUp(void) override
		{
			_registry.addComponent<components::Layout>(_entity).setFixedWidth(
				344.0f);
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity);
			_registry.addComponent<components::Color>(_entity);
			_registry.addComponent<components::Borders>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::Overlay>(_entity);
			_registry.addComponent<components::Elevation>(_entity);
			_registry.addComponent<components::Accessibility>(_entity);
			_registry.addComponent<components::Snackbar>(_entity)
				.setVariant(components::Snackbar::Variant::WithAction)
				.setMessage("Saved")
				.setAction("Undo")
				.setMessageEntity(_message)
				.setActionEntity(_action)
				.setOnDismiss([this]() {
					++_dismissals;
				})
				.setOnAction([this]() {
					++_actions;
				});

			_registry.addComponent<components::Transform>(_message);
			_registry.addComponent<components::Bound>(_message)
				.setWidth(60.0f)
				.setHeight(14.0f);
			_registry.addComponent<components::Color>(_message);
			_registry.addComponent<components::Text>(_message);
			_registry.addComponent<components::Transform>(_action);
			_registry.addComponent<components::Bound>(_action)
				.setWidth(40.0f)
				.setHeight(14.0f);
			_registry.addComponent<components::Color>(_action);

			guillaume::systems::Snackbar::install(_registry, _entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Get the snackbar configuration.
		 * @return The component under test.
		 */
		components::Snackbar &snackbar(void)
		{
			return _registry.getComponent<components::Snackbar>(_entity);
		}

		/**
		 * @brief Get the overlay of the snackbar.
		 * @return The overlay component.
		 */
		components::Overlay &overlay(void)
		{
			return _registry.getComponent<components::Overlay>(_entity);
		}

		/**
		 * @brief Get the color of an entity.
		 * @param identifier The entity.
		 * @return Its Color value.
		 */
		utility::graphic::Color32Bit colorOf(ecs::Entity::Identifier identifier)
		{
			return _registry.getComponent<components::Color>(identifier)
				.getColor();
		}
	};

}	 // namespace guillaume::systems::tests
