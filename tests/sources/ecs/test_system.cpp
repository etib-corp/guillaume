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

#include "ecs/test_system.hpp"

#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

#include "guillaume/ecs/entity_registry_container.hpp"
#include "guillaume/ecs/level_order_traveler.hpp"
#include "guillaume/ecs/parent_entity.hpp"
#include "guillaume/ecs/system_filler.hpp"

namespace guillaume::ecs::tests
{

	namespace
	{
		class Marker: public Component
		{
			public:
			Marker(void)		   = default;
			~Marker(void) override = default;
		};

		/**
		 * @brief Entity whose components live in a registry and are
		 * released with it, as the entities of the library do.
		 */
		class MarkedEntity: public Entity
		{
			public:
			explicit MarkedEntity(ComponentRegistry &registry)
			{
				attachHierarchy(registry);
				registry.addComponent<Marker>(getIdentifier());
				setSignature(getSignature() | getSignatureFromTypes<Marker>());
			}
		};

		class MarkedParentEntity: public ParentEntity
		{
			public:
			explicit MarkedParentEntity(ComponentRegistry &registry)
			{
				attachHierarchy(registry);
				registry.addComponent<Marker>(getIdentifier());
				setSignature(getSignature() | getSignatureFromTypes<Marker>());
			}
		};

		/**
		 * @brief System that reads the Marker of each entity, records the
		 * visit, then runs a callback given by the test.
		 */
		class RecordingSystem: public SystemFiller<Marker>
		{
			public:
			RecordingSystem(void)
				: SystemFiller<Marker>(Phase::Layout)
			{
			}

			void update(const Entity::Identifier &identifier) override
			{
				getComponent<Marker>(identifier);
				visited.push_back(identifier);
				if (onUpdate) {
					onUpdate(identifier, getEntityRegistry());
				}
			}

			bool isComponentRegistryBound(void) const
			{
				try {
					getComponentRegistry();
				} catch (const std::runtime_error &) {
					return false;
				}
				return true;
			}

			bool isEntityRegistryBound(void) const
			{
				try {
					getEntityRegistry();
				} catch (const std::runtime_error &) {
					return false;
				}
				return true;
			}

			std::vector<Entity::Identifier> visited;
			std::function<void(Entity::Identifier, EntityRegistry &)> onUpdate;
		};
	}	 // namespace

	TEST_F(TestSystem, RoutineVisitsEveryMatchingEntity)
	{
		ComponentRegistry components;
		EntityRegistryContainer entities;
		LevelOrderTraveler traveler;
		RecordingSystem system;

		auto first	= std::make_shared<MarkedEntity>(components);
		auto second = std::make_shared<MarkedEntity>(components);
		entities.addEntity(first);
		entities.addEntity(second);

		system.routine(components, entities, traveler);

		ASSERT_EQ(system.visited.size(), 2U);
		EXPECT_EQ(system.visited[0], first->getIdentifier());
		EXPECT_EQ(system.visited[1], second->getIdentifier());
	}

	TEST_F(TestSystem, RoutineSkipsEntityRemovedDuringThePass)
	{
		ComponentRegistry components;
		EntityRegistryContainer entities;
		LevelOrderTraveler traveler;
		RecordingSystem system;

		auto first	= std::make_shared<MarkedEntity>(components);
		auto second = std::make_shared<MarkedEntity>(components);
		auto third	= std::make_shared<MarkedEntity>(components);
		entities.addEntity(first);
		entities.addEntity(second);
		entities.addEntity(third);

		const auto firstIdentifier	= first->getIdentifier();
		const auto secondIdentifier = second->getIdentifier();
		const auto thirdIdentifier	= third->getIdentifier();

		system.onUpdate = [firstIdentifier,
						   secondIdentifier](Entity::Identifier identifier,
											 EntityRegistry &registry) {
			if (identifier == firstIdentifier) {
				registry.removeEntity(secondIdentifier);
			}
		};

		EXPECT_NO_THROW(system.routine(components, entities, traveler));

		ASSERT_EQ(system.visited.size(), 2U);
		EXPECT_EQ(system.visited[0], firstIdentifier);
		EXPECT_EQ(system.visited[1], thirdIdentifier);
	}

	TEST_F(TestSystem, RoutineSkipsChildrenOfParentRemovedDuringThePass)
	{
		ComponentRegistry components;
		EntityRegistryContainer entities;
		LevelOrderTraveler traveler;
		RecordingSystem system;

		auto first	= std::make_shared<MarkedEntity>(components);
		auto parent = std::make_shared<MarkedParentEntity>(components);
		auto child	= std::make_shared<MarkedEntity>(components);
		entities.addEntity(first);
		entities.addEntity(parent);
		parent->addEntity(child);

		const auto firstIdentifier	= first->getIdentifier();
		const auto parentIdentifier = parent->getIdentifier();

		system.onUpdate = [firstIdentifier,
						   parentIdentifier](Entity::Identifier identifier,
											 EntityRegistry &registry) {
			if (identifier == firstIdentifier) {
				registry.removeEntity(parentIdentifier);
			}
		};

		EXPECT_NO_THROW(system.routine(components, entities, traveler));

		ASSERT_EQ(system.visited.size(), 1U);
		EXPECT_EQ(system.visited[0], firstIdentifier);
	}

	TEST_F(TestSystem, RoutineAllowsEntityToRemoveItself)
	{
		ComponentRegistry components;
		EntityRegistryContainer entities;
		LevelOrderTraveler traveler;
		RecordingSystem system;

		auto first	= std::make_shared<MarkedEntity>(components);
		auto second = std::make_shared<MarkedEntity>(components);
		entities.addEntity(first);
		entities.addEntity(second);

		const auto firstIdentifier	= first->getIdentifier();
		const auto secondIdentifier = second->getIdentifier();

		system.onUpdate = [firstIdentifier](Entity::Identifier identifier,
											EntityRegistry &registry) {
			if (identifier == firstIdentifier) {
				registry.removeEntity(firstIdentifier);
			}
		};

		EXPECT_NO_THROW(system.routine(components, entities, traveler));

		ASSERT_EQ(system.visited.size(), 2U);
		EXPECT_EQ(system.visited[0], firstIdentifier);
		EXPECT_EQ(system.visited[1], secondIdentifier);
		EXPECT_EQ(entities.getEntities().size(), 1U);
	}

	TEST_F(TestSystem, RoutineLeavesEntityAddedDuringThePassForTheNextOne)
	{
		ComponentRegistry components;
		EntityRegistryContainer entities;
		LevelOrderTraveler traveler;
		RecordingSystem system;

		auto first	= std::make_shared<MarkedEntity>(components);
		auto second = std::make_shared<MarkedEntity>(components);
		auto added	= std::make_shared<MarkedEntity>(components);
		entities.addEntity(first);
		entities.addEntity(second);

		const auto firstIdentifier = first->getIdentifier();

		system.onUpdate = [firstIdentifier,
						   added](Entity::Identifier identifier,
								  EntityRegistry &registry) {
			if (identifier == firstIdentifier) {
				registry.addEntity(added);
			}
		};

		system.routine(components, entities, traveler);

		ASSERT_EQ(system.visited.size(), 2U);
		EXPECT_EQ(system.visited[0], firstIdentifier);
		EXPECT_EQ(system.visited[1], second->getIdentifier());

		system.visited.clear();
		system.onUpdate = nullptr;
		system.routine(components, entities, traveler);

		ASSERT_EQ(system.visited.size(), 3U);
		EXPECT_EQ(system.visited[2], added->getIdentifier());
	}

	TEST_F(TestSystem, RoutineUnbindsRegistriesWhenItReturns)
	{
		ComponentRegistry components;
		EntityRegistryContainer entities;
		LevelOrderTraveler traveler;
		RecordingSystem system;

		entities.addEntity(std::make_shared<MarkedEntity>(components));
		system.routine(components, entities, traveler);

		EXPECT_FALSE(system.isComponentRegistryBound());
		EXPECT_FALSE(system.isEntityRegistryBound());
	}

	TEST_F(TestSystem, RoutineUnbindsRegistriesWhenUpdateThrows)
	{
		ComponentRegistry components;
		EntityRegistryContainer entities;
		LevelOrderTraveler traveler;
		RecordingSystem system;

		entities.addEntity(std::make_shared<MarkedEntity>(components));

		system.onUpdate = [](Entity::Identifier, EntityRegistry &) {
			throw std::logic_error("update failed");
		};

		EXPECT_THROW(system.routine(components, entities, traveler),
					 std::logic_error);

		EXPECT_FALSE(system.isComponentRegistryBound());
		EXPECT_FALSE(system.isEntityRegistryBound());
	}
}	 // namespace guillaume::ecs::tests