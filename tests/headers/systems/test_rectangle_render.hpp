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

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/mesh_renderable.hpp"
#include "guillaume/systems/rectangle_render.hpp"

#include "mocks/engine_mock.hpp"

#include <utility/engine.hpp>

namespace guillaume::systems::tests
{

	/**
	 * @brief Fixture driving RectangleRender against the engine mock.
	 */
	class TestRectangleRender: public ::testing::Test
	{
		protected:
		std::unique_ptr<utility::Engine> _engine;
		guillaume::tests::EngineMock *_engineMock { nullptr };
		std::unique_ptr<RectangleRender> _system;
		ecs::ComponentRegistry _componentRegistry;

		void SetUp(void) override
		{
			_engine = std::make_unique<guillaume::tests::EngineMock>();
			_engineMock =
				static_cast<guillaume::tests::EngineMock *>(_engine.get());
			_system = std::make_unique<RectangleRender>(_engine);
			_system->bindComponentRegistry(_componentRegistry);
		}

		void TearDown(void) override
		{
			_system->unbindComponentRegistry();
			_system.reset();
			_engine.reset();
		}

		/**
		 * @brief Create a 100x50 white rectangle entity.
		 * @return The identifier of the created entity.
		 */
		ecs::Entity::Identifier createRectangle(void)
		{
			ecs::Entity entity;
			const auto id = entity.getIdentifier();

			_componentRegistry.registerComponentsForEntity<
				components::Transform, components::Bound, components::Color,
				components::Borders>(id);
			_componentRegistry.getComponent<components::Bound>(id)
				.setWidth(100.0f)
				.setHeight(50.0f);
			_componentRegistry.getComponent<components::Color>(id).setColor(
				utility::graphic::Color32Bit(255, 255, 255, 255));
			return id;
		}

		/**
		 * @brief Get the renderable passed to the last createObject call.
		 * @return The mesh renderable, or nullptr.
		 */
		std::shared_ptr<guillaume::MeshRenderable> lastRenderable(void) const
		{
			return std::dynamic_pointer_cast<guillaume::MeshRenderable>(
				_engineMock->lastCreated());
		}
	};

}	 // namespace guillaume::systems::tests
