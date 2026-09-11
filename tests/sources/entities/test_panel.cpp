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

#include "entities/test_panel.hpp"

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

	}	 // namespace

	TEST_F(TestPanel, ChildrenHaveDistinctLayersAndDepthOffsets)
	{
		ecs::ComponentRegistry registry;

		auto firstChild	 = std::make_shared<TestChild>(registry);
		auto secondChild = std::make_shared<TestChild>(registry);

		auto panel = std::make_shared<Panel>(
			registry,
			utility::graphic::PoseF(
				utility::graphic::PositionF(0.0f, 0.0f, -300.0f),
				utility::graphic::OrientationF()),
			utility::graphic::Color32Bit(255, 255, 255, 255), 16.0f, 16.0f,
			std::vector<std::shared_ptr<ecs::Entity>> { firstChild,
														secondChild });

		panel->initialize();
		panel->update();

		EXPECT_EQ(firstChild->getLayer(), 1);
		EXPECT_EQ(secondChild->getLayer(), 1);

		const float baseZ =
			registry.getComponent<components::Transform>(panel->getIdentifier())
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

	TEST_F(TestPanel, RepeatedUpdatesDoNotStackDepthOffsets)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		auto panel = std::make_shared<Panel>(
			registry,
			utility::graphic::PoseF(
				utility::graphic::PositionF(0.0f, 0.0f, -300.0f),
				utility::graphic::OrientationF()),
			utility::graphic::Color32Bit(255, 255, 255, 255), 16.0f, 16.0f,
			std::vector<std::shared_ptr<ecs::Entity>> { child });

		panel->initialize();
		panel->update();
		panel->update();

		const float baseZ =
			registry.getComponent<components::Transform>(panel->getIdentifier())
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

	TEST_F(TestPanel, BoundComputedFromChildrenBounds)
	{
		ecs::ComponentRegistry registry;

		auto firstChild	 = std::make_shared<TestChild>(registry);
		auto secondChild = std::make_shared<TestChild>(registry);

		auto &firstChildBound = registry.getComponent<components::Bound>(
			firstChild->getIdentifier());
		firstChildBound.setWidth(50.0f).setHeight(30.0f);

		auto &secondChildBound = registry.getComponent<components::Bound>(
			secondChild->getIdentifier());
		secondChildBound.setWidth(20.0f).setHeight(60.0f);

		auto &firstChildTransform =
			registry.getComponent<components::Transform>(
				firstChild->getIdentifier());
		firstChildTransform.setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(10.0f, 20.0f, -300.0f),
			utility::graphic::OrientationF()));

		auto &secondChildTransform =
			registry.getComponent<components::Transform>(
				secondChild->getIdentifier());
		secondChildTransform.setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(100.0f, 5.0f, -300.0f),
			utility::graphic::OrientationF()));

		auto panel = std::make_shared<Panel>(
			registry,
			utility::graphic::PoseF(
				utility::graphic::PositionF(0.0f, 0.0f, -300.0f),
				utility::graphic::OrientationF()),
			utility::graphic::Color32Bit(255, 255, 255, 255), 16.0f, 16.0f,
			std::vector<std::shared_ptr<ecs::Entity>> { firstChild,
														secondChild });

		panel->initialize();
		panel->update();

		const auto &panelBound =
			registry.getComponent<components::Bound>(panel->getIdentifier());

		// Width: max(10 + 50, 100 + 20) + 16 = 136.
		// Height: max(20 + 30, 5 + 60) + 16 = 81.
		EXPECT_FLOAT_EQ(panelBound.getWidth(), 136.0f);
		EXPECT_FLOAT_EQ(panelBound.getHeight(), 81.0f);
	}

	TEST_F(TestPanel, EmptyChildrenKeepPaddingSize)
	{
		ecs::ComponentRegistry registry;

		auto panel = std::make_shared<Panel>(
			registry,
			utility::graphic::PoseF(
				utility::graphic::PositionF(0.0f, 0.0f, -300.0f),
				utility::graphic::OrientationF()),
			utility::graphic::Color32Bit(255, 255, 255, 255), 16.0f, 16.0f,
			std::vector<std::shared_ptr<ecs::Entity>> {});

		panel->initialize();
		panel->update();

		const auto &panelBound =
			registry.getComponent<components::Bound>(panel->getIdentifier());

		EXPECT_FLOAT_EQ(panelBound.getWidth(), 16.0f);
		EXPECT_FLOAT_EQ(panelBound.getHeight(), 16.0f);
	}

	TEST_F(TestPanel, SetPoseLiftsChildrenToNewDepth)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		auto panel = std::make_shared<Panel>(
			registry,
			utility::graphic::PoseF(
				utility::graphic::PositionF(0.0f, 0.0f, -300.0f),
				utility::graphic::OrientationF()),
			utility::graphic::Color32Bit(255, 255, 255, 255), 16.0f, 16.0f,
			std::vector<std::shared_ptr<ecs::Entity>> { child });

		panel->initialize();
		panel->update();

		panel->setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(0.0f, 0.0f, -200.0f),
			utility::graphic::OrientationF()));

		const auto childZ =
			registry.getComponent<components::Transform>(child->getIdentifier())
				.getPose()
				.getPosition()
				.getZ();

		EXPECT_FLOAT_EQ(childZ, -199.0f);
	}

	TEST_F(TestPanel, SetPaddingUpdatesBound)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(child->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		auto panel = std::make_shared<Panel>(
			registry,
			utility::graphic::PoseF(
				utility::graphic::PositionF(0.0f, 0.0f, -300.0f),
				utility::graphic::OrientationF()),
			utility::graphic::Color32Bit(255, 255, 255, 255), 16.0f, 16.0f,
			std::vector<std::shared_ptr<ecs::Entity>> { child });

		panel->initialize();
		panel->update();

		panel->setPadding(8.0f);

		const auto &panelBound =
			registry.getComponent<components::Bound>(panel->getIdentifier());

		EXPECT_FLOAT_EQ(panelBound.getWidth(), 18.0f);
		EXPECT_FLOAT_EQ(panelBound.getHeight(), 18.0f);
	}

	TEST_F(TestPanel, SetEntitiesAttachesNewChildren)
	{
		ecs::ComponentRegistry registry;

		auto panel = std::make_shared<Panel>(
			registry,
			utility::graphic::PoseF(
				utility::graphic::PositionF(0.0f, 0.0f, -300.0f),
				utility::graphic::OrientationF()),
			utility::graphic::Color32Bit(255, 255, 255, 255), 16.0f, 16.0f,
			std::vector<std::shared_ptr<ecs::Entity>> {});

		panel->initialize();
		panel->update();

		auto child = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(child->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		panel->setEntities(std::vector<std::shared_ptr<ecs::Entity>> { child });

		EXPECT_EQ(child->getLayer(), 1);

		const auto &panelBound =
			registry.getComponent<components::Bound>(panel->getIdentifier());

		EXPECT_FLOAT_EQ(panelBound.getWidth(), 26.0f);
		EXPECT_FLOAT_EQ(panelBound.getHeight(), 26.0f);
	}

}	 // namespace guillaume::entities::tests
