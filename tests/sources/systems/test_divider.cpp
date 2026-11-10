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

#include "systems/test_divider.hpp"

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

namespace guillaume::systems::tests
{
	using Variant = components::Divider::Variant;

	TEST_F(TestDividerSystem, RunsInTheMeasurePhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Measure);
	}

	TEST_F(TestDividerSystem, FullWidthSpansTheWholeLength)
	{
		config().setLength(200.0f).setThickness(2.0f);

		_system.update(_entity);

		EXPECT_FLOAT_EQ(bound().getWidth(), 200.0f);
		EXPECT_FLOAT_EQ(bound().getHeight(), 2.0f);
		EXPECT_FLOAT_EQ(positionX(), 0.0f);
	}

	TEST_F(TestDividerSystem, InsetShiftsAndShortensTheLeadingEdge)
	{
		config().setLength(200.0f).setInset(16.0f).setVariant(Variant::Inset);

		_system.update(_entity);

		EXPECT_FLOAT_EQ(bound().getWidth(), 184.0f);
		EXPECT_FLOAT_EQ(positionX(), 16.0f);
	}

	TEST_F(TestDividerSystem, MiddleInsetsBothEdges)
	{
		config().setLength(200.0f).setInset(16.0f).setVariant(Variant::Middle);

		_system.update(_entity);

		EXPECT_FLOAT_EQ(bound().getWidth(), 168.0f);
		EXPECT_FLOAT_EQ(positionX(), 16.0f);
	}

	TEST_F(TestDividerSystem, WidthNeverGoesNegative)
	{
		config().setLength(10.0f).setInset(16.0f).setVariant(Variant::Middle);

		_system.update(_entity);

		EXPECT_FLOAT_EQ(bound().getWidth(), 0.0f);
	}

	TEST_F(TestDividerSystem, WritesThicknessAndColor)
	{
		const utility::graphic::Color32Bit color(10, 20, 30, 255);
		config()
			.setThickness(components::Divider::BoldThickness)
			.setColor(color);

		_system.update(_entity);

		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Line>(_entity).getThickness(),
			components::Divider::BoldThickness);
		EXPECT_EQ(_registry.getComponent<components::Color>(_entity)
					  .getColor()
					  .getBlue(),
				  30);
	}

	TEST_F(TestDividerSystem, ConfigurationChangeIsAppliedOnTheNextUpdate)
	{
		config().setLength(200.0f);
		_system.update(_entity);

		config().setVariant(Variant::Inset);
		_system.update(_entity);

		EXPECT_FLOAT_EQ(bound().getWidth(), 184.0f);
		EXPECT_FLOAT_EQ(positionX(), 16.0f);

		config().setVariant(Variant::FullWidth);
		_system.update(_entity);

		EXPECT_FLOAT_EQ(bound().getWidth(), 200.0f);
		EXPECT_FLOAT_EQ(positionX(), 0.0f);
	}

	TEST_F(TestDividerSystem, KeepsThePositionGivenByAnExternalMove)
	{
		config().setLength(200.0f).setVariant(Variant::Inset);
		_system.update(_entity);

		// A layout or the scene places the entity somewhere else.
		utility::graphic::PoseF moved;
		moved.setPosition(utility::graphic::PositionF(120.0f, 40.0f, 0.0f));
		_registry.getComponent<components::Transform>(_entity).setPose(moved);

		_system.update(_entity);

		// The divider stays where it was put, and keeps its size.
		EXPECT_FLOAT_EQ(positionX(), 120.0f);
		EXPECT_FLOAT_EQ(bound().getWidth(), 184.0f);

		// And it stays there on later frames.
		_system.update(_entity);

		EXPECT_FLOAT_EQ(positionX(), 120.0f);
	}

	TEST_F(TestDividerSystem, SteadyStateDoesNotMarkComponentsChanged)
	{
		config().setLength(200.0f).setVariant(Variant::Inset);
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
	}

	TEST_F(TestDividerSystem, SkipsEntityMissingARequiredComponent)
	{
		const ecs::Entity::Identifier incomplete { 2 };
		_registry.addComponent<components::Divider>(incomplete);

		EXPECT_NO_THROW(_system.update(incomplete));
	}
}	 // namespace guillaume::systems::tests
