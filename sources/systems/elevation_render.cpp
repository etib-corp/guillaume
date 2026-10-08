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

#include "guillaume/systems/elevation_render.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <vector>

#include "guillaume/mesh_renderable.hpp"

namespace guillaume::systems
{
	namespace
	{
		utility::graphic::PositionF rotateByOrientation(
			const utility::graphic::PositionF &position,
			const utility::graphic::OrientationF &orientation)
		{
			const auto normalized = orientation.normalized();
			const float qx		  = normalized.x;
			const float qy		  = normalized.y;
			const float qz		  = normalized.z;
			const float qw		  = normalized.w;

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

		std::vector<utility::math::Vector2F>
			buildRoundedRectOutline(float halfWidth, float halfHeight,
									float radius, int arcSegments)
		{
			const float pi = std::acos(-1.0f);

			if (arcSegments <= 0) {
				arcSegments = 1;
			}

			const float cornerRadius =
				std::max(0.0f, (std::min)({ radius, halfWidth, halfHeight }));

			std::vector<utility::math::Vector2F> localVertices;

			if (cornerRadius <= 0.0f) {
				localVertices = {
					utility::math::Vector2F({ -halfWidth, -halfHeight }),
					utility::math::Vector2F({ halfWidth, -halfHeight }),
					utility::math::Vector2F({ halfWidth, halfHeight }),
					utility::math::Vector2F({ -halfWidth, halfHeight }),
				};
				return localVertices;
			}

			auto appendArc = [&localVertices, arcSegments](
								 const utility::math::Vector2F &center,
								 float startAngle, float endAngle,
								 float arcRadius) {
				for (int i = 0; i <= arcSegments; ++i) {
					const float t =
						static_cast<float>(i) / static_cast<float>(arcSegments);
					const float angle =
						startAngle + (endAngle - startAngle) * t;
					localVertices.push_back(
						center
						+ utility::math::Vector2F(
							{ std::cos(angle) * arcRadius,
							  std::sin(angle) * arcRadius }));
				}
			};

			appendArc(utility::math::Vector2F({ halfWidth - cornerRadius,
												-halfHeight + cornerRadius }),
					  -pi / 2.0f, 0.0f, cornerRadius);
			appendArc(utility::math::Vector2F({ halfWidth - cornerRadius,
												halfHeight - cornerRadius }),
					  0.0f, pi / 2.0f, cornerRadius);
			appendArc(utility::math::Vector2F({ -halfWidth + cornerRadius,
												halfHeight - cornerRadius }),
					  pi / 2.0f, pi, cornerRadius);
			appendArc(utility::math::Vector2F({ -halfWidth + cornerRadius,
												-halfHeight + cornerRadius }),
					  pi, 3.0f * pi / 2.0f, cornerRadius);

			return localVertices;
		}

		void appendLayer(utility::graphic::Mesh &mesh,
						 const utility::graphic::PositionF &center,
						 const utility::graphic::OrientationF &orientation,
						 float halfWidth, float halfHeight, float radius,
						 const utility::graphic::Color32Bit &color)
		{
			const auto outline =
				buildRoundedRectOutline(halfWidth, halfHeight, radius, 8);

			const uint32_t base =
				static_cast<uint32_t>(mesh.getVertices().size());

			utility::graphic::VertexF centerVertex;
			centerVertex.setPosition(center);
			centerVertex.setColor(color);
			mesh.addVertex(centerVertex);

			for (const auto &localVertex: outline) {
				const utility::graphic::PositionF localPosition(
					localVertex[0], localVertex[1], 0.0f);
				const auto rotated =
					rotateByOrientation(localPosition, orientation);

				utility::graphic::VertexF vertex;
				vertex.setPosition(utility::graphic::PositionF(
					center[0] + rotated[0], center[1] + rotated[1],
					center[2] + rotated[2]));
				vertex.setColor(color);
				mesh.addVertex(vertex);
			}

			for (std::size_t i = 1; i < outline.size(); ++i) {
				mesh.addIndex(base);
				mesh.addIndex(base + static_cast<uint32_t>(i));
				mesh.addIndex(base + static_cast<uint32_t>(i + 1));
			}
			mesh.addIndex(base);
			mesh.addIndex(base + static_cast<uint32_t>(outline.size()));
			mesh.addIndex(base + 1u);
		}
	}	 // namespace

	ElevationRender::ElevationRender(std::unique_ptr<utility::Engine> &engine)
		: ecs::SystemFiller<components::Transform, components::Bound,
							components::Elevation>(ecs::Phase::Render)
		, _engine(engine)
	{
	}

	void ElevationRender::prepare(void)
	{
		markAllUnused();
	}

	void ElevationRender::cleanup(void)
	{
		removeUnused(*_engine);
	}

	void
		ElevationRender::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Elevation>(entityIdentifier)) {
			return;
		}

		const auto &pose =
			getComponent<components::Transform>(entityIdentifier).getPose();
		const auto &bound = getComponent<components::Bound>(entityIdentifier);
		const auto &elevation =
			getComponent<components::Elevation>(entityIdentifier);

		const utility::graphic::PositionF center(
			pose.getPosition().x + bound.getWidth() / 2.0f,
			pose.getPosition().y + bound.getHeight() / 2.0f,
			pose.getPosition().z);

		const float halfWidth  = bound.getWidth() / 2.0f;
		const float halfHeight = bound.getHeight() / 2.0f;

		const int layers		= std::max(1, elevation.getLayers());
		const auto &orientation = pose.getOrientation();
		const auto &color		= elevation.getColor();

		utility::graphic::Mesh mesh(std::vector<utility::graphic::VertexF> {},
									std::vector<uint32_t> {});

		for (int i = 0; i < layers; ++i) {
			const float t =
				static_cast<float>(i + 1) / static_cast<float>(layers);
			const float expansion =
				elevation.getSpread() * elevation.getLevel() * t;
			const float alpha =
				static_cast<float>(color.getAlpha()) * (1.0f - t);

			appendLayer(mesh, center, orientation, halfWidth + expansion,
						halfHeight + expansion,
						(std::min)(halfWidth, halfHeight) + expansion,
						color.withAlpha(static_cast<std::uint8_t>(alpha)));
		}

		auto renderable = std::make_shared<MeshRenderable>(
			std::vector<utility::graphic::Mesh> { mesh }, "mesh_material");

		RenderHandle *handle = find(entityIdentifier);
		if (handle == nullptr) {
			insert(entityIdentifier,
				   RenderHandle { _engine->createObject(renderable), renderable,
								  true });
		} else {
			if (!_engine->updateObject(renderable, handle->objectId)) {
				handle->objectId = _engine->createObject(renderable);
			}
			handle->renderable = renderable;
			handle->used	   = true;
		}
	}

}	 // namespace guillaume::systems
