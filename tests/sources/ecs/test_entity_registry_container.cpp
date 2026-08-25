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

#include "ecs/test_entity_registry.hpp"

#include <memory>

#include "guillaume/ecs/level_order_traveler.hpp"
#include "guillaume/ecs/parent_entity.hpp"
#include "guillaume/ecs/reverse_level_order_traveler.hpp"

namespace guillaume::ecs::tests
{

	class DummyEntity: public Entity
	{
	};

	class DummyParentEntity: public ParentEntity
	{
	};

	class TestEntityRegistryContainer final: public EntityRegistry
	{
		private:
		std::vector<std::shared_ptr<Entity>> _entities;

		protected:
		std::vector<std::shared_ptr<Entity>> &
			accessDirectEntities(void) override
		{
			return _entities;
		}

		const std::vector<std::shared_ptr<Entity>> &
			accessDirectEntities(void) const override
		{
			return _entities;
		}
	};

	TEST_F(TestEntityRegistry, AddEntityStoresIdentifierAndSignature)
	{
		TestEntityRegistryContainer registry;
		auto entity = std::make_shared<DummyEntity>();

		Entity::Signature signature;
		signature.set(3);
		entity->setSignature(signature);

		const auto identifier = entity->getIdentifier();

		registry.addEntity(entity);

		ASSERT_EQ(registry.getEntities().size(), 1U);
		EXPECT_EQ(registry.getEntities().front()->getIdentifier(), identifier);
		EXPECT_EQ(registry.getEntities().front()->getSignature(), signature);

		ASSERT_EQ(registry.getEntities().size(), 1U);
		EXPECT_EQ(registry.getEntities().front()->getIdentifier(), identifier);
	}

	TEST_F(TestEntityRegistry, AddEntityTracksAllEntityIdentifiersInOrder)
	{
		TestEntityRegistryContainer registry;
		auto firstEntity  = std::make_shared<DummyEntity>();
		auto secondEntity = std::make_shared<DummyEntity>();

		const auto firstIdentifier	= firstEntity->getIdentifier();
		const auto secondIdentifier = secondEntity->getIdentifier();

		registry.addEntity(firstEntity);
		registry.addEntity(secondEntity);

		const auto &entities = registry.getEntities();
		ASSERT_EQ(entities.size(), 2U);
		EXPECT_EQ(entities[0]->getIdentifier(), firstIdentifier);
		EXPECT_EQ(entities[1]->getIdentifier(), secondIdentifier);
	}

	TEST_F(TestEntityRegistry, GetEntityWithSignatureTraversesHierarchyInBfs)
	{
		TestEntityRegistryContainer registry;

		auto rootMatch = std::make_shared<DummyEntity>();
		Entity::Signature rootSignature;
		rootSignature.set(5);
		rootMatch->setSignature(rootSignature);
		const auto rootIdentifier = rootMatch->getIdentifier();

		auto parent					= std::make_shared<DummyParentEntity>();
		const auto parentIdentifier = parent->getIdentifier();

		auto childMatch = std::make_shared<DummyEntity>();
		childMatch->setSignature(rootSignature);
		const auto childIdentifier = childMatch->getIdentifier();

		parent->addEntity(childMatch);
		registry.addEntity(rootMatch);
		registry.addEntity(parent);

		const auto matchingIdentifiers =
			registry.getEntityWithSignature(rootSignature);

		ASSERT_EQ(matchingIdentifiers.size(), 2U);
		EXPECT_EQ(matchingIdentifiers[0], rootIdentifier);
		EXPECT_EQ(matchingIdentifiers[1], childIdentifier);
		EXPECT_NE(parentIdentifier, Entity::InvalidIdentifier);
	}

	TEST_F(TestEntityRegistry, LevelOrderTravelerTraversesTopDown)
	{
		TestEntityRegistryContainer registry;
		LevelOrderTraveler traveler;

		auto firstRoot				   = std::make_shared<DummyEntity>();
		const auto firstRootIdentifier = firstRoot->getIdentifier();

		auto parent					= std::make_shared<DummyParentEntity>();
		auto child					= std::make_shared<DummyEntity>();
		const auto parentIdentifier = parent->getIdentifier();
		const auto childIdentifier	= child->getIdentifier();

		parent->addEntity(child);
		registry.addEntity(firstRoot);
		registry.addEntity(parent);

		const auto entities = traveler.travel(registry);

		ASSERT_EQ(entities.size(), 3U);
		EXPECT_EQ(entities[0]->getIdentifier(), firstRootIdentifier);
		EXPECT_EQ(entities[1]->getIdentifier(), parentIdentifier);
		EXPECT_EQ(entities[2]->getIdentifier(), childIdentifier);
	}

	TEST_F(TestEntityRegistry, ReverseLevelOrderTravelerTraversesBottomUp)
	{
		TestEntityRegistryContainer registry;
		ReverseLevelOrderTraveler traveler;

		auto firstRoot				   = std::make_shared<DummyEntity>();
		const auto firstRootIdentifier = firstRoot->getIdentifier();

		auto parent					= std::make_shared<DummyParentEntity>();
		auto child					= std::make_shared<DummyEntity>();
		const auto parentIdentifier = parent->getIdentifier();
		const auto childIdentifier	= child->getIdentifier();

		parent->addEntity(child);
		registry.addEntity(firstRoot);
		registry.addEntity(parent);

		const auto entities = traveler.travel(registry);

		ASSERT_EQ(entities.size(), 3U);
		EXPECT_EQ(entities[0]->getIdentifier(), childIdentifier);
		EXPECT_EQ(entities[1]->getIdentifier(), parentIdentifier);
		EXPECT_EQ(entities[2]->getIdentifier(), firstRootIdentifier);
	}

	TEST_F(TestEntityRegistry,
		   GetEntityWithSignatureUsesProvidedTraversalStrategy)
	{
		TestEntityRegistryContainer registry;
		ReverseLevelOrderTraveler traveler;

		auto firstRoot = std::make_shared<DummyEntity>();
		Entity::Signature signature;
		signature.set(2);
		firstRoot->setSignature(signature);
		const auto firstRootIdentifier = firstRoot->getIdentifier();

		auto parent = std::make_shared<DummyParentEntity>();
		auto child	= std::make_shared<DummyEntity>();
		child->setSignature(signature);
		const auto childIdentifier = child->getIdentifier();

		parent->addEntity(child);
		registry.addEntity(firstRoot);
		registry.addEntity(parent);

		const auto matchingIdentifiers =
			registry.getEntityWithSignature(signature, traveler);

		ASSERT_EQ(matchingIdentifiers.size(), 2U);
		EXPECT_EQ(matchingIdentifiers[0], childIdentifier);
		EXPECT_EQ(matchingIdentifiers[1], firstRootIdentifier);
	}

}	 // namespace guillaume::ecs::tests