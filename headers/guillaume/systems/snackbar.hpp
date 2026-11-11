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
	 * @brief Styles a snackbar and runs its dismissal.
	 *
	 * Reads components::Snackbar and the active theme, and writes the bar's
	 * Layout, Color, Borders, Elevation, Overlay and Accessibility, plus the
	 * content and color of the message and action children. The auto-dismiss
	 * countdown itself is run by systems::Overlay. Runs in the Layout phase,
	 * after the children have been measured.
	 */
	class Snackbar: public ecs::SystemFiller<components::Snackbar>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		Snackbar(void);

		/**
		 * @brief Default destructor.
		 */
		~Snackbar(void) override = default;

		/**
		 * @brief Make an entity behave as a snackbar.
		 *
		 * Sets the accessibility semantics, wires the overlay dismissal to
		 * this system and, when the action child is already known, makes it
		 * pressable. Call once, after the entity has its
		 * components::Snackbar, Overlay, Elevation, Accessibility, Layout,
		 * Transform, Bound, Color and Borders.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The snackbar entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Make the action child pressable.
		 *
		 * Adds the interaction components to the action child recorded in
		 * components::Snackbar and wires its press to `pressAction`. Does
		 * nothing without an action child. Call once the child exists.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The snackbar entity.
		 */
		static void
			installAction(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one snackbar and arrange its children.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The snackbar entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Show the snackbar and restart its auto-dismiss countdown.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The snackbar entity.
		 */
		static void show(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Hide the snackbar without calling the dismiss callback.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The snackbar entity.
		 */
		static void hide(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Dismiss as a timeout, Escape or the action would: hide the
		 * snackbar, then call the dismiss callback. Ignored while hidden.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The snackbar entity.
		 */
		static void dismiss(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Activate the action as a press would: call the action
		 * callback, then dismiss. Ignored while hidden.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The snackbar entity.
		 */
		static void
			pressAction(ecs::ComponentRegistry &componentRegistry,
						const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style the snackbar with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
