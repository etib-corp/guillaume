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

#include "entities/test_layout.hpp"

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
		 * @brief Build a Layout with the given geometry parameters.
		 */
		std::shared_ptr<Layout> makeLayout(
			ecs::ComponentRegistry &registry,
			const std::vector<std::shared_ptr<ecs::Entity>> &entities,
			float padding = 16.0f, float spacing = 8.0f,
			Layout::Axis axis			   = Layout::Axis::Horizontal,
			Layout::MainAxisAlignment main = Layout::MainAxisAlignment::Start,
			Layout::CrossAxisAlignment cross =
				Layout::CrossAxisAlignment::Center)
		{
			return std::make_shared<Layout>(
				registry,
				utility::graphic::PoseF(
					utility::graphic::PositionF(0.0f, 0.0f, -300.0f),
					utility::graphic::OrientationF()),
				utility::graphic::Color32Bit(255, 255, 255, 255), 16.0f, axis,
				main, cross, spacing, padding, false, 0.0f, false, 0.0f,
				entities);
		}

		float childX(ecs::ComponentRegistry &registry,
					 const std::shared_ptr<TestChild> &child)
		{
			return registry
				.getComponent<components::Transform>(child->getIdentifier())
				.getPose()
				.getPosition()
				.getX();
		}

		float childY(ecs::ComponentRegistry &registry,
					 const std::shared_ptr<TestChild> &child)
		{
			return registry
				.getComponent<components::Transform>(child->getIdentifier())
				.getPose()
				.getPosition()
				.getY();
		}

		float childZ(ecs::ComponentRegistry &registry,
					 const std::shared_ptr<TestChild> &child)
		{
			return registry
				.getComponent<components::Transform>(child->getIdentifier())
				.getPose()
				.getPosition()
				.getZ();
		}

	}	 // namespace

	TEST_F(TestLayout, ChildrenHaveDistinctLayersAndDepthOffsets)
	{
		ecs::ComponentRegistry registry;

		auto firstChild	 = std::make_shared<TestChild>(registry);
		auto secondChild = std::make_shared<TestChild>(registry);

		auto layout = makeLayout(registry, { firstChild, secondChild });

		layout->initialize();
		layout->update();

		EXPECT_EQ(firstChild->getLayer(), 1);
		EXPECT_EQ(secondChild->getLayer(), 1);

		const float baseZ =
			registry
				.getComponent<components::Transform>(layout->getIdentifier())
				.getPose()
				.getPosition()
				.getZ();

		EXPECT_FLOAT_EQ(baseZ, -300.0f);
		EXPECT_FLOAT_EQ(childZ(registry, firstChild), baseZ + 1.0f);
		EXPECT_FLOAT_EQ(childZ(registry, secondChild), baseZ + 1.0f);
	}

	TEST_F(TestLayout, RepeatedUpdatesDoNotStackDepthOffsets)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		auto layout = makeLayout(registry, { child });

		layout->initialize();
		layout->update();
		layout->update();

		const float baseZ =
			registry
				.getComponent<components::Transform>(layout->getIdentifier())
				.getPose()
				.getPosition()
				.getZ();

		EXPECT_FLOAT_EQ(childZ(registry, child), baseZ + 1.0f);
	}

	TEST_F(TestLayout, RowBoundComputedFromChildrenBounds)
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

		auto layout = makeLayout(registry, { firstChild, secondChild });

		layout->initialize();
		layout->update();

		const auto &bound =
			registry.getComponent<components::Bound>(layout->getIdentifier());

		// Width: 50 + 20 + spacing(8) + 2 * padding(16) = 110.
		// Height: max(30, 60) + 2 * padding(16) = 92.
		EXPECT_FLOAT_EQ(bound.getWidth(), 110.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 92.0f);
	}

	TEST_F(TestLayout, ColumnBoundComputedFromChildrenBounds)
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

		auto layout = makeLayout(registry, { firstChild, secondChild }, 16.0f,
								 8.0f, Layout::Axis::Vertical);

		layout->initialize();
		layout->update();

		const auto &bound =
			registry.getComponent<components::Bound>(layout->getIdentifier());

		// Width: max(50, 20) + 2 * padding(16) = 82.
		// Height: 30 + 60 + spacing(8) + 2 * padding(16) = 130.
		EXPECT_FLOAT_EQ(bound.getWidth(), 82.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 130.0f);
	}

	TEST_F(TestLayout, RowPositionsChildrenWithPaddingAndSpacing)
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

		auto layout = makeLayout(registry, { firstChild, secondChild });

		layout->initialize();
		layout->update();

		// First child at padding(16), second at 16 + 10 + spacing(8) = 34.
		EXPECT_FLOAT_EQ(childX(registry, firstChild), 16.0f);
		EXPECT_FLOAT_EQ(childX(registry, secondChild), 34.0f);

		// Cross axis centered: padding(16) + (10 - 10) / 2 = 16.
		EXPECT_FLOAT_EQ(childY(registry, firstChild), 16.0f);
		EXPECT_FLOAT_EQ(childY(registry, secondChild), 16.0f);
	}

	TEST_F(TestLayout, EmptyChildrenKeepPaddingSize)
	{
		ecs::ComponentRegistry registry;

		auto layout = makeLayout(registry, {});

		layout->initialize();
		layout->update();

		const auto &bound =
			registry.getComponent<components::Bound>(layout->getIdentifier());

		// No children: width = height = 2 * padding(16) = 32.
		EXPECT_FLOAT_EQ(bound.getWidth(), 32.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 32.0f);
	}

	TEST_F(TestLayout, SetPoseLiftsChildrenToNewDepth)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		auto layout = makeLayout(registry, { child });

		layout->initialize();
		layout->update();

		layout->setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(0.0f, 0.0f, -200.0f),
			utility::graphic::OrientationF()));

		EXPECT_FLOAT_EQ(childZ(registry, child), -199.0f);
	}

	TEST_F(TestLayout, SetPaddingUpdatesBound)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(child->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		auto layout = makeLayout(registry, { child }, 16.0f, 0.0f);

		layout->initialize();
		layout->update();

		layout->setPadding(8.0f);

		const auto &bound =
			registry.getComponent<components::Bound>(layout->getIdentifier());

		// Width: 10 + 2 * 8 = 26.
		EXPECT_FLOAT_EQ(bound.getWidth(), 26.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 26.0f);
	}

	TEST_F(TestLayout, SetSpacingUpdatesBound)
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

		auto layout = makeLayout(registry, { firstChild, secondChild });

		layout->initialize();
		layout->update();

		layout->setSpacing(4.0f);

		const auto &bound =
			registry.getComponent<components::Bound>(layout->getIdentifier());

		// Width: 10 + 10 + spacing(4) + 2 * padding(16) = 56.
		EXPECT_FLOAT_EQ(bound.getWidth(), 56.0f);
	}

	TEST_F(TestLayout, SetAxisUpdatesBound)
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

		auto layout = makeLayout(registry, { firstChild, secondChild });

		layout->initialize();
		layout->update();

		layout->setAxis(Layout::Axis::Vertical);

		const auto &bound =
			registry.getComponent<components::Bound>(layout->getIdentifier());

		// Width: max(20, 20) + 2 * padding(16) = 52.
		// Height: 10 + 10 + spacing(8) + 2 * padding(16) = 60.
		EXPECT_FLOAT_EQ(bound.getWidth(), 52.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 60.0f);
	}

	TEST_F(TestLayout, SetEntitiesAttachesNewChildren)
	{
		ecs::ComponentRegistry registry;

		auto layout = makeLayout(registry, {});

		layout->initialize();
		layout->update();

		auto child = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(child->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		layout->setEntities(
			std::vector<std::shared_ptr<ecs::Entity>> { child });

		EXPECT_EQ(child->getLayer(), 1);

		const auto &bound =
			registry.getComponent<components::Bound>(layout->getIdentifier());

		// Width: 10 + 2 * padding(16) = 42.
		EXPECT_FLOAT_EQ(bound.getWidth(), 42.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 42.0f);
	}

	TEST_F(TestLayout, FixedSizeOverridesAutoSizing)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(child->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		auto layout = makeLayout(registry, { child });

		layout->initialize();
		layout->update();

		layout->setFixedWidth(200.0f);
		layout->setFixedHeight(100.0f);

		const auto &bound =
			registry.getComponent<components::Bound>(layout->getIdentifier());

		EXPECT_FLOAT_EQ(bound.getWidth(), 200.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 100.0f);

		layout->clearFixedWidth();
		layout->clearFixedHeight();

		// Back to auto: width = 10 + 2 * padding(16) = 42.
		EXPECT_FLOAT_EQ(bound.getWidth(), 42.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 42.0f);
	}

	TEST_F(TestLayout, MainAxisAlignmentCenter)
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

		auto layout = makeLayout(registry, { firstChild, secondChild }, 16.0f,
								 8.0f, Layout::Axis::Horizontal,
								 Layout::MainAxisAlignment::Center);
		layout->setFixedWidth(200.0f);

		layout->initialize();
		layout->update();

		// Content region = 200 - 2 * padding(16) = 168.
		// Content = 10 + 10 + spacing(8) = 28.
		// Offset = 16 + (168 - 28) / 2 = 86.
		EXPECT_FLOAT_EQ(childX(registry, firstChild), 86.0f);
		EXPECT_FLOAT_EQ(childX(registry, secondChild), 86.0f + 18.0f);
	}

	TEST_F(TestLayout, MainAxisAlignmentEnd)
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

		auto layout = makeLayout(registry, { firstChild, secondChild }, 16.0f,
								 8.0f, Layout::Axis::Horizontal,
								 Layout::MainAxisAlignment::End);
		layout->setFixedWidth(200.0f);

		layout->initialize();
		layout->update();

		// Offset = 16 + (168 - 28) = 156.
		EXPECT_FLOAT_EQ(childX(registry, firstChild), 156.0f);
		EXPECT_FLOAT_EQ(childX(registry, secondChild), 174.0f);
	}

	TEST_F(TestLayout, MainAxisAlignmentSpaceBetween)
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

		auto layout = makeLayout(registry, { firstChild, secondChild }, 16.0f,
								 8.0f, Layout::Axis::Horizontal,
								 Layout::MainAxisAlignment::SpaceBetween);
		layout->setFixedWidth(200.0f);

		layout->initialize();
		layout->update();

		// Region = 168, raw = 20, gap = (168 - 20) / 1 = 148.
		EXPECT_FLOAT_EQ(childX(registry, firstChild), 16.0f);
		EXPECT_FLOAT_EQ(childX(registry, secondChild), 16.0f + 10.0f + 148.0f);
	}

	TEST_F(TestLayout, CrossAxisAlignmentStart)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(child->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		auto layout = makeLayout(registry, { child }, 16.0f, 8.0f,
								 Layout::Axis::Horizontal,
								 Layout::MainAxisAlignment::Start,
								 Layout::CrossAxisAlignment::Start);

		layout->initialize();
		layout->update();

		EXPECT_FLOAT_EQ(childY(registry, child), 16.0f);
	}

	TEST_F(TestLayout, CrossAxisAlignmentEnd)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(child->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		auto layout = makeLayout(
			registry, { child }, 16.0f, 8.0f, Layout::Axis::Horizontal,
			Layout::MainAxisAlignment::Start, Layout::CrossAxisAlignment::End);
		layout->setFixedHeight(100.0f);

		layout->initialize();
		layout->update();

		// Cross region = 100 - 32 = 68. Offset = 16 + 68 - 10 = 74.
		EXPECT_FLOAT_EQ(childY(registry, child), 74.0f);
	}

	TEST_F(TestLayout, LayoutComponentSettersMarkChanged)
	{
		ecs::ComponentRegistry registry;

		auto layout = makeLayout(registry, {});
		layout->initialize();
		layout->update();

		auto &component =
			registry.getComponent<components::Layout>(layout->getIdentifier());

		component.setHasChanged(false);
		component.setSpacing(3.0f);
		EXPECT_TRUE(component.hasChanged());
	}

}	 // namespace guillaume::entities::tests
