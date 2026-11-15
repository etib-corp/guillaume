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

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/loading_indicator.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Draws an indeterminate loading indicator: a rotating arc
	 * (circular variant) or a bar sliding along a track (linear variant).
	 *
	 * Runs in the Layout phase on every entity that has a
	 * components::LoadingIndicator, after systems::Layout.
	 * - **Reads**: components::LoadingIndicator (variant, size, color
	 *   override, loop progress, opacity, the identifier of the animated
	 *   child) and the active theme.
	 * - **Writes** on the widget: components::Layout (fixed size),
	 *   components::Borders and a transparent components::Color.
	 * - **Writes** on the animated child: its Color (faded by the opacity),
	 *   its Transform and Bound, and either the thickness and angles of its
	 *   components::Arc or the radius of its components::Borders.
	 *
	 * Two components::Animation drive the widget, and both restyle it on
	 * every sample, so the indicator moves even between two frames of this
	 * system:
	 * - the one on the **child** loops and writes the loop progress (0 to
	 *   1), which becomes the start angle of the arc or the offset of the
	 *   bar. It is wired by installChildren();
	 * - the one on the **widget** runs the fade in and out and writes the
	 *   opacity (0 to 1). It is wired by install().
	 *
	 * @code
	 * systems::LoadingIndicator::install(registry, indicator);
	 * // once the child entity is recorded in the component:
	 * systems::LoadingIndicator::installChildren(registry, indicator);
	 * systems::LoadingIndicator::stop(registry, indicator);
	 * @endcode
	 *
	 * @see components::LoadingIndicator, entities::LoadingIndicator,
	 * systems::Animation, systems::ProgressIndicator
	 */
	class LoadingIndicator:
		public ecs::SystemFiller<components::LoadingIndicator>
	{
		public:
		/**
		 * @brief Create the system in the Layout phase.
		 */
		LoadingIndicator(void);

		/**
		 * @brief Default destructor.
		 */
		~LoadingIndicator(void) override = default;

		/**
		 * @brief Give a loading indicator its accessibility role and its
		 * fade.
		 *
		 * Sets the role of its components::Accessibility to ProgressBar,
		 * and sets up the widget's own components::Animation so that each
		 * sample moves the opacity from the value recorded by startFade()
		 * to its target and calls apply(). Call it once, after the entity
		 * has its components::LoadingIndicator, Animation, Accessibility,
		 * Layout, Borders, Color, Transform and Bound.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the sample handler and must outlive the entity.
		 * @param entityIdentifier The loading indicator entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity has
		 * no components::Accessibility or components::Animation.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Wire the looping animation of the animated child and start
		 * it.
		 *
		 * Makes the components::Animation of the child recorded in
		 * components::LoadingIndicator loop, write the loop progress on
		 * each sample and call apply(), then plays it: the indicator runs
		 * as soon as this returns. Call it each time a child has been
		 * recorded in the component, for instance after the variant changed
		 * and the child was rebuilt. Does nothing when the child has no
		 * components::Animation.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the sample handler and must outlive the entity.
		 * @param entityIdentifier The loading indicator entity.
		 */
		static void
			installChildren(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Size one loading indicator and draw its animated child at
		 * the current loop progress and opacity.
		 *
		 * This is what update() runs each frame, and what both animations
		 * call on each sample. Call it directly after changing
		 * components::LoadingIndicator to see the result before the next
		 * frame.
		 *
		 * A circular indicator is a square of the configured size holding an
		 * arc of three quarters of a turn whose start angle follows the
		 * progress. A linear indicator is a track of
		 * components::LoadingIndicator::LinearWidth by LinearHeight in which
		 * a shorter bar slides from the left edge to the right edge. The
		 * child is left untouched while it has no Bound or no Color.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The loading indicator entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::LoadingIndicator, Layout, Borders or Color.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Start the looping animation and fade the indicator in.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The loading indicator entity.
		 */
		static void start(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Stop the looping animation.
		 *
		 * The indicator also fades out when its component is set to hide
		 * when stopped; otherwise it stays visible, frozen where it
		 * stopped.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The loading indicator entity.
		 */
		static void stop(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the looping animation is running.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The loading indicator entity.
		 * @return True when the animated child has a components::Animation
		 * that is playing; false when there is no such child.
		 */
		static bool isRunning(const ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Start fading towards the opacity the running state calls
		 * for.
		 *
		 * The target opacity is 1 while the indicator is running, or when it
		 * does not hide when stopped, and 0 otherwise. The fade starts from
		 * the current opacity and is played by the widget's own
		 * components::Animation. Does nothing when the indicator is already
		 * at the target. start() and stop() call it; call it yourself after
		 * changing the hide-when-stopped setting.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The loading indicator entity.
		 */
		static void startFade(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the color of the arc or bar at full opacity.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The loading indicator entity.
		 * @return The color override of components::LoadingIndicator when
		 * one is set, otherwise the primary color of the active theme.
		 */
		static utility::graphic::Color32Bit
			indicatorColor(const ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the stroke width of the circular arc, in dp.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The loading indicator entity.
		 * @return The configured size multiplied by
		 * components::LoadingIndicator::StrokeRatio (4 dp for a 48 dp
		 * indicator).
		 */
		static float
			strokeWidth(const ecs::ComponentRegistry &componentRegistry,
						const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Run apply() on one loading indicator; called by the frame
		 * loop.
		 *
		 * Logs a warning and does nothing when the entity has no
		 * components::LoadingIndicator.
		 * @param entityIdentifier The loading indicator entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
