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

#include "guillaume/components/floating_action_button_menu.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Opens and closes a floating action button menu (speed dial): a
	 * trigger button that reveals a column of action buttons above it.
	 *
	 * The menu entity is a vertical layout container. Its children are
	 * buttons built by the widget class (entities::FloatingActionButtonMenu)
	 * and recorded by identifier in components::FloatingActionButtonMenu:
	 * one trigger and one button per action.
	 *
	 * - **Reads**: components::FloatingActionButtonMenu (open flag, trigger
	 *   icon, actions, child identifiers) and the components::Button of
	 *   each child.
	 * - **Writes**: on each action button, the visible flag of
	 *   components::Button and the focusable flag of
	 *   components::Accessibility; on the trigger, the icon glyph (the
	 *   CloseGlyph while open) and the expanded flag of
	 *   components::Accessibility; on the menu, the children of its
	 *   components::Layout (the actions only while open, then the trigger),
	 *   which it then arranges through systems::Layout::apply.
	 * - **Phase**: Layout. It is registered after systems::Layout and after
	 *   the button variant systems, so the buttons it arranges have their
	 *   size for this frame.
	 *
	 * @code
	 * // Close the menu from code, for example after an action ran.
	 * systems::FloatingActionButtonMenu::close(registry, identifier);
	 * @endcode
	 *
	 * The list of children in the menu's components::Layout is overwritten
	 * each frame: add or remove actions through the component (and call
	 * install again), not through the layout.
	 *
	 * @see guillaume::components::FloatingActionButtonMenu,
	 * guillaume::systems::FloatingActionButton, guillaume::systems::Layout
	 */
	class FloatingActionButtonMenu:
		public ecs::SystemFiller<components::FloatingActionButtonMenu>
	{
		public:
		/// Name of the glyph the trigger shows while the menu is open, in
		/// place of the icon configured in the component.
		static constexpr const char *CloseGlyph = "close";

		/**
		 * @brief Create the system for the Layout phase, matching entities
		 * that have a components::FloatingActionButtonMenu.
		 */
		FloatingActionButtonMenu(void);

		/**
		 * @brief Default destructor.
		 */
		~FloatingActionButtonMenu(void) override = default;

		/**
		 * @brief Make an entity behave as a floating action button menu.
		 *
		 * Does three things:
		 * - configures the menu's components::Layout as a vertical column,
		 *   packed at the end, centered across, with
		 *   components::FloatingActionButtonMenu::Spacing between buttons
		 *   and no padding;
		 * - makes a click on the trigger call toggle, and copies the
		 *   accessibility label of the component onto the trigger;
		 * - gives each action button the `onClick` of the action at the
		 *   same index.
		 *
		 * Call it once the trigger and the action buttons exist and are
		 * recorded in the component, and again whenever the actions or
		 * their buttons change. A child identifier that is invalid or has
		 * no components::Button is skipped. It does not show or hide
		 * anything: `apply` does.
		 * @param componentRegistry The registry holding the components. The
		 * trigger handler keeps its address, so it must outlive the menu.
		 * @param entityIdentifier The menu entity. It must have a
		 * components::FloatingActionButtonMenu and a components::Layout.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Bring the buttons in line with the open flag of the
		 * component, then lay out the column.
		 *
		 * Each action button is made visible and focusable when the menu is
		 * open, hidden and not focusable when it is closed, and restyled
		 * when its visibility changed. The trigger shows CloseGlyph while
		 * open and the configured icon otherwise, and reports its expanded
		 * state to accessibility. The layout children become the open
		 * actions followed by the trigger, and systems::Layout::apply
		 * places them at once.
		 *
		 * The frame calls it through `update`; open, close and toggle call
		 * it on a change. Call it yourself after writing
		 * `components::FloatingActionButtonMenu::setOpen` directly, which
		 * changes the flag without calling the state-change callback.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Open the menu: show its actions at once, then call the
		 * state-change callback with `true`.
		 *
		 * Does nothing, and calls nothing, when the menu is already open.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void open(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Close the menu: hide its actions at once, then call the
		 * state-change callback with `false`.
		 *
		 * Does nothing, and calls nothing, when the menu is already closed.
		 * Clicking an action does not close the menu by itself; call this
		 * from the action's handler when that is wanted.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void close(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Open the menu when it is closed, close it when it is open,
		 * and call the state-change callback with the new state.
		 *
		 * This is what a click on the trigger does.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void toggle(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Frame entry point: call `apply` on one menu.
		 * @param entityIdentifier The menu entity. An entity without a
		 * components::FloatingActionButtonMenu is logged and skipped.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		private:
		/**
		 * @brief Shared body of open, close and toggle.
		 *
		 * When the state really changes: writes the open flag, calls
		 * `apply`, then calls the state-change callback of the component
		 * (if any) with the new state. The callback is copied first, so it
		 * may replace itself.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 * @param open True to open the menu, false to close it.
		 */
		static void setOpen(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							bool open);
	};

}	 // namespace guillaume::systems
