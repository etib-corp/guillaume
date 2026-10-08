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

#include "guillaume/components/bound.hpp"
#include "guillaume/components/clip.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/scrim.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/mesh_renderable.hpp"
#include "guillaume/systems/clip.hpp"
#include "guillaume/systems/elevation_render.hpp"
#include "guillaume/systems/scrim_render.hpp"

#include "mocks/engine_mock.hpp"

#include <utility/engine.hpp>
#include <utility/graphic/pose.hpp>

namespace guillaume::systems::tests
{

	class TestSurfaceRender: public ::testing::Test
	{
		protected:
		std::unique_ptr<utility::Engine> _engine;
		guillaume::tests::EngineMock *_engineMock { nullptr };
		std::unique_ptr<ElevationRender> _elevationSystem;
		std::unique_ptr<ScrimRender> _scrimSystem;
		std::unique_ptr<Clip> _clipSystem;
		ecs::ComponentRegistry _componentRegistry;

		TestSurfaceRender(void)			  = default;
		~TestSurfaceRender(void) override = default;

		void SetUp(void) override
		{
			_engine = std::make_unique<guillaume::tests::EngineMock>();
			_engineMock =
				static_cast<guillaume::tests::EngineMock *>(_engine.get());
			_elevationSystem = std::make_unique<ElevationRender>(_engine);
			_scrimSystem	 = std::make_unique<ScrimRender>(_engine);
			_clipSystem		 = std::make_unique<Clip>(_engine);
			_elevationSystem->bindComponentRegistry(_componentRegistry);
			_scrimSystem->bindComponentRegistry(_componentRegistry);
			_clipSystem->bindComponentRegistry(_componentRegistry);
		}

		void TearDown(void) override
		{
			_elevationSystem->unbindComponentRegistry();
			_scrimSystem->unbindComponentRegistry();
			_clipSystem->unbindComponentRegistry();
			_clipSystem.reset();
			_scrimSystem.reset();
			_elevationSystem.reset();
			_engine.reset();
		}

		template<typename... ComponentTypes>
		ecs::Entity::Identifier createEntity(
			const utility::graphic::PoseF &pose = utility::graphic::PoseF(),
			float width = 100.0f, float height = 50.0f)
		{
			ecs::Entity entity;
			const auto id = entity.getIdentifier();

			_componentRegistry.registerComponentsForEntity<
				components::Transform, components::Bound, ComponentTypes...>(
				id);

			_componentRegistry.getComponent<components::Transform>(id).setPose(
				pose);
			_componentRegistry.getComponent<components::Bound>(id)
				.setWidth(width)
				.setHeight(height);

			return id;
		}

		std::size_t lastMeshVertexCount(void) const
		{
			const auto renderable =
				std::dynamic_pointer_cast<guillaume::MeshRenderable>(
					_engineMock->lastCreated());
			if (renderable == nullptr || renderable->getMeshes().empty()) {
				return 0;
			}
			return renderable->getMeshes().front()->getVertices().size();
		}

		std::size_t lastMeshIndexCount(void) const
		{
			const auto renderable =
				std::dynamic_pointer_cast<guillaume::MeshRenderable>(
					_engineMock->lastCreated());
			if (renderable == nullptr || renderable->getMeshes().empty()) {
				return 0;
			}
			return renderable->getMeshes().front()->getIndices().size();
		}
	};

}	 // namespace guillaume::systems::tests
