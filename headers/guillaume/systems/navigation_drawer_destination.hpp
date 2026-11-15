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
	 * @brief Styles one destination of a navigation drawer and places its
	 * icon, label and trailing text.
	 *
	 * Runs in the Layout phase on every entity that has a
	 * components::NavigationDrawerDestination. It is registered before
	 * systems::NavigationDrawer: the destination is styled on its own
	 * first, then the drawer that owns it gives it its final size and
	 * position.
	 * - **Reads**: components::NavigationDrawerDestination (label, trailing
	 *   text, collapsed and disabled flags, child identifiers), the selected
	 *   flag of the destination's components::Selectable, its hover, press
	 *   and enabled state and the active theme.
	 * - **Writes** on the destination: components::Layout (axis, padding,
	 *   fixed height, child list), components::Borders, components::Color,
	 *   the enabled flag of components::StyleState, and the label and
	 *   selected state of components::Accessibility.
	 * - **Writes** on the children: the Color of the lead group, icon,
	 *   label and trailing text, and the Transform and Bound of all of
	 *   them.
	 *
	 * A destination has two shapes, chosen by the collapsed flag of its
	 * component:
	 * - **regular**: a row holding the lead group (icon and label) and,
	 *   pushed to the far edge, the trailing text. The whole row is the
	 *   active pill;
	 * - **collapsed** (the drawer shown as a rail): the lead group, which
	 *   is then the active pill around the icon, stacked above the label.
	 *
	 * This system does not change the selection: a press only calls the
	 * press callback of the component. systems::NavigationDrawer::install
	 * replaces that callback with its own, which selects the destination.
	 *
	 * @see components::NavigationDrawerDestination,
	 * systems::NavigationDrawer, entities::NavigationDrawerDestination
	 */
	class NavigationDrawerDestination:
		public ecs::SystemFiller<components::NavigationDrawerDestination>
	{
		public:
		/**
		 * @brief Create the system in the Layout phase.
		 */
		NavigationDrawerDestination(void);

		/**
		 * @brief Default destructor.
		 */
		~NavigationDrawerDestination(void) override = default;

		/**
		 * @brief Make a destination pressable.
		 *
		 * Adds the interaction components (hover and press state, focus,
		 * the Button accessibility role) and wires an activation to press()
		 * and a hover or press change to apply(). Call it once, after the
		 * entity has its components::NavigationDrawerDestination, Layout,
		 * Transform, Bound, Color, Borders and Selectable.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the handlers and must outlive the entity.
		 * @param entityIdentifier The destination entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one destination from its selection, enabled and
		 * interaction state, then arrange it.
		 *
		 * This is what update() runs each frame. systems::NavigationDrawer
		 * calls it after sizing the destination, and the interaction
		 * components call it when the pointer enters, leaves or presses the
		 * destination.
		 *
		 * The active pill is filled only for a selected, enabled
		 * destination, and carries the hover or press state layer. A
		 * disabled destination draws its icon and texts with the disabled
		 * content color. The disabled flag of the component is copied to
		 * the enabled flag of components::StyleState on every call, so set
		 * it with setDisabled() rather than on the style state. Ends with
		 * arrange().
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::NavigationDrawerDestination, Layout, Borders or Color.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Place the children of one destination without restyling
		 * it.
		 *
		 * The layout children are, in order, the lead group, the label when
		 * the destination is collapsed (otherwise the label is inside the
		 * lead group) and the trailing text, skipping the ones that do not
		 * exist. The content of the lead group is then placed inside it.
		 * systems::NavigationDrawer calls it after moving the destination,
		 * so the children follow.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 */
		static void arrange(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Activate the destination as a press on it would.
		 *
		 * Calls the press callback of
		 * components::NavigationDrawerDestination when there is one. Does
		 * nothing when the destination is disabled. The selection is not
		 * changed here; see the class description.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 */
		static void press(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Enable or disable the destination and restyle it.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 * @param disabled True to make the destination ignore presses and
		 * draw it dimmed, false to enable it.
		 */
		static void setDisabled(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								bool disabled);

		/**
		 * @brief Whether the destination ignores input.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 * @return True when the disabled flag of
		 * components::NavigationDrawerDestination is set or the entity
		 * itself has been disabled.
		 */
		static bool isDisabled(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the destination is the selected one.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity. May be
		 * ecs::Entity::InvalidIdentifier.
		 * @return True when the entity has a components::Selectable that is
		 * selected; false when it has none.
		 */
		static bool isSelected(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Run apply() on one destination; called by the frame loop.
		 *
		 * Logs a warning and does nothing when the entity has no
		 * components::NavigationDrawerDestination.
		 * @param entityIdentifier The destination entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
