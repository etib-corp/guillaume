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

#include "guillaume/systems/shape_mesh.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace guillaume::systems::shape
{
	utility::graphic::PositionF rotatePositionByQuaternion(
		const utility::graphic::PositionF &position,
		const utility::graphic::OrientationF &orientation)
	{
		const auto normalizedOrientation = orientation.normalized();
		const float qx					 = normalizedOrientation.x;
		const float qy					 = normalizedOrientation.y;
		const float qz					 = normalizedOrientation.z;
		const float qw					 = normalizedOrientation.w;

		const float positionX = position[0];
		const float positionY = position[1];
		const float positionZ = position[2];

		const float crossX = (qy * positionZ) - (qz * positionY);
		const float crossY = (qz * positionX) - (qx * positionZ);
		const float crossZ = (qx * positionY) - (qy * positionX);

		const float tX = 2.0f * crossX;
		const float tY = 2.0f * crossY;
		const float tZ = 2.0f * crossZ;

		const float crossTX = (qy * tZ) - (qz * tY);
		const float crossTY = (qz * tX) - (qx * tZ);
		const float crossTZ = (qx * tY) - (qy * tX);

		return utility::graphic::PositionF(positionX + (qw * tX) + crossTX,
										   positionY + (qw * tY) + crossTY,
										   positionZ + (qw * tZ) + crossTZ);
	}

	std::vector<utility::graphic::PositionF> transformToWorldVertices(
		const std::vector<utility::math::Vector2F> &localVertices,
		const utility::graphic::PositionF &center,
		const utility::graphic::OrientationF &orientation)
	{
		std::vector<utility::graphic::PositionF> worldVertices;
		worldVertices.reserve(localVertices.size());
		for (const auto &localVertex: localVertices) {
			const utility::graphic::PositionF localPosition(
				localVertex[0], localVertex[1], 0.0f);
			const auto rotatedPosition =
				rotatePositionByQuaternion(localPosition, orientation);
			worldVertices.push_back(utility::graphic::PositionF(
				center[0] + rotatedPosition[0], center[1] + rotatedPosition[1],
				center[2] + rotatedPosition[2]));
		}
		return worldVertices;
	}

	utility::graphic::VertexF
		createVertex(const utility::graphic::PositionF &position,
					 const utility::graphic::Color32Bit &color)
	{
		utility::graphic::VertexF vertex;
		vertex.setPosition(position);
		vertex.setColor(color);
		return vertex;
	}

	void buildTriangleFanVertices(
		utility::graphic::Mesh &mesh, const utility::graphic::PositionF &center,
		const std::vector<utility::graphic::PositionF> &outline,
		const utility::graphic::Color32Bit &color)
	{
		mesh.addVertex(createVertex(center, color));

		for (const auto &outlineVertex: outline) {
			mesh.addVertex(createVertex(outlineVertex, color));
		}

		if (outline.size() >= 2) {
			mesh.addIndex(0);
			mesh.addIndex(1);
			mesh.addIndex(2);

			for (size_t i = 2; i < outline.size(); ++i) {
				mesh.addIndex(0);
				mesh.addIndex(static_cast<uint32_t>(i));
				mesh.addIndex(static_cast<uint32_t>(i + 1));
			}

			mesh.addIndex(0);
			mesh.addIndex(static_cast<uint32_t>(outline.size()));
			mesh.addIndex(1);
		} else if (outline.size() == 1) {
			mesh.addIndex(0);
			mesh.addIndex(1);
			mesh.addIndex(1);
		}
	}

	void buildRingMesh(utility::graphic::Mesh &mesh,
					   const utility::graphic::PositionF &center,
					   const utility::graphic::OrientationF &orientation,
					   float outerHalfWidth, float outerHalfHeight,
					   float thickness, float startAngle, float sweepAngle,
					   int segments, bool closed,
					   const utility::graphic::Color32Bit &color)
	{
		if (segments <= 0) {
			segments = 1;
		}

		const float outerWidth	= std::abs(outerHalfWidth);
		const float outerHeight = std::abs(outerHalfHeight);
		const float innerWidth =
			std::max(0.0f, outerWidth - std::abs(thickness));
		const float innerHeight =
			std::max(0.0f, outerHeight - std::abs(thickness));

		const int sampleCount = closed ? segments : segments + 1;
		const float step	  = sweepAngle / static_cast<float>(segments);

		std::vector<utility::math::Vector2F> localVertices;
		localVertices.reserve(static_cast<std::size_t>(sampleCount) * 2u);

		for (int i = 0; i < sampleCount; ++i) {
			const float angle = startAngle + step * static_cast<float>(i);
			const float cos	  = std::cos(angle);
			const float sin	  = std::sin(angle);
			localVertices.push_back(utility::math::Vector2F(
				{ outerWidth * cos, outerHeight * sin }));
			localVertices.push_back(utility::math::Vector2F(
				{ innerWidth * cos, innerHeight * sin }));
		}

		for (const auto &worldVertex:
			 transformToWorldVertices(localVertices, center, orientation)) {
			mesh.addVertex(createVertex(worldVertex, color));
		}

		for (int i = 0; i < segments; ++i) {
			const int nextIndex	  = closed ? (i + 1) % sampleCount : i + 1;
			const uint32_t outerA = static_cast<uint32_t>(2 * i);
			const uint32_t innerA = static_cast<uint32_t>(2 * i + 1);
			const uint32_t outerB = static_cast<uint32_t>(2 * nextIndex);
			const uint32_t innerB = static_cast<uint32_t>(2 * nextIndex + 1);

			mesh.addIndex(outerA);
			mesh.addIndex(innerA);
			mesh.addIndex(outerB);

			mesh.addIndex(innerA);
			mesh.addIndex(innerB);
			mesh.addIndex(outerB);
		}
	}

	void buildLineMesh(utility::graphic::Mesh &mesh,
					   const utility::graphic::PositionF &center,
					   const utility::graphic::OrientationF &orientation,
					   float halfLength, float halfThickness, bool dashed,
					   float dashLength, float gapLength,
					   const utility::graphic::Color32Bit &color)
	{
		const float length = std::abs(halfLength);
		const float half   = std::abs(halfThickness);

		std::vector<utility::math::Vector2F> localVertices;

		auto appendQuad = [&localVertices, half](float start, float end) {
			localVertices.push_back(utility::math::Vector2F({ start, -half }));
			localVertices.push_back(utility::math::Vector2F({ end, -half }));
			localVertices.push_back(utility::math::Vector2F({ end, half }));
			localVertices.push_back(utility::math::Vector2F({ start, half }));
		};

		if (!dashed || dashLength <= 0.0f) {
			appendQuad(-length, length);
		} else {
			const float period = dashLength + std::max(0.0f, gapLength);
			float cursor	   = -length;

			while (cursor < length) {
				const float dashEnd = std::min(cursor + dashLength, length);
				appendQuad(cursor, dashEnd);
				if (period <= 0.0f) {
					break;
				}
				cursor += period;
			}
		}

		for (const auto &worldVertex:
			 transformToWorldVertices(localVertices, center, orientation)) {
			mesh.addVertex(createVertex(worldVertex, color));
		}

		const uint32_t quadCount =
			static_cast<uint32_t>(localVertices.size() / 4u);
		for (uint32_t i = 0; i < quadCount; ++i) {
			const uint32_t base = i * 4u;
			mesh.addIndex(base + 0u);
			mesh.addIndex(base + 1u);
			mesh.addIndex(base + 2u);

			mesh.addIndex(base + 0u);
			mesh.addIndex(base + 2u);
			mesh.addIndex(base + 3u);
		}
	}

}	 // namespace guillaume::systems::shape
