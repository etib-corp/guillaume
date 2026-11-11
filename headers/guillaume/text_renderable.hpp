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
#include <cstdint>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/mesh.hpp>
#include <utility/graphic/pose.hpp>
#include <utility/graphic/position.hpp>
#include <utility/graphic/text/text.hpp>
#include <utility/ressource_provider.hpp>

#include "guillaume/systems/shape_mesh.hpp"

namespace guillaume
{
	/**
	 * @brief Text renderable that bakes the entity pose into its glyph mesh.
	 *
	 * `utility::graphic::Text` stores its glyph quads in local (object) space
	 * and relies on the renderer applying `Renderable::getModelMatrix()`. Every
	 * other drawable Guillaume emits (shapes, images, models) bakes world
	 * coordinates into its vertices and uses an identity pose, so a renderer
	 * that draws vertex buffers directly would leave text at the mesh origin.
	 *
	 * This subclass keeps the `Text` renderable type (so the font material and
	 * the engine's text path are preserved) but rewrites the glyph vertices to
	 * world space and resets the pose to identity. Text then lands on its
	 * entity position whether or not the renderer applies the model matrix.
	 *
	 * @see systems::TextRender
	 * @see systems::GlyphRender
	 */
	class TextRenderable: public utility::graphic::Text
	{
		public:
		/**
		 * @brief Build a text renderable anchored on the entity pose.
		 *
		 * @param ressourceProvider The resource provider used to load the font.
		 * @param anchor The entity pose; its position is the glyph-box
		 * top-left.
		 * @param color The text color.
		 * @param content The text content.
		 * @param fontSize The font size.
		 * @param font The font path.
		 */
		TextRenderable(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			const utility::graphic::PoseF &anchor,
			const utility::graphic::Color32Bit &color,
			const std::string &content, uint32_t fontSize,
			const std::string &font)
			: utility::graphic::Text(ressourceProvider,
									 utility::graphic::PoseF(), color, content,
									 fontSize, font)
		{
			bake(anchor);
		}

		/**
		 * @brief Default destructor.
		 */
		~TextRenderable(void) override = default;

		private:
		/**
		 * @brief Rewrite the local glyph mesh into world space.
		 *
		 * @param anchor The entity pose to anchor the glyph-box top-left on.
		 */
		void bake(const utility::graphic::PoseF &anchor)
		{
			if (_meshes.empty() || _meshes.front() == nullptr) {
				return;
			}

			const auto &source	 = _meshes.front();
			const auto &vertices = source->getVertices();

			if (vertices.empty()) {
				return;
			}

			float minX = std::numeric_limits<float>::max();
			float maxY = std::numeric_limits<float>::lowest();

			for (const auto &vertex: vertices) {
				const auto &position = vertex.getPosition();
				minX				 = std::min(minX, position[0]);
				maxY				 = std::max(maxY, position[1]);
			}

			// Solve T + R * localTopLeft = anchorPosition so the glyph-box
			// top-left lands exactly on the entity position.
			const utility::graphic::PositionF localTopLeft(minX, maxY, 0.0f);
			const auto rotatedOffset =
				systems::shape::rotatePositionByQuaternion(
					localTopLeft, anchor.getOrientation());
			const auto &anchorPosition = anchor.getPosition();

			const utility::graphic::PositionF origin(
				anchorPosition[0] - rotatedOffset[0],
				anchorPosition[1] - rotatedOffset[1],
				anchorPosition[2] - rotatedOffset[2]);

			auto baked = std::make_shared<utility::graphic::Mesh>(
				std::vector<utility::graphic::VertexF> {},
				std::vector<uint32_t> {});

			for (const auto &vertex: vertices) {
				const auto rotated = systems::shape::rotatePositionByQuaternion(
					vertex.getPosition(), anchor.getOrientation());

				auto bakedVertex = vertex;
				bakedVertex.setPosition(utility::graphic::PositionF(
					origin[0] + rotated[0], origin[1] + rotated[1],
					origin[2] + rotated[2]));
				baked->addVertex(bakedVertex);
			}

			for (const auto index: source->getIndices()) {
				baked->addIndex(index);
			}

			_meshes.front() = baked;
			_pose			= utility::graphic::PoseF();
		}
	};

}	 // namespace guillaume
