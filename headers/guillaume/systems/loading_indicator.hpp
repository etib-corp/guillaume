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
	 * @brief Sizes a loading indicator and draws its animated child.
	 *
	 * Reads components::LoadingIndicator and the active theme, and writes the
	 * size of the widget plus the geometry, position and color of its sliding
	 * bar or rotating arc. Runs in the Layout phase, after the Layout system.
	 */
	class LoadingIndicator:
		public ecs::SystemFiller<components::LoadingIndicator>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		LoadingIndicator(void);

		/**
		 * @brief Default destructor.
		 */
		~LoadingIndicator(void) override = default;

		/**
		 * @brief Make an entity behave as this widget.
		 *
		 * Sets its accessibility role and wires its own animation, which
		 * drives the fade, to this system. Call once, after the entity has
		 * its components::LoadingIndicator, an Animation, an Accessibility
		 * and the components of a surface.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Wire and start the looping animation of the animated child.
		 *
		 * Call each time a child has been stored in the component. A child
		 * without an Animation is left alone.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 */
		static void
			installChildren(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Size one widget and draw its animated child.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Start the looping animation, and fade the indicator in.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 */
		static void start(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Stop the looping animation, and fade the indicator out when
		 * it hides when stopped.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 */
		static void stop(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the looping animation is running.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 * @return True when the animated child exists and is playing.
		 */
		static bool isRunning(const ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Fade toward the opacity implied by the running state.
		 *
		 * The target is 1 while running or when the indicator does not hide
		 * when stopped, 0 otherwise. Does nothing when already there.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 */
		static void startFade(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the indicator color before fading.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 * @return The color override, or the theme `Primary` without one.
		 */
		static utility::graphic::Color32Bit
			indicatorColor(const ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the stroke width of the circular arc.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 * @return The diameter scaled by the M3 ratio (4 dp at 48 dp).
		 */
		static float
			strokeWidth(const ecs::ComponentRegistry &componentRegistry,
						const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Size and draw the widget with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
