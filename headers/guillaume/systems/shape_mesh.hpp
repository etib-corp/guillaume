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

/**
 * @file
 * @brief Mesh builders shared by the shape render systems.
 *
 * The functions of guillaume::systems::shape turn a center, an orientation
 * and a few lengths into the vertices and triangle indices of a flat shape:
 * a filled outline, a ring or arc, a straight stroke, a rounded-rectangle
 * border. They read no component and keep no state.
 *
 * Used by RectangleRender, EllipseRender, RingRender, ArcRender, LineRender,
 * ScrimRender and FocusRingRender.
 *
 * Shapes are described in their own plane (X to the right, Y along the
 * height) around a center, then rotated by the orientation and moved to the
 * center. Lengths are in the unit of components::Bound (dp).
 *
 * Every builder writes indices counted from the first vertex of the mesh:
 * pass an empty mesh, one per shape.
 */

#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/mesh.hpp>
#include <utility/graphic/orientation.hpp>
#include <utility/graphic/pose.hpp>
#include <utility/math/vector.hpp>

namespace guillaume::systems::shape
{
	/**
	 * @brief Rotate a 3D point around the origin by a quaternion.
	 * @param position The point to rotate.
	 * @param orientation The rotation. It is normalized before use.
	 * @return The rotated point.
	 */
	utility::graphic::PositionF rotatePositionByQuaternion(
		const utility::graphic::PositionF &position,
		const utility::graphic::OrientationF &orientation);

	/**
	 * @brief Move 2D points of a shape's plane into world space.
	 *
	 * Each point gets a zero depth, is rotated by @p orientation and is then
	 * translated by @p center.
	 * @param localVertices The points, relative to the center of the shape.
	 * @param center World position of the center of the shape.
	 * @param orientation World orientation of the shape.
	 * @return One world position per input point, in the same order.
	 */
	std::vector<utility::graphic::PositionF> transformToWorldVertices(
		const std::vector<utility::math::Vector2F> &localVertices,
		const utility::graphic::PositionF &center,
		const utility::graphic::OrientationF &orientation);

	/**
	 * @brief Make a mesh vertex from a position and a color.
	 * @param position World position of the vertex.
	 * @param color Color of the vertex.
	 * @return The vertex, with no other attribute set.
	 */
	utility::graphic::VertexF
		createVertex(const utility::graphic::PositionF &position,
					 const utility::graphic::Color32Bit &color);

	/**
	 * @brief Fill a closed outline with a fan of triangles around its center.
	 *
	 * Adds the center as vertex 0, then the outline, and one triangle per
	 * outline edge, including the edge that closes the outline. The result is
	 * correct for convex outlines such as rectangles, rounded rectangles and
	 * ellipses.
	 * @param mesh The mesh to fill. It must be empty: indices start at 0.
	 * @param center World position shared by every triangle.
	 * @param outline World positions of the outline, in drawing order. With
	 * fewer than two points no visible triangle is produced.
	 * @param color Color given to every vertex.
	 */
	void buildTriangleFanVertices(
		utility::graphic::Mesh &mesh, const utility::graphic::PositionF &center,
		const std::vector<utility::graphic::PositionF> &outline,
		const utility::graphic::Color32Bit &color);

	/**
	 * @brief Build a ring, or a part of one, as a strip of triangles.
	 *
	 * The outer edge is the ellipse of half extents (@p outerHalfWidth,
	 * @p outerHalfHeight). The inner edge is the same ellipse with both half
	 * extents reduced by @p thickness, never below zero. With @p closed true
	 * the last segment joins the first one (a full ring: pass a sweep of
	 * `2 * pi`); with @p closed false the strip has two flat ends (an arc).
	 *
	 * @param mesh The mesh to fill. It must be empty: indices start at 0.
	 * @param center World position of the center of the ring.
	 * @param orientation World orientation of the ring.
	 * @param outerHalfWidth Outer half extent along X, in dp. The sign is
	 * ignored.
	 * @param outerHalfHeight Outer half extent along Y, in dp. The sign is
	 * ignored.
	 * @param thickness Distance between the outer and inner edges, in dp.
	 * @param startAngle Angle of the first sample, in radians. Zero points
	 * along +X and the angle grows towards +Y.
	 * @param sweepAngle Angle covered from @p startAngle, in radians. A
	 * negative value sweeps the other way.
	 * @param segments Number of quads along the ring. A value of zero or
	 * less is treated as 1.
	 * @param closed True to join the end to the start.
	 * @param color Color given to every vertex.
	 */
	void buildRingMesh(utility::graphic::Mesh &mesh,
					   const utility::graphic::PositionF &center,
					   const utility::graphic::OrientationF &orientation,
					   float outerHalfWidth, float outerHalfHeight,
					   float thickness, float startAngle, float sweepAngle,
					   int segments, bool closed,
					   const utility::graphic::Color32Bit &color);

	/**
	 * @brief Build a straight stroke along X, solid or dashed.
	 *
	 * The stroke runs from `-halfLength` to `+halfLength` around the center
	 * and is centered on the X axis. A dashed stroke starts with a dash at
	 * the left end; the last dash is cut at the right end.
	 *
	 * @param mesh The mesh to fill. It must be empty: indices start at 0.
	 * @param center World position of the middle of the stroke.
	 * @param orientation World orientation of the stroke.
	 * @param halfLength Half the length of the stroke, in dp. The sign is
	 * ignored.
	 * @param halfThickness Half the thickness of the stroke, in dp. The sign
	 * is ignored.
	 * @param dashed True to split the stroke into dashes.
	 * @param dashLength Length of one dash, in dp. With a value of zero or
	 * less the stroke is drawn solid even when @p dashed is true.
	 * @param gapLength Empty length between two dashes, in dp. A negative
	 * value is treated as zero.
	 * @param color Color given to every vertex.
	 */
	void buildLineMesh(utility::graphic::Mesh &mesh,
					   const utility::graphic::PositionF &center,
					   const utility::graphic::OrientationF &orientation,
					   float halfLength, float halfThickness, bool dashed,
					   float dashLength, float gapLength,
					   const utility::graphic::Color32Bit &color);

	/**
	 * @brief Build the border of a rounded rectangle as a band of constant
	 * width.
	 *
	 * The band lies between the rounded rectangle of half extents
	 * (@p halfWidth, @p halfHeight) and corner radius @p radius, and the same
	 * rectangle inset by @p width on every side, whose corner radius is
	 * reduced by @p width (never below zero). It grows inwards: the outer
	 * edge never exceeds the given half extents.
	 *
	 * @param mesh The mesh to fill. It must be empty: indices start at 0.
	 * @param center World position of the center of the rectangle.
	 * @param orientation World orientation of the rectangle.
	 * @param halfWidth Outer half width, in dp.
	 * @param halfHeight Outer half height, in dp.
	 * @param radius Outer corner radius, in dp. It is clamped between zero
	 * and the smaller half extent.
	 * @param width Width of the band, in dp. It is clamped to the smaller
	 * half extent.
	 * @param arcSegments Number of segments per corner. A value of zero or
	 * less is treated as 1.
	 * @param color Color given to every vertex.
	 */
	void buildRoundedStrokeMesh(
		utility::graphic::Mesh &mesh, const utility::graphic::PositionF &center,
		const utility::graphic::OrientationF &orientation, float halfWidth,
		float halfHeight, float radius, float width, int arcSegments,
		const utility::graphic::Color32Bit &color);

}	 // namespace guillaume::systems::shape
