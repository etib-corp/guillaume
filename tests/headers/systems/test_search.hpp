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

#include <guillaume/components/accessibility.hpp>
#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/elevation.hpp>
#include <guillaume/components/focus.hpp>
#include <guillaume/components/glyph.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/line.hpp>
#include <guillaume/components/mouse_button_interaction.hpp>
#include <guillaume/components/overlay.hpp>
#include <guillaume/components/search.hpp>
#include <guillaume/components/text.hpp>
#include <guillaume/components/text_field.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/search.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::Search on bare entity identifiers,
	 * without any entity object.
	 *
	 * The field has every child: leading icon, query, placeholder, clear
	 * icon, loading glyph and trailing icon. Its query is empty and it has
	 * no trailing glyph.
	 */
	class TestSearchSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;			   ///< Component storage.
		ecs::Entity::Identifier _entity { 1 };		   ///< Field under test.
		ecs::Entity::Identifier _icon { 2 };		   ///< Its leading icon.
		ecs::Entity::Identifier _query { 3 };		   ///< Its query child.
		ecs::Entity::Identifier _placeholder { 4 };	   ///< Its placeholder.
		ecs::Entity::Identifier _clear { 5 };		   ///< Its clear icon.
		ecs::Entity::Identifier _loading { 6 };		   ///< Its loading glyph.
		ecs::Entity::Identifier _trailing { 7 };	   ///< Its trailing icon.
		guillaume::systems::Search _system;			   ///< System under test.
		int _iconPresses { 0 };		   ///< Leading handler calls.
		int _clearPresses { 0 };	   ///< Clear handler calls.
		int _trailingPresses { 0 };	   ///< Trailing handler calls.

		void SetUp(void) override
		{
			_registry.addComponent<components::Layout>(_entity);
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity);
			_registry.addComponent<components::Color>(_entity);
			_registry.addComponent<components::Borders>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::Overlay>(_entity);
			_registry.addComponent<components::TextField>(_entity);
			_registry.addComponent<components::Elevation>(_entity);
			_registry.addComponent<components::Line>(_entity);
			_registry.addComponent<components::Focus>(_entity);
			_registry.addComponent<components::Accessibility>(_entity);
			_registry.addComponent<components::Search>(_entity)
				.setPlaceholder("Search")
				.setSearchIcon(_icon)
				.setTextEntity(_query)
				.setPlaceholderEntity(_placeholder)
				.setClearIcon(_clear)
				.setLoadingIcon(_loading)
				.setTrailingIcon(_trailing)
				.setOnIconPress([this]() {
					++_iconPresses;
				})
				.setOnClearIconPress([this]() {
					++_clearPresses;
				})
				.setOnTrailingIconPress([this]() {
					++_trailingPresses;
				});

			for (const auto icon: { _icon, _clear, _loading, _trailing }) {
				addContent(icon, 24.0f);
				_registry.addComponent<components::Glyph>(icon);
			}
			for (const auto text: { _query, _placeholder }) {
				addContent(text, 80.0f);
				_registry.addComponent<components::Text>(text);
			}

			guillaume::systems::Search::install(_registry, _entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Give an entity the components of measured content.
		 * @param identifier The entity.
		 * @param width Its measured width.
		 */
		void addContent(ecs::Entity::Identifier identifier, float width)
		{
			_registry.addComponent<components::Transform>(identifier);
			_registry.addComponent<components::Bound>(identifier)
				.setWidth(width)
				.setHeight(24.0f);
			_registry.addComponent<components::Color>(identifier);
		}

		/**
		 * @brief Get the configuration of the field under test.
		 * @return The search component.
		 */
		components::Search &search(void)
		{
			return _registry.getComponent<components::Search>(_entity);
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
		 * @return The layout children of the field, in order.
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
