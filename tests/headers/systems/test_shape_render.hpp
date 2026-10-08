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

#include <gtest/gtest.h>

#include <memory>

#include "guillaume/ecs/component_registry.hpp"

#include "guillaume/components/arc.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/line.hpp"
#include "guillaume/components/ring.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/mesh_renderable.hpp"
#include "guillaume/systems/arc_render.hpp"
#include "guillaume/systems/line_render.hpp"
#include "guillaume/systems/ring_render.hpp"

#include "mocks/engine_mock.hpp"

#include <utility/engine.hpp>
#include <utility/graphic/pose.hpp>

namespace guillaume::systems::tests
{

	class TestShapeRender: public ::testing::Test
	{
		protected:
		std::unique_ptr<utility::Engine> _engine;
		guillaume::tests::EngineMock *_engineMock { nullptr };
		std::unique_ptr<RingRender> _ringSystem;
		std::unique_ptr<ArcRender> _arcSystem;
		std::unique_ptr<LineRender> _lineSystem;
		ecs::ComponentRegistry _componentRegistry;

		TestShapeRender(void)			= default;
		~TestShapeRender(void) override = default;

		void SetUp(void) override
		{
			_engine = std::make_unique<guillaume::tests::EngineMock>();
			_engineMock =
				static_cast<guillaume::tests::EngineMock *>(_engine.get());
			_ringSystem = std::make_unique<RingRender>(_engine);
			_arcSystem	= std::make_unique<ArcRender>(_engine);
			_lineSystem = std::make_unique<LineRender>(_engine);
			_ringSystem->bindComponentRegistry(_componentRegistry);
			_arcSystem->bindComponentRegistry(_componentRegistry);
			_lineSystem->bindComponentRegistry(_componentRegistry);
		}

		void TearDown(void) override
		{
			_ringSystem->unbindComponentRegistry();
			_arcSystem->unbindComponentRegistry();
			_lineSystem->unbindComponentRegistry();
			_lineSystem.reset();
			_arcSystem.reset();
			_ringSystem.reset();
			_engine.reset();
		}

		/**
		 * @brief Create an entity carrying transform/bound/color and a shape.
		 * @tparam ShapeComponent The shape component type to attach.
		 * @param pose Initial pose.
		 * @param width Bounding box width.
		 * @param height Bounding box height.
		 * @param color Fill color.
		 * @return The identifier of the created entity.
		 */
		template<typename ShapeComponent>
		ecs::Entity::Identifier createShapeEntity(
			const utility::graphic::PoseF &pose = utility::graphic::PoseF(),
			float width = 100.0f, float height = 50.0f,
			const utility::graphic::Color32Bit &color = { 255, 255, 255, 255 })
		{
			ecs::Entity entity;
			const auto id = entity.getIdentifier();

			_componentRegistry.registerComponentsForEntity<
				components::Transform, components::Bound, components::Color,
				ShapeComponent>(id);

			_componentRegistry.getComponent<components::Transform>(id).setPose(
				pose);
			_componentRegistry.getComponent<components::Bound>(id)
				.setWidth(width)
				.setHeight(height);
			_componentRegistry.getComponent<components::Color>(id).setColor(
				color);

			return id;
		}

		/**
		 * @brief Get the number of vertices of the last created mesh.
		 * @return Vertex count.
		 */
		std::size_t lastMeshVertexCount(void) const
		{
			const auto &created = _engineMock->lastCreated();
			const auto renderable =
				std::dynamic_pointer_cast<guillaume::MeshRenderable>(created);
			if (renderable == nullptr || renderable->getMeshes().empty()) {
				return 0;
			}
			return renderable->getMeshes().front()->getVertices().size();
		}

		/**
		 * @brief Get the number of indices of the last created mesh.
		 * @return Index count.
		 */
		std::size_t lastMeshIndexCount(void) const
		{
			const auto &created = _engineMock->lastCreated();
			const auto renderable =
				std::dynamic_pointer_cast<guillaume::MeshRenderable>(created);
			if (renderable == nullptr || renderable->getMeshes().empty()) {
				return 0;
			}
			return renderable->getMeshes().front()->getIndices().size();
		}
	};

}	 // namespace guillaume::systems::tests
