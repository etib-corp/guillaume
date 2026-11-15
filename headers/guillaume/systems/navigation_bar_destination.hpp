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
	 * @brief Styles one destination of a navigation bar (an icon in a pill
	 * with a label under it and an optional badge) and places those
	 * children.
	 *
	 * Runs in the Layout phase on every entity that has a
	 * components::NavigationBarDestination. It is registered before
	 * systems::NavigationBar: the destination is styled on its own first,
	 * then the bar that owns it gives it its final size and position and
	 * calls apply() and arrange() again.
	 * - **Reads**: components::NavigationBarDestination (label, icons,
	 *   badge label, show-label flag, child identifiers), the selected flag
	 *   of the destination's components::Selectable, its hover and press
	 *   state and the active theme.
	 * - **Writes** on the destination: components::Layout (a centered
	 *   column and its child list), a transparent components::Color and
	 *   components::Borders, and the label and selected state of its
	 *   components::Accessibility.
	 * - **Writes** on the children: the Color of the indicator pill, the
	 *   Glyph name and Color of the icon, the Color of the label, the
	 *   variant and label of the badge's components::Badge, and the
	 *   Transform and Bound of all of them.
	 *
	 * This system does not change the selection: a press only calls the
	 * press callback of the component. systems::NavigationBar::install
	 * replaces that callback with its own, which selects the destination.
	 *
	 * @see components::NavigationBarDestination, systems::NavigationBar,
	 * entities::NavigationBarDestination
	 */
	class NavigationBarDestination:
		public ecs::SystemFiller<components::NavigationBarDestination>
	{
		public:
		/**
		 * @brief Create the system in the Layout phase.
		 */
		NavigationBarDestination(void);

		/**
		 * @brief Default destructor.
		 */
		~NavigationBarDestination(void) override = default;

		/**
		 * @brief Make a destination pressable.
		 *
		 * Adds the interaction components (hover and press state, focus,
		 * the Tab accessibility role) and wires an activation to press()
		 * and a hover or press change to apply(). Call it once, after the
		 * entity has its components::NavigationBarDestination, Layout,
		 * Transform, Bound, Color, Borders and Selectable.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the handlers and must outlive the entity.
		 * @param entityIdentifier The destination entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one destination from its selection and interaction
		 * state, then arrange it.
		 *
		 * This is what update() runs each frame. systems::NavigationBar
		 * calls it after sizing the destination, and the interaction
		 * components call it when the pointer enters, leaves or presses the
		 * destination.
		 *
		 * A selected destination has a filled indicator pill and shows its
		 * focused icon when it has one. The hover or press state layer is
		 * drawn on the pill. A label that is not shown is painted
		 * transparent and keeps its place in the column. The badge child,
		 * when it exists, shows the badge label, or a dot when the label is
		 * empty. Ends with arrange().
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::NavigationBarDestination, Layout, Color or Borders.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Place the children of one destination without restyling
		 * it.
		 *
		 * Stacks the indicator and the label in a column, places the icon
		 * inside the indicator, and puts the badge at the offset
		 * (components::NavigationBarDestination::BadgeOffsetX, BadgeOffsetY)
		 * from the origin of the indicator, over the corner of the icon.
		 * systems::NavigationBar calls it after moving the destination, so
		 * the children follow.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 */
		static void arrange(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Activate the destination as a press on it would.
		 *
		 * Calls the press callback of
		 * components::NavigationBarDestination when there is one. The
		 * selection is not changed here; see the class description.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 */
		static void press(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Set the badge label and restyle the destination.
		 *
		 * Only the content of an existing badge child changes: this does
		 * not create or remove the badge entity.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The destination entity.
		 * @param badge The text shown in the badge, such as a count. An
		 * empty string shows a dot.
		 */
		static void setBadge(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier,
							 const std::string &badge);

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
		 * components::NavigationBarDestination.
		 * @param entityIdentifier The destination entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
