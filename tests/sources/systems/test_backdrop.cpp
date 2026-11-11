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

#include "systems/test_backdrop.hpp"

#include <utility/graphic/pose.hpp>

#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		std::uint8_t scrimAlpha(ecs::ComponentRegistry &registry,
								ecs::Entity::Identifier entity)
		{
			return registry.getComponent<components::Scrim>(entity)
				.getColor()
				.getAlpha();
		}
	}	 // namespace

	TEST_F(TestBackdropSystem, HiddenBackdropIsFullyTransparent)
	{
		_system.update(_entity);

		EXPECT_EQ(scrimAlpha(_registry, _entity), 0);
	}

	TEST_F(TestBackdropSystem, VisibleBackdropFadesWithProgress)
	{
		const auto full = getActiveColor(SchemeColorRole::Backdrop).getAlpha();
		auto &state		= _registry.getComponent<components::Backdrop>(_entity);

		state.setVisible(true).setProgress(0.0f);
		_system.update(_entity);
		EXPECT_EQ(scrimAlpha(_registry, _entity), 0);

		state.setProgress(1.0f);
		_system.update(_entity);
		EXPECT_EQ(scrimAlpha(_registry, _entity), full);

		state.setProgress(0.5f);
		_system.update(_entity);
		EXPECT_GT(scrimAlpha(_registry, _entity), 0);
		EXPECT_LT(scrimAlpha(_registry, _entity), full);
	}

	TEST_F(TestBackdropSystem, ShowingRestartsTheFadeIn)
	{
		auto &state = _registry.getComponent<components::Backdrop>(_entity);
		auto &animation =
			_registry.getComponent<components::Animation>(_entity);
		state.setProgress(0.8f);

		Backdrop::setVisible(_registry, _entity, true);

		EXPECT_TRUE(state.isVisible());
		EXPECT_FLOAT_EQ(state.getProgress(), 0.0f);
		EXPECT_TRUE(animation.isPlaying());
		EXPECT_FLOAT_EQ(animation.getElapsed(), 0.0f);
	}

	TEST_F(TestBackdropSystem, HidingStopsTheAnimationAndClearsTheScrim)
	{
		Backdrop::setVisible(_registry, _entity, true);
		_registry.getComponent<components::Backdrop>(_entity).setProgress(1.0f);
		_system.update(_entity);
		ASSERT_GT(scrimAlpha(_registry, _entity), 0);

		Backdrop::setVisible(_registry, _entity, false);

		EXPECT_FALSE(
			_registry.getComponent<components::Animation>(_entity).isPlaying());
		EXPECT_EQ(scrimAlpha(_registry, _entity), 0);
	}

	TEST_F(TestBackdropSystem, FollowCentersTheBackdropOnItsOwner)
	{
		utility::graphic::PoseF ownerPose;
		ownerPose.setPosition(utility::graphic::PositionF(10.0f, 20.0f, 0.0f));

		Backdrop::follow(_registry, _entity, ownerPose, 100.0f, 50.0f);

		const auto position =
			_registry.getComponent<components::Transform>(_entity)
				.getPose()
				.getPosition();
		const auto &orientation = ownerPose.getOrientation();
		const auto expected		= ownerPose.getPosition()
			- orientation.getRight() * ((Backdrop::Extent - 100.0f) / 2.0f)
			- orientation.getUp() * ((Backdrop::Extent - 50.0f) / 2.0f);

		EXPECT_NEAR(position[0], expected[0], 1e-2f);
		EXPECT_NEAR(position[1], expected[1], 1e-2f);
	}

	TEST_F(TestBackdropSystem, SteadyStateDoesNotMarkComponentsChanged)
	{
		Backdrop::setVisible(_registry, _entity, true);
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
	}
}	 // namespace guillaume::systems::tests
