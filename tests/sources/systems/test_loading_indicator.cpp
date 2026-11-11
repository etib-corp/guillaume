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

#include "systems/test_loading_indicator.hpp"

#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Variant = components::LoadingIndicator::Variant;

		constexpr float Pi = 3.14159265358979323846f;
	}	 // namespace

	TEST_F(TestLoadingIndicatorSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestLoadingIndicatorSystem, InstallSetsTheProgressBarRole)
	{
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_entity)
					  .getRole(),
				  components::AccessibilityRole::ProgressBar);
		EXPECT_FLOAT_EQ(_registry.getComponent<components::Animation>(_entity)
							.getDuration(),
						getMotionDuration(theme::MotionDuration::Short4));
	}

	TEST_F(TestLoadingIndicatorSystem, CircularIsASquareHoldingAnArc)
	{
		_system.update(_entity);

		const auto &bound = _registry.getComponent<components::Bound>(_entity);
		const auto &child = _registry.getComponent<components::Bound>(_child);
		const auto &arc	  = _registry.getComponent<components::Arc>(_child);
		EXPECT_FLOAT_EQ(bound.getWidth(), 48.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 48.0f);
		EXPECT_FLOAT_EQ(child.getWidth(), 48.0f);
		EXPECT_FLOAT_EQ(child.getHeight(), 48.0f);
		EXPECT_FLOAT_EQ(arc.getThickness(), 4.0f);
		EXPECT_FLOAT_EQ(arc.getStartAngle(), -Pi / 2.0f);
		EXPECT_FLOAT_EQ(arc.getSweepAngle(), 1.5f * Pi);
		EXPECT_FLOAT_EQ(offsetX(), 0.0f);
	}

	TEST_F(TestLoadingIndicatorSystem, SizeScalesTheCircleAndItsStroke)
	{
		state().setSize(24.0f);

		_system.update(_entity);

		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getWidth(),
			24.0f);
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_child).getHeight(),
			24.0f);
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Arc>(_child).getThickness(),
			2.0f);
		EXPECT_FLOAT_EQ(guillaume::systems::LoadingIndicator::strokeWidth(
							_registry, _entity),
						2.0f);
	}

	TEST_F(TestLoadingIndicatorSystem, ArcRotatesWithTheAnimation)
	{
		_registry.getComponent<components::Animation>(_child).advance(0.75f);

		const float progress = state().getProgress();
		EXPECT_GT(progress, 0.0f);
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Arc>(_child).getStartAngle(),
			-Pi / 2.0f + progress * 2.0f * Pi);
	}

	TEST_F(TestLoadingIndicatorSystem, LinearIsABarHoldingASlidingSegment)
	{
		state().setVariant(Variant::Linear);

		_system.update(_entity);

		const auto &bound = _registry.getComponent<components::Bound>(_entity);
		const auto &child = _registry.getComponent<components::Bound>(_child);
		EXPECT_FLOAT_EQ(bound.getWidth(), 200.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 4.0f);
		EXPECT_FLOAT_EQ(child.getWidth(), 80.0f);
		EXPECT_FLOAT_EQ(child.getHeight(), 4.0f);
		EXPECT_FLOAT_EQ(offsetX(), 0.0f);

		_registry.getComponent<components::Animation>(_child).advance(0.75f);

		EXPECT_GT(offsetX(), 0.0f);
		EXPECT_FLOAT_EQ(offsetX(), state().getProgress() * 120.0f);
	}

	TEST_F(TestLoadingIndicatorSystem, TheWidgetItselfIsTransparent)
	{
		_registry.getComponent<components::Color>(_entity).setColor(
			getActiveColor(SchemeColorRole::Primary));

		_system.update(_entity);

		EXPECT_EQ(_registry.getComponent<components::Color>(_entity)
					  .getColor()
					  .getAlpha(),
				  0);
	}

	TEST_F(TestLoadingIndicatorSystem, InstallStartsALoopingAnimation)
	{
		const auto &animation =
			_registry.getComponent<components::Animation>(_child);

		EXPECT_TRUE(animation.isLooping());
		EXPECT_FLOAT_EQ(animation.getDuration(),
						theme::MotionScale::IndeterminateCycle);
		EXPECT_TRUE(guillaume::systems::LoadingIndicator::isRunning(_registry,
																	_entity));
	}

	TEST_F(TestLoadingIndicatorSystem, StopAndStartControlTheAnimation)
	{
		guillaume::systems::LoadingIndicator::stop(_registry, _entity);
		EXPECT_FALSE(guillaume::systems::LoadingIndicator::isRunning(_registry,
																	 _entity));

		guillaume::systems::LoadingIndicator::start(_registry, _entity);
		EXPECT_TRUE(guillaume::systems::LoadingIndicator::isRunning(_registry,
																	_entity));
	}

	TEST_F(TestLoadingIndicatorSystem, StoppingFadesTheIndicatorOut)
	{
		auto &fade = _registry.getComponent<components::Animation>(_entity);
		const auto &color = _registry.getComponent<components::Color>(_child);
		_system.update(_entity);
		EXPECT_EQ(color.getColor(), getActiveColor(SchemeColorRole::Primary));

		guillaume::systems::LoadingIndicator::stop(_registry, _entity);
		EXPECT_TRUE(fade.isPlaying());
		EXPECT_FLOAT_EQ(state().getOpacity(), 1.0f);

		fade.advance(1.0f);
		EXPECT_FLOAT_EQ(state().getOpacity(), 0.0f);
		EXPECT_EQ(color.getColor().getAlpha(), 0);

		guillaume::systems::LoadingIndicator::start(_registry, _entity);
		fade.advance(1.0f);
		EXPECT_FLOAT_EQ(state().getOpacity(), 1.0f);
		EXPECT_EQ(color.getColor(), getActiveColor(SchemeColorRole::Primary));
	}

	TEST_F(TestLoadingIndicatorSystem, StaysVisibleWhenItDoesNotHide)
	{
		state().setHidesWhenStopped(false);

		guillaume::systems::LoadingIndicator::stop(_registry, _entity);

		EXPECT_FALSE(
			_registry.getComponent<components::Animation>(_entity).isPlaying());
		EXPECT_FLOAT_EQ(state().getOpacity(), 1.0f);
	}

	TEST_F(TestLoadingIndicatorSystem, ColorOverrideReachesTheChild)
	{
		const auto tertiary = getActiveColor(SchemeColorRole::Tertiary);
		EXPECT_EQ(guillaume::systems::LoadingIndicator::indicatorColor(
					  _registry, _entity),
				  getActiveColor(SchemeColorRole::Primary));

		state().setIndicatorColor(tertiary);
		_system.update(_entity);

		EXPECT_EQ(guillaume::systems::LoadingIndicator::indicatorColor(
					  _registry, _entity),
				  tertiary);
		EXPECT_EQ(_registry.getComponent<components::Color>(_child).getColor(),
				  tertiary);
	}

	TEST_F(TestLoadingIndicatorSystem, AWidgetWithoutChildIsOnlySized)
	{
		state().setIndicator(ecs::Entity::InvalidIdentifier);

		_system.update(_entity);

		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getWidth(),
			48.0f);
		EXPECT_FALSE(guillaume::systems::LoadingIndicator::isRunning(_registry,
																	 _entity));
	}

	TEST_F(TestLoadingIndicatorSystem, AnUnchangedIndicatorIsNotMarkedChanged)
	{
		for (const auto variant: { Variant::Circular, Variant::Linear }) {
			state().setVariant(variant);
			_system.update(_entity);
			_registry.resetChangedFlags();

			_system.update(_entity);

			EXPECT_FALSE(_registry.hasChanged(_entity));
			EXPECT_FALSE(_registry.hasChanged(_child));
		}
	}

}	 // namespace guillaume::systems::tests
