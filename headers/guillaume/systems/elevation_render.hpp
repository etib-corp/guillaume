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

#include <memory>

#include <utility/engine.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/systems/render_handle_map.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Draws the soft shadow of every entity that has a
	 * components::Elevation.
	 *
	 * Runs in the Render phase on entities with Transform, Bound and
	 * Elevation. It reads the three and writes no component: its output is
	 * one engine object per entity, rebuilt and uploaded every frame.
	 *
	 * The shadow is a stack of components::Elevation::getLayers shapes
	 * centered on the bound. Each one is larger than the previous one (up to
	 * `spread * level` dp beyond the bound on every side) and more
	 * transparent (from the alpha of the shadow color down to zero), which
	 * gives a blurred edge.
	 *
	 * Things to know:
	 * - Nothing is drawn while components::Elevation::hasShadow is false
	 *   (level 0 or a fully transparent color); a shadow drawn on an earlier
	 *   frame is then removed.
	 * - The corners of the shadow are always fully rounded (the radius is
	 *   half the smaller side of the bound): components::Borders is not
	 *   read.
	 * - The shadow is drawn at the depth of the entity itself.
	 *
	 * @see RenderHandleMap, components::Elevation
	 * @see systems::RectangleRender
	 */
	class ElevationRender:
		public RenderHandleMap,
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::Elevation>
	{
		private:
		utility::Engine &_engine;	 ///< Engine the shadows are uploaded to

		public:
		/**
		 * @brief Create the system in the Render phase.
		 * @param engine The engine the shadows are uploaded to. It must
		 * outlive the system.
		 */
		ElevationRender(utility::Engine &engine);

		/**
		 * @brief Destroy the system. The engine objects it created are left
		 * in the engine.
		 */
		~ElevationRender(void) override = default;

		/**
		 * @brief Mark every shadow as not drawn yet. Called once per frame
		 * before the entities are visited.
		 */
		void prepare(void) override;

		/**
		 * @brief Remove the shadows of the entities that were not drawn this
		 * frame, including those whose elevation dropped to zero. Called once
		 * per frame after the entities.
		 */
		void cleanup(void) override;

		/**
		 * @brief Rebuild the shadow mesh of one entity and upload it,
		 * creating the engine object on the first frame.
		 * @param entityIdentifier The entity to draw. It is skipped when it
		 * has no visible shadow, and skipped with a log message when one of
		 * the three components is missing.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
