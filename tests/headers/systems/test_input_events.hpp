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
#include <utility>

#include <utility/graphic/pose.hpp>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>
#include <guillaume/ecs/level_order_traveler.hpp>
#include <guillaume/ecs/system.hpp>
#include <guillaume/event/event_bus.hpp>

namespace guillaume::systems::tests
{

	/**
	 * @brief Fixture running input systems the way the frame loop does:
	 * events are published on a bus, then a system runs its routine on the
	 * entities whose signature matches.
	 */
	class TestInputEvents: public ::testing::Test
	{
		protected:
		event::EventBus _eventBus;						 ///< Event dispatch.
		ecs::ComponentRegistry _componentRegistry;		 ///< Components.
		ecs::EntityRegistryContainer _entityRegistry;	 ///< Entities.
		ecs::LevelOrderTraveler _traveler;				 ///< Visit order.

		TestInputEvents(void)			= default;
		~TestInputEvents(void) override = default;

		/**
		 * @brief Add a 100 x 50 entity whose signature is exactly Transform,
		 * Bound and the listed components.
		 * @tparam Components The other components of the entity.
		 * @param x X of the top-left corner.
		 * @param y Y of the top-left corner (the entity extends down).
		 * @return The identifier of the entity.
		 */
		template<typename... Components>
		ecs::Entity::Identifier addEntity(float x = 0.0f, float y = 0.0f)
		{
			auto entity			  = std::make_unique<ecs::Entity>();
			const auto identifier = entity->getIdentifier();
			entity->setSignature(
				ecs::Entity::getSignatureFromTypes<
					components::Transform, components::Bound, Components...>());
			_entityRegistry.addEntity(std::move(entity));

			_componentRegistry.addComponent<components::Transform>(identifier)
				.setPose(utility::graphic::PoseF(
					utility::graphic::PositionF(x, y, 0.0f),
					utility::graphic::OrientationF()));
			_componentRegistry.addComponent<components::Bound>(identifier)
				.setWidth(100.0f)
				.setHeight(50.0f);
			(_componentRegistry.addComponent<Components>(identifier), ...);
			return identifier;
		}

		/**
		 * @brief Build the pose of a hand in front of the entities, pointing
		 * straight at the point (x, y) of their plane.
		 * @param x X of the point aimed at.
		 * @param y Y of the point aimed at.
		 * @return The pose.
		 */
		static utility::graphic::PoseF handAt(float x, float y)
		{
			return utility::graphic::PoseF(
				utility::graphic::PositionF(x, y, 10.0f),
				utility::graphic::OrientationF());
		}

		/**
		 * @brief Publish a hand event whose pose and aim point at (x, y).
		 * @tparam EventType The hand event type.
		 * @param x X of the point aimed at.
		 * @param y Y of the point aimed at.
		 * @return The event, already published.
		 */
		template<typename EventType>
		std::shared_ptr<EventType> makeHandEvent(float x, float y)
		{
			auto event = std::make_shared<EventType>();
			event->setPose(handAt(x, y));
			event->setAim(handAt(x, y));
			return event;
		}

		/**
		 * @brief Run one frame of a system.
		 * @param system The system.
		 */
		void run(ecs::System &system)
		{
			system.routine(_componentRegistry, _entityRegistry, _traveler);
		}
	};

}	 // namespace guillaume::systems::tests
