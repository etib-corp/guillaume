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
	float RectangleRender::extractYawRadians(
		const components::Transform::Rotation &rotation) const
	{
		const auto normalizedRotation = rotation.normalizedQuaternion();
		const float x				  = normalizedRotation.getX();
		const float y				  = normalizedRotation.getY();
		const float z				  = normalizedRotation.getZ();
		const float w				  = normalizedRotation.getW();

		const float sinYaw = 2.0f * ((w * z) + (x * y));
		const float cosYaw = 1.0f - (2.0f * ((y * y) + (z * z)));
		return std::atan2(sinYaw, cosYaw);
	}

	utility::math::Vector<float, 2> RectangleRender::rotateVector(
		const utility::math::Vector<float, 2> &vector,
		const float angleRadians) const
	{
		const float cosine = std::cos(angleRadians);
		const float sine   = std::sin(angleRadians);
		return utility::math::Vector<float, 2>(
			{ vector[0] * cosine - vector[1] * sine,
			  vector[0] * sine + vector[1] * cosine });
	}

	float RectangleRender::extractAverageRadius(
		const components::Borders &borders) const
	{
		return (borders.getTopLeftRadius() + borders.getTopRightRadius()
				+ borders.getBottomRightRadius()
				+ borders.getBottomLeftRadius())
			/ 4.0f;
	}

	std::vector<utility::math::Vector<float, 2>>
		RectangleRender::buildAxisAlignedRectVertices(
			const float halfWidth, const float halfHeight) const
	{
		return {
			utility::math::Vector<float, 2>({ -halfWidth, -halfHeight }),
			utility::math::Vector<float, 2>({ halfWidth, -halfHeight }),
			utility::math::Vector<float, 2>({ halfWidth, halfHeight }),
			utility::math::Vector<float, 2>({ -halfWidth, halfHeight }),
		};
	}

	void RectangleRender::appendRoundedCornerArc(
		std::vector<utility::math::Vector<float, 2>> &localVertices,
		const utility::math::Vector<float, 2> &arcCenter,
		const float startAngle, const float endAngle, const float radius,
		const int arcSegments) const
	{
		for (int i = 0; i <= arcSegments; ++i) {
			const float t =
				static_cast<float>(i) / static_cast<float>(arcSegments);
			const float angle = startAngle + (endAngle - startAngle) * t;
			localVertices.push_back(
				arcCenter
				+ utility::math::Vector<float, 2>(
					{ std::cos(angle) * radius, std::sin(angle) * radius }));
		}
	}

	std::vector<utility::math::Vector<float, 2>>
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

		std::vector<utility::math::Vector<float, 2>> localVertices;
		localVertices.reserve(static_cast<std::size_t>(4 * (arcSegments + 2)));

		const utility::math::Vector<float, 2> topLeftCenter(
			{ -halfWidth + cornerRadius, -halfHeight + cornerRadius });
		const utility::math::Vector<float, 2> topRightCenter(
			{ halfWidth - cornerRadius, -halfHeight + cornerRadius });
		const utility::math::Vector<float, 2> bottomRightCenter(
			{ halfWidth - cornerRadius, halfHeight - cornerRadius });
		const utility::math::Vector<float, 2> bottomLeftCenter(
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

	std::vector<utility::math::Vector<float, 2>>
		RectangleRender::transformToWorldVertices(
			const std::vector<utility::math::Vector<float, 2>> &localVertices,
			const utility::math::Vector<float, 2> &center,
			const float angleRadians) const
	{
		std::vector<utility::math::Vector<float, 2>> worldVertices;
		worldVertices.reserve(localVertices.size());
		for (const auto &localVertex: localVertices) {
			worldVertices.push_back(rotateVector(localVertex, angleRadians)
									+ center);
		}
		return worldVertices;
	}

	std::vector<utility::math::Vector<float, 2>>
		RectangleRender::buildRoundedRectVertices(
			const utility::math::Vector<float, 2> &center, float angleRadians,
			const utility::math::Vector<float, 2> &scale,
			const utility::math::Vector<float, 2> &size, float radius,
			int arcSegments, float epsilon)
	{
		const float halfWidth	 = (size[0] / 2.0f) * std::abs(scale[0]);
		const float halfHeight	 = (size[1] / 2.0f) * std::abs(scale[1]);
		const auto localVertices = buildLocalRoundedRectVertices(
			halfWidth, halfHeight, radius, arcSegments, epsilon);
		return transformToWorldVertices(localVertices, center, angleRadians);
	}

	void RectangleRender::buildTriangleFanVertices(
		const utility::math::Vector<float, 2> &center,
		const std::vector<utility::math::Vector<float, 2>> &outline,
		const utility::graphics::Color32Bit &color)
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

	utility::graphics::Vertex<float, uint8_t> RectangleRender::createVertex(
		const utility::math::Vector<float, 2> &position,
		const utility::graphics::Color32Bit &color) const
	{
		utility::graphics::Vertex<float, uint8_t> vertex;
		vertex.setPosition(
			utility::graphics::Position({ position[0], position[1], 0.0f }));
		vertex.setColor(color);
		return vertex;
	}

	RectangleRender::RectangleRender(Renderer &renderer)
		: ecs::SystemFiller<components::Transform, components::Bound,
							components::Color, components::Borders>()
		, _renderer(renderer)
	{
	}

	RectangleRender::~RectangleRender(void)
	{
	}

	void
		RectangleRender::update(
			const ecs::Entity::Identifier &entityIdentifier)
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

		const auto position = transformComponent.getPosition();
		const auto rotation = transformComponent.getRotation();
		const auto scale	= transformComponent.getScale();
		const auto bound	= boundComponent.getSize();
		const auto color	= colorComponent.getColor();
		const float radius	= extractAverageRadius(bordersComponent);

		const utility::math::Vector<float, 2> center(
			{ position[0], position[1] - (bound[1] * scale[1] / 2.0f) });
		const float angleRadians   = this->extractYawRadians(rotation);
		const auto roundedVertices = buildRoundedRectVertices(
			center, angleRadians,
			utility::math::Vector<float, 2>({ scale[0], scale[1] }),
			utility::math::Vector<float, 2>({ bound[0], bound[1] }), radius);

		_vertices.clear();
		buildTriangleFanVertices(center, roundedVertices, color);

		_renderer.drawVertices(_vertices);
	}

}	 // namespace guillaume::systems