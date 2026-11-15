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

#include "systems/test_progress_indicator.hpp"

#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Variant = components::ProgressIndicator::Variant;

		constexpr float Pi = 3.14159265358979323846f;
	}	 // namespace

	TEST_F(TestProgressIndicatorSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestProgressIndicatorSystem, InstallDrawsTheCurrentValueAtOnce)
	{
		EXPECT_FLOAT_EQ(state().getDisplayed(), 0.5f);
		EXPECT_FALSE(
			_registry.getComponent<components::Animation>(_entity).isPlaying());
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_entity)
					  .getRole(),
				  components::AccessibilityRole::ProgressBar);
	}

	TEST_F(TestProgressIndicatorSystem, LinearFillSpansTheValue)
	{
		_system.update(_entity);

		const auto &bound = _registry.getComponent<components::Bound>(_entity);
		const auto &track = _registry.getComponent<components::Bound>(_track);
		const auto &fill  = _registry.getComponent<components::Bound>(_fill);
		EXPECT_FLOAT_EQ(bound.getWidth(), 200.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 4.0f);
		EXPECT_FLOAT_EQ(track.getWidth(), 200.0f);
		EXPECT_FLOAT_EQ(track.getHeight(), 4.0f);
		EXPECT_FLOAT_EQ(fill.getWidth(), 100.0f);
		EXPECT_FLOAT_EQ(fill.getHeight(), 4.0f);
		EXPECT_FLOAT_EQ(offsetX(_fill), 0.0f);
	}

	TEST_F(TestProgressIndicatorSystem, TrackLengthSizesTheBar)
	{
		state().setTrackLength(120.0f);

		_system.update(_entity);

		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getWidth(),
			120.0f);
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_fill).getWidth(), 60.0f);
	}

	TEST_F(TestProgressIndicatorSystem, CircularArcSpansTheValue)
	{
		state().setVariant(Variant::CircularDeterminate);

		_system.update(_entity);

		const auto &bound = _registry.getComponent<components::Bound>(_entity);
		const auto &track = _registry.getComponent<components::Bound>(_track);
		const auto &fill  = _registry.getComponent<components::Bound>(_fill);
		const auto &arc	  = _registry.getComponent<components::Arc>(_fill);
		EXPECT_FLOAT_EQ(bound.getWidth(), 48.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 48.0f);
		EXPECT_FLOAT_EQ(track.getWidth(), 48.0f);
		EXPECT_FLOAT_EQ(fill.getHeight(), 48.0f);
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Ring>(_track).getThickness(),
			4.0f);
		EXPECT_FLOAT_EQ(arc.getThickness(), 4.0f);
		EXPECT_FLOAT_EQ(arc.getStartAngle(), Pi / 2.0f);
		EXPECT_FLOAT_EQ(arc.getSweepAngle(), -Pi);
	}

	TEST_F(TestProgressIndicatorSystem, ValueChangesAreAnimated)
	{
		auto &animation =
			_registry.getComponent<components::Animation>(_entity);
		const auto &fill = _registry.getComponent<components::Bound>(_fill);

		guillaume::systems::ProgressIndicator::setValue(_registry, _entity,
														3.0f);

		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Value>(_entity).getValue(),
			1.0f);
		EXPECT_TRUE(animation.isPlaying());
		EXPECT_FLOAT_EQ(animation.getDuration(),
						getMotionDuration(theme::MotionDuration::Short4));
		EXPECT_FLOAT_EQ(state().getDisplayed(), 0.5f);
		EXPECT_FLOAT_EQ(fill.getWidth(), 100.0f);

		animation.advance(1.0f);

		EXPECT_FLOAT_EQ(state().getDisplayed(), 1.0f);
		EXPECT_FLOAT_EQ(fill.getWidth(), 200.0f);
	}

	TEST_F(TestProgressIndicatorSystem, DeterminateExposesAPercentage)
	{
		_system.update(_entity);

		const auto &value =
			_registry.getComponent<components::Accessibility>(_entity)
				.getValue();
		ASSERT_TRUE(value.has_value());
		EXPECT_FLOAT_EQ(value->min, 0.0f);
		EXPECT_FLOAT_EQ(value->max, 100.0f);
		EXPECT_FLOAT_EQ(value->now, 50.0f);
	}

	TEST_F(TestProgressIndicatorSystem, IndeterminateLinearSlidesASegment)
	{
		auto &animation = _registry.getComponent<components::Animation>(_fill);
		state().setVariant(Variant::LinearIndeterminate);
		guillaume::systems::ProgressIndicator::installChildren(_registry,
															   _entity);

		_system.update(_entity);

		EXPECT_TRUE(animation.isLooping());
		EXPECT_TRUE(animation.isPlaying());
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_fill).getWidth(),
			200.0f * components::ProgressIndicator::IndeterminateSegment);
		EXPECT_FLOAT_EQ(offsetX(_fill), 0.0f);
		EXPECT_FALSE(_registry.getComponent<components::Accessibility>(_entity)
						 .getValue()
						 .has_value());

		animation.advance(0.75f);

		EXPECT_GT(offsetX(_fill), 0.0f);
		EXPECT_FLOAT_EQ(offsetX(_fill), state().getCycle() * 80.0f);
	}

	TEST_F(TestProgressIndicatorSystem, IndeterminateCircularRotatesAQuarter)
	{
		auto &animation = _registry.getComponent<components::Animation>(_fill);
		const auto &arc = _registry.getComponent<components::Arc>(_fill);
		state().setVariant(Variant::CircularIndeterminate);
		guillaume::systems::ProgressIndicator::installChildren(_registry,
															   _entity);

		_system.update(_entity);

		EXPECT_FLOAT_EQ(arc.getStartAngle(), Pi / 2.0f);
		EXPECT_FLOAT_EQ(arc.getSweepAngle(), -Pi / 2.0f);

		animation.advance(0.75f);

		EXPECT_GT(state().getCycle(), 0.0f);
		EXPECT_FLOAT_EQ(arc.getStartAngle(),
						Pi / 2.0f - state().getCycle() * 2.0f * Pi);
		EXPECT_FLOAT_EQ(arc.getSweepAngle(), -Pi / 2.0f);
	}

	TEST_F(TestProgressIndicatorSystem, DeterminateFillIsNotAnimated)
	{
		guillaume::systems::ProgressIndicator::installChildren(_registry,
															   _entity);

		EXPECT_FALSE(
			_registry.getComponent<components::Animation>(_fill).isPlaying());
	}

	TEST_F(TestProgressIndicatorSystem, HidingFadesTrackAndFill)
	{
		auto &animation =
			_registry.getComponent<components::Animation>(_entity);
		const auto &track = _registry.getComponent<components::Color>(_track);
		const auto &fill  = _registry.getComponent<components::Color>(_fill);
		_system.update(_entity);
		EXPECT_EQ(track.getColor(),
				  getActiveColor(SchemeColorRole::SurfaceVariant));
		EXPECT_EQ(fill.getColor(), getActiveColor(SchemeColorRole::Primary));

		guillaume::systems::ProgressIndicator::setVisible(_registry, _entity,
														  false);
		EXPECT_TRUE(animation.isPlaying());
		animation.advance(1.0f);

		EXPECT_FLOAT_EQ(state().getOpacity(), 0.0f);
		EXPECT_EQ(track.getColor().getAlpha(), 0);
		EXPECT_EQ(fill.getColor().getAlpha(), 0);
		EXPECT_FLOAT_EQ(state().getDisplayed(), 0.5f);

		guillaume::systems::ProgressIndicator::setVisible(_registry, _entity,
														  true);
		animation.advance(1.0f);

		EXPECT_FLOAT_EQ(state().getOpacity(), 1.0f);
		EXPECT_EQ(fill.getColor(), getActiveColor(SchemeColorRole::Primary));
	}

	TEST_F(TestProgressIndicatorSystem, ColorOverrideReachesTheFill)
	{
		const auto tertiary = getActiveColor(SchemeColorRole::Tertiary);

		state().setIndicatorColor(tertiary);
		_system.update(_entity);

		EXPECT_EQ(guillaume::systems::ProgressIndicator::indicatorColor(
					  _registry, _entity),
				  tertiary);
		EXPECT_EQ(_registry.getComponent<components::Color>(_fill).getColor(),
				  tertiary);
		EXPECT_EQ(_registry.getComponent<components::Color>(_track).getColor(),
				  getActiveColor(SchemeColorRole::SurfaceVariant));
	}

	TEST_F(TestProgressIndicatorSystem, AnUnchangedIndicatorIsNotMarkedChanged)
	{
		for (const auto variant:
			 { Variant::LinearDeterminate, Variant::CircularDeterminate }) {
			state().setVariant(variant);
			_system.update(_entity);
			_registry.resetChangedFlags();

			_system.update(_entity);

			EXPECT_FALSE(_registry.hasChanged(_entity));
			EXPECT_FALSE(_registry.hasChanged(_track));
			EXPECT_FALSE(_registry.hasChanged(_fill));
		}
	}

}	 // namespace guillaume::systems::tests
