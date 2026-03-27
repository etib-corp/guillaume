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

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/renderer.hpp"

namespace guillaume::systems
{

	/**
	 * @brief System handling rectangle rendering from ECS components.
	 * @see components::Transform
	 * @see components::Bound
	 * @see components::Color
	 * @see components::Borders
	 */
	class RectangleRender:
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::Color, components::Borders>
	{
		private:
		Renderer &_renderer;	///< Renderer instance
		std::vector<utility::graphics::Vertex<float, uint8_t>>
			_vertices;	  ///< Reused draw buffer to avoid per-frame
						  ///< allocations.

		private:
		/**
		 * @brief Extract the yaw angle in radians from a 3D rotation.
		 * @param rotation The transform rotation.
		 * @return Rotation around Z as radians.
		 */
		float extractYawRadians(
			const components::Transform::Rotation &rotation) const;

		/**
		 * @brief Rotate a 2D vector around the origin.
		 * @param vector The vector to rotate.
		 * @param angleRadians Rotation angle in radians.
		 * @return Rotated vector.
		 */
		utility::math::Vector<float, 2>
			rotateVector(const utility::math::Vector<float, 2> &vector,
						 float angleRadians) const;

		/**
		 * @brief Compute one corner radius value from Borders component values.
		 * @param borders Borders component.
		 * @return Average radius across the four corners.
		 */
		float extractAverageRadius(const components::Borders &borders) const;

		/**
		 * @brief Build local vertices for a non-rounded axis-aligned rectangle.
		 * @param halfWidth Half rectangle width.
		 * @param halfHeight Half rectangle height.
		 * @return Rectangle local vertices in clockwise order.
		 */
		std::vector<utility::math::Vector<float, 2>>
			buildAxisAlignedRectVertices(float halfWidth,
										 float halfHeight) const;

		/**
		 * @brief Append one rounded corner arc to a local vertex list.
		 * @param localVertices Target local vertex list.
		 * @param arcCenter Arc center in local space.
		 * @param startAngle Start angle in radians.
		 * @param endAngle End angle in radians.
		 * @param radius Arc radius.
		 * @param arcSegments Number of interpolation segments.
		 */
		void appendRoundedCornerArc(
			std::vector<utility::math::Vector<float, 2>> &localVertices,
			const utility::math::Vector<float, 2> &arcCenter, float startAngle,
			float endAngle, float radius, int arcSegments) const;

		/**
		 * @brief Build local rounded rectangle vertices before world transform.
		 * @param halfWidth Half rectangle width.
		 * @param halfHeight Half rectangle height.
		 * @param radius Corner radius before clamping.
		 * @param arcSegments Number of segments per rounded corner.
		 * @param epsilon Threshold used to consider radius as zero.
		 * @return Local-space outline vertices.
		 */
		std::vector<utility::math::Vector<float, 2>>
			buildLocalRoundedRectVertices(float halfWidth, float halfHeight,
										  float radius, int arcSegments,
										  float epsilon) const;

		/**
		 * @brief Transform local vertices to world-space using center and yaw.
		 * @param localVertices Vertices in local space.
		 * @param center Rectangle world center.
		 * @param angleRadians World rotation in radians.
		 * @return World-space vertices.
		 */
		std::vector<utility::math::Vector<float, 2>> transformToWorldVertices(
			const std::vector<utility::math::Vector<float, 2>> &localVertices,
			const utility::math::Vector<float, 2> &center,
			float angleRadians) const;

		/**
		 * @brief Build the outline vertices of a rounded rectangle in world
		 * space.
		 * @param center Rectangle center.
		 * @param angleRadians Rectangle rotation in radians.
		 * @param scale Rectangle scale.
		 * @param size Rectangle size before scaling.
		 * @param radius Corner radius before clamping.
		 * @param arcSegments Number of segments per rounded corner.
		 * @param epsilon Threshold used to consider radius as zero.
		 * @return World-space outline vertices.
		 */
		std::vector<utility::math::Vector<float, 2>> buildRoundedRectVertices(
			const utility::math::Vector<float, 2> &center, float angleRadians,
			const utility::math::Vector<float, 2> &scale,
			const utility::math::Vector<float, 2> &size, float radius,
			int arcSegments = 16, float epsilon = 0.001f);

		/**
		 * @brief Convert an outline to OpenGL triangle fan vertices.
		 * @param center Fan anchor.
		 * @param outline Outline vertices in draw order.
		 * @param color Vertex color.
		 * @note This method appends into the internal _vertices buffer.
		 */
		void buildTriangleFanVertices(
			const utility::math::Vector<float, 2> &center,
			const std::vector<utility::math::Vector<float, 2>> &outline,
			const utility::graphics::Color32Bit &color);

		/**
		 * @brief Create one drawable vertex from a 2D point and color.
		 * @param position 2D position.
		 * @param color Vertex color.
		 * @return Render vertex.
		 */
		utility::graphics::Vertex<float, uint8_t>
			createVertex(const utility::math::Vector<float, 2> &position,
						 const utility::graphics::Color32Bit &color) const;

		public:
		/**
		 * @brief Construct a rectangle rendering system.
		 * @param renderer The renderer used to draw rectangles.
		 */
		RectangleRender(Renderer &renderer);

		/**
		 * @brief Default destructor.
		 */
		~RectangleRender(void);

		/**
		 * @brief Update the RectangleRender system for one entity.
		 * @param entityIdentifier The target entity identifier.
		 */
		void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems