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

#include "entities/test_view.hpp"

#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/entity_filler.hpp>
#include <guillaume/systems/layout.hpp>

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
		 * @brief Build a View with the given geometry parameters.
		 */
		std::shared_ptr<View> makeView(
			ecs::ComponentRegistry &registry,
			const std::vector<std::shared_ptr<ecs::Entity>> &entities,
			float padding = 16.0f, float spacing = 8.0f,
			View::Axis axis				   = View::Axis::Horizontal,
			View::MainAxisAlignment main   = View::MainAxisAlignment::Start,
			View::CrossAxisAlignment cross = View::CrossAxisAlignment::Center)
		{
			return View::create(
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

	TEST_F(TestView, ChildrenHaveDistinctLayersAndDepthOffsets)
	{
		ecs::ComponentRegistry registry;

		auto firstChild	 = std::make_shared<TestChild>(registry);
		auto secondChild = std::make_shared<TestChild>(registry);

		auto layout = makeView(registry, { firstChild, secondChild });

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

	TEST_F(TestView, RepeatedUpdatesDoNotStackDepthOffsets)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		auto layout = makeView(registry, { child });

		const float baseZ =
			registry
				.getComponent<components::Transform>(layout->getIdentifier())
				.getPose()
				.getPosition()
				.getZ();

		EXPECT_FLOAT_EQ(childZ(registry, child), baseZ + 1.0f);
	}

	TEST_F(TestView, RowBoundComputedFromChildrenBounds)
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

		auto layout = makeView(registry, { firstChild, secondChild });

		const auto &bound =
			registry.getComponent<components::Bound>(layout->getIdentifier());

		// Width: 50 + 20 + spacing(8) + 2 * padding(16) = 110.
		// Height: max(30, 60) + 2 * padding(16) = 92.
		EXPECT_FLOAT_EQ(bound.getWidth(), 110.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 92.0f);
	}

	TEST_F(TestView, ColumnBoundComputedFromChildrenBounds)
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

		auto layout = makeView(registry, { firstChild, secondChild }, 16.0f,
							   8.0f, View::Axis::Vertical);

		const auto &bound =
			registry.getComponent<components::Bound>(layout->getIdentifier());

		// Width: max(50, 20) + 2 * padding(16) = 82.
		// Height: 30 + 60 + spacing(8) + 2 * padding(16) = 130.
		EXPECT_FLOAT_EQ(bound.getWidth(), 82.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 130.0f);
	}

	TEST_F(TestView, RowPositionsChildrenWithPaddingAndSpacing)
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

		auto layout = makeView(registry, { firstChild, secondChild });

		// First child at padding(16), second at 16 + 10 + spacing(8) = 34.
		EXPECT_FLOAT_EQ(childX(registry, firstChild), 16.0f);
		EXPECT_FLOAT_EQ(childX(registry, secondChild), 34.0f);

		// Cross axis centered: padding(16) + (10 - 10) / 2 = 16, measured
		// down from the top edge (y points up).
		EXPECT_FLOAT_EQ(childY(registry, firstChild), -16.0f);
		EXPECT_FLOAT_EQ(childY(registry, secondChild), -16.0f);
	}

	TEST_F(TestView, EmptyChildrenKeepPaddingSize)
	{
		ecs::ComponentRegistry registry;

		auto layout = makeView(registry, {});

		const auto &bound =
			registry.getComponent<components::Bound>(layout->getIdentifier());

		// No children: width = height = 2 * padding(16) = 32.
		EXPECT_FLOAT_EQ(bound.getWidth(), 32.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 32.0f);
	}

	TEST_F(TestView, SetPoseLiftsChildrenToNewDepth)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		auto layout = makeView(registry, { child });

		layout->setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(0.0f, 0.0f, -200.0f),
			utility::graphic::OrientationF()));

		EXPECT_FLOAT_EQ(childZ(registry, child), -199.0f);
	}

	TEST_F(TestView, SetPaddingUpdatesBound)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(child->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		auto layout = makeView(registry, { child }, 16.0f, 0.0f);

		layout->setPadding(8.0f);

		const auto &bound =
			registry.getComponent<components::Bound>(layout->getIdentifier());

		// Width: 10 + 2 * 8 = 26.
		EXPECT_FLOAT_EQ(bound.getWidth(), 26.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 26.0f);
	}

	TEST_F(TestView, SetSpacingUpdatesBound)
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

		auto layout = makeView(registry, { firstChild, secondChild });

		layout->setSpacing(4.0f);

		const auto &bound =
			registry.getComponent<components::Bound>(layout->getIdentifier());

		// Width: 10 + 10 + spacing(4) + 2 * padding(16) = 56.
		EXPECT_FLOAT_EQ(bound.getWidth(), 56.0f);
	}

	TEST_F(TestView, SetAxisUpdatesBound)
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

		auto layout = makeView(registry, { firstChild, secondChild });

		layout->setAxis(View::Axis::Vertical);

		const auto &bound =
			registry.getComponent<components::Bound>(layout->getIdentifier());

		// Width: max(20, 20) + 2 * padding(16) = 52.
		// Height: 10 + 10 + spacing(8) + 2 * padding(16) = 60.
		EXPECT_FLOAT_EQ(bound.getWidth(), 52.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 60.0f);
	}

	TEST_F(TestView, SetEntitiesAttachesNewChildren)
	{
		ecs::ComponentRegistry registry;

		auto layout = makeView(registry, {});

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

	TEST_F(TestView, FixedSizeOverridesAutoSizing)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(child->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		auto layout = makeView(registry, { child });

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

	TEST_F(TestView, MainAxisAlignmentCenter)
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

		auto layout =
			makeView(registry, { firstChild, secondChild }, 16.0f, 8.0f,
					 View::Axis::Horizontal, View::MainAxisAlignment::Center);
		layout->setFixedWidth(200.0f);

		// Content region = 200 - 2 * padding(16) = 168.
		// Content = 10 + 10 + spacing(8) = 28.
		// Offset = 16 + (168 - 28) / 2 = 86.
		EXPECT_FLOAT_EQ(childX(registry, firstChild), 86.0f);
		EXPECT_FLOAT_EQ(childX(registry, secondChild), 86.0f + 18.0f);
	}

	TEST_F(TestView, MainAxisAlignmentEnd)
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

		auto layout =
			makeView(registry, { firstChild, secondChild }, 16.0f, 8.0f,
					 View::Axis::Horizontal, View::MainAxisAlignment::End);
		layout->setFixedWidth(200.0f);

		// Offset = 16 + (168 - 28) = 156.
		EXPECT_FLOAT_EQ(childX(registry, firstChild), 156.0f);
		EXPECT_FLOAT_EQ(childX(registry, secondChild), 174.0f);
	}

	TEST_F(TestView, MainAxisAlignmentSpaceBetween)
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

		auto layout = makeView(registry, { firstChild, secondChild }, 16.0f,
							   8.0f, View::Axis::Horizontal,
							   View::MainAxisAlignment::SpaceBetween);
		layout->setFixedWidth(200.0f);

		// Region = 168, raw = 20, gap = (168 - 20) / 1 = 148.
		EXPECT_FLOAT_EQ(childX(registry, firstChild), 16.0f);
		EXPECT_FLOAT_EQ(childX(registry, secondChild), 16.0f + 10.0f + 148.0f);
	}

	TEST_F(TestView, CrossAxisAlignmentStart)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(child->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		auto layout = makeView(
			registry, { child }, 16.0f, 8.0f, View::Axis::Horizontal,
			View::MainAxisAlignment::Start, View::CrossAxisAlignment::Start);

		// 16 dp down from the top edge (y points up).
		EXPECT_FLOAT_EQ(childY(registry, child), -16.0f);
	}

	TEST_F(TestView, CrossAxisAlignmentEnd)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(child->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		auto layout = makeView(
			registry, { child }, 16.0f, 8.0f, View::Axis::Horizontal,
			View::MainAxisAlignment::Start, View::CrossAxisAlignment::End);
		layout->setFixedHeight(100.0f);

		// Cross region = 100 - 32 = 68. Offset = 16 + 68 - 10 = 74, measured
		// down from the top edge (y points up).
		EXPECT_FLOAT_EQ(childY(registry, child), -74.0f);
	}

	TEST_F(TestView, LayoutComponentSettersMarkChanged)
	{
		ecs::ComponentRegistry registry;

		auto layout = makeView(registry, {});

		auto &component =
			registry.getComponent<components::Layout>(layout->getIdentifier());

		component.setHasChanged(false);
		component.setSpacing(3.0f);
		EXPECT_TRUE(component.hasChanged());
	}

	TEST_F(TestView, ComponentsWrittenByIdentifierSurviveAnUpdate)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		registry.getComponent<components::Bound>(child->getIdentifier())
			.setWidth(10.0f)
			.setHeight(10.0f);

		auto layout = makeView(registry, { child });

		const utility::graphic::Color32Bit color(1, 2, 3, 4);
		registry.getComponent<components::Color>(layout->getIdentifier())
			.setColor(color);
		registry.getComponent<components::Layout>(layout->getIdentifier())
			.setFixedWidth(300.0f);
		systems::Layout::apply(
			registry, layout->getIdentifier(),
			registry.getComponent<components::Layout>(layout->getIdentifier())
				.getChildren(),
			systems::Layout::layerOf(registry, layout->getIdentifier()));

		EXPECT_EQ(
			registry.getComponent<components::Color>(layout->getIdentifier())
				.getColor(),
			color);
		EXPECT_TRUE(
			registry.getComponent<components::Layout>(layout->getIdentifier())
				.hasFixedWidth());
		EXPECT_FLOAT_EQ(
			registry.getComponent<components::Bound>(layout->getIdentifier())
				.getWidth(),
			300.0f);
	}

	TEST_F(TestView, ConstructorWritesTheConfigurationToComponents)
	{
		ecs::ComponentRegistry registry;

		auto child = std::make_shared<TestChild>(registry);

		auto layout = makeView(
			registry, { child }, 4.0f, 2.0f, View::Axis::Vertical,
			View::MainAxisAlignment::End, View::CrossAxisAlignment::Start);

		const auto &component =
			registry.getComponent<components::Layout>(layout->getIdentifier());

		EXPECT_EQ(component.getAxis(), View::Axis::Vertical);
		EXPECT_EQ(component.getMainAxisAlignment(),
				  View::MainAxisAlignment::End);
		EXPECT_EQ(component.getCrossAxisAlignment(),
				  View::CrossAxisAlignment::Start);
		EXPECT_FLOAT_EQ(component.getPadding(), 4.0f);
		EXPECT_FLOAT_EQ(component.getSpacing(), 2.0f);
		EXPECT_FALSE(component.hasFixedWidth());
		ASSERT_EQ(component.getChildren().size(), 1U);
		EXPECT_EQ(component.getChildren().front(), child->getIdentifier());
		EXPECT_FLOAT_EQ(
			registry.getComponent<components::Borders>(layout->getIdentifier())
				.getTopLeftRadius(),
			16.0f);
	}

	TEST_F(TestView, AttachedEntitiesAreParentedButNotOwned)
	{
		ecs::ComponentRegistry registry;

		auto first	= std::make_shared<TestChild>(registry);
		auto second = std::make_shared<TestChild>(registry);

		auto layout = makeView(registry, { first });

		layout->setEntities(
			std::vector<std::shared_ptr<ecs::Entity>> { first, second });

		EXPECT_EQ(first->getParent(), layout);
		EXPECT_EQ(second->getParent(), layout);
		// The view keeps no handle and does not register what is attached.
		EXPECT_TRUE(layout->getEntities().empty());
		EXPECT_EQ(first.use_count(), 1);
		EXPECT_EQ(second.use_count(), 1);
	}

}	 // namespace guillaume::entities::tests
