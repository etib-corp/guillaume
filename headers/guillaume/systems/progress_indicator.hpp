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
	 * @brief Sizes a progress indicator and draws its track and fill.
	 *
	 * Reads components::ProgressIndicator, the target value and the active
	 * theme, and writes the size and accessibility value of the widget plus
	 * the geometry, position and color of its track and fill. Runs in the
	 * Layout phase, after the Layout system.
	 */
	class ProgressIndicator:
		public ecs::SystemFiller<components::ProgressIndicator>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		ProgressIndicator(void);

		/**
		 * @brief Default destructor.
		 */
		~ProgressIndicator(void) override = default;

		/**
		 * @brief Make an entity behave as this widget.
		 *
		 * Normalizes its Value to [0, 1], draws the current value without a
		 * transition, sets its accessibility role, and wires value changes
		 * and its own animation, which drives the value transition and the
		 * fade, to this system. Call once, after the entity has its
		 * components::ProgressIndicator, a Value, an Animation, an
		 * Accessibility and the components of a surface.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Wire and start the looping animation of the fill child.
		 *
		 * Call each time children have been stored in the component. Does
		 * nothing for a determinate variant or a fill without an Animation.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 */
		static void
			installChildren(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Size one widget and draw its track and fill.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Set the target value; the drawn value follows it over a
		 * 200 ms transition.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 * @param value The new normalized value, clamped to [0, 1].
		 */
		static void setValue(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier,
							 float value);

		/**
		 * @brief Show or hide the indicator with a 200 ms fade.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 * @param visible The target visibility.
		 */
		static void setVisible(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   bool visible);

		/**
		 * @brief Start a 200 ms transition toward the target value and
		 * opacity, from what is currently drawn.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 */
		static void
			startTransition(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the fill color before fading.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 * @return The color override, or the theme `Primary` without one.
		 */
		static utility::graphic::Color32Bit
			indicatorColor(const ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Size and draw the widget with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
