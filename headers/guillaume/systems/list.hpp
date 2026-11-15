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

#include "guillaume/components/list.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Styles a list, stacks its rows and folds its accordions.
	 *
	 * Runs in the Layout phase on every entity that has a
	 * components::List. It is registered after systems::Layout, so the
	 * texts and icons of the rows are already measured when the rows are
	 * sized.
	 * - **Reads**: components::List (variant, entries, the row entities
	 *   built for them), the hover, press and enabled state of each row and
	 *   the active theme.
	 * - **Writes** on the list: components::Layout (vertical stack, child
	 *   list, a default width of components::List::DefaultWidth when none
	 *   is fixed), components::Color and components::Borders.
	 * - **Writes** on each row: the Layout of the row, of its leading group
	 *   and of its content column, its enabled state, its Accessibility
	 *   label, hint and expanded state, the chevron Glyph of an accordion,
	 *   and the Color of the row, its texts and its icons. Rows are then
	 *   sized and placed through systems::Layout::apply.
	 *
	 * An entry is named by its components::List::Path: `{2}` is the third
	 * top-level entry, `{2, 0}` the first child of that entry. An entry with
	 * children is an accordion. Only the direct children of an expanded
	 * top-level accordion are shown. The other nested rows are not
	 * destroyed: they are left out of the layout, painted transparent and
	 * disabled.
	 *
	 * @code
	 * systems::List::install(registry, list);        // rows become pressable
	 * systems::List::setExpanded(registry, list, 2, true);
	 * systems::List::pressItem(registry, list, {2, 0});
	 * @endcode
	 *
	 * @see components::List, entities::List, systems::Layout
	 */
	class List: public ecs::SystemFiller<components::List>
	{
		public:
		/// Index path of an entry: top-level index, then child indices.
		using Path = components::List::Path;

		/**
		 * @brief Create the system in the Layout phase.
		 */
		List(void);

		/**
		 * @brief Default destructor.
		 */
		~List(void) override = default;

		/**
		 * @brief Make every row of a list pressable.
		 *
		 * Calls installRow() on each row recorded in components::List,
		 * nested rows included. Call it after the row entities have been
		 * created and recorded with components::List::setRows; rows
		 * recorded later need their own installRow(). The list entity needs
		 * its components::List, Layout, Transform, Bound, Color and Borders.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the row handlers and must outlive the entity.
		 * @param entityIdentifier The list entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Make one row pressable.
		 *
		 * Adds the interaction components (hover and press state, focus,
		 * the ListItem accessibility role) to the pressable container of
		 * the row, and wires its activation to pressItem() and its hover
		 * and press changes to styleRow(). A subheader or a divider has no
		 * pressable container and is left alone.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the row handlers and must outlive the entity.
		 * @param entityIdentifier The list entity.
		 * @param path Index path of the row. A path that names no row is
		 * ignored.
		 */
		static void installRow(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   const Path &path);

		/**
		 * @brief Style one list, then size and place its rows.
		 *
		 * This is what update() runs each frame. Call it directly after
		 * changing components::List to see the result before the next
		 * frame, which is what the setters of entities::List do.
		 *
		 * Every row is configured and colored, including the folded ones.
		 * The layout children of the list are its top-level rows, each
		 * followed by its children while it is an expanded accordion. The
		 * SingleLine variant hides the supporting text of every row.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The list entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::List, Layout, Color or Borders.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Recolor one row without arranging the list again.
		 *
		 * Writes the Color of the row, its headline, its supporting text
		 * and its icons from the enabled, hovered and pressed state of the
		 * row. A folded row is painted transparent. This is the cheap
		 * restyle run when the pointer enters, leaves or presses a row.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The list entity.
		 * @param path Index path of the row. A path that names no row is
		 * ignored.
		 */
		static void styleRow(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier,
							 const Path &path);

		/**
		 * @brief Activate an entry as a press on its row would.
		 *
		 * A top-level accordion is expanded or collapsed first; then the
		 * press handler of the entry is called when it has one. Does nothing
		 * for a disabled entry or for a row that is folded away. Use it to
		 * trigger an entry from code or from a test.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The list entity.
		 * @param path Index path of the entry. A path that names no entry is
		 * ignored.
		 */
		static void pressItem(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier,
							  const Path &path);

		/**
		 * @brief Enable or disable a top-level entry and restyle the list.
		 *
		 * A disabled entry is drawn with the disabled content color and
		 * ignores pressItem().
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The list entity.
		 * @param index Position of the entry among the top-level entries,
		 * starting at 0. An index past the end changes nothing.
		 * @param disabled True to disable the entry, false to enable it.
		 */
		static void
			setItemDisabled(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							std::size_t index, bool disabled);

		/**
		 * @brief Expand or collapse a top-level accordion and arrange the
		 * list again.
		 *
		 * The press handler of the entry is not called.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The list entity.
		 * @param index Position of the entry among the top-level entries,
		 * starting at 0. An index past the end, or an entry without
		 * children, changes nothing.
		 * @param expanded True to show the children of the entry, false to
		 * fold them away.
		 */
		static void setExpanded(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								std::size_t index, bool expanded);

		/**
		 * @brief Whether a top-level entry is expanded.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The list entity.
		 * @param index Position of the entry among the top-level entries,
		 * starting at 0.
		 * @return True when the entry exists and its `expanded` flag is set;
		 * false for an index past the end.
		 */
		static bool isExpanded(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   std::size_t index);

		/**
		 * @brief Run apply() on one list; called by the frame loop.
		 *
		 * Logs a warning and does nothing when the entity has no
		 * components::List.
		 * @param entityIdentifier The list entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
