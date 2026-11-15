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
	 * @brief Sizes and colors a badge (the small dot or counter drawn over
	 * another widget) and fades it in and out.
	 *
	 * Runs in the Layout phase on every entity that has a
	 * components::Badge. It is registered after systems::Layout, so the
	 * label is already measured when the badge is sized around it.
	 * - **Reads**: components::Badge (variant, label, size and color
	 *   overrides, visibility, opacity), the Bound of the label child and
	 *   the active theme.
	 * - **Writes** on the badge: components::Borders (fully rounded),
	 *   components::Layout (fixed size) and components::Color, then the
	 *   Transform and Bound of the label through systems::Layout::apply.
	 * - **Writes** on the label child: its Text content, Typography font
	 *   size and Color.
	 *
	 * Visibility is an opacity from 0 to 1 stored in components::Badge and
	 * multiplied into the alpha of both colors. setVisible() starts the
	 * badge's components::Animation, whose samples call fade().
	 *
	 * @code
	 * // On an entity that has Badge, Animation, Layout, Borders and Color:
	 * systems::Badge::install(registry, badge);
	 * systems::Badge::apply(registry, badge);
	 * systems::Badge::setVisible(registry, badge, false); // fades out
	 * @endcode
	 *
	 * @see components::Badge, entities::Badge, systems::Animation
	 */
	class Badge: public ecs::SystemFiller<components::Badge>
	{
		public:
		/**
		 * @brief Create the system in the Layout phase.
		 */
		Badge(void);

		/**
		 * @brief Default destructor.
		 */
		~Badge(void) override = default;

		/**
		 * @brief Prepare the visibility fade of a badge.
		 *
		 * Sets the duration and easing of the entity's
		 * components::Animation from the theme and makes each of its
		 * samples call fade(). Call it once, after the entity has its
		 * components::Badge and components::Animation and before the first
		 * setVisible().
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the sample handler and must outlive the entity.
		 * @param entityIdentifier The badge entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity has
		 * no components::Animation.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Size and color one badge and its label.
		 *
		 * This is what update() runs each frame. Call it directly after
		 * changing components::Badge to see the result before the next
		 * frame, which is what the setters of entities::Badge do.
		 *
		 * The badge is `sizeOf()` high. It is as wide as it is high, except
		 * that a label wider than that grows it into a pill with
		 * components::Badge::HorizontalPadding on each side. The label font
		 * size is half the badge height. The colors default to the theme's
		 * error and onError roles.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The badge entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::Badge, Layout, Borders or Color.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the height of a badge, in dp.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The badge entity.
		 * @return The size override of components::Badge when one is set,
		 * otherwise the size of the variant: 6 (dot), 16 (small) or 20
		 * (large).
		 */
		static float sizeOf(const ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the badge should have a label child.
		 *
		 * entities::Badge uses it to decide whether to create the label
		 * entity.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The badge entity.
		 * @return False for the Dot variant or an empty label, true
		 * otherwise.
		 */
		static bool needsLabel(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Show or hide the badge with a fade.
		 *
		 * Records the target visibility and the opacity the fade starts
		 * from, then plays the badge's components::Animation. Starting from
		 * the current opacity keeps a fade that is reversed half-way
		 * continuous. Does nothing when the visibility is already the
		 * requested one. Call install() first: it is what makes the
		 * animation change the opacity.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The badge entity.
		 * @param visible True to fade in, false to fade out.
		 */
		static void setVisible(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   bool visible);

		/**
		 * @brief Move the visibility fade to a given progress and restyle.
		 *
		 * Called by the animation sample handler wired by install(). Sets
		 * the opacity between the value recorded by setVisible() and the
		 * target (1 when visible, 0 when hidden), then calls apply().
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The badge entity.
		 * @param progress Eased progress of the fade, from 0 (start) to 1
		 * (target reached).
		 */
		static void fade(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier,
						 float progress);

		/**
		 * @brief Run apply() on one badge; called by the frame loop.
		 *
		 * Logs a warning and does nothing when the entity has no
		 * components::Badge.
		 * @param entityIdentifier The badge entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
