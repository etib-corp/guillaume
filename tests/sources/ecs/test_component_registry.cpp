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

#include "ecs/test_component_registry.hpp"

#include <memory>

#include "guillaume/ecs/entity.hpp"

namespace guillaume::ecs::tests
{

	class DummyComponent: public Component
	{
		public:
		DummyComponent(void)		   = default;
		~DummyComponent(void) override = default;
	};

	TEST_F(TestComponentRegistry, HasChangedIsFalseForMissingComponent)
	{
		ComponentRegistry registry;

		EXPECT_FALSE(registry.hasChanged(Entity::InvalidIdentifier));
	}

	TEST_F(TestComponentRegistry, HasChangedTracksOnlyTheTargetEntity)
	{
		ComponentRegistry registry;

		auto firstEntity  = std::make_shared<Entity>();
		auto secondEntity = std::make_shared<Entity>();

		const auto firstIdentifier	= firstEntity->getIdentifier();
		const auto secondIdentifier = secondEntity->getIdentifier();

		registry.addComponent<DummyComponent>(firstIdentifier);
		registry.addComponent<DummyComponent>(secondIdentifier);

		EXPECT_FALSE(registry.hasChanged(firstIdentifier));
		EXPECT_FALSE(registry.hasChanged(secondIdentifier));

		registry.getComponent<DummyComponent>(firstIdentifier)
			.setHasChanged(true);

		EXPECT_TRUE(registry.hasChanged(firstIdentifier));
		EXPECT_FALSE(registry.hasChanged(secondIdentifier));
	}

	TEST_F(TestComponentRegistry, ResetChangedFlagsClearsAllDirtyFlags)
	{
		ComponentRegistry registry;

		auto firstEntity  = std::make_shared<Entity>();
		auto secondEntity = std::make_shared<Entity>();

		const auto firstIdentifier	= firstEntity->getIdentifier();
		const auto secondIdentifier = secondEntity->getIdentifier();

		registry.addComponent<DummyComponent>(firstIdentifier);
		registry.addComponent<DummyComponent>(secondIdentifier);

		registry.getComponent<DummyComponent>(firstIdentifier)
			.setHasChanged(true);
		registry.getComponent<DummyComponent>(secondIdentifier)
			.setHasChanged(true);

		EXPECT_TRUE(registry.hasChanged(firstIdentifier));
		EXPECT_TRUE(registry.hasChanged(secondIdentifier));

		registry.resetChangedFlags();

		EXPECT_FALSE(registry.hasChanged(firstIdentifier));
		EXPECT_FALSE(registry.hasChanged(secondIdentifier));
	}

}	 // namespace guillaume::ecs::tests
