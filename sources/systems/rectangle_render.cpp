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

#include "guillaume/systems/rectangle_render.hpp"
#include "guillaume/mesh_renderable.hpp"
#include "guillaume/systems/shape_mesh.hpp"

#include <algorithm>
#include <cmath>
#include <memory>

namespace guillaume::systems
{
	float RectangleRender::extractAverageRadius(
		const components::Borders &borders) const
	{
		return (borders.getTopLeftRadius() + borders.getTopRightRadius()
				+ borders.getBottomRightRadius()
				+ borders.getBottomLeftRadius())
			/ 4.0f;
	}

	std::vector<utility::math::Vector2F>
		RectangleRender::buildAxisAlignedRectVertices(
			const float halfWidth, const float halfHeight) const
	{
		return {
			utility::math::Vector2F({ -halfWidth, -halfHeight }),
			utility::math::Vector2F({ halfWidth, -halfHeight }),
			utility::math::Vector2F({ halfWidth, halfHeight }),
			utility::math::Vector2F({ -halfWidth, halfHeight }),
		};
	}

	void RectangleRender::appendRoundedCornerArc(
		std::vector<utility::math::Vector2F> &localVertices,
		const utility::math::Vector2F &arcCenter, const float startAngle,
		const float endAngle, const float radius, const int arcSegments) const
	{
		for (int i = 0; i <= arcSegments; ++i) {
			const float t =
				static_cast<float>(i) / static_cast<float>(arcSegments);
			const float angle = startAngle + (endAngle - startAngle) * t;
			localVertices.push_back(
				arcCenter
				+ utility::math::Vector2F(
					{ std::cos(angle) * radius, std::sin(angle) * radius }));
		}
	}

	std::vector<utility::math::Vector2F>
		RectangleRender::buildLocalRoundedRectVertices(
			const float halfWidth, const float halfHeight, const float radius,
			int arcSegments, const float epsilon) const
	{
		const float pi = std::acos(-1.0f);

		if (arcSegments <= 0) {
			arcSegments = 1;
		}

		const float cornerRadius =
			std::max(0.0f, (std::min)({ radius, halfWidth, halfHeight }));

		if (cornerRadius <= epsilon) {
			return buildAxisAlignedRectVertices(halfWidth, halfHeight);
		}

		std::vector<utility::math::Vector2F> localVertices;
		localVertices.reserve(static_cast<std::size_t>(4 * (arcSegments + 2)));

		const utility::math::Vector2F topLeftCenter(
			{ -halfWidth + cornerRadius, -halfHeight + cornerRadius });
		const utility::math::Vector2F topRightCenter(
			{ halfWidth - cornerRadius, -halfHeight + cornerRadius });
		const utility::math::Vector2F bottomRightCenter(
			{ halfWidth - cornerRadius, halfHeight - cornerRadius });
		const utility::math::Vector2F bottomLeftCenter(
			{ -halfWidth + cornerRadius, halfHeight - cornerRadius });

		appendRoundedCornerArc(localVertices, topRightCenter, -pi / 2.0f, 0.0f,
							   cornerRadius, arcSegments);
		appendRoundedCornerArc(localVertices, bottomRightCenter, 0.0f,
							   pi / 2.0f, cornerRadius, arcSegments);
		appendRoundedCornerArc(localVertices, bottomLeftCenter, pi / 2.0f, pi,
							   cornerRadius, arcSegments);
		appendRoundedCornerArc(localVertices, topLeftCenter, pi,
							   3.0f * pi / 2.0f, cornerRadius, arcSegments);

		return localVertices;
	}

	std::vector<utility::graphic::PositionF>
		RectangleRender::buildRoundedRectVertices(
			const utility::graphic::PositionF &center,
			const utility::graphic::OrientationF &orientation,
			const utility::math::Vector2F &scale,
			const utility::graphic::SizeF &size, float radius, int arcSegments,
			float epsilon)
	{
		const float halfWidth  = (size.getWidth() / 2.0f) * std::abs(scale[0]);
		const float halfHeight = (size.getHeight() / 2.0f) * std::abs(scale[1]);
		const auto localVertices = buildLocalRoundedRectVertices(
			halfWidth, halfHeight, radius, arcSegments, epsilon);
		return shape::transformToWorldVertices(localVertices, center,
											   orientation);
	}

	RectangleRender::RectangleRender(utility::Engine &engine)
		: ecs::SystemFiller<components::Transform, components::Bound,
							components::Color, components::Borders>(
			  ecs::Phase::Render)
		, _engine(engine)
	{
	}

	RectangleRender::~RectangleRender(void)
	{
		clear();
	}

	void RectangleRender::prepare(void)
	{
		markAllUnused();
	}

	void RectangleRender::cleanup(void)
	{
		removeUnused(_engine);
	}

	void
		RectangleRender::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug() << "Updating RectangleRender system for entity "
							<< entityIdentifier;

		if (!requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Color>(entityIdentifier)
			|| !requireComponent<components::Borders>(entityIdentifier)) {
			return;
		}

		const auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);
		const auto &boundComponent =
			getComponent<components::Bound>(entityIdentifier);
		const auto &colorComponent =
			getComponent<components::Color>(entityIdentifier);
		const auto &bordersComponent =
			getComponent<components::Borders>(entityIdentifier);

		const auto &pose  = transformComponent.getPose();
		const auto &color = colorComponent.getColor();

		getLogger().debug()
			<< "Rendering rectangle with pose: " << pose
			<< ", size: " << boundComponent.getWidth() << "x"
			<< boundComponent.getHeight() << ", color: " << color;

		const utility::graphic::PositionF center(
			pose.getPosition().x + boundComponent.getWidth() / 2.0f,
			pose.getPosition().y + boundComponent.getHeight() / 2.0f,
			pose.getPosition().z);

		const auto roundedVertices = buildRoundedRectVertices(
			center, pose.getOrientation(),
			utility::math::Vector2F({ 1.0f, 1.0f }), boundComponent,
			extractAverageRadius(bordersComponent), 16, 0.001f);

		utility::graphic::Mesh mesh(std::vector<utility::graphic::VertexF> {},
									std::vector<uint32_t> {});
		shape::buildTriangleFanVertices(mesh, center, roundedVertices, color);

		std::vector<utility::graphic::Mesh> meshes;
		if (!mesh.getVertices().empty()) {
			meshes.push_back(mesh);
		}

		if (bordersComponent.hasStroke()) {
			const utility::graphic::PositionF strokeCenter(
				-pose.getOrientation().getForward() * StrokeLift);
			utility::graphic::Mesh stroke(
				std::vector<utility::graphic::VertexF> {},
				std::vector<uint32_t> {});
			shape::buildRoundedStrokeMesh(
				stroke,
				utility::graphic::PositionF(center[0] + strokeCenter[0],
											center[1] + strokeCenter[1],
											center[2] + strokeCenter[2]),
				pose.getOrientation(), boundComponent.getWidth() / 2.0f,
				boundComponent.getHeight() / 2.0f,
				extractAverageRadius(bordersComponent),
				bordersComponent.getWidth(), 16, bordersComponent.getColor());
			if (!stroke.getVertices().empty()) {
				meshes.push_back(stroke);
			}
		}

		// Transparent fill and no visible stroke: submit nothing, so the
		// object is removed at the end of the frame.
		if (meshes.empty()) {
			return;
		}

		auto renderable =
			std::make_shared<MeshRenderable>(meshes, "mesh_material");

		RenderHandle *handle = find(entityIdentifier);
		if (handle == nullptr) {
			size_t objectId = _engine.createObject(renderable);
			insert(entityIdentifier,
				   RenderHandle { objectId, renderable, true });
		} else {
			if (!_engine.updateObject(renderable, handle->objectId)) {
				handle->objectId = _engine.createObject(renderable);
			}
			handle->renderable = renderable;
			handle->used	   = true;
		}
	}

}	 // namespace guillaume::systems