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

#include <algorithm>
#include <cmath>

namespace guillaume::systems
{
	utility::graphic::PositionF RectangleRender::rotatePositionByQuaternion(
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
		RectangleRender::transformToWorldVertices(
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
		RectangleRender::buildRoundedRectVertices(
			const utility::graphic::PositionF &center,
			const utility::graphic::OrientationF &orientation,
			const utility::math::Vector2F &scale,
			const utility::math::Vector2F &size, float radius, int arcSegments,
			float epsilon)
	{
		const float halfWidth	 = (size[0] / 2.0f) * std::abs(scale[0]);
		const float halfHeight	 = (size[1] / 2.0f) * std::abs(scale[1]);
		const auto localVertices = buildLocalRoundedRectVertices(
			halfWidth, halfHeight, radius, arcSegments, epsilon);
		return transformToWorldVertices(localVertices, center, orientation);
	}

	void RectangleRender::buildTriangleFanVertices(
		const utility::graphic::PositionF &center,
		const std::vector<utility::graphic::PositionF> &outline,
		const utility::graphic::Color32Bit &color)
	{
		_vertices.reserve(outline.size() + 2);

		// OpenGL triangle fan expects the first vertex to be the fan anchor.
		_vertices.push_back(createVertex(center, color));
		for (const auto &outlineVertex: outline) {
			_vertices.push_back(createVertex(outlineVertex, color));
		}

		if (!outline.empty()) {
			_vertices.push_back(createVertex(outline.front(), color));
		}
	}

	utility::graphic::VertexF RectangleRender::createVertex(
		const utility::graphic::PositionF &position,
		const utility::graphic::Color32Bit &color) const
	{
		utility::graphic::VertexF vertex;
		vertex.setPosition(position);
		vertex.setColor(color);
		return vertex;
	}

	RectangleRender::RectangleRender(Renderer &renderer)
		: ecs::SystemFiller<components::Transform, components::Bound,
							components::Color, components::Borders>(
			  ecs::System::Phase::Render)
		, _renderer(renderer)
	{
	}

	RectangleRender::~RectangleRender(void)
	{
	}

	void
		RectangleRender::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug("Updating RectangleRender system for entity "
						  + std::to_string(entityIdentifier));

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

		const auto pose		   = transformComponent.getPose();
		const auto position	   = pose.getPosition();
		const auto orientation = pose.getOrientation();
		const auto width	   = boundComponent.getWidth();
		const auto height	   = boundComponent.getHeight();
		const auto color	   = colorComponent.getColor();
		const float radius	   = extractAverageRadius(bordersComponent);

		const utility::graphic::PositionF center(
			position[0], position[1] - (height / 2.0f), position[2]);
		const auto roundedVertices = buildRoundedRectVertices(
			center, orientation, utility::math::Vector2F({ 1.0f, 1.0f }),
			utility::math::Vector2F({ (float)width, (float)height }), radius);

		_vertices.clear();
		buildTriangleFanVertices(center, roundedVertices, color);

		_renderer.drawVertices(_vertices);
	}

}	 // namespace guillaume::systems