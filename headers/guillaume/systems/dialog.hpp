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
	 * @brief Styles a dialog, arranges its content and drives its backdrop.
	 *
	 * Reads components::Dialog, the dialog's components::Overlay visibility
	 * and the active theme, and writes the panel's Layout, Color, Borders,
	 * Elevation, Overlay and Accessibility, the content and color of the
	 * icon, title and message children, and the visibility and pose of the
	 * backdrop. Runs in the Layout phase, after the children have been
	 * measured.
	 */
	class Dialog: public ecs::SystemFiller<components::Dialog>
	{
		public:
		/// M3 dialog padding, in dp.
		static constexpr float Padding = theme::SpacingScale::get(6);

		/// Gap between dialog sections, in dp.
		static constexpr float Spacing = theme::SpacingScale::get(4);

		/**
		 * @brief Default constructor.
		 */
		Dialog(void);

		/**
		 * @brief Default destructor.
		 */
		~Dialog(void) override = default;

		/**
		 * @brief Make an entity behave as a dialog.
		 *
		 * Sets the accessibility semantics of the panel. Call once, after
		 * the entity has its components::Dialog, Overlay, Elevation,
		 * Accessibility, Layout, Transform, Bound, Color and Borders.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The dialog entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Mark the title child as a header for accessibility.
		 *
		 * Does nothing without a title child. Call once the child recorded
		 * in components::Dialog exists.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The dialog entity.
		 */
		static void
			installTitle(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one dialog, arrange its children and place its
		 * backdrop.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The dialog entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Show the dialog and its backdrop.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The dialog entity.
		 */
		static void show(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Hide the dialog and its backdrop without calling the
		 * dismiss handler.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The dialog entity.
		 */
		static void hide(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Dismiss as the user would: hide the dialog, then call the
		 * dismiss handler of its components::Overlay. Ignored while hidden.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The dialog entity.
		 */
		static void dismiss(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Activate an action as a press on its button would: call
		 * its press handler. The dialog stays open.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The dialog entity.
		 * @param index Index of the action in components::Dialog (ignored
		 * when out of range).
		 */
		static void pressAction(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								std::size_t index);

		/**
		 * @brief Style the dialog with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
