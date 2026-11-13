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
	 * @brief Styles a menu, sizes its rows to the widest one and places the
	 * menu below or above its anchor.
	 *
	 * Runs in the Layout phase on every entity that has a
	 * components::Menu. It is registered after systems::Layout, so the
	 * labels and icons of the rows are already measured when the menu is
	 * sized.
	 * - **Reads**: components::Menu (mode, items, the row entities built
	 *   for them, anchor entity and side), the hover, press and enabled
	 *   state of each row, the Transform and Bound of the anchor and the
	 *   active theme.
	 * - **Writes** on the menu: components::Layout (vertical stack, fixed
	 *   width, child list), components::Borders, components::Elevation,
	 *   components::Color, the modal and dismissal flags of
	 *   components::Overlay, and components::Transform (the anchored pose).
	 * - **Writes** on each row: its Layout, Borders and enabled state, and
	 *   the Color of the row, its label and its icons. Rows and their
	 *   content are then placed through systems::Layout::apply.
	 *
	 * Whether the menu is shown is the `visible` flag of its
	 * components::Overlay; show(), hide() and dismiss() change it. A menu is
	 * never modal and closes on Escape or on a click outside. Pressing an
	 * item does not close it: the press handler decides.
	 *
	 * The Transform of the menu is overwritten every frame while the anchor
	 * has a Transform and a Bound. Without such an anchor the menu keeps
	 * the pose it was given.
	 *
	 * @code
	 * systems::Menu::install(registry, menu);
	 * systems::Menu::show(registry, menu);
	 * systems::Menu::pressItem(registry, menu, 0); // as a click would
	 * systems::Menu::dismiss(registry, menu);
	 * @endcode
	 *
	 * @see components::Menu, components::Overlay, entities::Menu,
	 * systems::Overlay
	 */
	class Menu: public ecs::SystemFiller<components::Menu>
	{
		public:
		/**
		 * @brief Create the system in the Layout phase.
		 */
		Menu(void);

		/**
		 * @brief Default destructor.
		 */
		~Menu(void) override = default;

		/**
		 * @brief Give a menu its accessibility semantics and make its rows
		 * pressable.
		 *
		 * Sets the role of the menu's components::Accessibility to Menu,
		 * makes the menu itself non-focusable, and calls installRow() on
		 * every row recorded in components::Menu. Call it once, after the
		 * entity has its components::Menu, Overlay, Elevation,
		 * Accessibility, Layout, Transform, Bound, Color and Borders; rows
		 * recorded later need their own installRow().
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the row handlers and must outlive the entity.
		 * @param entityIdentifier The menu entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity has
		 * no components::Accessibility or components::Menu.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Make one row pressable.
		 *
		 * Adds the interaction components (hover and press state, focus,
		 * the MenuItem accessibility role) to the row entity, and wires its
		 * activation to pressItem() and its hover and press changes to
		 * styleRow(). When an item exists at the same index, its label
		 * becomes the accessible name of the row and its disabled flag the
		 * enabled state of the row.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the row handlers and must outlive the entity.
		 * @param entityIdentifier The menu entity.
		 * @param index Position of the row in the row list of
		 * components::Menu, starting at 0. An index past the end is ignored.
		 */
		static void installRow(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   std::size_t index);

		/**
		 * @brief Style one menu, size its rows and move it next to its
		 * anchor.
		 *
		 * This is what update() runs each frame. Call it directly after
		 * changing components::Menu to see the result before the next
		 * frame, which is what the setters of entities::Menu do.
		 *
		 * The menu is as wide as its widest row, kept between
		 * components::Menu::MinWidth and components::Menu::MaxWidth; every
		 * row takes that width and is 48 dp high, or 32 dp for a dense
		 * item. With the Bottom anchor position the menu starts 4 dp below
		 * the anchor, otherwise it ends 4 dp above it; in both cases their
		 * left edges are aligned.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::Menu, Layout, Borders, Elevation, Color or Overlay, or
		 * when a recorded row has no components::Layout.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether a menu is open.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 * @return True when its Overlay is visible. A closed menu draws
		 * nothing and ignores presses on its rows.
		 */
		static bool isOpen(const ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Recolor one row without sizing the menu again.
		 *
		 * Writes the Color of the row (its hover or press state layer), of
		 * its label and of its icons; a disabled row uses the disabled
		 * content color. This is the cheap restyle run when the pointer
		 * enters, leaves or presses a row.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 * @param index Position of the row in the row list of
		 * components::Menu, starting at 0. An index past the end is ignored.
		 */
		static void styleRow(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier,
							 std::size_t index);

		/**
		 * @brief Activate an item as a press on its row would.
		 *
		 * Calls the press handler of the item when it has one. Does nothing
		 * for a disabled item. The menu stays open: a handler that should
		 * close it calls hide() or dismiss() itself.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 * @param index Position of the item in the item list of
		 * components::Menu, starting at 0. An index past the end is ignored.
		 */
		static void pressItem(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier,
							  std::size_t index);

		/**
		 * @brief Enable or disable an item and restyle the menu.
		 *
		 * A disabled item is drawn with the disabled content color and
		 * ignores pressItem().
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 * @param index Position of the item in the item list of
		 * components::Menu, starting at 0.
		 * @param disabled True to disable the item, false to enable it.
		 */
		static void
			setItemDisabled(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							std::size_t index, bool disabled);

		/**
		 * @brief Place the menu next to its anchor, then show it.
		 *
		 * Calls apply() first, so the menu appears at the anchor's current
		 * position, then sets the `visible` flag of its
		 * components::Overlay.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void show(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Hide the menu without telling anyone.
		 *
		 * Clears the `visible` flag of its components::Overlay. The dismiss
		 * handler is not called: use dismiss() when the application must be
		 * notified.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void hide(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Close the menu as the user would with Escape or a click
		 * outside.
		 *
		 * Hides the menu, then calls the dismiss handler of its
		 * components::Overlay when there is one. Does nothing when the menu
		 * is already hidden.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void dismiss(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Run apply() on one menu; called by the frame loop.
		 *
		 * Logs a warning and does nothing when the entity has no
		 * components::Menu.
		 * @param entityIdentifier The menu entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
