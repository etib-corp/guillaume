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
#include <guillaume/components/backdrop.hpp>
#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/dialog.hpp>
#include <guillaume/components/elevation.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/overlay.hpp>
#include <guillaume/components/scrim.hpp>
#include <guillaume/components/text.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/dialog.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::Dialog on bare entity identifiers,
	 * without any entity object.
	 */
	class TestDialogSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;			 ///< Component storage.
		ecs::Entity::Identifier _entity { 1 };		 ///< Dialog under test.
		ecs::Entity::Identifier _icon { 2 };		 ///< Its hero icon child.
		ecs::Entity::Identifier _title { 3 };		 ///< Its title child.
		ecs::Entity::Identifier _message { 4 };		 ///< Its message child.
		ecs::Entity::Identifier _actionRow { 5 };	 ///< Its action row child.
		ecs::Entity::Identifier _backdrop { 6 };	 ///< Its backdrop child.
		guillaume::systems::Dialog _system;			 ///< System under test.
		int _dismissals { 0 };						 ///< Dismiss handler calls.
		int _confirmations { 0 };					 ///< "OK" handler calls.

		void SetUp(void) override
		{
			_registry.addComponent<components::Layout>(_entity);
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity);
			_registry.addComponent<components::Color>(_entity);
			_registry.addComponent<components::Borders>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::Overlay>(_entity)
				.setOnDismissHandler([this]() {
					++_dismissals;
				});
			_registry.addComponent<components::Elevation>(_entity);
			_registry.addComponent<components::Accessibility>(_entity);
			_registry.addComponent<components::Dialog>(_entity)
				.setTitle("Title")
				.setMessage("Message")
				.setActions({ { "Cancel", {} },
							  { "OK",
								[this]() {
									++_confirmations;
								} } })
				.setIconEntity(_icon)
				.setTitleEntity(_title)
				.setMessageEntity(_message)
				.setActionRow(_actionRow)
				.setBackdrop(_backdrop);

			addChild(_icon, 24.0f, 24.0f);
			addChild(_title, 80.0f, 32.0f);
			_registry.addComponent<components::Text>(_title);
			addChild(_message, 120.0f, 20.0f);
			_registry.addComponent<components::Text>(_message);
			addChild(_actionRow, 272.0f, 40.0f);

			_registry.addComponent<components::Transform>(_backdrop);
			_registry.addComponent<components::Bound>(_backdrop);
			_registry.addComponent<components::Scrim>(_backdrop);
			_registry.addComponent<components::Animation>(_backdrop);
			_registry.addComponent<components::Backdrop>(_backdrop);

			guillaume::systems::Dialog::install(_registry, _entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Give a child the components the dialog arranges and
		 * colors.
		 * @param identifier The child.
		 * @param width Its measured width.
		 * @param height Its measured height.
		 */
		void addChild(ecs::Entity::Identifier identifier, float width,
					  float height)
		{
			_registry.addComponent<components::Transform>(identifier);
			_registry.addComponent<components::Bound>(identifier)
				.setWidth(width)
				.setHeight(height);
			_registry.addComponent<components::Color>(identifier);
		}

		/**
		 * @brief Get the dialog configuration.
		 * @return The component under test.
		 */
		components::Dialog &dialog(void)
		{
			return _registry.getComponent<components::Dialog>(_entity);
		}

		/**
		 * @brief Get the overlay of the dialog.
		 * @return The overlay component.
		 */
		components::Overlay &overlay(void)
		{
			return _registry.getComponent<components::Overlay>(_entity);
		}

		/**
		 * @brief Get the layout of the dialog.
		 * @return The layout component.
		 */
		components::Layout &layout(void)
		{
			return _registry.getComponent<components::Layout>(_entity);
		}

		/**
		 * @brief Get the state of the backdrop.
		 * @return The backdrop component.
		 */
		components::Backdrop &backdrop(void)
		{
			return _registry.getComponent<components::Backdrop>(_backdrop);
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

		/**
		 * @brief Check whether any entity of the dialog is marked changed.
		 * @return True when the dialog or one of its children changed.
		 */
		bool anythingChanged(void)
		{
			for (const auto identifier:
				 { _entity, _icon, _title, _message, _actionRow, _backdrop }) {
				if (_registry.hasChanged(identifier)) {
					return true;
				}
			}
			return false;
		}
	};

}	 // namespace guillaume::systems::tests
