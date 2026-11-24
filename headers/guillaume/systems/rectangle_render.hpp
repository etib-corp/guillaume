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

#include <vector>

#include <utility/engine.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/systems/render_handle_map.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Draws the background of an entity: a filled rectangle with
	 * rounded corners and an optional border.
	 *
	 * Runs in the Render phase on every entity that has Transform, Bound,
	 * Color and Borders. It reads the four and writes no component: its
	 * output is one engine object per entity, rebuilt and uploaded every
	 * frame.
	 * - components::Transform gives the top-left corner and the orientation.
	 * - components::Bound gives the width and height, in dp.
	 * - components::Color gives the fill color.
	 * - components::Borders gives the corner radius and, when
	 *   components::Borders::hasStroke is true, the width and color of a
	 *   border drawn inside the rectangle.
	 *
	 * Things to know:
	 * - Each corner is drawn with its own radius from components::Borders,
	 *   for the fill and for the border. Each radius is clamped to half the
	 *   smaller side.
	 * - The border is a second mesh lifted by StrokeLift towards the viewer
	 *   so it is not hidden by the fill.
	 * - An entity is drawn even when its color is fully transparent.
	 *
	 * @see RenderHandleMap, shape::buildRoundedStrokeMesh
	 * @see components::Borders, systems::ElevationRender
	 */
	class RectangleRender:
		public RenderHandleMap,
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::Color, components::Borders>
	{
		private:
		utility::Engine &_engine;	 ///< Engine the rectangles are uploaded to

		public:
		/**
		 * @brief Distance the border is moved towards the viewer, along the
		 * forward axis of the entity, so it draws over the fill. Half the
		 * depth of one layer.
		 */
		static constexpr float StrokeLift = 0.5f;

		/**
		 * @brief Number of segments of each rounded corner, for the fill and
		 * for the border.
		 */
		static constexpr int ArcSegments = 16;

		/**
		 * @brief Create the system in the Render phase.
		 * @param engine The engine the rectangles are uploaded to. It must
		 * outlive the system.
		 */
		RectangleRender(utility::Engine &engine);

		/**
		 * @brief Forget the engine objects without removing them from the
		 * engine.
		 */
		~RectangleRender(void);

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
		 * @brief Rebuild the fill and border meshes of one entity and upload
		 * them, creating the engine object on the first frame.
		 * @param entityIdentifier The entity to draw. It is skipped, with a
		 * log message, when one of the four components is missing.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
