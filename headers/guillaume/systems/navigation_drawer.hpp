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
	 * @brief Styles a navigation drawer, stacks its entries in a column
	 * and keeps exactly one of its destinations selected.
	 *
	 * Runs in the Layout phase on every entity that has a
	 * components::NavigationDrawer. It is registered after
	 * systems::NavigationDrawerDestination, so each destination has been
	 * styled on its own before the drawer sizes and places it.
	 * - **Reads**: components::NavigationDrawer (variant, entries, the
	 *   entities built for them, callbacks), the components::Selectable of
	 *   each destination, the visibility of the drawer's
	 *   components::Overlay and the active theme.
	 * - **Writes** on the drawer: components::Layout (vertical, fixed
	 *   width and height, child list), components::Borders,
	 *   components::Color and the modal and dismissal flags of
	 *   components::Overlay, then the Transform and Bound of the entries
	 *   through systems::Layout::apply.
	 * - **Writes** on the children: the fixed width of each destination
	 *   (the rest is styled by systems::NavigationDrawerDestination::apply,
	 *   which this system calls), and the visibility, pose and size of the
	 *   backdrop entity through systems::Backdrop.
	 *
	 * An *entry* is a destination, a section header or a divider. The
	 * `index` taken by the functions below counts destinations only: it is
	 * a position in components::NavigationDrawer::getDestinationEntities,
	 * starting at 0.
	 *
	 * The variant decides the shape: Standard and Modal drawers are
	 * components::NavigationDrawer::Width wide, a Collapsed one is a rail of
	 * components::NavigationDrawer::CollapsedWidth. Only a Modal drawer is
	 * a modal overlay with a backdrop, and only it closes on a click
	 * outside; every variant closes on Escape.
	 *
	 * @code
	 * systems::NavigationDrawer::install(registry, drawer);
	 * systems::NavigationDrawer::selectFirstEnabled(registry, drawer);
	 * systems::NavigationDrawer::show(registry, drawer);
	 * systems::NavigationDrawer::press(registry, drawer, 2); // as a click
	 * @endcode
	 *
	 * @see components::NavigationDrawer,
	 * systems::NavigationDrawerDestination, systems::Selection,
	 * systems::Backdrop
	 */
	class NavigationDrawer:
		public ecs::SystemFiller<components::NavigationDrawer>
	{
		public:
		/// Space between the edge of the drawer and its entries: 12 dp.
		static constexpr float Padding = theme::SpacingScale::get(3);

		/// Gap between two stacked entries: 4 dp.
		static constexpr float Spacing = theme::SpacingScale::get(1);

		/**
		 * @brief Create the system in the Layout phase.
		 */
		NavigationDrawer(void);

		/**
		 * @brief Default destructor.
		 */
		~NavigationDrawer(void) override = default;

		/**
		 * @brief Connect a navigation drawer to its destinations and section
		 * headers.
		 *
		 * Forbids an empty selection in the drawer's
		 * components::SelectionGroup. For every destination recorded in
		 * components::NavigationDrawer, replaces its press callback with a
		 * call to press() and its selection-changed handler with a call to
		 * destinationSelected(). Gives every section header entity the
		 * Header accessibility role and its title as label (adding a
		 * components::Accessibility when it has none). The handlers capture
		 * the index of the destination, so call install() again each time
		 * the children change.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the handlers and must outlive the entity.
		 * @param entityIdentifier The navigation drawer entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity has
		 * no components::NavigationDrawer or components::SelectionGroup.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Shape and color the drawer, style its destinations, stack
		 * its entries and place its backdrop.
		 *
		 * This is what update() runs each frame. Call it directly after
		 * changing components::NavigationDrawer to see the result before
		 * the next frame, which is what the setters of
		 * entities::NavigationDrawer do.
		 *
		 * The drawer is attached to the leading edge, so only its trailing
		 * corners are rounded (none are for a Collapsed drawer). Each
		 * destination is as wide as the drawer minus Padding on both sides.
		 * The backdrop, when the component records one, is visible only for
		 * a Modal drawer whose overlay is visible, and is given the pose and
		 * size of the drawer.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::NavigationDrawer, Layout, Borders, Color or Overlay.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the drawer width of a variant, in dp.
		 * @param variant The drawer variant.
		 * @return components::NavigationDrawer::CollapsedWidth (80) for the
		 * Collapsed variant, components::NavigationDrawer::Width (320)
		 * otherwise.
		 */
		static float widthOf(components::NavigationDrawer::Variant variant);

		/**
		 * @brief Select a destination from code.
		 *
		 * Selects the destination, deselects the others and restyles the
		 * drawer. The index-change callback is not called, and a disabled
		 * destination is selected like any other: use press() to behave as
		 * the user would.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 * @param index Index of the destination to select, sections and
		 * dividers not counted. An index past the end is ignored.
		 */
		static void select(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   std::size_t index);

		/**
		 * @brief Select the first destination that is not disabled, then
		 * restyle.
		 *
		 * Use it to give a drawer its initial selection. When every
		 * destination is disabled, the selection is left as it is and the
		 * drawer is only restyled. No callback is called.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 */
		static void
			selectFirstEnabled(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Activate a destination as a press on it would.
		 *
		 * Selects the destination, then calls the index-change callback of
		 * components::NavigationDrawer when the selected index is different
		 * from the one before the press. Does nothing for a disabled
		 * destination.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 * @param index Index of the pressed destination, sections and
		 * dividers not counted. An index past the end is ignored.
		 */
		static void press(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier,
						  std::size_t index);

		/**
		 * @brief React to a destination that became selected without going
		 * through the drawer.
		 *
		 * Called by the selection-changed handler wired by install(), for
		 * instance when code selects the components::Selectable of a
		 * destination directly. Deselects the other destinations and
		 * restyles the drawer. Does nothing while select() is itself
		 * changing the selection, which is what keeps the two from calling
		 * each other for ever.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 * @param index Index of the destination that became selected. An
		 * index past the end is ignored.
		 */
		static void
			destinationSelected(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								std::size_t index);

		/**
		 * @brief Enable or disable one destination and restyle it.
		 *
		 * Stores the flag in components::NavigationDrawer and forwards it to
		 * systems::NavigationDrawerDestination::setDisabled. A disabled
		 * destination that is selected stays selected.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 * @param index Index of the destination, sections and dividers not
		 * counted. An index past the end is ignored.
		 * @param disabled True to disable the destination, false to enable
		 * it.
		 */
		static void setDestinationDisabled(
			ecs::ComponentRegistry &componentRegistry,
			const ecs::Entity::Identifier &entityIdentifier, std::size_t index,
			bool disabled);

		/**
		 * @brief Show the drawer, and its backdrop when it is modal.
		 *
		 * Sets the `visible` flag of the drawer's components::Overlay and
		 * calls apply().
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 */
		static void show(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Hide the drawer and its backdrop.
		 *
		 * Clears the `visible` flag of the drawer's components::Overlay and
		 * calls apply(). The dismiss handler of the overlay is not called.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 */
		static void hide(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the index of the selected destination.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The navigation drawer entity.
		 * @return The index of the first destination whose
		 * components::Selectable is selected, sections and dividers not
		 * counted. Also 0 when no destination is selected, so 0 alone does
		 * not prove that the first one is.
		 */
		static std::size_t
			selectedIndex(const ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Run apply() on one navigation drawer; called by the frame
		 * loop.
		 *
		 * Logs a warning and does nothing when the entity has no
		 * components::NavigationDrawer.
		 * @param entityIdentifier The navigation drawer entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
