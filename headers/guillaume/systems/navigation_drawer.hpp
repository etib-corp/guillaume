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

#include "guillaume/components/navigation_drawer.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Styles a navigation drawer and arranges its entries.
	 *
	 * Reads components::NavigationDrawer, the selection of its destination
	 * children and its components::Overlay, and writes the sheet geometry,
	 * corners and color of the variant, the width of the destinations, the
	 * column the entries are placed in, the overlay flags and the modal
	 * backdrop. Each destination is driven by identifier through
	 * systems::NavigationDrawerDestination and the backdrop through
	 * systems::Backdrop. Runs in the Layout phase.
	 */
	class NavigationDrawer:
		public ecs::SystemFiller<components::NavigationDrawer>
	{
		public:
		/// Space around the entries, in dp.
		static constexpr float Padding = theme::SpacingScale::get(3);

		/// Gap between entries, in dp.
		static constexpr float Spacing = theme::SpacingScale::get(1);

		/**
		 * @brief Default constructor.
		 */
		NavigationDrawer(void);

		/**
		 * @brief Default destructor.
		 */
		~NavigationDrawer(void) override = default;

		/**
		 * @brief Make an entity behave as a navigation drawer.
		 *
		 * Forbids an empty selection, wires the press and the selection
		 * change of every destination recorded in the component to this
		 * system, and gives every section header child the Header role and
		 * its title. Call it again after the children change. The entity
		 * must have a components::NavigationDrawer and a
		 * components::SelectionGroup.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Apply the geometry and colors of the variant, style the
		 * destinations, place the entries in a column and drive the overlay
		 * flags and the backdrop.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the drawer width of a variant.
		 * @param variant The variant.
		 * @return The width, in dp.
		 */
		static float widthOf(components::NavigationDrawer::Variant variant);

		/**
		 * @brief Select a destination programmatically (no callback); the
		 * others are deselected.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 * @param index The destination index: sections and dividers are not
		 * counted (ignored when out of range).
		 */
		static void select(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   std::size_t index);

		/**
		 * @brief Select the first enabled destination, then restyle.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 */
		static void
			selectFirstEnabled(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief React to the user pressing a destination: select it and,
		 * when the selection changed, call the index change callback. A
		 * disabled destination is ignored.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 * @param index The destination index (ignored when out of range).
		 */
		static void press(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier,
						  std::size_t index);

		/**
		 * @brief React to a destination becoming selected from outside the
		 * drawer: deselect the others and restyle. Does nothing while the
		 * drawer is changing the selection itself.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 * @param index The destination index (ignored when out of range).
		 */
		static void
			destinationSelected(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								std::size_t index);

		/**
		 * @brief Enable or disable one destination.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 * @param index The destination index.
		 * @param disabled True to disable.
		 */
		static void setDestinationDisabled(
			ecs::ComponentRegistry &componentRegistry,
			const ecs::Entity::Identifier &entityIdentifier, std::size_t index,
			bool disabled);

		/**
		 * @brief Show the drawer, and its backdrop when it is modal.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 */
		static void show(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Hide the drawer and its backdrop.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 */
		static void hide(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the index of the selected destination.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 * @return The index of the first selected destination, or 0.
		 */
		static std::size_t
			selectedIndex(const ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style and arrange the navigation drawer with the given
		 * identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
