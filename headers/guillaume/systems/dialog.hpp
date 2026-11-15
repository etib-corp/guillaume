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

#include "guillaume/components/dialog.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Styles a dialog panel, stacks its icon, title, message and
	 * action row, and shows or hides the backdrop behind it.
	 *
	 * Runs in the Layout phase on every entity that has a
	 * components::Dialog. It is registered after systems::Layout, so the
	 * children are already measured when it arranges them.
	 * - **Reads**: components::Dialog (variant, title, message, actions,
	 *   child identifiers), the visibility of the dialog's
	 *   components::Overlay and the active theme.
	 * - **Writes** on the panel: components::Layout (vertical, 320 dp wide),
	 *   components::Borders, components::Color, components::Elevation, the
	 *   label of components::Accessibility and the modal and dismissal
	 *   flags of components::Overlay, then the Transform and Bound of the
	 *   children through systems::Layout::apply.
	 * - **Writes** on the children: the Text and Color of the title and
	 *   message, the Color of the icon, and the visibility, pose and size
	 *   of the backdrop entity through systems::Backdrop.
	 *
	 * Whether the dialog is shown is the `visible` flag of its
	 * components::Overlay; show(), hide() and dismiss() change it. A dialog
	 * is always modal. It closes on Escape, and on a click outside unless
	 * its variant is FullScreen.
	 *
	 * @code
	 * systems::Dialog::show(registry, dialog);
	 * // ... later, from a "Cancel" action:
	 * systems::Dialog::dismiss(registry, dialog);
	 * @endcode
	 *
	 * @see components::Dialog, components::Overlay, entities::Dialog,
	 * systems::Backdrop
	 */
	class Dialog: public ecs::SystemFiller<components::Dialog>
	{
		public:
		/// Space between the edge of the panel and its content: 24 dp.
		static constexpr float Padding = theme::SpacingScale::get(6);

		/// Gap between two stacked sections of the dialog: 16 dp.
		static constexpr float Spacing = theme::SpacingScale::get(4);

		/**
		 * @brief Create the system in the Layout phase.
		 */
		Dialog(void);

		/**
		 * @brief Default destructor.
		 */
		~Dialog(void) override = default;

		/**
		 * @brief Give a dialog panel its accessibility semantics.
		 *
		 * Sets the role of the panel's components::Accessibility to Dialog,
		 * makes the panel itself non-focusable, then calls installTitle().
		 * Call it once, after the entity has its components::Dialog,
		 * Overlay, Elevation, Accessibility, Layout, Transform, Bound, Color
		 * and Borders.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The dialog entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity has
		 * no components::Accessibility or components::Dialog.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Expose the title child to assistive technology as a header.
		 *
		 * Adds a components::Accessibility to the title entity recorded in
		 * components::Dialog when it has none, sets its role to Header and
		 * its label to the title text, and makes it non-focusable. Call it
		 * again when the title child is created after install(). Does
		 * nothing when no title child is recorded.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The dialog entity.
		 */
		static void
			installTitle(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one dialog, arrange its children and place its
		 * backdrop.
		 *
		 * This is what update() runs each frame. Call it directly after
		 * changing components::Dialog to see the result before the next
		 * frame, which is what the setters of entities::Dialog do.
		 *
		 * The children are stacked in the order icon, title, message, action
		 * row, skipping the ones that do not exist; they are centered
		 * horizontally when the dialog has an icon and start-aligned
		 * otherwise. A FullScreen dialog has a fixed height of
		 * components::Dialog::FullScreenHeight, square corners and no
		 * backdrop. The backdrop is visible only while the dialog is, and is
		 * given the pose and size of the panel.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The dialog entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * one of the components listed in install().
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Show the dialog and its backdrop.
		 *
		 * Sets the `visible` flag of the dialog's components::Overlay and
		 * calls apply().
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The dialog entity.
		 */
		static void show(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Hide the dialog and its backdrop without telling anyone.
		 *
		 * Clears the `visible` flag of the dialog's components::Overlay and
		 * calls apply(). The dismiss handler is not called: use dismiss()
		 * when the application must be notified.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The dialog entity.
		 */
		static void hide(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Close the dialog as the user would with Escape or a click
		 * outside.
		 *
		 * Hides the dialog, then calls the dismiss handler of its
		 * components::Overlay when there is one. Does nothing when the
		 * dialog is already hidden.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The dialog entity.
		 */
		static void dismiss(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Activate an action as a press on its button would.
		 *
		 * Calls the press handler of the action when it has one. The dialog
		 * stays open: a handler that should close it calls hide() or
		 * dismiss() itself.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The dialog entity.
		 * @param index Position of the action in the action list of
		 * components::Dialog, starting at 0. An index past the end is
		 * ignored.
		 */
		static void pressAction(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								std::size_t index);

		/**
		 * @brief Run apply() on one dialog; called by the frame loop.
		 *
		 * Logs a warning and does nothing when the entity has no
		 * components::Dialog.
		 * @param entityIdentifier The dialog entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
