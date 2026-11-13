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
#include "systems/test_view.hpp"

namespace guillaume::systems::tests
{
	namespace
	{
		using Axis				 = components::Layout::Axis;
		using MainAxisAlignment	 = components::Layout::MainAxisAlignment;
		using CrossAxisAlignment = components::Layout::CrossAxisAlignment;
	}	 // namespace

	TEST_F(TestViewSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestViewSystem, RowSizesTheViewFromItsChildren)
	{
		_system.update(_view);

		// Width: 50 + 20 + spacing(8) + 2 * padding(16) = 110.
		// Height: max(30, 60) + 2 * padding(16) = 92.
		EXPECT_FLOAT_EQ(bound().getWidth(), 110.0f);
		EXPECT_FLOAT_EQ(bound().getHeight(), 92.0f);
	}

	TEST_F(TestViewSystem, RowPlacesChildrenWithPaddingAndSpacing)
	{
		_system.update(_view);

		// First at padding(16), second at 16 + 50 + spacing(8) = 74.
		EXPECT_FLOAT_EQ(offsetX(_first), 16.0f);
		EXPECT_FLOAT_EQ(offsetX(_second), 74.0f);

		// Cross axis centered in a 60 high region: 16 + (60 - 30) / 2 = 31,
		// measured down from the top edge (y points up).
		EXPECT_FLOAT_EQ(offsetY(_first), -31.0f);
		EXPECT_FLOAT_EQ(offsetY(_second), -16.0f);
	}

	TEST_F(TestViewSystem, ColumnSizesAndPlacesChildren)
	{
		layout().setAxis(Axis::Vertical);

		_system.update(_view);

		// Width: max(50, 20) + 2 * padding(16) = 82.
		// Height: 30 + 60 + spacing(8) + 2 * padding(16) = 130.
		EXPECT_FLOAT_EQ(bound().getWidth(), 82.0f);
		EXPECT_FLOAT_EQ(bound().getHeight(), 130.0f);

		// Stacked down from the top edge (y points up).
		EXPECT_FLOAT_EQ(offsetY(_first), -16.0f);
		EXPECT_FLOAT_EQ(offsetY(_second), -54.0f);

		// Cross axis centered in a 50 wide region: 16 + (50 - 20) / 2 = 31.
		EXPECT_FLOAT_EQ(offsetX(_first), 16.0f);
		EXPECT_FLOAT_EQ(offsetX(_second), 31.0f);
	}

	TEST_F(TestViewSystem, CrossAxisAlignmentMovesChildren)
	{
		// Offsets are measured down from the top edge (y points up).
		layout().setCrossAxisAlignment(CrossAxisAlignment::Start);
		_system.update(_view);
		EXPECT_FLOAT_EQ(offsetY(_first), -16.0f);

		layout().setCrossAxisAlignment(CrossAxisAlignment::End);
		_system.update(_view);
		// Padding(16) + region(60) - height(30) = 46.
		EXPECT_FLOAT_EQ(offsetY(_first), -46.0f);
	}

	TEST_F(TestViewSystem, FixedSizeOverridesAutoSizing)
	{
		layout().setFixedWidth(200.0f).setFixedHeight(100.0f);

		_system.update(_view);

		EXPECT_FLOAT_EQ(bound().getWidth(), 200.0f);
		EXPECT_FLOAT_EQ(bound().getHeight(), 100.0f);

		layout().clearFixedWidth().clearFixedHeight();

		_system.update(_view);

		EXPECT_FLOAT_EQ(bound().getWidth(), 110.0f);
		EXPECT_FLOAT_EQ(bound().getHeight(), 92.0f);
	}

	TEST_F(TestViewSystem, MainAxisAlignmentUsesTheFixedSize)
	{
		layout().setFixedWidth(200.0f);

		// Region: 200 - 2 * 16 = 168, content: 50 + 8 + 20 = 78.
		layout().setMainAxisAlignment(MainAxisAlignment::Center);
		_system.update(_view);
		EXPECT_FLOAT_EQ(offsetX(_first), 16.0f + (168.0f - 78.0f) / 2.0f);

		layout().setMainAxisAlignment(MainAxisAlignment::End);
		_system.update(_view);
		EXPECT_FLOAT_EQ(offsetX(_first), 16.0f + 168.0f - 78.0f);
		EXPECT_FLOAT_EQ(offsetX(_second), 200.0f - 16.0f - 20.0f);

		layout().setMainAxisAlignment(MainAxisAlignment::SpaceBetween);
		_system.update(_view);
		EXPECT_FLOAT_EQ(offsetX(_first), 16.0f);
		EXPECT_FLOAT_EQ(offsetX(_second), 200.0f - 16.0f - 20.0f);
	}

	TEST_F(TestViewSystem, AnEmptyViewKeepsItsPaddingSize)
	{
		layout().clearChildren();

		_system.update(_view);

		EXPECT_FLOAT_EQ(bound().getWidth(), 32.0f);
		EXPECT_FLOAT_EQ(bound().getHeight(), 32.0f);
	}

	TEST_F(TestViewSystem, OnlyLayoutChildrenAreArranged)
	{
		layout().setChildren({ _second });

		_system.update(_view);

		// The first child is not a layout child: it keeps its pose and does
		// not count in the size.
		EXPECT_FLOAT_EQ(position(_first).getX(), 0.0f);
		EXPECT_FLOAT_EQ(position(_first).getY(), 0.0f);
		EXPECT_FLOAT_EQ(offsetX(_second), 16.0f);
		EXPECT_FLOAT_EQ(bound().getWidth(), 52.0f);
	}

	TEST_F(TestViewSystem, AChildWithoutGeometryIsSkipped)
	{
		const ecs::Entity::Identifier missing { 4 };
		layout().setChildren({ missing, _first, _second });

		_system.update(_view);

		EXPECT_FLOAT_EQ(offsetX(_first), 16.0f);
		EXPECT_FLOAT_EQ(bound().getWidth(), 110.0f);
	}

	TEST_F(TestViewSystem, ChildrenAreLiftedInFrontOfTheView)
	{
		_system.update(_view);

		EXPECT_FLOAT_EQ(offsetZ(_first), 1.0f);
		EXPECT_FLOAT_EQ(offsetZ(_second), 1.0f);

		// Repeated updates do not stack the offset.
		_system.update(_view);

		EXPECT_FLOAT_EQ(offsetZ(_first), 1.0f);
	}

	TEST_F(TestViewSystem, TheLiftFollowsTheHierarchyLayer)
	{
		_registry.getComponent<components::Hierarchy>(_view).setLayer(2);

		_system.update(_view);

		EXPECT_EQ(guillaume::systems::Layout::layerOf(_registry, _view), 2U);
		EXPECT_FLOAT_EQ(offsetZ(_first), 3.0f);
	}

	TEST_F(TestViewSystem, ChildrenFollowAPoseWrittenByIdentifier)
	{
		_system.update(_view);

		_registry.getComponent<components::Transform>(_view).setPose(
			utility::graphic::PoseF(
				utility::graphic::PositionF(10.0f, 20.0f, -200.0f),
				utility::graphic::OrientationF()));

		_system.update(_view);

		// 16 right of x = 10 and 31 down from y = 20 (y points up).
		EXPECT_FLOAT_EQ(position(_first).getX(), 26.0f);
		EXPECT_FLOAT_EQ(position(_first).getY(), -11.0f);
		EXPECT_FLOAT_EQ(position(_first).getZ(), -199.0f);
	}

	TEST_F(TestViewSystem, AChildResizeIsPickedUpOnTheNextUpdate)
	{
		_system.update(_view);

		_registry.getComponent<components::Bound>(_first).setWidth(100.0f);

		_system.update(_view);

		EXPECT_FLOAT_EQ(bound().getWidth(), 160.0f);
		EXPECT_FLOAT_EQ(offsetX(_second), 124.0f);
	}

	TEST_F(TestViewSystem, ComponentsWrittenByIdentifierAreNotOverwritten)
	{
		const utility::graphic::Color32Bit color(1, 2, 3, 4);

		_system.update(_view);

		_registry.getComponent<components::Color>(_view).setColor(color);
		_registry.getComponent<components::Borders>(_view).setBorderRadius(
			7.0f);
		layout().setFixedWidth(300.0f).setPadding(4.0f);

		_system.update(_view);
		_system.update(_view);

		EXPECT_EQ(_registry.getComponent<components::Color>(_view).getColor(),
				  color);
		EXPECT_FLOAT_EQ(_registry.getComponent<components::Borders>(_view)
							.getTopLeftRadius(),
						7.0f);
		EXPECT_TRUE(layout().hasFixedWidth());
		EXPECT_FLOAT_EQ(layout().getFixedWidth(), 300.0f);
		EXPECT_FLOAT_EQ(layout().getPadding(), 4.0f);
		EXPECT_FLOAT_EQ(bound().getWidth(), 300.0f);
		EXPECT_FLOAT_EQ(offsetX(_first), 4.0f);
	}

	TEST_F(TestViewSystem, AnUnchangedViewIsNotMarkedChanged)
	{
		_system.update(_view);
		_registry.resetChangedFlags();

		_system.update(_view);

		EXPECT_FALSE(_registry.hasChanged(_view));
		EXPECT_FALSE(_registry.hasChanged(_first));
		EXPECT_FALSE(_registry.hasChanged(_second));
	}

}	 // namespace guillaume::systems::tests
