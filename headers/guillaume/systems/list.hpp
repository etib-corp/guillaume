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
	 * @brief Styles a list, arranges its rows and folds its accordions.
	 *
	 * Reads components::List, the interaction state of the rows and the
	 * active theme, and writes the list's Layout, Color and Borders, the
	 * Layout, Color, enabled state and accessibility state of every row, the
	 * Layout of its leading group and content column, and the color of its
	 * texts and icons. Runs in the Layout phase, after the row content has
	 * been measured.
	 */
	class List: public ecs::SystemFiller<components::List>
	{
		public:
		/// Index path of an entry: top-level index, then child indices.
		using Path = components::List::Path;

		/**
		 * @brief Default constructor.
		 */
		List(void);

		/**
		 * @brief Default destructor.
		 */
		~List(void) override = default;

		/**
		 * @brief Make an entity behave as a list.
		 *
		 * Makes every row already recorded in components::List pressable.
		 * Call after the rows have been recorded; the entity needs its
		 * components::List, Layout, Transform, Bound, Color and Borders.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The list entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Make one row pressable.
		 *
		 * Adds the interaction components to the row recorded in
		 * components::List, wires its press to `pressItem` and its hover and
		 * press changes to `styleRow`. Subheaders and dividers are left
		 * alone. Call once the row exists.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The list entity.
		 * @param path Index path of the row (ignored when it names none).
		 */
		static void installRow(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   const Path &path);

		/**
		 * @brief Style one list, size and place its rows, and show the
		 * children of its expanded accordions only.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The list entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Color one row, its texts and its icons from its enabled,
		 * interaction and folded state.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The list entity.
		 * @param path Index path of the row (ignored when it names none).
		 */
		static void styleRow(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier,
							 const Path &path);

		/**
		 * @brief Activate an entry as a press on its row would: toggle it
		 * when it is a top-level accordion, then call its press handler.
		 * Ignored for a disabled entry and for the child of a collapsed
		 * accordion.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The list entity.
		 * @param path Index path of the entry (ignored when it names none).
		 */
		static void pressItem(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier,
							  const Path &path);

		/**
		 * @brief Enable or disable a top-level entry and restyle the list.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The list entity.
		 * @param index Index of the entry (ignored when out of range).
		 * @param disabled Whether the entry is disabled.
		 */
		static void
			setItemDisabled(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							std::size_t index, bool disabled);

		/**
		 * @brief Expand or collapse a top-level accordion and re-arrange the
		 * list.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The list entity.
		 * @param index Index of the entry (ignored when out of range or when
		 * the entry has no children).
		 * @param expanded Whether the accordion is expanded.
		 */
		static void setExpanded(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								std::size_t index, bool expanded);

		/**
		 * @brief Check whether a top-level accordion is expanded.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The list entity.
		 * @param index Index of the entry.
		 * @return True when the entry exists and is expanded.
		 */
		static bool isExpanded(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   std::size_t index);

		/**
		 * @brief Style the list with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
