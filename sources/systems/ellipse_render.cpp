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

#include "guillaume/systems/ellipse_render.hpp"
#include "guillaume/mesh_renderable.hpp"

#include <algorithm>
#include <cmath>
#include <memory>

namespace guillaume::systems
{
	utility::graphic::PositionF EllipseRender::rotatePositionByQuaternion(
		const utility::graphic::PositionF &position,
		const utility::graphic::OrientationF &orientation) const
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

	std::vector<utility::math::Vector2F>
		EllipseRender::buildLocalEllipseVertices(const float halfWidth,
												 const float halfHeight,
												 int segments) const
	{
		const float pi = std::acos(-1.0f);

		if (segments <= 0) {
			segments = 1;
		}

		std::vector<utility::math::Vector2F> localVertices;
		localVertices.reserve(static_cast<std::size_t>(segments + 1));

		// Parametric ellipse: x = a * cos(t), y = b * sin(t)
		for (int i = 0; i <= segments; ++i) {
			const float t =
				(static_cast<float>(i) / static_cast<float>(segments)) * 2.0f
				* pi;
			const float x = halfWidth * std::cos(t);
			const float y = halfHeight * std::sin(t);
			localVertices.push_back(utility::math::Vector2F({ x, y }));
		}

		return localVertices;
	}

	std::vector<utility::graphic::PositionF>
		EllipseRender::transformToWorldVertices(
			const std::vector<utility::math::Vector2F> &localVertices,
			const utility::graphic::PositionF &center,
			const utility::graphic::OrientationF &orientation) const
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

	std::vector<utility::graphic::PositionF>
		EllipseRender::buildEllipseVertices(
			const utility::graphic::PositionF &center,
			const utility::graphic::OrientationF &orientation,
			const utility::math::Vector2F &scale,
			const utility::graphic::SizeF &size, int segments)
	{
		const float halfWidth  = (size.getWidth() / 2.0f) * std::abs(scale[0]);
		const float halfHeight = (size.getHeight() / 2.0f) * std::abs(scale[1]);
		const auto localVertices =
			buildLocalEllipseVertices(halfWidth, halfHeight, segments);
		return transformToWorldVertices(localVertices, center, orientation);
	}

	void EllipseRender::buildTriangleFanVertices(
		utility::graphic::Mesh &mesh, const utility::graphic::PositionF &center,
		const std::vector<utility::graphic::PositionF> &outline,
		const utility::graphic::Color32Bit &color)
	{
		// Add center vertex as the fan anchor (index 0)
		mesh.addVertex(createVertex(center, color));

		// Add all outline vertices
		for (const auto &outlineVertex: outline) {
			mesh.addVertex(createVertex(outlineVertex, color));
		}

		// Build indices for triangle fan
		// Triangle fan: center vertex (0) + two consecutive outline vertices
		// For N outline vertices, we need N triangles
		// Indices pattern: (0,1,2), (0,2,3), (0,3,4), ..., (0,N,N+1),
		// (0,N+1,1)
		if (outline.size() >= 2) {
			// First triangle: (0, 1, 2)
			mesh.addIndex(0);
			mesh.addIndex(1);
			mesh.addIndex(2);

			// Remaining triangles: (0, i, i+1) for i from 2 to N
			for (size_t i = 2; i < outline.size(); ++i) {
				mesh.addIndex(0);
				mesh.addIndex(static_cast<uint32_t>(i));
				mesh.addIndex(static_cast<uint32_t>(i + 1));
			}

			// Close the fan: connect last vertex back to first outline vertex
			mesh.addIndex(0);
			mesh.addIndex(static_cast<uint32_t>(outline.size()));
			mesh.addIndex(1);
		} else if (outline.size() == 1) {
			// Degenerate case: single outline vertex, create a degenerate
			// triangle
			mesh.addIndex(0);
			mesh.addIndex(1);
			mesh.addIndex(1);
		}
	}

	utility::graphic::VertexF EllipseRender::createVertex(
		const utility::graphic::PositionF &position,
		const utility::graphic::Color32Bit &color) const
	{
		utility::graphic::VertexF vertex;
		vertex.setPosition(position);
		vertex.setColor(color);
		return vertex;
	}

	EllipseRender::EllipseRender(std::unique_ptr<Engine> &engine)
		: ecs::SystemFiller<components::Transform, components::Bound,
							components::Color, components::Ellipse>(
			  ecs::Phase::Render)
		, _engine(engine)
	{
	}

	EllipseRender::~EllipseRender(void)
	{
		clear();
	}

	void EllipseRender::prepare(void)
	{
		markAllUnused();
	}

	void EllipseRender::cleanup(void)
	{
		removeUnused(*_engine);
	}

	void EllipseRender::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug()
			<< "Updating EllipseRender system for entity " << entityIdentifier;

		if (!requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Color>(entityIdentifier)
			|| !requireComponent<components::Ellipse>(entityIdentifier)) {
			return;
		}

		const auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);
		const auto &boundComponent =
			getComponent<components::Bound>(entityIdentifier);
		const auto &colorComponent =
			getComponent<components::Color>(entityIdentifier);
		const auto &ellipseComponent =
			getComponent<components::Ellipse>(entityIdentifier);

		const auto pose		= transformComponent.getPose();
		const auto color	= colorComponent.getColor();
		const auto segments = ellipseComponent.getSegments();

		getLogger().debug()
			<< "Rendering ellipse with pose: " << pose
			<< ", size: " << boundComponent.getWidth() << "x"
			<< boundComponent.getHeight() << ", segments: " << segments
			<< ", color: " << color;

		const utility::graphic::PositionF center(
			pose.getPosition().x + boundComponent.getWidth() / 2.0f,
			pose.getPosition().y + boundComponent.getHeight() / 2.0f,
			pose.getPosition().z);

		const auto ellipseVertices = buildEllipseVertices(
			center, pose.getOrientation(),
			utility::math::Vector2F({ 1.0f, 1.0f }), boundComponent, segments);

		utility::graphic::Mesh mesh(std::vector<utility::graphic::VertexF> {},
									std::vector<uint32_t> {});
		buildTriangleFanVertices(mesh, center, ellipseVertices, color);

		auto renderable = std::make_shared<MeshRenderable>(
			std::vector<utility::graphic::Mesh> { mesh }, "mesh_material");

		RenderHandle *handle = find(entityIdentifier);
		if (handle == nullptr) {
			size_t objectId = _engine->createObject(renderable);
			insert(entityIdentifier,
				   RenderHandle { objectId, renderable, true });
		} else {
			if (!_engine->updateObject(renderable, handle->objectId)) {
				handle->objectId = _engine->createObject(renderable);
			}
			handle->renderable = renderable;
			handle->used	   = true;
		}
	}

}	 // namespace guillaume::systems
