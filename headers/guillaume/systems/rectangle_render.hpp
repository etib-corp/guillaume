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

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/renderer.hpp"

namespace guillaume::systems
{

	/**
	 * @brief System handling rectangle rendering from ECS components.
	 * @see components::Transform
	 * @see components::Bound
	 * @see components::Color
	 * @see components::Borders
	 */
	class RectangleRender:
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::Color, components::Borders>
	{
		private:
		using Vec2	   = utility::math::Vector<float, 2>;
		using Position = utility::math::Vector<float, 3>;
		using Vertex   = utility::graphics::Vertex<float, uint8_t>;

		static constexpr float kEpsilon = 0.001f;
		static constexpr float kPi		= 3.14159265358979323846f;
		static constexpr float kDegreesToRadians =
			0.01745329251994329576923690768489f;

		Renderer &_renderer;	///< Renderer instance

		static Vec2 rotateVec(const Vec2 &vec, float angle);
		static float vectorLength(const Vec2 &vec);
		static Vec2 normalize(const Vec2 &vec);
		static std::vector<Vec2> roundedRectVertices(const Vec2 &center,
													 float theta, float sx,
													 float sy, float sizeX,
													 float sizeY, float radius,
													 int arcSegments = 16);
		static Vertex
			createVertex(const Vec2 &pos,
						 const utility::graphics::Color<uint8_t> &color);

		public:
		/**
		 * @brief Construct a rectangle rendering system.
		 * @param renderer The renderer used to draw rectangles.
		 */
		RectangleRender(Renderer &renderer);

		/**
		 * @brief Default destructor.
		 */
		~RectangleRender(void);

		/**
		 * @brief Update the RectangleRender system for one entity.
		 * @param componentRegistry The component registry.
		 * @param entityIdentifier The target entity identifier.
		 */
		void update(ecs::ComponentRegistry &componentRegistry,
					const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems