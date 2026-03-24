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

	RectangleRender::Vec2 RectangleRender::rotateVec(const Vec2 &vec,
													 const float angle)
	{
		float ct = std::cos(angle);
		float st = std::sin(angle);
		return Vec2({ vec[0] * ct - vec[1] * st, vec[0] * st + vec[1] * ct });
	}

	float RectangleRender::vectorLength(const Vec2 &vec)
	{
		return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
	}

	RectangleRender::Vec2 RectangleRender::normalize(const Vec2 &vec)
	{
		float len = vectorLength(vec);
		if (len < kEpsilon) {
			return Vec2({ 0.0f, 0.0f });
		}
		return Vec2({ vec[0] / len, vec[1] / len });
	}

	std::vector<RectangleRender::Vec2> RectangleRender::roundedRectVertices(
		const Vec2 &center, float theta, float sx, float sy, float sizeX,
		float sizeY, float radius, int arcSegments)
	{
		if (arcSegments <= 0) {
			arcSegments = 1;
		}

		const float hw = (sizeX / 2.0f) * std::abs(sx);
		const float hh = (sizeY / 2.0f) * std::abs(sy);
		const float r  = std::max(0.0f, (std::min)({ radius, hw, hh }));

		std::vector<Vec2> localVertices;

		if (r <= kEpsilon) {
			localVertices = {
				Vec2({ -hw, -hh }),
				Vec2({ hw, -hh }),
				Vec2({ hw, hh }),
				Vec2({ -hw, hh }),
			};
		} else {
			localVertices.reserve(
				static_cast<std::size_t>(4 * (arcSegments + 2)));

			const Vec2 topLeftCenter({ -hw + r, -hh + r });
			const Vec2 topRightCenter({ hw - r, -hh + r });
			const Vec2 bottomRightCenter({ hw - r, hh - r });
			const Vec2 bottomLeftCenter({ -hw + r, hh - r });

			const auto appendArc = [&](const Vec2 &arcCenter, float startAngle,
									   float endAngle) {
				for (int i = 0; i <= arcSegments; ++i) {
					const float t =
						static_cast<float>(i) / static_cast<float>(arcSegments);
					const float angle =
						startAngle + (endAngle - startAngle) * t;
					localVertices.push_back(
						arcCenter
						+ Vec2({ std::cos(angle) * r, std::sin(angle) * r }));
				}
			};

			appendArc(topRightCenter, -kPi / 2.0f, 0.0f);
			appendArc(bottomRightCenter, 0.0f, kPi / 2.0f);
			appendArc(bottomLeftCenter, kPi / 2.0f, kPi);
			appendArc(topLeftCenter, kPi, 3.0f * kPi / 2.0f);
		}

		std::vector<Vec2> worldVertices;
		worldVertices.reserve(localVertices.size());
		for (const auto &localVertex: localVertices) {
			worldVertices.push_back(rotateVec(localVertex, theta) + center);
		}

		return worldVertices;
	}

	RectangleRender::Vertex RectangleRender::createVertex(
		const Vec2 &pos, const utility::graphics::Color<uint8_t> &color)
	{
		Vertex v;
		v.setPosition(Position({ pos[0], pos[1], 0.0f }));
		v.setColor(color);
		return v;
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
		RectangleRender::update(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug("Updating RectangleRender system for entity "
						  + std::to_string(entityIdentifier));

		if (!componentRegistry.hasComponent<components::Bound>(
				entityIdentifier)) {
			getLogger().warning("Entity " + std::to_string(entityIdentifier)
								+ " does not have Bound component");
			return;
		}

		if (!componentRegistry.hasComponent<components::Transform>(
				entityIdentifier)) {
			getLogger().warning("Entity " + std::to_string(entityIdentifier)
								+ " does not have Transform component");
			return;
		}

		if (!componentRegistry.hasComponent<components::Color>(
				entityIdentifier)) {
			getLogger().warning("Entity " + std::to_string(entityIdentifier)
								+ " does not have Color component");
			return;
		}

		if (!componentRegistry.hasComponent<components::Borders>(
				entityIdentifier)) {
			getLogger().warning("Entity " + std::to_string(entityIdentifier)
								+ " does not have Borders component");
			return;
		}

		const auto &transformComponent =
			componentRegistry.getComponent<components::Transform>(
				entityIdentifier);
		const auto &boundComponent =
			componentRegistry.getComponent<components::Bound>(entityIdentifier);
		const auto &colorComponent =
			componentRegistry.getComponent<components::Color>(entityIdentifier);
		const auto &bordersComponent =
			componentRegistry.getComponent<components::Borders>(
				entityIdentifier);

		const auto position = transformComponent.getPosition();
		const auto rotation = transformComponent.getRotation();
		const auto scale	= transformComponent.getScale();
		const auto bound	= boundComponent.getSize();
		const auto color	= colorComponent.getColor();

		const float radius = (bordersComponent.getTopLeftRadius()
							  + bordersComponent.getTopRightRadius()
							  + bordersComponent.getBottomRightRadius()
							  + bordersComponent.getBottomLeftRadius())
			/ 4.0f;

		const Vec2 center(
			{ position[0], position[1] - (bound[1] * scale[1] / 2.0f) });
		const float theta = rotation[2] * kDegreesToRadians;

		const auto roundedVertices = roundedRectVertices(
			center, theta, scale[0], scale[1], bound[0], bound[1], radius);

		std::vector<Vertex> vertices;
		vertices.reserve(roundedVertices.size() + 2);

		// OpenGL triangle fan expects the first vertex to be the fan anchor.
		vertices.push_back(createVertex(center, color));
		for (const auto &roundedVertex: roundedVertices) {
			vertices.push_back(createVertex(roundedVertex, color));
		}

		if (!roundedVertices.empty()) {
			vertices.push_back(createVertex(roundedVertices.front(), color));
		}

		_renderer.drawVertices(vertices);
	}

}	 // namespace guillaume::systems