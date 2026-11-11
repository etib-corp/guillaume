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

#include <string>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/navigation_bar_destination.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Styles and arranges one destination of a navigation bar.
	 *
	 * Reads components::NavigationBarDestination and the selection of the
	 * destination, and writes the color of its active indicator, the glyph
	 * and color of its icon, the color of its label, the variant and label of
	 * its badge, its semantics, and the placement of those children. Runs in
	 * the Layout phase, after the children have been measured.
	 */
	class NavigationBarDestination:
		public ecs::SystemFiller<components::NavigationBarDestination>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		NavigationBarDestination(void);

		/**
		 * @brief Default destructor.
		 */
		~NavigationBarDestination(void) override = default;

		/**
		 * @brief Make an entity behave as a navigation bar destination.
		 *
		 * Adds the interaction components with the Tab role and wires a
		 * press to press() and a hover or press change to apply(). The entity
		 * must have a components::NavigationBarDestination, a Layout, a
		 * Transform, a Bound, a Color, a Borders and a Selectable.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style the destination for its selection, then arrange it.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Place the indicator and the label in a column, the icon in
		 * the indicator and the badge on the corner of the icon.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 */
		static void arrange(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief React to the user pressing the destination: call its press
		 * callback.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 */
		static void press(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Set the badge label and restyle. An existing badge child
		 * shows the label, or a dot when the label is empty.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 * @param badge The badge label.
		 */
		static void setBadge(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier,
							 const std::string &badge);

		/**
		 * @brief Whether the destination is selected.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity (may be invalid).
		 * @return True when its components::Selectable is selected.
		 */
		static bool isSelected(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style and arrange the destination with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
