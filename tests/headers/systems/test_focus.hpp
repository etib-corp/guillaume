/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, distribute, sublicense, and/or sell copies
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
#include "guillaume/ecs/entity_registry.hpp"
#include "guillaume/ecs/entity_registry_container.hpp"
#include "guillaume/ecs/system_phase.hpp"

#include "guillaume/components/focus.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/mouse_button_interaction.hpp"
#include "guillaume/components/hand_button_interaction.hpp"
#include "guillaume/components/hand_pinch_interaction.hpp"
#include "guillaume/components/hand_poke_interaction.hpp"

#include "guillaume/systems/focus.hpp"
#include "guillaume/event/event_bus.hpp"
#include "guillaume/engine.hpp"

#include <utility/graphic/mesh.hpp>
#include <utility/graphic/text/text.hpp>
#include <utility/graphic/view.hpp>
#include <utility/math/vector.hpp>
#include <utility/event/mouse_button_event.hpp>
#include <utility/event/hand_button_event.hpp>

namespace guillaume::systems::tests
{

	class EngineStub: public Engine
	{
		public:
		void clear(void) override
		{
		}
		void present(void) override
		{
		}
		size_t addMesh(const utility::graphic::Mesh &,
					   const std::string &) override
		{
			return 0;
		}
		bool removeObject(size_t) override
		{
			return true;
		}
		utility::math::Vector2F
			measureText(const utility::graphic::Text &) const override
		{
			return { 0.0f, 0.0f };
		}
		size_t addText(const utility::graphic::Text &) override
		{
			return 0;
		}
		utility::graphic::ViewF getView(void) const override
		{
			return utility::graphic::ViewF();
		}
		void addScene(size_t) override
		{
		}
		void pollEvents(void) override
		{
		}
		void update(void) override
		{
		}
	};

	class TestFocusSystem: public ::testing::Test
	{
		protected:
		std::unique_ptr<event::EventBus> _eventBus;
		std::unique_ptr<Engine> _engine;
		std::unique_ptr<Focus> _focusSystem;
		ecs::ComponentRegistry _componentRegistry;
		ecs::EntityRegistryContainer _entityRegistry;

		TestFocusSystem(void)			= default;
		~TestFocusSystem(void) override = default;

		void SetUp(void) override
		{
			_eventBus	 = std::make_unique<event::EventBus>();
			_engine		 = std::make_unique<EngineStub>();
			_focusSystem = std::make_unique<Focus>(*_eventBus, _engine);
			_focusSystem->bindComponentRegistry(_componentRegistry);
		}

		void TearDown(void) override
		{
			_focusSystem->unbindComponentRegistry();
			_focusSystem.reset();
			_engine.reset();
			_eventBus.reset();
		}

		/**
		 * @brief Create an entity with Focus, Transform, Bound, and interaction
		 * components.
		 * @return The identifier of the created entity.
		 */
		ecs::Entity::Identifier createFocusableEntity(
			const utility::graphic::PoseF &pose = utility::graphic::PoseF(),
			float width = 100.0f, float height = 50.0f)
		{
			auto entity = std::make_unique<ecs::Entity>();
			auto id		= entity->getIdentifier();
			entity->setSignature(
				ecs::Entity::getSignatureFromTypes<
					components::Focus, components::Transform, components::Bound,
					components::MouseButtonInteraction,
					components::HandButtonInteraction,
					components::HandPinchInteraction,
					components::HandPokeInteraction>());
			_entityRegistry.addEntity(std::move(entity));

			_componentRegistry.addComponent<components::Focus>(id);
			_componentRegistry.addComponent<components::Transform>(id);
			_componentRegistry.addComponent<components::Bound>(id);
			_componentRegistry.addComponent<components::MouseButtonInteraction>(
				id);
			_componentRegistry.addComponent<components::HandButtonInteraction>(
				id);
			_componentRegistry.addComponent<components::HandPinchInteraction>(
				id);
			_componentRegistry.addComponent<components::HandPokeInteraction>(
				id);

			auto &transform =
				_componentRegistry.getComponent<components::Transform>(id);
			transform.setPose(pose);

			auto &bound =
				_componentRegistry.getComponent<components::Bound>(id);
			bound.setWidth(static_cast<unsigned int>(width));
			bound.setHeight(static_cast<unsigned int>(height));

			return id;
		}
	};

}	 // namespace guillaume::systems::tests
