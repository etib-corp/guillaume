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
	 * - The four corners share one radius, the average of the four radii of
	 *   components::Borders. The radius is clamped to half the smaller side.
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

		private:
		/**
		 * @brief Reduce the four corner radii of a Borders component to the
		 * single radius this system draws with.
		 * @param borders The component to read.
		 * @return The mean of the four corner radii, in dp.
		 */
		float extractAverageRadius(const components::Borders &borders) const;

		/**
		 * @brief Build the four corners of a rectangle with square corners,
		 * centered on the origin.
		 * @param halfWidth Half the width, in dp.
		 * @param halfHeight Half the height, in dp.
		 * @return The corners, starting at (-halfWidth, -halfHeight) and
		 * going through (+halfWidth, -halfHeight).
		 */
		std::vector<utility::math::Vector2F>
			buildAxisAlignedRectVertices(float halfWidth,
										 float halfHeight) const;

		/**
		 * @brief Append the points of one corner arc to an outline.
		 *
		 * Adds `arcSegments + 1` points, the first at @p startAngle and the
		 * last at @p endAngle.
		 * @param localVertices The outline to append to.
		 * @param arcCenter Center of the arc, relative to the center of the
		 * rectangle.
		 * @param startAngle Angle of the first point, in radians.
		 * @param endAngle Angle of the last point, in radians.
		 * @param radius Radius of the arc, in dp.
		 * @param arcSegments Number of segments of the arc. Must be at least
		 * 1.
		 */
		void appendRoundedCornerArc(
			std::vector<utility::math::Vector2F> &localVertices,
			const utility::math::Vector2F &arcCenter, float startAngle,
			float endAngle, float radius, int arcSegments) const;

		/**
		 * @brief Build the outline of a rounded rectangle centered on the
		 * origin.
		 * @param halfWidth Half the width, in dp.
		 * @param halfHeight Half the height, in dp.
		 * @param radius Corner radius, in dp. It is clamped between zero and
		 * the smaller half extent.
		 * @param arcSegments Number of segments per corner. A value of zero
		 * or less is treated as 1.
		 * @param epsilon Radius, in dp, at or below which the corners are
		 * drawn square (four points only).
		 * @return The outline points in drawing order.
		 */
		std::vector<utility::math::Vector2F>
			buildLocalRoundedRectVertices(float halfWidth, float halfHeight,
										  float radius, int arcSegments,
										  float epsilon) const;

		/**
		 * @brief Build the outline of a rounded rectangle in world space.
		 * @param center World position of the center of the rectangle.
		 * @param orientation World orientation of the rectangle.
		 * @param scale Factors applied to the width and to the height. Their
		 * sign is ignored.
		 * @param size Width and height before scaling, in dp.
		 * @param radius Corner radius, in dp. It is clamped between zero and
		 * the smaller scaled half extent.
		 * @param arcSegments Number of segments per corner.
		 * @param epsilon Radius, in dp, at or below which the corners are
		 * drawn square.
		 * @return The outline points in drawing order.
		 */
		std::vector<utility::graphic::PositionF> buildRoundedRectVertices(
			const utility::graphic::PositionF &center,
			const utility::graphic::OrientationF &orientation,
			const utility::math::Vector2F &scale,
			const utility::graphic::SizeF &size, float radius,
			int arcSegments = 16, float epsilon = 0.001f);

		public:
		/**
		 * @brief Distance the border is moved towards the viewer, along the
		 * forward axis of the entity, so it draws over the fill. Half the
		 * depth of one layer.
		 */
		static constexpr float StrokeLift = 0.5f;

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
