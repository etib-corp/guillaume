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
	 * @brief Styles an app bar and arranges its children.
	 *
	 * Reads components::AppBar and the active theme, and writes the bar's
	 * Layout, Color, Elevation and Bound, plus the content, typescale role
	 * and accessible name of its title child and the content color of its
	 * navigation button. Runs in the Layout phase, after the children have
	 * been measured.
	 */
	class AppBar: public ecs::SystemFiller<components::AppBar>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		AppBar(void);

		/**
		 * @brief Default destructor.
		 */
		~AppBar(void) override = default;

		/**
		 * @brief Make the title child of an app bar a header.
		 *
		 * Adds a components::Accessibility with the Header role to the title
		 * recorded in components::AppBar. Does nothing without a title child.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The app bar entity.
		 */
		static void
			installTitle(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one app bar and arrange its children.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The app bar entity.
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
		 * @brief Get the title typescale role of a variant.
		 * @param variant The app bar variant.
		 * @return titleLarge (small/center/bottom), headlineSmall (medium) or
		 * headlineMedium (large).
		 */
		static theme::TypeRole titleRole(components::AppBar::Variant variant);

		/**
		 * @brief Whether the app bar lacks its navigation button child.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The app bar entity.
		 * @return True when a navigation icon is set and no child is recorded.
		 */
		static bool needsNavigationButton(
			const ecs::ComponentRegistry &componentRegistry,
			const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the app bar lacks its title child.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The app bar entity.
		 * @return True when a title is set and no child is recorded.
		 */
		static bool needsTitle(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the app bar lacks its action button children.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The app bar entity.
		 * @return True when the recorded children do not match the actions.
		 */
		static bool
			needsActionButtons(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style the app bar with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
