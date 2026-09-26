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

#include "guillaume/event/event_bus.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/overlay.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/systems/overlay.hpp"

#include "mocks/engine_mock.hpp"

#include <utility/engine.hpp>
#include <utility/graphic/pose.hpp>

namespace guillaume::systems::tests
{

	class TestOverlay: public ::testing::Test
	{
		protected:
		event::EventBus _eventBus;
		std::unique_ptr<utility::Engine> _engine;
		std::unique_ptr<Overlay> _overlaySystem;
		ecs::ComponentRegistry _componentRegistry;

		TestOverlay(void)			= default;
		~TestOverlay(void) override = default;

		void SetUp(void) override
		{
			_engine		   = std::make_unique<guillaume::tests::EngineMock>();
			_overlaySystem = std::make_unique<Overlay>(_eventBus, _engine);
			_overlaySystem->bindComponentRegistry(_componentRegistry);
		}

		void TearDown(void) override
		{
			_overlaySystem->unbindComponentRegistry();
			_overlaySystem.reset();
			_engine.reset();
		}

		/**
		 * @brief Create an overlay entity with a bound and a transform.
		 * @param x Bound origin X.
		 * @param y Bound origin Y.
		 * @param width Bound width.
		 * @param height Bound height.
		 * @return The entity identifier.
		 */
		ecs::Entity::Identifier createOverlayEntity(float x		 = 0.0f,
													float y		 = 0.0f,
													float width	 = 100.0f,
													float height = 50.0f)
		{
			ecs::Entity entity;
			const auto id = entity.getIdentifier();

			_componentRegistry.addComponent<components::Overlay>(id);
			_componentRegistry.addComponent<components::Transform>(id);
			_componentRegistry.addComponent<components::Bound>(id);

			_componentRegistry.getComponent<components::Transform>(id).setPose(
				utility::graphic::PoseF(utility::graphic::PositionF(x, y, 0.0f),
										utility::graphic::OrientationF()));
			_componentRegistry.getComponent<components::Bound>(id)
				.setWidth(width)
				.setHeight(height);

			return id;
		}
	};

}	 // namespace guillaume::systems::tests
