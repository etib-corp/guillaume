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
#include <string>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/navigation_bar.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Styles a navigation bar and arranges its destinations.
	 *
	 * Reads components::NavigationBar and the selection of its destination
	 * children, and writes the bar color, the common size of the
	 * destinations, whether each of them shows its label, and the row the
	 * destinations are placed in. Each destination is driven by identifier
	 * through systems::NavigationBarDestination. Runs in the Layout phase.
	 */
	class NavigationBar: public ecs::SystemFiller<components::NavigationBar>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		NavigationBar(void);

		/**
		 * @brief Default destructor.
		 */
		~NavigationBar(void) override = default;

		/**
		 * @brief Make an entity behave as a navigation bar.
		 *
		 * Forbids an empty selection, gives the bar the TabList role, and
		 * wires the press and the selection change of every destination
		 * recorded in the component to this system. Call it again after the
		 * destination children change. The entity must have a
		 * components::NavigationBar and a components::SelectionGroup.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation bar entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Color the bar, size and style its destinations and place
		 * them in a row.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation bar entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Select a destination programmatically (no callback); the
		 * others are deselected.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation bar entity.
		 * @param index The destination index (ignored when out of range).
		 */
		static void select(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   std::size_t index);

		/**
		 * @brief React to the user pressing a destination: call the press
		 * callback, select the destination and, when the selection changed,
		 * call the index change callback.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation bar entity.
		 * @param index The destination index (ignored when out of range).
		 */
		static void press(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier,
						  std::size_t index);

		/**
		 * @brief React to a destination becoming selected from outside the
		 * bar: deselect the others and restyle. Does nothing while the bar
		 * is changing the selection itself.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation bar entity.
		 * @param index The destination index (ignored when out of range).
		 */
		static void
			destinationSelected(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								std::size_t index);

		/**
		 * @brief Set the badge label of a destination.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation bar entity.
		 * @param index The destination index (ignored when out of range).
		 * @param badge The badge label.
		 */
		static void setBadge(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier,
							 std::size_t index, const std::string &badge);

		/**
		 * @brief Get the index of the selected destination.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation bar entity.
		 * @return The index of the first selected destination, or 0.
		 */
		static std::size_t
			selectedIndex(const ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style and arrange the navigation bar with the given
		 * identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
