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

#include "guillaume/components/snackbar.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Styles a snackbar (a short message with an optional action
	 * that disappears on its own) and handles its dismissal.
	 *
	 * Runs in the Layout phase on every entity that has a
	 * components::Snackbar. It is registered after systems::Layout, so the
	 * message and action are already measured when it arranges them.
	 * - **Reads**: components::Snackbar (variant, message, action label,
	 *   duration, visible flag, callbacks, child identifiers) and the
	 *   active theme.
	 * - **Writes** on the bar: components::Layout (a row, fixed height,
	 *   child list), components::Borders, components::Color,
	 *   components::Elevation, the label of components::Accessibility and
	 *   the flags, auto-dismiss delay and visibility of
	 *   components::Overlay, then the Transform and Bound of the children
	 *   through systems::Layout::apply.
	 * - **Writes** on the children: the Text and Color of the message and
	 *   the Color of the action.
	 *
	 * The `visible` flag of components::Snackbar is the source of truth:
	 * apply() copies it to the snackbar's components::Overlay. A hidden
	 * snackbar is not removed, it is painted transparent with no elevation.
	 *
	 * This system does not count time. apply() gives the duration, in
	 * seconds, to the overlay as its auto-dismiss delay; systems::Overlay
	 * runs the countdown and handles Escape, and reports both through the
	 * overlay's dismiss handler, which install() points at dismiss().
	 *
	 * @code
	 * systems::Snackbar::install(registry, snackbar);
	 * systems::Snackbar::show(registry, snackbar); // countdown starts
	 * @endcode
	 *
	 * @see components::Snackbar, components::Overlay, entities::Snackbar,
	 * systems::Overlay
	 */
	class Snackbar: public ecs::SystemFiller<components::Snackbar>
	{
		public:
		/**
		 * @brief Create the system in the Layout phase.
		 */
		Snackbar(void);

		/**
		 * @brief Default destructor.
		 */
		~Snackbar(void) override = default;

		/**
		 * @brief Give a snackbar its accessibility semantics and connect
		 * its dismissal.
		 *
		 * Sets the role of its components::Accessibility to Alert with a
		 * polite live region, makes the dismiss handler of its
		 * components::Overlay call dismiss(), then calls installAction().
		 * Call it once, after the entity has its components::Snackbar,
		 * Overlay, Elevation, Accessibility, Layout, Transform, Bound, Color
		 * and Borders.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the handlers and must outlive the entity.
		 * @param entityIdentifier The snackbar entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::Accessibility, Overlay or Snackbar.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Make the action child a button that calls pressAction().
		 *
		 * Adds the interaction components to the action child recorded in
		 * components::Snackbar and gives it the action label as accessible
		 * name. Call it when the action child is created after install().
		 * Does nothing when no action child is recorded.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the press handler and must outlive the entity.
		 * @param entityIdentifier The snackbar entity.
		 */
		static void
			installAction(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one snackbar and arrange its children.
		 *
		 * This is what update() runs each frame. Call it directly after
		 * changing components::Snackbar to see the result before the next
		 * frame, which is what the setters of entities::Snackbar do.
		 *
		 * The bar is 48 dp high, or 68 dp for the MultiLine variant, with
		 * the message at one end and the action at the other. When the
		 * visible flag of the component and the one of the overlay differ,
		 * the overlay's elapsed time is reset to 0 before its flag is
		 * updated, which is what restarts the auto-dismiss countdown each
		 * time the snackbar is shown.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The snackbar entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * one of the components listed in install().
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Show the snackbar and restart its auto-dismiss countdown.
		 *
		 * Sets the visible flag of components::Snackbar and calls apply().
		 * Showing a snackbar that is already shown does not restart the
		 * countdown.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The snackbar entity.
		 */
		static void show(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Hide the snackbar without telling anyone.
		 *
		 * Clears the visible flag of components::Snackbar and calls
		 * apply(). The dismiss callback is not called: it is reserved for
		 * dismissals the application did not ask for.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The snackbar entity.
		 */
		static void hide(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Close the snackbar as the timeout, Escape or its action
		 * would.
		 *
		 * Hides the snackbar, then calls the dismiss callback of
		 * components::Snackbar when there is one. Does nothing when the
		 * snackbar is already hidden.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The snackbar entity.
		 */
		static void dismiss(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Activate the action as a press on it would.
		 *
		 * Calls the action callback of components::Snackbar when there is
		 * one, then dismiss(), so the dismiss callback is called too. Does
		 * nothing when the snackbar is hidden.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The snackbar entity.
		 */
		static void
			pressAction(ecs::ComponentRegistry &componentRegistry,
						const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Run apply() on one snackbar; called by the frame loop.
		 *
		 * Logs a warning and does nothing when the entity has no
		 * components::Snackbar.
		 * @param entityIdentifier The snackbar entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
