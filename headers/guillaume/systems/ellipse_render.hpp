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

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/ellipse.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/engine.hpp"
#include "guillaume/systems/render_handle_map.hpp"

namespace guillaume::systems
{
	/**
	 * @brief System handling ellipse rendering from ECS components.
	 * @see components::Transform
	 * @see components::Bound
	 * @see components::Color
	 * @see components::Ellipse
	 */
	class EllipseRender:
		public RenderHandleMap,
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::Color, components::Ellipse>
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
		 * @brief Build local vertices for an ellipse.
		 * @param halfWidth Semi-axis along X.
		 * @param halfHeight Semi-axis along Y.
		 * @param segments Number of outline segments.
		 * @return Ellipse local vertices in draw order.
		 */
		std::vector<utility::math::Vector2F>
			buildLocalEllipseVertices(float halfWidth, float halfHeight,
									  int segments) const;

		/**
		 * @brief Transform local vertices to world-space using center and
		 * orientation.
		 * @param localVertices Vertices in local space.
		 * @param center Ellipse world center.
		 * @param orientation Ellipse world orientation.
		 * @return World-space vertices.
		 */
		std::vector<utility::graphic::PositionF> transformToWorldVertices(
			const std::vector<utility::math::Vector2F> &localVertices,
			const utility::graphic::PositionF &center,
			const utility::graphic::OrientationF &orientation) const;

		/**
		 * @brief Build the outline vertices of an ellipse in world space.
		 * @param center Ellipse center.
		 * @param orientation Ellipse orientation.
		 * @param scale Ellipse scale.
		 * @param size Ellipse size before scaling.
		 * @param segments Number of outline segments.
		 * @return World-space outline vertices.
		 */
		std::vector<utility::graphic::PositionF> buildEllipseVertices(
			const utility::graphic::PositionF &center,
			const utility::graphic::OrientationF &orientation,
			const utility::math::Vector2F &scale,
			const utility::graphic::SizeF &size, int segments);

		/**
		 * @brief Convert an outline to OpenGL triangle fan vertices.
		 * @param mesh Mesh receiving the fan vertices and indices.
		 * @param center Fan anchor.
		 * @param outline Outline vertices in draw order.
		 * @param color Vertex color.
		 */
		void buildTriangleFanVertices(
			utility::graphic::Mesh &mesh,
			const utility::graphic::PositionF &center,
			const std::vector<utility::graphic::PositionF> &outline,
			const utility::graphic::Color32Bit &color);

		/**
		 * @brief Create one drawable vertex from a position and color.
		 * @param position Vertex position.
		 * @param color Vertex color.
		 * @return Render vertex.
		 */
		utility::graphic::VertexF
			createVertex(const utility::graphic::PositionF &position,
						 const utility::graphic::Color32Bit &color) const;

		public:
		/**
		 * @brief Construct an ellipse rendering system.
		 * @param engine The engine used to draw ellipses.
		 */
		EllipseRender(std::unique_ptr<Engine> &engine);

		/**
		 * @brief Default destructor.
		 */
		~EllipseRender(void);

		/**
		 * @brief Prepare the EllipseRender system before rendering.
		 *
		 * This function is called before call update on all entities and can be
		 * used to set up any necessary state or resources.
		 */
		void prepare(void) override;

		/**
		 * @brief Clean up the EllipseRender system after rendering.
		 *
		 * This function is called after call update on all entities and can be
		 * used to release any resources or reset state.
		 */
		void cleanup(void) override;

		/**
		 * @brief Update the EllipseRender system for one entity.
		 * @param entityIdentifier The target entity identifier.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
