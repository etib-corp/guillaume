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
	 * @brief Opens and closes a floating action button menu (speed dial).
	 *
	 * Reads components::FloatingActionButtonMenu and writes the visibility of
	 * the action buttons, the trigger's icon and expanded state, and the
	 * column of children the Layout system arranges (the actions only while
	 * the menu is open). Runs in the Layout phase, after the buttons have
	 * been styled.
	 */
	class FloatingActionButtonMenu:
		public ecs::SystemFiller<components::FloatingActionButtonMenu>
	{
		public:
		/// Glyph shown on the trigger while the menu is open.
		static constexpr const char *CloseGlyph = "close";

		/**
		 * @brief Default constructor.
		 */
		FloatingActionButtonMenu(void);

		/**
		 * @brief Default destructor.
		 */
		~FloatingActionButtonMenu(void) override = default;

		/**
		 * @brief Make an entity behave as a floating action button menu.
		 *
		 * Configures the column layout, labels the trigger recorded in the
		 * component and wires its click to toggle(), and gives every action
		 * button the click handler of its action. Call it again after the
		 * button children change. The entity must have a
		 * components::FloatingActionButtonMenu and a components::Layout.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Show or hide the actions, update the trigger and lay out the
		 * column.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Open the menu and call the state-change callback, unless it
		 * is already open.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void open(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Close the menu and call the state-change callback, unless it
		 * is already closed.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void close(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Open the menu when closed, close it when open.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 */
		static void toggle(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Refresh the menu with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		private:
		/**
		 * @brief Set the open state; on a change, restyle and call the
		 * state-change callback.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The menu entity.
		 * @param open The new open state.
		 */
		static void setOpen(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							bool open);
	};

}	 // namespace guillaume::systems
