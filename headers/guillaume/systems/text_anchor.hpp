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

#include <algorithm>
#include <limits>

#include <utility/graphic/mesh.hpp>
#include <utility/graphic/pose.hpp>
#include <utility/graphic/position.hpp>
#include <utility/graphic/text/text.hpp>

#include "guillaume/systems/shape_mesh.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Anchor a text renderable on the top-left of its glyph box.
	 *
	 * `utility::graphic::Text` builds its glyph quads in local space with the
	 * origin near the ascender line, so `Transform.position` does not land on
	 * the top-left of the drawn box the way it does for the shape renderers.
	 * This offsets the renderable pose so the top-left corner of the glyph
	 * box maps exactly to @p anchor's position, keeping every drawable on the
	 * same top-left origin convention as `systems::RectangleRender` and
	 * friends.
	 *
	 * The renderable is left untouched when its mesh holds no vertex.
	 *
	 * @param text The text renderable to re-anchor (updated in place).
	 * @param anchor The pose whose position is the requested top-left corner.
	 */
	inline void anchorTextTopLeft(utility::graphic::Text &text,
								  const utility::graphic::PoseF &anchor)
	{
		float minX	   = std::numeric_limits<float>::max();
		float maxY	   = std::numeric_limits<float>::lowest();
		bool hasVertex = false;

		for (const auto &mesh: text.getMeshes()) {
			if (mesh == nullptr) {
				continue;
			}

			for (const auto &vertex: mesh->getVertices()) {
				const auto &position = vertex.getPosition();
				minX				 = std::min(minX, position[0]);
				maxY				 = std::max(maxY, position[1]);
				hasVertex			 = true;
			}
		}

		if (!hasVertex) {
			return;
		}

		// The glyph box spans [minX, ...] x [..., maxY] in local space, so
		// its top-left corner is (minX, maxY). Solve T + R * localTopLeft =
		// anchorPosition for the model translation T.
		const utility::graphic::PositionF localTopLeft(minX, maxY, 0.0f);
		const auto rotatedOffset = shape::rotatePositionByQuaternion(
			localTopLeft, anchor.getOrientation());
		const auto &anchorPosition = anchor.getPosition();

		text.setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(anchorPosition[0] - rotatedOffset[0],
										anchorPosition[1] - rotatedOffset[1],
										anchorPosition[2] - rotatedOffset[2]),
			anchor.getOrientation()));
	}

}	 // namespace guillaume::systems
