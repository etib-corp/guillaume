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
#include "guillaume/components/color.hpp"
#include "guillaume/components/ring.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/systems/render_handle_map.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Draws a full ring (a circle outline with a thickness) for every
	 * entity that has a components::Ring.
	 *
	 * Runs in the Render phase on entities with Transform, Bound, Color and
	 * Ring. It reads the four and writes no component: its output is one
	 * engine object per entity, rebuilt and uploaded every frame.
	 * - components::Transform gives the top-left corner of the bound and the
	 *   orientation.
	 * - components::Bound gives the outer diameters, in dp: the outer edge
	 *   touches the four sides of the bound.
	 * - components::Color gives the color of the ring.
	 * - components::Ring gives the thickness, measured inwards from the
	 *   outer edge, and the number of segments.
	 *
	 * @see RenderHandleMap, shape::buildRingMesh
	 * @see components::Ring, systems::ArcRender
	 */
	class RingRender:
		public RenderHandleMap,
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::Color, components::Ring>
	{
		private:
		utility::Engine &_engine;	 ///< Engine the rings are uploaded to

		public:
		/**
		 * @brief Create the system in the Render phase.
		 * @param engine The engine the rings are uploaded to. It must outlive
		 * the system.
		 */
		RingRender(utility::Engine &engine);

		/**
		 * @brief Destroy the system. The engine objects it created are left
		 * in the engine.
		 */
		~RingRender(void) override = default;

		/**
		 * @brief Mark every engine object as not drawn yet. Called once per
		 * frame before the entities are visited.
		 */
		void prepare(void) override;

		/**
		 * @brief Remove the engine objects of the entities that were not
		 * drawn this frame. Called once per frame after the entities.
		 */
		void cleanup(void) override;

		/**
		 * @brief Rebuild the mesh of one ring and upload it, creating the
		 * engine object on the first frame.
		 * @param entityIdentifier The entity to draw. It is skipped, with a
		 * log message, when one of the four components is missing.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
