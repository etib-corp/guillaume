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

#include "guillaume/components/badge.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Sizes, colors and fades a badge.
	 *
	 * Reads components::Badge and the active theme, and writes the badge's
	 * Layout, Borders, Bound and Color, plus the content, font size and color
	 * of its label child. The visibility fade is driven by the badge's
	 * components::Animation. Runs in the Layout phase, after the label has
	 * been measured.
	 */
	class Badge: public ecs::SystemFiller<components::Badge>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		Badge(void);

		/**
		 * @brief Default destructor.
		 */
		~Badge(void) override = default;

		/**
		 * @brief Make an entity fade as a badge.
		 *
		 * Sets the fade duration and easing on the entity's
		 * components::Animation and wires its samples to fade(). Call once,
		 * after the entity has its components::Badge and Animation.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The badge entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Size and color one badge and its label.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The badge entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the resolved badge height.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The badge entity.
		 * @return The size override, or the size of the variant, in dp.
		 */
		static float sizeOf(const ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the badge shows a label child.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The badge entity.
		 * @return True unless the variant is Dot or the label is empty.
		 */
		static bool needsLabel(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Show or hide the badge, starting the fade from the current
		 * opacity. Does nothing when the visibility is unchanged.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The badge entity.
		 * @param visible The target visibility.
		 */
		static void setVisible(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   bool visible);

		/**
		 * @brief Run one step of the visibility fade and restyle.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The badge entity.
		 * @param progress Eased progress of the fade, in [0, 1].
		 */
		static void fade(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier,
						 float progress);

		/**
		 * @brief Style the badge with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
