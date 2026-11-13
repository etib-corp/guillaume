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

#include "guillaume/components/app_bar.hpp"
#include "guillaume/theme/typescale.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Styles an app bar and lays out its navigation button, title and
	 * action buttons in a row.
	 *
	 * Runs in the Layout phase on every entity that has a
	 * components::AppBar. It is registered after systems::Layout, so the
	 * children are already measured when it arranges them.
	 * - **Reads**: components::AppBar (variant, title, elevated flag, child
	 *   identifiers) and the active theme.
	 * - **Writes** on the bar: components::Layout (horizontal axis, padding,
	 *   fixed height, child list), components::Color and
	 *   components::Elevation, then the Transform and Bound of the children
	 *   through systems::Layout::apply.
	 * - **Writes** on the children: the Text, Typography and Accessibility
	 *   label of the title, and the content color of the navigation button.
	 *
	 * A system cannot create entities: entities::AppBar builds the children
	 * and records their identifiers in components::AppBar. The `needs...`
	 * functions tell it which children are still missing.
	 *
	 * The bar needs components::Layout, Color and Elevation next to its
	 * components::AppBar: apply() throws
	 * ecs::EntityComponentNotFoundException when one is missing.
	 *
	 * @see components::AppBar, entities::AppBar, systems::Layout
	 */
	class AppBar: public ecs::SystemFiller<components::AppBar>
	{
		public:
		/**
		 * @brief Create the system in the Layout phase.
		 */
		AppBar(void);

		/**
		 * @brief Default destructor.
		 */
		~AppBar(void) override = default;

		/**
		 * @brief Expose the title child to assistive technology as a header.
		 *
		 * Adds a components::Accessibility to the title entity recorded in
		 * components::AppBar when it has none, sets its role to Header and
		 * its label to the title text, and makes it non-focusable. Call it
		 * once, right after the title child has been created and recorded.
		 * Does nothing when no title child is recorded.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The app bar entity.
		 */
		static void
			installTitle(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one app bar and arrange its children.
		 *
		 * This is what update() runs each frame. Call it directly after
		 * changing components::AppBar to see the result before the next
		 * frame, which is what the setters of entities::AppBar do, or to
		 * drive an app bar in a test without a frame loop.
		 *
		 * The children are laid out in the order navigation button, title,
		 * action buttons, skipping the ones that do not exist. A
		 * center-aligned bar centers a lone title and otherwise spreads its
		 * children across the bar.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The app bar entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::AppBar, Layout, Color or Elevation.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the fixed height of a variant.
		 * @param variant The app bar variant.
		 * @return 64 (small, center-aligned), 112 (medium), 152 (large) or
		 * 80 (bottom), in dp.
		 */
		static float height(components::AppBar::Variant variant);

		/**
		 * @brief Get the typescale role the title of a variant is drawn
		 * with.
		 * @param variant The app bar variant.
		 * @return TitleLarge (small, center-aligned, bottom), HeadlineSmall
		 * (medium) or HeadlineMedium (large).
		 */
		static theme::TypeRole titleRole(components::AppBar::Variant variant);

		/**
		 * @brief Whether the navigation button child still has to be
		 * created.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The app bar entity.
		 * @return True when the navigation action has an icon and no
		 * navigation button is recorded in components::AppBar.
		 */
		static bool needsNavigationButton(
			const ecs::ComponentRegistry &componentRegistry,
			const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the title child still has to be created.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The app bar entity.
		 * @return True when the title text is not empty and no title entity
		 * is recorded in components::AppBar.
		 */
		static bool needsTitle(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the action button children have to be created
		 * again.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The app bar entity.
		 * @return True when the number of recorded action buttons differs
		 * from the number of actions. Only the counts are compared, not the
		 * content of the actions.
		 */
		static bool
			needsActionButtons(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Run apply() on one app bar; called by the frame loop.
		 *
		 * Logs a warning and does nothing when the entity has no
		 * components::AppBar.
		 * @param entityIdentifier The app bar entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
