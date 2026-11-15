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

#include "systems/test_badge.hpp"

#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Variant = components::Badge::Variant;
	}	 // namespace

	TEST_F(TestBadgeSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestBadgeSystem, VariantSetsTheHeight)
	{
		_system.update(_entity);
		EXPECT_FLOAT_EQ(bound().getHeight(), components::Badge::LargeSize);

		badge().setVariant(Variant::Small);
		_system.update(_entity);
		EXPECT_FLOAT_EQ(bound().getHeight(), components::Badge::SmallSize);

		badge().setVariant(Variant::Dot);
		_system.update(_entity);
		EXPECT_FLOAT_EQ(bound().getHeight(), components::Badge::DotSize);
		EXPECT_FLOAT_EQ(guillaume::systems::Badge::sizeOf(_registry, _entity),
						components::Badge::DotSize);
	}

	TEST_F(TestBadgeSystem, SizeOverrideWinsOverTheVariant)
	{
		badge().setSize(28.0f);
		_system.update(_entity);
		EXPECT_FLOAT_EQ(bound().getHeight(), 28.0f);
		EXPECT_FLOAT_EQ(bound().getWidth(), 28.0f);

		badge().setSize(std::nullopt);
		_system.update(_entity);
		EXPECT_FLOAT_EQ(bound().getHeight(), components::Badge::LargeSize);
	}

	TEST_F(TestBadgeSystem, WideLabelGrowsTheBadgeIntoAPill)
	{
		badge().setVariant(Variant::Small);
		_registry.getComponent<components::Bound>(_label).setWidth(30.0f);

		_system.update(_entity);

		EXPECT_FLOAT_EQ(bound().getWidth(),
						30.0f + 2.0f * components::Badge::HorizontalPadding);
		EXPECT_FLOAT_EQ(bound().getHeight(), components::Badge::SmallSize);
	}

	TEST_F(TestBadgeSystem, DotIgnoresTheLabelWidth)
	{
		badge().setVariant(Variant::Dot);
		_registry.getComponent<components::Bound>(_label).setWidth(30.0f);

		_system.update(_entity);

		EXPECT_FLOAT_EQ(bound().getWidth(), components::Badge::DotSize);
	}

	TEST_F(TestBadgeSystem, ContainerIsErrorColoredAndFullyRounded)
	{
		_system.update(_entity);

		EXPECT_EQ(_registry.getComponent<components::Color>(_entity).getColor(),
				  getActiveColor(SchemeColorRole::Error));
		EXPECT_EQ(_registry.getComponent<components::Color>(_label).getColor(),
				  getActiveColor(SchemeColorRole::OnError));
		EXPECT_FLOAT_EQ(_registry.getComponent<components::Borders>(_entity)
							.getTopLeftRadius(),
						getShapeRadius(theme::ShapeRole::Full));
	}

	TEST_F(TestBadgeSystem, ColorOverridesReplaceTheSchemeColors)
	{
		const auto container = getActiveColor(SchemeColorRole::Tertiary);
		const auto content	 = getActiveColor(SchemeColorRole::OnTertiary);
		badge().setContainerColor(container).setContentColor(content);

		_system.update(_entity);

		EXPECT_EQ(_registry.getComponent<components::Color>(_entity).getColor(),
				  container);
		EXPECT_EQ(_registry.getComponent<components::Color>(_label).getColor(),
				  content);
	}

	TEST_F(TestBadgeSystem, LabelGetsItsContentAndHalfTheBadgeSize)
	{
		badge().setLabel("42");

		_system.update(_entity);

		const auto &text = _registry.getComponent<components::Text>(_label);
		EXPECT_EQ(text.getContent(), "42");
		EXPECT_FLOAT_EQ(text.getFontSize(),
						components::Badge::LargeSize
							* components::Badge::LabelScale);
	}

	TEST_F(TestBadgeSystem, OnlyLabelledNonDotBadgesNeedALabel)
	{
		EXPECT_TRUE(guillaume::systems::Badge::needsLabel(_registry, _entity));

		badge().setVariant(Variant::Dot);
		EXPECT_FALSE(guillaume::systems::Badge::needsLabel(_registry, _entity));

		badge().setVariant(Variant::Small).setLabel("");
		EXPECT_FALSE(guillaume::systems::Badge::needsLabel(_registry, _entity));
	}

	TEST_F(TestBadgeSystem, SetVisibleFadesThroughTheAnimation)
	{
		_system.update(_entity);
		auto &animation =
			_registry.getComponent<components::Animation>(_entity);
		EXPECT_FLOAT_EQ(animation.getDuration(),
						getMotionDuration(theme::MotionDuration::Short3));

		guillaume::systems::Badge::setVisible(_registry, _entity, false);
		EXPECT_FALSE(badge().isVisible());
		EXPECT_TRUE(animation.isPlaying());
		EXPECT_FLOAT_EQ(badge().getOpacity(), 1.0f);

		animation.advance(1.0f);
		EXPECT_FLOAT_EQ(badge().getOpacity(), 0.0f);
		EXPECT_EQ(alphaOf(_entity), 0);
		EXPECT_EQ(alphaOf(_label), 0);

		guillaume::systems::Badge::setVisible(_registry, _entity, true);
		animation.advance(1.0f);
		EXPECT_FLOAT_EQ(badge().getOpacity(), 1.0f);
		EXPECT_EQ(alphaOf(_entity), 255);
	}

	TEST_F(TestBadgeSystem, FadeStepsInterpolateFromTheStartingOpacity)
	{
		guillaume::systems::Badge::setVisible(_registry, _entity, false);
		guillaume::systems::Badge::fade(_registry, _entity, 0.5f);
		EXPECT_FLOAT_EQ(badge().getOpacity(), 0.5f);
		EXPECT_EQ(alphaOf(_entity), 128);

		// Reversing mid-fade starts from the opacity reached so far.
		guillaume::systems::Badge::setVisible(_registry, _entity, true);
		EXPECT_FLOAT_EQ(badge().getFadeFrom(), 0.5f);
		guillaume::systems::Badge::fade(_registry, _entity, 0.5f);
		EXPECT_FLOAT_EQ(badge().getOpacity(), 0.75f);
	}

	TEST_F(TestBadgeSystem, SettingTheSameVisibilityDoesNotStartAFade)
	{
		guillaume::systems::Badge::setVisible(_registry, _entity, true);

		EXPECT_FALSE(
			_registry.getComponent<components::Animation>(_entity).isPlaying());
	}

	TEST_F(TestBadgeSystem, AnUnchangedBadgeIsNotMarkedChanged)
	{
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
		EXPECT_FALSE(_registry.hasChanged(_label));
	}

}	 // namespace guillaume::systems::tests
