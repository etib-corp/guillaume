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
#include <guillaume/components/menu.hpp>
#include <guillaume/components/overlay.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/menu.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::Menu on bare entity identifiers,
	 * without any entity object.
	 *
	 * The menu has two rows: "Copy" with a leading icon, and a dense "Paste"
	 * with a trailing icon.
	 */
	class TestMenuSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;			   ///< Component storage.
		ecs::Entity::Identifier _entity { 1 };		   ///< Menu under test.
		ecs::Entity::Identifier _copyRow { 10 };	   ///< First row.
		ecs::Entity::Identifier _copyIcon { 11 };	   ///< Its leading icon.
		ecs::Entity::Identifier _copyLabel { 12 };	   ///< Its label.
		ecs::Entity::Identifier _pasteRow { 20 };	   ///< Second (dense) row.
		ecs::Entity::Identifier _pasteLabel { 21 };	   ///< Its label.
		ecs::Entity::Identifier _pasteIcon { 22 };	   ///< Its trailing icon.
		ecs::Entity::Identifier _anchor { 30 };		   ///< Anchor entity.
		guillaume::systems::Menu _system;			   ///< System under test.
		int _copies { 0 };		  ///< "Copy" handler calls.
		int _pastes { 0 };		  ///< "Paste" handler calls.
		int _dismissals { 0 };	  ///< Dismiss handler calls.

		void SetUp(void) override
		{
			components::Menu::Item copy;
			copy.label		 = "Copy";
			copy.leadingIcon = "content_copy";
			copy.onPress	 = [this]() {
				++_copies;
			};
			components::Menu::Item paste;
			paste.label		   = "Paste";
			paste.trailingIcon = "content_paste";
			paste.dense		   = true;
			paste.onPress	   = [this]() {
				 ++_pastes;
			};

			components::Menu::Row copyRow;
			copyRow.view	= _copyRow;
			copyRow.content = { _copyIcon, _copyLabel };
			copyRow.label	= _copyLabel;
			copyRow.icons	= { _copyIcon };
			components::Menu::Row pasteRow;
			pasteRow.view	 = _pasteRow;
			pasteRow.content = { _pasteLabel, _pasteIcon };
			pasteRow.label	 = _pasteLabel;
			pasteRow.icons	 = { _pasteIcon };

			addSurface(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::Overlay>(_entity)
				.setOnDismissHandler([this]() {
					++_dismissals;
				});
			_registry.addComponent<components::Elevation>(_entity);
			_registry.addComponent<components::Accessibility>(_entity);
			_registry.addComponent<components::Menu>(_entity)
				.setItems({ copy, paste })
				.setRows({ copyRow, pasteRow })
				.setRowsDirty(false);

			addSurface(_copyRow);
			addContent(_copyIcon, 24.0f, 24.0f);
			addContent(_copyLabel, 40.0f, 24.0f);
			addSurface(_pasteRow);
			addContent(_pasteLabel, 44.0f, 24.0f);
			addContent(_pasteIcon, 24.0f, 24.0f);

			_registry.addComponent<components::Transform>(_anchor).setPose(
				utility::graphic::PoseF(
					utility::graphic::PositionF(10.0f, 100.0f, 0.0f),
					utility::graphic::OrientationF()));
			_registry.addComponent<components::Bound>(_anchor)
				.setWidth(40.0f)
				.setHeight(40.0f);

			guillaume::systems::Menu::install(_registry, _entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Give an entity the components of a layout surface.
		 * @param identifier The entity.
		 */
		void addSurface(ecs::Entity::Identifier identifier)
		{
			_registry.addComponent<components::Layout>(identifier);
			_registry.addComponent<components::Transform>(identifier);
			_registry.addComponent<components::Bound>(identifier);
			_registry.addComponent<components::Color>(identifier);
			_registry.addComponent<components::Borders>(identifier);
		}

		/**
		 * @brief Give a row child the components the menu measures, places
		 * and colors.
		 * @param identifier The child.
		 * @param width Its measured width.
		 * @param height Its measured height.
		 */
		void addContent(ecs::Entity::Identifier identifier, float width,
						float height)
		{
			_registry.addComponent<components::Transform>(identifier);
			_registry.addComponent<components::Bound>(identifier)
				.setWidth(width)
				.setHeight(height);
			_registry.addComponent<components::Color>(identifier);
		}

		/**
		 * @brief Get the menu configuration.
		 * @return The component under test.
		 */
		components::Menu &menu(void)
		{
			return _registry.getComponent<components::Menu>(_entity);
		}

		/**
		 * @brief Get the overlay of the menu.
		 * @return The overlay component.
		 */
		components::Overlay &overlay(void)
		{
			return _registry.getComponent<components::Overlay>(_entity);
		}

		/**
		 * @brief Get the bound of an entity.
		 * @param identifier The entity.
		 * @return Its Bound component.
		 */
		components::Bound &boundOf(ecs::Entity::Identifier identifier)
		{
			return _registry.getComponent<components::Bound>(identifier);
		}

		/**
		 * @brief Get the position of an entity.
		 * @param identifier The entity.
		 * @return The position of its Transform.
		 */
		utility::graphic::PositionF
			positionOf(ecs::Entity::Identifier identifier)
		{
			return _registry.getComponent<components::Transform>(identifier)
				.getPose()
				.getPosition();
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
		 * @brief Check whether any entity of the menu is marked changed.
		 * @return True when the menu, a row or a row child changed.
		 */
		bool anythingChanged(void)
		{
			for (const auto identifier:
				 { _entity, _copyRow, _copyIcon, _copyLabel, _pasteRow,
				   _pasteLabel, _pasteIcon }) {
				if (_registry.hasChanged(identifier)) {
					return true;
				}
			}
			return false;
		}
	};

}	 // namespace guillaume::systems::tests
