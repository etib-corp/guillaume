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

#include "systems/test_animation.hpp"

#include "guillaume/motion/easing.hpp"

namespace guillaume::systems::tests
{

	TEST_F(TestAnimation, EasingEndpoints)
	{
		using guillaume::motion::Easing;

		for (const auto easing:
			 { Easing::Linear, Easing::Standard, Easing::Decelerate,
			   Easing::Accelerate, Easing::Emphasized }) {
			EXPECT_NEAR(guillaume::motion::applyEasing(easing, 0.0f), 0.0f,
						1e-4f);
			EXPECT_NEAR(guillaume::motion::applyEasing(easing, 1.0f), 1.0f,
						1e-4f);
		}
	}

	TEST_F(TestAnimation, LinearEasingIsIdentity)
	{
		EXPECT_FLOAT_EQ(guillaume::motion::applyEasing(
							guillaume::motion::Easing::Linear, 0.25f),
						0.25f);
	}

	TEST_F(TestAnimation, AdvanceReportsEasedProgress)
	{
		components::Animation animation;
		float lastProgress = -1.0f;

		animation.setDuration(1.0f);
		animation.setEasing(guillaume::motion::Easing::Linear);
		animation.setOnSampleHandler([&lastProgress](float progress) {
			lastProgress = progress;
		});
		animation.play();

		animation.advance(0.5f);
		EXPECT_FLOAT_EQ(lastProgress, 0.5f);
		EXPECT_TRUE(animation.isPlaying());
	}

	TEST_F(TestAnimation, AdvanceCompletesAnimation)
	{
		components::Animation animation;
		int completed	   = 0;
		float lastProgress = -1.0f;

		animation.setDuration(1.0f);
		animation.setEasing(guillaume::motion::Easing::Linear);
		animation.setOnSampleHandler([&lastProgress](float progress) {
			lastProgress = progress;
		});
		animation.setOnCompleteHandler([&completed]() {
			++completed;
		});
		animation.play();

		animation.advance(2.0f);

		EXPECT_FLOAT_EQ(lastProgress, 1.0f);
		EXPECT_EQ(completed, 1);
		EXPECT_FALSE(animation.isPlaying());
	}

	TEST_F(TestAnimation, LoopingAnimationRestarts)
	{
		components::Animation animation;
		int completed = 0;

		animation.setDuration(1.0f);
		animation.setEasing(guillaume::motion::Easing::Linear);
		animation.setLooping(true);
		animation.setOnCompleteHandler([&completed]() {
			++completed;
		});
		animation.play();

		animation.advance(1.5f);

		EXPECT_EQ(completed, 1);
		EXPECT_TRUE(animation.isPlaying());
		EXPECT_FLOAT_EQ(animation.getElapsed(), 0.0f);
	}

	TEST_F(TestAnimation, NotPlayingDoesNotAdvance)
	{
		components::Animation animation;
		int samples = 0;

		animation.setDuration(1.0f);
		animation.setOnSampleHandler([&samples](float) {
			++samples;
		});

		animation.advance(0.5f);

		EXPECT_EQ(samples, 0);
		EXPECT_FLOAT_EQ(animation.getElapsed(), 0.0f);
	}

	TEST_F(TestAnimation, SystemUsesEngineDeltaTime)
	{
		ecs::Entity entity;
		const auto identifier = entity.getIdentifier();

		_componentRegistry.addComponent<components::Animation>(identifier);
		auto &animation =
			_componentRegistry.getComponent<components::Animation>(identifier);
		animation.setDuration(1.0f);
		animation.setEasing(guillaume::motion::Easing::Linear);
		animation.play();

		_engineMock->deltaTime = 0.05f;

		_animationSystem->prepare();
		_animationSystem->update(identifier);

		EXPECT_FLOAT_EQ(animation.getElapsed(), 0.05f);
	}

	TEST_F(TestAnimation, SystemFallsBackToInternalClock)
	{
		ecs::Entity entity;
		const auto identifier = entity.getIdentifier();

		_componentRegistry.addComponent<components::Animation>(identifier);
		auto &animation =
			_componentRegistry.getComponent<components::Animation>(identifier);
		animation.setDuration(10.0f);
		animation.play();

		_engineMock->deltaTime = 0.0f;

		_animationSystem->prepare();
		_animationSystem->update(identifier);

		EXPECT_TRUE(animation.isPlaying());
		EXPECT_GE(animation.getElapsed(), 0.0f);
	}

}	 // namespace guillaume::systems::tests
