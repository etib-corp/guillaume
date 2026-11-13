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

#include "guillaume/components/progress_indicator.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Draws a progress indicator: a track and a fill that shows a
	 * value from 0 to 1, as a bar or as a ring.
	 *
	 * Runs in the Layout phase on every entity that has a
	 * components::ProgressIndicator, after systems::Layout.
	 * - **Reads**: components::ProgressIndicator (variant, sizes, color
	 *   override, the value and opacity being drawn, the identifiers of
	 *   the track and fill children), the target value in
	 *   components::Value and the active theme.
	 * - **Writes** on the widget: components::Layout (fixed size),
	 *   components::Borders, a transparent components::Color, and the
	 *   value of components::Accessibility (0 to 100, or none when
	 *   indeterminate).
	 * - **Writes** on the children: the Color, Transform and Bound of the
	 *   track and of the fill, the thickness of the track's
	 *   components::Ring, and the thickness and angles of the fill's
	 *   components::Arc or the radius of its components::Borders.
	 *
	 * Two numbers are kept apart: the **target** value, stored in
	 * components::Value, and the **displayed** value, stored in
	 * components::ProgressIndicator. When the target changes, the widget's
	 * components::Animation moves the displayed value to it over
	 * theme::MotionDuration::Short4 (200 ms). The same animation runs the
	 * fade started by setVisible().
	 *
	 * The indeterminate variants ignore the value: a components::Animation
	 * on the fill child loops and moves a fixed-size segment along the
	 * track or around the ring. It is wired by installChildren().
	 *
	 * @code
	 * systems::ProgressIndicator::install(registry, indicator);
	 * systems::ProgressIndicator::setValue(registry, indicator, 0.4f);
	 * @endcode
	 *
	 * @see components::ProgressIndicator, components::Value,
	 * entities::ProgressIndicator, systems::LoadingIndicator
	 */
	class ProgressIndicator:
		public ecs::SystemFiller<components::ProgressIndicator>
	{
		public:
		/**
		 * @brief Create the system in the Layout phase.
		 */
		ProgressIndicator(void);

		/**
		 * @brief Default destructor.
		 */
		~ProgressIndicator(void) override = default;

		/**
		 * @brief Prepare the value, the transition and the accessibility
		 * role of a progress indicator.
		 *
		 * - Sets the range of the entity's components::Value to 0 to 1,
		 *   without step.
		 * - Takes the value held at that moment as the displayed value, so
		 *   it is drawn without a transition.
		 * - Makes every later change of the value call startTransition().
		 * - Sets up the widget's own components::Animation so that each
		 *   sample moves the displayed value and the opacity towards their
		 *   targets and calls apply().
		 * - Sets the role of components::Accessibility to ProgressBar.
		 *
		 * Call it once, after the entity has its
		 * components::ProgressIndicator, Value, Animation, Accessibility,
		 * Layout, Borders, Color, Transform and Bound.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the handlers and must outlive the entity.
		 * @param entityIdentifier The progress indicator entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::ProgressIndicator, Value, Animation or Accessibility.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Wire the looping animation of an indeterminate indicator
		 * and start it.
		 *
		 * Makes the components::Animation of the fill child loop, write the
		 * cycle position (0 to 1) on each sample and call apply(), then
		 * plays it. Call it each time the track and fill children have been
		 * recorded in the component, for instance after the variant changed
		 * and they were rebuilt. Does nothing for a determinate variant or
		 * when the fill has no components::Animation.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the sample handler and must outlive the entity.
		 * @param entityIdentifier The progress indicator entity.
		 */
		static void
			installChildren(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Size one progress indicator and draw its track and fill
		 * from the displayed value, the cycle position and the opacity.
		 *
		 * This is what update() runs each frame, and what the animations
		 * call on each sample. Call it directly after changing
		 * components::ProgressIndicator to see the result before the next
		 * frame.
		 *
		 * A linear indicator is a track of the configured length and
		 * components::ProgressIndicator::LinearHeight; its fill starts at
		 * the left edge and covers the displayed fraction of the track. A
		 * circular indicator is a ring of the configured diameter; its fill
		 * is an arc starting at the top and sweeping the displayed fraction
		 * of a turn. The accessibility value published is the target value,
		 * not the displayed one. The children are left untouched while the
		 * track or the fill has no Bound or no Color.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The progress indicator entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::ProgressIndicator, Layout, Borders, Color or
		 * Accessibility, or components::Value for a determinate variant.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Set the target value.
		 *
		 * Writes components::Value. Once install() has been called, a real
		 * change starts the transition that moves the drawn value to the
		 * new target.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The progress indicator entity.
		 * @param value The new progress, from 0 (empty) to 1 (complete).
		 * Values outside that range are clamped.
		 */
		static void setValue(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier,
							 float value);

		/**
		 * @brief Show or hide the indicator with a fade.
		 *
		 * Records the target visibility in components::ProgressIndicator
		 * and calls startTransition(). Does nothing when the visibility is
		 * already the requested one.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The progress indicator entity.
		 * @param visible True to fade in, false to fade out.
		 */
		static void setVisible(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   bool visible);

		/**
		 * @brief Start moving the drawn value and opacity towards their
		 * targets.
		 *
		 * Records what is drawn at that moment as the starting point, plays
		 * the widget's components::Animation and calls apply(). Starting
		 * from what is drawn keeps a transition that is interrupted by a new
		 * one continuous. setValue() and setVisible() end up here; there is
		 * usually no need to call it directly.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The progress indicator entity.
		 */
		static void
			startTransition(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the color of the fill at full opacity.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The progress indicator entity.
		 * @return The color override of components::ProgressIndicator when
		 * one is set, otherwise the primary color of the active theme.
		 */
		static utility::graphic::Color32Bit
			indicatorColor(const ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Run apply() on one progress indicator; called by the frame
		 * loop.
		 *
		 * Logs a warning and does nothing when the entity has no
		 * components::ProgressIndicator.
		 * @param entityIdentifier The progress indicator entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
