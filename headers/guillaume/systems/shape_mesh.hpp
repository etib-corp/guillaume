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

#include <utility/graphic/color.hpp>
#include <utility/graphic/mesh.hpp>
#include <utility/graphic/orientation.hpp>
#include <utility/graphic/pose.hpp>
#include <utility/math/vector.hpp>

namespace guillaume::systems::shape
{
	/**
	 * @brief Rotate a 3D point by an orientation quaternion.
	 * @param position Point to rotate.
	 * @param orientation Orientation used as the rotation.
	 * @return Rotated point.
	 */
	utility::graphic::PositionF rotatePositionByQuaternion(
		const utility::graphic::PositionF &position,
		const utility::graphic::OrientationF &orientation);

	/**
	 * @brief Transform local 2D vertices to world space.
	 * @param localVertices Vertices in local space.
	 * @param center World center.
	 * @param orientation World orientation.
	 * @return World-space vertices.
	 */
	std::vector<utility::graphic::PositionF> transformToWorldVertices(
		const std::vector<utility::math::Vector2F> &localVertices,
		const utility::graphic::PositionF &center,
		const utility::graphic::OrientationF &orientation);

	/**
	 * @brief Create one drawable vertex from a position and color.
	 * @param position Vertex position.
	 * @param color Vertex color.
	 * @return Render vertex.
	 */
	utility::graphic::VertexF
		createVertex(const utility::graphic::PositionF &position,
					 const utility::graphic::Color32Bit &color);

	/**
	 * @brief Convert an outline to triangle fan vertices and indices.
	 * @param mesh Mesh receiving the fan vertices and indices.
	 * @param center Fan anchor.
	 * @param outline Outline vertices in draw order.
	 * @param color Vertex color.
	 */
	void buildTriangleFanVertices(
		utility::graphic::Mesh &mesh, const utility::graphic::PositionF &center,
		const std::vector<utility::graphic::PositionF> &outline,
		const utility::graphic::Color32Bit &color);

	/**
	 * @brief Build an annulus (ring) mesh from an axis-aligned bounding box.
	 *
	 * The outer boundary is the bounding box half extents; the inner boundary
	 * is the outer boundary shrunk by `thickness`. When @p closed is true the
	 * strip wraps around (full ring, sweep usually `2 * pi`); otherwise the
	 * strip is left open (an arc segment with flat ends).
	 *
	 * @param mesh Mesh receiving the vertices and indices.
	 * @param center World center of the bounding box.
	 * @param orientation World orientation.
	 * @param outerHalfWidth Outer half extent along X.
	 * @param outerHalfHeight Outer half extent along Y.
	 * @param thickness Radial thickness of the ring.
	 * @param startAngle Angle of the first sample, in radians.
	 * @param sweepAngle Angular span of the ring, in radians.
	 * @param segments Number of segments along the ring.
	 * @param closed Whether the ring wraps around.
	 * @param color Vertex color.
	 */
	void buildRingMesh(utility::graphic::Mesh &mesh,
					   const utility::graphic::PositionF &center,
					   const utility::graphic::OrientationF &orientation,
					   float outerHalfWidth, float outerHalfHeight,
					   float thickness, float startAngle, float sweepAngle,
					   int segments, bool closed,
					   const utility::graphic::Color32Bit &color);

	/**
	 * @brief Build a horizontal stroke mesh from an axis-aligned bounding box.
	 *
	 * The line runs from the left edge to the right edge of the bounding box,
	 * vertically centered, with the given thickness. When @p dashed is true the
	 * stroke is split into dashes separated by gaps.
	 *
	 * @param mesh Mesh receiving the vertices and indices.
	 * @param center World center of the bounding box.
	 * @param orientation World orientation.
	 * @param halfLength Half length of the stroke along X.
	 * @param halfThickness Half thickness of the stroke along Y.
	 * @param dashed Whether to draw a dashed stroke.
	 * @param dashLength Length of one dash when dashed.
	 * @param gapLength Length of the gap between dashes when dashed.
	 * @param color Vertex color.
	 */
	void buildLineMesh(utility::graphic::Mesh &mesh,
					   const utility::graphic::PositionF &center,
					   const utility::graphic::OrientationF &orientation,
					   float halfLength, float halfThickness, bool dashed,
					   float dashLength, float gapLength,
					   const utility::graphic::Color32Bit &color);

}	 // namespace guillaume::systems::shape
