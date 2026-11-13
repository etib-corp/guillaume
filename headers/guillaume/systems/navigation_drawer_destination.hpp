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

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/navigation_drawer_destination.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Styles and arranges one destination of a navigation drawer.
	 *
	 * Reads components::NavigationDrawerDestination and the selection of the
	 * destination, and writes its row or rail layout, its enabled state, the
	 * active pill, the color of its icon, label and trailing text, its
	 * semantics, and the placement of those children. Runs in the Layout
	 * phase, after the children have been measured.
	 */
	class NavigationDrawerDestination:
		public ecs::SystemFiller<components::NavigationDrawerDestination>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		NavigationDrawerDestination(void);

		/**
		 * @brief Default destructor.
		 */
		~NavigationDrawerDestination(void) override = default;

		/**
		 * @brief Make an entity behave as a navigation drawer destination.
		 *
		 * Adds the interaction components with the Button role and wires a
		 * press to press() and a hover or press change to apply(). The entity
		 * must have a components::NavigationDrawerDestination, a Layout, a
		 * Transform, a Bound, a Color, a Borders and a Selectable.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style the destination for its selection and enabled state,
		 * then arrange it.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Place the lead group and the trailing text in a row, or the
		 * indicator above the label for a collapsed item, and the content of
		 * the lead group inside it.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 */
		static void arrange(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief React to the user pressing the destination: call its press
		 * callback, unless it is disabled.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 */
		static void press(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Enable or disable the destination and restyle it.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 * @param disabled True to disable.
		 */
		static void setDisabled(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								bool disabled);

		/**
		 * @brief Whether the destination is disabled.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 * @return True when its configuration or its style state disables it.
		 */
		static bool isDisabled(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

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
