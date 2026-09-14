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

#include "entities/test_container.hpp"

#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/entity_filler.hpp>

namespace guillaume::entities::tests
{

	namespace
	{

		/**
		 * @brief Minimal child entity holding a transform and a bound.
		 */
		class TestChild:
			public ecs::EntityFiller<components::Transform, components::Bound>
		{
			public:
			explicit TestChild(ecs::ComponentRegistry &registry)
				: ecs::EntityFiller<components::Transform, components::Bound>(
					  registry)
			{
			}
		};

		/**
		 * @brief Build a Container with default pose/color and the given
		 * geometry parameters.
		 */
		std::shared_ptr<Container> makeContainer(
			ecs::ComponentRegistry &registry,
			const std::vector<std::shared_ptr<ecs::Entity>> &entities,
			float padding = 16.0f, float spacing = 8.0f, float margin = 0.0f,
			Container::Direction direction = Container::Direction::Row)
		{
			return std::make_shared<Container>(
				registry,
				utility::graphic::PoseF(
					utility::graphic::PositionF(0.0f, 0.0f, -300.0f),
					utility::graphic::OrientationF()),
				utility::graphic::Color32Bit(255, 255, 255, 255), 16.0f,
				padding, spacing, margin, direction, entities);
		}

	}	 // namespace

	TEST_F(TestContainer, ChildrenHaveDistinctLayersAndDepthOffsets)
	{
		ecs::ComponentRegistry registry;

		auto firstChild	 = std::make_shared<TestChild>(registry);
		auto secondChild = std::make_shared<TestChild>(registry);

		auto container = makeContainer(registry, { firstChild, secondChild });

		container->initialize();
		container->update();

		EXPECT_EQ(firstChild->getLayer(), 1);
		EXPECT_EQ(secondChild->getLayer(), 1);

		const float baseZ =
			registry
				.getComponent<components::Transform>(container->getIdentifier())
				.getPose()
				.getPosition()
				.getZ();
		const auto firstChildZ	= registry
									  .getComponent<components::Transform>(
										  firstChild->getIdentifier())
									  .getPose()
									  .getPosition()
									  .getZ();
		const auto secondChildZ = registry
									  .getComponent<components::Transform>(
										  secondChild->getIdentifier())
									  .getPose()
									  .getPosition()
									  .getZ();

		EXPECT_FLOAT_EQ(firstChildZ, baseZ + 1.0f);
		EXPECT_FLOAT_EQ(secondChildZ, baseZ + 1.0f);
		EXPECT_LT(baseZ, firstChildZ);
		EXPECT_LT(baseZ, secondChildZ);
	}

	TEST_F(TestContainer, RepeatedUpdatesDoNotStackDepthOffsets)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		auto container = makeContainer(registry, { child });

		container->initialize();
		container->update();
		container->update();

		const float baseZ =
			registry
				.getComponent<components::Transform>(container->getIdentifier())
				.getPose()
				.getPosition()
				.getZ();
		const auto childZ =
			registry.getComponent<components::Transform>(child->getIdentifier())
				.getPose()
				.getPosition()
				.getZ();

		EXPECT_FLOAT_EQ(childZ, baseZ + 1.0f);
	}

	TEST_F(TestContainer, RowBoundComputedFromChildrenBounds)
	{
		ecs::ComponentRegistry registry;

		auto firstChild	 = std::make_shared<TestChild>(registry);
		auto secondChild = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(firstChild->getIdentifier())
			.setWidth(50.0f)
			.setHeight(30.0f);
		registry.getComponent<components::Bound>(secondChild->getIdentifier())
			.setWidth(20.0f)
			.setHeight(60.0f);

		auto container = makeContainer(registry, { firstChild, secondChild });

		container->initialize();
		container->update();

		const auto &bound = registry.getComponent<components::Bound>(
			container->getIdentifier());

		// Width: 50 + 20 + spacing(8) + 2 * padding(16) = 110.
		// Height: max(30, 60) + 2 * padding(16) = 92.
		EXPECT_FLOAT_EQ(bound.getWidth(), 110.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 92.0f);
	}

	TEST_F(TestContainer, ColumnBoundComputedFromChildrenBounds)
	{
		ecs::ComponentRegistry registry;

		auto firstChild	 = std::make_shared<TestChild>(registry);
		auto secondChild = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(firstChild->getIdentifier())
			.setWidth(50.0f)
			.setHeight(30.0f);
		registry.getComponent<components::Bound>(secondChild->getIdentifier())
			.setWidth(20.0f)
			.setHeight(60.0f);

		auto container =
			makeContainer(registry, { firstChild, secondChild }, 16.0f, 8.0f,
						  0.0f, Container::Direction::Column);

		container->initialize();
		container->update();

		const auto &bound = registry.getComponent<components::Bound>(
			container->getIdentifier());

		// Width: max(50, 20) + 2 * padding(16) = 82.
		// Height: 30 + 60 + spacing(8) + 2 * padding(16) = 130.
		EXPECT_FLOAT_EQ(bound.getWidth(), 82.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 130.0f);
	}

	TEST_F(TestContainer, RowPositionsChildrenWithPaddingAndSpacing)
	{
		ecs::ComponentRegistry registry;

		auto firstChild	 = std::make_shared<TestChild>(registry);
		auto secondChild = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(firstChild->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);
		registry.getComponent<components::Bound>(secondChild->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		auto container = makeContainer(registry, { firstChild, secondChild });

		container->initialize();
		container->update();

		const auto firstX  = registry
								 .getComponent<components::Transform>(
									 firstChild->getIdentifier())
								 .getPose()
								 .getPosition()
								 .getX();
		const auto secondX = registry
								 .getComponent<components::Transform>(
									 secondChild->getIdentifier())
								 .getPose()
								 .getPosition()
								 .getX();

		// First child at padding(16), second at 16 + 10 + spacing(8) = 34.
		EXPECT_FLOAT_EQ(firstX, 16.0f);
		EXPECT_FLOAT_EQ(secondX, 34.0f);
	}

	TEST_F(TestContainer, EmptyChildrenKeepPaddingSize)
	{
		ecs::ComponentRegistry registry;

		auto container = makeContainer(registry, {});

		container->initialize();
		container->update();

		const auto &bound = registry.getComponent<components::Bound>(
			container->getIdentifier());

		// No children: width = height = 2 * padding(16) = 32.
		EXPECT_FLOAT_EQ(bound.getWidth(), 32.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 32.0f);
	}

	TEST_F(TestContainer, SetPoseLiftsChildrenToNewDepth)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		auto container = makeContainer(registry, { child });

		container->initialize();
		container->update();

		container->setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(0.0f, 0.0f, -200.0f),
			utility::graphic::OrientationF()));

		const auto childZ =
			registry.getComponent<components::Transform>(child->getIdentifier())
				.getPose()
				.getPosition()
				.getZ();

		EXPECT_FLOAT_EQ(childZ, -199.0f);
	}

	TEST_F(TestContainer, SetPaddingUpdatesBound)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(child->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		auto container = makeContainer(registry, { child }, 16.0f, 0.0f);

		container->initialize();
		container->update();

		container->setPadding(8.0f);

		const auto &bound = registry.getComponent<components::Bound>(
			container->getIdentifier());

		// Width: 10 + 2 * 8 = 26.
		EXPECT_FLOAT_EQ(bound.getWidth(), 26.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 26.0f);
	}

	TEST_F(TestContainer, SetSpacingUpdatesBound)
	{
		ecs::ComponentRegistry registry;

		auto firstChild	 = std::make_shared<TestChild>(registry);
		auto secondChild = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(firstChild->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);
		registry.getComponent<components::Bound>(secondChild->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		auto container = makeContainer(registry, { firstChild, secondChild });

		container->initialize();
		container->update();

		container->setSpacing(4.0f);

		const auto &bound = registry.getComponent<components::Bound>(
			container->getIdentifier());

		// Width: 10 + 10 + spacing(4) + 2 * padding(16) = 56.
		EXPECT_FLOAT_EQ(bound.getWidth(), 56.0f);
	}

	TEST_F(TestContainer, SetMarginUpdatesBound)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(child->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		auto container = makeContainer(registry, { child }, 16.0f, 0.0f);

		container->initialize();
		container->update();

		container->setMargin(5.0f);

		const auto &bound = registry.getComponent<components::Bound>(
			container->getIdentifier());

		// Width: 10 + 2 * (16 + 5) = 52.
		EXPECT_FLOAT_EQ(bound.getWidth(), 52.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 52.0f);
	}

	TEST_F(TestContainer, SetDirectionUpdatesBound)
	{
		ecs::ComponentRegistry registry;

		auto firstChild	 = std::make_shared<TestChild>(registry);
		auto secondChild = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(firstChild->getIdentifier())
			.setWidth(20.0f)
			.setHeight(10.0f);
		registry.getComponent<components::Bound>(secondChild->getIdentifier())
			.setWidth(20.0f)
			.setHeight(10.0f);

		auto container = makeContainer(registry, { firstChild, secondChild });

		container->initialize();
		container->update();

		container->setDirection(Container::Direction::Column);

		const auto &bound = registry.getComponent<components::Bound>(
			container->getIdentifier());

		// Width: max(20, 20) + 2 * padding(16) = 52.
		// Height: 10 + 10 + spacing(8) + 2 * padding(16) = 60.
		EXPECT_FLOAT_EQ(bound.getWidth(), 52.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 60.0f);
	}

	TEST_F(TestContainer, SetEntitiesAttachesNewChildren)
	{
		ecs::ComponentRegistry registry;

		auto container = makeContainer(registry, {});

		container->initialize();
		container->update();

		auto child = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(child->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		container->setEntities(
			std::vector<std::shared_ptr<ecs::Entity>> { child });

		EXPECT_EQ(child->getLayer(), 1);

		const auto &bound = registry.getComponent<components::Bound>(
			container->getIdentifier());

		// Width: 10 + 2 * padding(16) = 42.
		EXPECT_FLOAT_EQ(bound.getWidth(), 42.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 42.0f);
	}

}	 // namespace guillaume::entities::tests
