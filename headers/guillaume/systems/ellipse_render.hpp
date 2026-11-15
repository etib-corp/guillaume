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

#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/ellipse.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/systems/render_handle_map.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Draws a filled ellipse (a disc when the bound is square) for
	 * every entity that has a components::Ellipse.
	 *
	 * Runs in the Render phase on entities with Transform, Bound, Color and
	 * Ellipse. It reads the four and writes no component: its output is one
	 * engine object per entity, rebuilt and uploaded every frame.
	 * - components::Transform gives the top-left corner of the bound and the
	 *   orientation.
	 * - components::Bound gives the two diameters, in dp: the ellipse touches
	 *   the four sides of the bound.
	 * - components::Color gives the fill color.
	 * - components::Ellipse gives the number of segments of the outline.
	 *
	 * @see RenderHandleMap, shape::buildTriangleFanVertices
	 * @see components::Ellipse, systems::RingRender
	 */
	class EllipseRender:
		public RenderHandleMap,
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::Color, components::Ellipse>
	{
		private:
		utility::Engine &_engine;	 ///< Engine the ellipses are uploaded to

		private:
		/**
		 * @brief Build the outline of an ellipse centered on the origin.
		 * @param halfWidth Half the width of the ellipse, in dp.
		 * @param halfHeight Half the height of the ellipse, in dp.
		 * @param segments Number of segments of the outline. A value of zero
		 * or less is treated as 1.
		 * @return `segments + 1` points in drawing order; the last one
		 * repeats the first.
		 */
		std::vector<utility::math::Vector2F>
			buildLocalEllipseVertices(float halfWidth, float halfHeight,
									  int segments) const;

		/**
		 * @brief Build the outline of an ellipse in world space.
		 * @param center World position of the center of the ellipse.
		 * @param orientation World orientation of the ellipse.
		 * @param scale Factors applied to the width and to the height. Their
		 * sign is ignored.
		 * @param size Width and height before scaling, in dp.
		 * @param segments Number of segments of the outline.
		 * @return The outline points in drawing order.
		 */
		std::vector<utility::graphic::PositionF> buildEllipseVertices(
			const utility::graphic::PositionF &center,
			const utility::graphic::OrientationF &orientation,
			const utility::math::Vector2F &scale,
			const utility::graphic::SizeF &size, int segments);

		public:
		/**
		 * @brief Create the system in the Render phase.
		 * @param engine The engine the ellipses are uploaded to. It must
		 * outlive the system.
		 */
		EllipseRender(utility::Engine &engine);

		/**
		 * @brief Forget the engine objects without removing them from the
		 * engine.
		 */
		~EllipseRender(void);

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
		 * @brief Rebuild the mesh of one ellipse and upload it, creating the
		 * engine object on the first frame.
		 * @param entityIdentifier The entity to draw. It is skipped, with a
		 * log message, when one of the four components is missing.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
