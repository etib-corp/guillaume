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
	 * @brief Styles a navigation bar, shares its width between its
	 * destinations and keeps exactly one of them selected.
	 *
	 * Runs in the Layout phase on every entity that has a
	 * components::NavigationBar. It is registered after
	 * systems::NavigationBarDestination, so each destination has been
	 * styled on its own before the bar sizes and places it.
	 * - **Reads**: components::NavigationBar (variant, destination
	 *   entities, callbacks), the components::Selectable of each
	 *   destination, the bar's fixed width or Bound, and the active theme.
	 * - **Writes** on the bar: components::Color and the child list of
	 *   components::Layout, then the Transform and Bound of the
	 *   destinations through systems::Layout::apply.
	 * - **Writes** on each destination: the fixed width and height of its
	 *   components::Layout and the show-label flag of its
	 *   components::NavigationBarDestination; the rest of the destination
	 *   is styled by systems::NavigationBarDestination::apply, which this
	 *   system calls.
	 *
	 * Destinations are named by their index in
	 * components::NavigationBar::getDestinationEntities, starting at 0. A
	 * child without a components::NavigationBarDestination is placed in the
	 * row but otherwise ignored.
	 *
	 * The bar is 80 dp high (components::NavigationBar::Height). With the
	 * WithLabels variant every destination shows its label; with the other
	 * variant only the selected one does.
	 *
	 * @code
	 * systems::NavigationBar::install(registry, bar);
	 * systems::NavigationBar::select(registry, bar, 1); // no callback
	 * systems::NavigationBar::press(registry, bar, 2);  // as a click would
	 * @endcode
	 *
	 * @see components::NavigationBar, systems::NavigationBarDestination,
	 * systems::Selection, entities::NavigationBar
	 */
	class NavigationBar: public ecs::SystemFiller<components::NavigationBar>
	{
		public:
		/**
		 * @brief Create the system in the Layout phase.
		 */
		NavigationBar(void);

		/**
		 * @brief Default destructor.
		 */
		~NavigationBar(void) override = default;

		/**
		 * @brief Connect a navigation bar to its destinations.
		 *
		 * Forbids an empty selection in the bar's
		 * components::SelectionGroup, gives the bar the TabList
		 * accessibility role (adding a components::Accessibility when it
		 * has none), and for every destination recorded in
		 * components::NavigationBar replaces its press callback with a call
		 * to press() and its selection-changed handler with a call to
		 * destinationSelected(). The handlers capture the index of the
		 * destination, so call install() again each time the destination
		 * children change.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the handlers and must outlive the entity.
		 * @param entityIdentifier The navigation bar entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity has
		 * no components::NavigationBar or components::SelectionGroup.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Color the bar, size and style its destinations and place
		 * them in a row.
		 *
		 * This is what update() runs each frame. Call it directly after
		 * changing components::NavigationBar to see the result before the
		 * next frame, which is what the setters of entities::NavigationBar
		 * do.
		 *
		 * The width of the bar minus its padding is divided equally between
		 * the destinations. The width used is the fixed width of the bar's
		 * components::Layout when it has one, otherwise the width of its
		 * components::Bound.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation bar entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::NavigationBar, Color or Layout.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Select a destination from code.
		 *
		 * Selects the destination, deselects the others and restyles the
		 * bar. Neither the press callback nor the index-change callback is
		 * called: use press() to behave as the user would.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation bar entity.
		 * @param index Index of the destination to select. An index past
		 * the end is ignored.
		 */
		static void select(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   std::size_t index);

		/**
		 * @brief Activate a destination as a press on it would.
		 *
		 * In this order: calls the press callback of
		 * components::NavigationBar with the index, selects the
		 * destination, then calls the index-change callback when the
		 * selected index is different from the one before the press. The
		 * press callback is therefore also called when the user presses the
		 * destination that is already selected.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation bar entity.
		 * @param index Index of the pressed destination. An index past the
		 * end is ignored and no callback is called.
		 */
		static void press(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier,
						  std::size_t index);

		/**
		 * @brief React to a destination that became selected without going
		 * through the bar.
		 *
		 * Called by the selection-changed handler wired by install(), for
		 * instance when code selects the components::Selectable of a
		 * destination directly. Deselects the other destinations and
		 * restyles the bar. Does nothing while select() is itself changing
		 * the selection, which is what keeps the two from calling each
		 * other for ever.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation bar entity.
		 * @param index Index of the destination that became selected. An
		 * index past the end is ignored.
		 */
		static void
			destinationSelected(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								std::size_t index);

		/**
		 * @brief Set the badge of a destination and restyle that
		 * destination.
		 *
		 * Stores the label in components::NavigationBar and forwards it to
		 * systems::NavigationBarDestination::setBadge.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation bar entity.
		 * @param index Index of the destination. An index past the end is
		 * ignored.
		 * @param badge The text shown in the badge, such as a count. An
		 * empty string shows a dot.
		 */
		static void setBadge(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier,
							 std::size_t index, const std::string &badge);

		/**
		 * @brief Get the index of the selected destination.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation bar entity.
		 * @return The index of the first destination whose
		 * components::Selectable is selected. Also 0 when no destination is
		 * selected, so 0 alone does not prove that the first one is.
		 */
		static std::size_t
			selectedIndex(const ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Run apply() on one navigation bar; called by the frame
		 * loop.
		 *
		 * Logs a warning and does nothing when the entity has no
		 * components::NavigationBar.
		 * @param entityIdentifier The navigation bar entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
