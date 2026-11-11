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

#include <cstddef>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/menu.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Styles a menu, sizes its rows and anchors it.
	 *
	 * Reads components::Menu, the interaction state of the rows and the
	 * active theme, and writes the menu's Layout, Color, Borders, Elevation,
	 * Overlay and Transform, the Layout, Color, Borders and enabled state of
	 * every row, and the color of the row labels and icons. Runs in the
	 * Layout phase, after the row content has been measured.
	 */
	class Menu: public ecs::SystemFiller<components::Menu>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		Menu(void);

		/**
		 * @brief Default destructor.
		 */
		~Menu(void) override = default;

		/**
		 * @brief Make an entity behave as a menu.
		 *
		 * Sets the accessibility semantics of the menu and makes every row
		 * already recorded in components::Menu pressable. Call once, after
		 * the entity has its components::Menu, Overlay, Elevation,
		 * Accessibility, Layout, Transform, Bound, Color and Borders.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Make one row pressable.
		 *
		 * Adds the interaction components to the row recorded in
		 * components::Menu, wires its press to `pressItem` and its hover and
		 * press changes to `styleRow`. Call once the row exists.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 * @param index Index of the row (ignored when out of range).
		 */
		static void installRow(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   std::size_t index);

		/**
		 * @brief Style one menu, size its rows and move it next to its
		 * anchor.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Color one row, its label and its icons from its enabled
		 * and interaction state.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 * @param index Index of the row (ignored when out of range).
		 */
		static void styleRow(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier,
							 std::size_t index);

		/**
		 * @brief Activate an item as a press on its row would: call its
		 * press handler. Ignored for a disabled item. The menu stays open.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 * @param index Index of the item (ignored when out of range).
		 */
		static void pressItem(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier,
							  std::size_t index);

		/**
		 * @brief Enable or disable an item and restyle its row.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 * @param index Index of the item (ignored when out of range).
		 * @param disabled Whether the item is disabled.
		 */
		static void
			setItemDisabled(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							std::size_t index, bool disabled);

		/**
		 * @brief Move the menu next to its anchor, then show it.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void show(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Hide the menu without calling the dismiss handler.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void hide(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Dismiss as the user would: hide the menu, then call the
		 * dismiss handler of its components::Overlay. Ignored while hidden.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void dismiss(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style the menu with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
