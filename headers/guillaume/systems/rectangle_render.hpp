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

#include <map>
#include <optional>

#include <utility/cache.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/engine.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Key structure for caching rectangle rendering results.
	 */
	struct RectangleRenderCacheKey {
		utility::graphic::PoseF pose;	 ///< The pose of the rectangle,
										 ///< including position and orientation
		utility::graphic::SizeF
			size;	 ///< The size of the rectangle, including width and height
		components::Borders borders;	///< The borders of the rectangle,
										///< including corner radius
		utility::graphic::Color32Bit
			color;	  ///< The color of the rectangle, including RGBA values

		/**
		 * @brief Equality operator for RectangleRenderCacheKey.
		 * @param other The other RectangleRenderCacheKey to compare with.
		 * @return True if the keys are equal, false otherwise.
		 */
		bool operator==(const RectangleRenderCacheKey &other) const
		{
			if (pose != other.pose) {
				return false;
			}
			if (size != other.size) {
				return false;
			}
			if (borders != other.borders) {
				return false;
			}
			if (color != other.color) {
				return false;
			}
			return true;
		}

		/**
		 * @brief Inequality operator for RectangleRenderCacheKey.
		 * @param other The other RectangleRenderCacheKey to compare with.
		 * @return True if the keys are not equal, false otherwise.
		 */
		bool operator!=(const RectangleRenderCacheKey &other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Less-than operator for RectangleRenderCacheKey.
		 * @param other The other RectangleRenderCacheKey to compare with.
		 * @return True if this key is less than the other, false otherwise.
		 */
		bool operator<(const RectangleRenderCacheKey &other) const
		{
			if (pose != other.pose) {
				return pose < other.pose;
			}
			if (size != other.size) {
				return size < other.size;
			}
			if (borders != other.borders) {
				return borders < other.borders;
			}
			if (color != other.color) {
				return color < other.color;
			}
			return false;
		}
	};

	/**
	 * @brief Entry structure for caching rectangle rendering results.
	 */
	struct RectangleRenderCacheEntry {
		bool used;	  ///< Flag indicating whether the cache entry has been used
					  ///< in the current frame
		size_t value;	 ///< The cached value associated with the glyph
						 ///< rendering result
	};

	/**
	 * @brief System handling rectangle rendering from ECS components.
	 * @see components::Transform
	 * @see components::Bound
	 * @see components::Color
	 * @see components::Borders
	 */
	class RectangleRender:
		public utility::Cache<RectangleRenderCacheKey,
							  RectangleRenderCacheEntry>,
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::Color, components::Borders>
	{
		private:
		std::unique_ptr<Engine> &_engine;	 ///< Engine instance

		private:
		/**
		 * @brief Rotate a 3D point by a quaternion.
		 * @param position Point to rotate.
		 * @param orientation Orientation used as rotation.
		 * @return Rotated point.
		 */
		utility::graphic::PositionF rotatePositionByQuaternion(
			const utility::graphic::PositionF &position,
			const utility::graphic::OrientationF &orientation) const;

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
		std::vector<utility::math::Vector2F>
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
			std::vector<utility::math::Vector2F> &localVertices,
			const utility::math::Vector2F &arcCenter, float startAngle,
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
		std::vector<utility::math::Vector2F>
			buildLocalRoundedRectVertices(float halfWidth, float halfHeight,
										  float radius, int arcSegments,
										  float epsilon) const;

		/**
		 * @brief Transform local vertices to world-space using center and
		 * orientation.
		 * @param localVertices Vertices in local space.
		 * @param center Rectangle world center.
		 * @param orientation Rectangle world orientation.
		 * @return World-space vertices.
		 */
		std::vector<utility::graphic::PositionF> transformToWorldVertices(
			const std::vector<utility::math::Vector2F> &localVertices,
			const utility::graphic::PositionF &center,
			const utility::graphic::OrientationF &orientation) const;

		/**
		 * @brief Build the outline vertices of a rounded rectangle in world
		 * space.
		 * @param center Rectangle center.
		 * @param orientation Rectangle orientation.
		 * @param scale Rectangle scale.
		 * @param size Rectangle size before scaling.
		 * @param radius Corner radius before clamping.
		 * @param arcSegments Number of segments per rounded corner.
		 * @param epsilon Threshold used to consider radius as zero.
		 * @return World-space outline vertices.
		 */
		std::vector<utility::graphic::PositionF> buildRoundedRectVertices(
			const utility::graphic::PositionF &center,
			const utility::graphic::OrientationF &orientation,
			const utility::math::Vector2F &scale,
			const utility::graphic::SizeF &size, float radius,
			int arcSegments = 16, float epsilon = 0.001f);

		/**
		 * @brief Convert an outline to OpenGL triangle fan vertices.
		 * @param center Fan anchor.
		 * @param outline Outline vertices in draw order.
		 * @param color Vertex color.
		 * @note This method appends into the internal _vertices buffer.
		 */
		void buildTriangleFanVertices(
			utility::graphic::Mesh &mesh,
			const utility::graphic::PositionF &center,
			const std::vector<utility::graphic::PositionF> &outline,
			const utility::graphic::Color32Bit &color);

		/**
		 * @brief Create one drawable vertex from a 2D point and color.
		 * @param position 2D position.
		 * @param color Vertex color.
		 * @return Render vertex.
		 */
		utility::graphic::VertexF
			createVertex(const utility::graphic::PositionF &position,
						 const utility::graphic::Color32Bit &color) const;

		public:
		/**
		 * @brief Construct a rectangle rendering system.
		 * @param engine The engine used to draw rectangles.
		 */
		RectangleRender(std::unique_ptr<Engine> &engine);

		/**
		 * @brief Default destructor.
		 */
		~RectangleRender(void);

		/**
		 * @brief Prepare the RectangleRender system before rendering.
		 *
		 * This function is called before call update on all entities and can be
		 * used to set up any necessary state or resources.
		 */
		void prepare(void) override;

		/**
		 * @brief Clean up the RectangleRender system after rendering.
		 *
		 * This function is called after call update on all entities and can be
		 * used to release any resources or reset state.
		 */
		void cleanup(void) override;

		/**
		 * @brief Update the RectangleRender system for one entity.
		 * @param entityIdentifier The target entity identifier.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems