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

#include "guillaume/components/tooltip.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Styles a tooltip, shows it after its anchor has been hovered
	 * for a while and places it next to that anchor.
	 *
	 * Runs in the Layout phase on every entity that has a
	 * components::Tooltip. It is registered after systems::Layout, so the
	 * title and body are already measured when it arranges them.
	 * - **Reads**: components::Tooltip (variant, text, anchor, enter and
	 *   leave delays, viewport, visible flag, child identifiers), the hover
	 *   state, Transform and Bound of the anchor, and the active theme.
	 * - **Writes** on the tooltip: components::Layout (a column, fixed
	 *   height, child list), components::Elevation, components::Borders,
	 *   components::Color, the flags and visibility of
	 *   components::Overlay, the label of components::Accessibility, the
	 *   hover timers of components::Tooltip and, when it is shown,
	 *   components::Transform.
	 * - **Writes** on the children: the Text of the body and the Color of
	 *   the title and body.
	 *
	 * The `visible` flag of components::Tooltip is the source of truth:
	 * apply() copies it to the tooltip's components::Overlay. A hidden
	 * tooltip is not removed, it is painted transparent with no elevation.
	 *
	 * The hover delays are counted by the tooltip's own
	 * components::Animation, which install() turns into a looping
	 * one-second ticker: each sample is converted back into elapsed seconds
	 * and passed to advance(). The ticker runs only while the tooltip has
	 * an anchor.
	 *
	 * The tooltip is placed when it is shown and when the viewport changes.
	 * It does not follow an anchor that moves while it is visible.
	 *
	 * @code
	 * systems::Tooltip::install(registry, tooltip);
	 * systems::Tooltip::setViewport(registry, tooltip, 1280.0f, 720.0f);
	 * systems::Tooltip::setAnchor(registry, tooltip, button);
	 * // From here on, hovering `button` shows the tooltip.
	 * @endcode
	 *
	 * @see components::Tooltip, components::Overlay, entities::Tooltip,
	 * systems::Animation
	 */
	class Tooltip: public ecs::SystemFiller<components::Tooltip>
	{
		public:
		/**
		 * @brief Create the system in the Layout phase.
		 */
		Tooltip(void);

		/**
		 * @brief Default destructor.
		 */
		~Tooltip(void) override = default;

		/**
		 * @brief Give a tooltip its accessibility semantics, its ticker and
		 * its dismissal.
		 *
		 * Sets the role of its components::Accessibility to Text with a
		 * polite live region; makes its components::Animation a linear,
		 * looping, one-second ticker whose samples call advance() with the
		 * time elapsed since the previous sample; and makes the dismiss
		 * handler of its components::Overlay (Escape) call hide(). The
		 * ticker is not started here, see setAnchor(). Call it once, after
		 * the entity has its components::Tooltip, Overlay, Elevation,
		 * Animation, Accessibility, Layout, Transform, Bound, Color and
		 * Borders.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the handlers and must outlive the entity.
		 * @param entityIdentifier The tooltip entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::Accessibility, Animation or Overlay.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one tooltip and arrange its children.
		 *
		 * This is what update() runs each frame. Call it directly after
		 * changing components::Tooltip to see the result before the next
		 * frame, which is what the setters of entities::Tooltip do. It does
		 * not move the tooltip; place() does.
		 *
		 * A plain tooltip is 32 dp high and shows its body only. A Rich
		 * tooltip is 64 dp high, shows its title above the body and has a
		 * shadow while visible. The width follows the content.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::Tooltip, Layout, Elevation, Borders, Color, Overlay or
		 * Accessibility.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Attach the tooltip to the entity whose hover triggers it.
		 *
		 * Records the anchor, resets the hover timers and starts the
		 * ticker. Passing ecs::Entity::InvalidIdentifier detaches the
		 * tooltip and stops the ticker; a tooltip that is visible at that
		 * moment stays visible until hide() is called.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 * @param anchorIdentifier The entity to watch, or
		 * ecs::Entity::InvalidIdentifier for none. It needs a Transform and
		 * a Bound for the tooltip to be placed, and hover interaction
		 * components for the hover to be detected.
		 */
		static void setAnchor(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier,
							  ecs::Entity::Identifier anchorIdentifier);

		/**
		 * @brief Set the area the tooltip must stay inside.
		 *
		 * The area starts at the origin. When the tooltip is visible it is
		 * placed again at once. Without a viewport the tooltip is always
		 * placed under its anchor and never clamped.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 * @param width Width of the area, in dp.
		 * @param height Height of the area, in dp.
		 */
		static void setViewport(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								float width, float height);

		/**
		 * @brief Show the tooltip next to its anchor at once, without
		 * waiting for the enter delay.
		 *
		 * Sets the visible flag of components::Tooltip, then calls place()
		 * and apply().
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 */
		static void show(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Hide the tooltip at once, without waiting for the leave
		 * delay.
		 *
		 * Clears the visible flag of components::Tooltip and calls apply().
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 */
		static void hide(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Advance the hover timers by some elapsed time.
		 *
		 * While the anchor is hovered, the hover time grows and the tooltip
		 * is shown once it reaches the enter delay. While the anchor is not
		 * hovered, the hover time is reset, and a visible tooltip is hidden
		 * once it has been left for the leave delay. Does nothing when
		 * hasAnchor() is false. The ticker calls it on every sample; a test
		 * can call it directly to simulate time passing.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 * @param deltaTime Time elapsed since the previous call, in seconds.
		 */
		static void advance(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							float deltaTime);

		/**
		 * @brief Move the tooltip next to its anchor.
		 *
		 * The tooltip is centered horizontally on the anchor and placed
		 * right under it, with the anchor's depth and orientation. When a
		 * viewport is set, a tooltip that would cross its bottom edge is
		 * placed above the anchor instead, and the position is then clamped
		 * so the tooltip stays inside the viewport. Does nothing when
		 * hasAnchor() is false or when the anchor has no Bound.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 */
		static void place(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the tooltip is attached to an entity that still
		 * exists.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 * @return False when the anchor is ecs::Entity::InvalidIdentifier or
		 * has no components::Transform in the registry, which is the case
		 * once the anchor entity has been destroyed.
		 */
		static bool hasAnchor(const ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Run apply() on one tooltip; called by the frame loop.
		 *
		 * Logs a warning and does nothing when the entity has no
		 * components::Tooltip.
		 * @param entityIdentifier The tooltip entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
