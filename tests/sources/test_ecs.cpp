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

#include "test_ecs.hpp"

#include <memory>

#include <guillaume/ecs/component_storage.hpp>
#include <guillaume/ecs/component_type_id.hpp>
#include <guillaume/ecs/entity.hpp>

namespace guillaume::tests
{
	namespace
	{
		class DummyComponent: public ecs::Component
		{
			public:
			DummyComponent(void)		   = default;
			~DummyComponent(void) override = default;

			explicit DummyComponent(int value)
				: value(value)
			{
			}

			int value { 0 };
		};

		class OtherComponent: public ecs::Component
		{
			public:
			OtherComponent(void)		   = default;
			~OtherComponent(void) override = default;
		};
	}	 // namespace

	TEST_F(Test, ComponentTypeIdsAreStablePerType)
	{
		const auto first  = ecs::ComponentTypeId::get<DummyComponent>();
		const auto first2 = ecs::ComponentTypeId::get<DummyComponent>();
		const auto other  = ecs::ComponentTypeId::get<OtherComponent>();

		EXPECT_EQ(first, first2);
		EXPECT_LT(first, ecs::MaxComponentTypes);
		EXPECT_LT(other, ecs::MaxComponentTypes);
	}

	TEST_F(Test, ComponentTypeIdsDifferAcrossTypes)
	{
		EXPECT_NE(ecs::ComponentTypeId::get<DummyComponent>(),
				  ecs::ComponentTypeId::get<OtherComponent>());
	}

	TEST_F(Test, StorageStartsEmpty)
	{
		ecs::ComponentStorage<DummyComponent> storage;

		EXPECT_FALSE(storage.has(ecs::Entity::InvalidIdentifier));
		EXPECT_EQ(storage.find(ecs::Entity::InvalidIdentifier), nullptr);
	}

	TEST_F(Test, EmplaceReturnsReferenceToStoredComponent)
	{
		ecs::ComponentStorage<DummyComponent> storage;
		auto entity			  = std::make_shared<ecs::Entity>();
		const auto identifier = entity->getIdentifier();

		ecs::Component &stored = storage.emplace(identifier, 42);

		EXPECT_EQ(stored.hasChanged(), false);
		EXPECT_TRUE(storage.has(identifier));
		EXPECT_EQ(storage.find(identifier)->value, 42);
	}

	TEST_F(Test, EmplaceReplacesExistingComponent)
	{
		ecs::ComponentStorage<DummyComponent> storage;
		auto entity			  = std::make_shared<ecs::Entity>();
		const auto identifier = entity->getIdentifier();

		storage.emplace(identifier, 1);
		storage.emplace(identifier, 99);

		EXPECT_EQ(storage.find(identifier)->value, 99);
	}

	TEST_F(Test, FindReturnsNullptrForMissingEntity)
	{
		ecs::ComponentStorage<DummyComponent> storage;
		auto entity			  = std::make_shared<ecs::Entity>();
		const auto identifier = entity->getIdentifier();

		storage.emplace(identifier, 1);
		const auto other = std::make_shared<ecs::Entity>();

		EXPECT_EQ(storage.find(other->getIdentifier()), nullptr);
	}

	TEST_F(Test, RemoveErasesComponentAndFlags)
	{
		ecs::ComponentStorage<DummyComponent> storage;
		auto entity			  = std::make_shared<ecs::Entity>();
		const auto identifier = entity->getIdentifier();

		storage.emplace(identifier, 7);
		storage.remove(identifier);

		EXPECT_FALSE(storage.has(identifier));
		EXPECT_FALSE(storage.hasChanged(identifier));
	}

	TEST_F(Test, HasChangedTracksChangedFlag)
	{
		ecs::ComponentStorage<DummyComponent> storage;
		auto entity			  = std::make_shared<ecs::Entity>();
		const auto identifier = entity->getIdentifier();

		storage.emplace(identifier, 1);
		EXPECT_FALSE(storage.hasChanged(identifier));

		storage.find(identifier)->setHasChanged(true);
		EXPECT_TRUE(storage.hasChanged(identifier));
	}

	TEST_F(Test, HasChangedIsFalseWhenFlagReset)
	{
		ecs::ComponentStorage<DummyComponent> storage;
		auto entity			  = std::make_shared<ecs::Entity>();
		const auto identifier = entity->getIdentifier();

		storage.emplace(identifier, 1);
		storage.find(identifier)->setHasChanged(true);

		storage.resetChangedFlags();

		EXPECT_FALSE(storage.hasChanged(identifier));
	}
}	 // namespace guillaume::tests
