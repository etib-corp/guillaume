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

#include "test_application.hpp"

#include <stdexcept>
#include <string>
#include <vector>

#include <cmath>
#include <memory>
#include <utility>

#include <utility/system_io/default_system_io.hpp>

#include <utility/graphic/orientation.hpp>
#include <utility/graphic/position.hpp>
#include <utility/graphic/view.hpp>

#include "guillaume/components/bound.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/ecs/entity.hpp"
#include "guillaume/local_storage.hpp"
#include "guillaume/session_storage.hpp"

#include "mocks/engine_mock.hpp"

namespace guillaume::tests
{
	namespace
	{
		class StubScene: public guillaume::Scene
		{
			public:
			using guillaume::Scene::Scene;
		};

		/**
		 * @brief Scene that registers a root entity from `onEnter`.
		 *
		 * Used to verify the scene manager places entities that scenes build
		 * in `onEnter` rather than before they exist.
		 */
		class PlacementRecordingScene: public guillaume::Scene
		{
			public:
			static PlacementRecordingScene *instance;
			static ecs::Entity::Identifier entityIdentifier;

			using guillaume::Scene::Scene;

			void onEnter(void) override
			{
				instance = this;

				auto entity		 = std::make_shared<ecs::Entity>();
				entityIdentifier = entity->getIdentifier();

				getComponentRegistry()
					.registerComponentsForEntity<components::Transform,
												 components::Bound>(
						entityIdentifier);
				getComponentRegistry()
					.getComponent<components::Bound>(entityIdentifier)
					.setWidth(100.0f)
					.setHeight(50.0f);

				addEntity(entity);
			}
		};

		/// Lifecycle calls recorded by the transition scenes, in order.
		std::vector<std::string> lifecycleLog;

		class SecondScene;

		/**
		 * @brief Default scene of the transition tests; records its
		 * lifecycle and exposes itself so a test can request a switch.
		 */
		class FirstScene: public guillaume::Scene
		{
			public:
			static FirstScene *instance;

			using guillaume::Scene::Scene;

			void onEnter(void) override
			{
				instance = this;
				lifecycleLog.push_back("first.enter");
			}

			void onExit(void) override
			{
				lifecycleLog.push_back("first.exit");
			}
		};

		/**
		 * @brief Target scene of the transition tests.
		 */
		class SecondScene: public guillaume::Scene
		{
			public:
			using guillaume::Scene::Scene;

			void onEnter(void) override
			{
				lifecycleLog.push_back("second.enter");
			}

			void onExit(void) override
			{
				lifecycleLog.push_back("second.exit");
			}
		};

		FirstScene *FirstScene::instance = nullptr;

		PlacementRecordingScene *PlacementRecordingScene::instance = nullptr;
		ecs::Entity::Identifier PlacementRecordingScene::entityIdentifier =
			ecs::Entity::InvalidIdentifier;
	}	 // namespace

	TEST_F(TestApplication, ConstructsWithRessourceProviderAndEngine)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		guillaume::Application<StubScene> application(
			ressourceProvider, std::make_unique<EngineMock>());

		EXPECT_FALSE(application.shouldQuit());
	}

	TEST_F(TestApplication, RejectsNullEngine)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		EXPECT_THROW(
			guillaume::Application<StubScene>(ressourceProvider, nullptr),
			std::invalid_argument);
	}

	TEST_F(TestApplication, RoutineRunsOneFrame)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		guillaume::Application<StubScene> application(
			ressourceProvider, std::make_unique<EngineMock>());

		EXPECT_NO_THROW(application.routine());
	}

	TEST_F(TestApplication, PlacesEntitiesFromTheirTopLeftCorner)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		guillaume::LocalStorage localStorage;
		guillaume::SessionStorage sessionStorage;
		StubScene scene(ressourceProvider, localStorage, sessionStorage);

		auto &registry = scene.getComponentRegistry();

		auto entity			= std::make_shared<guillaume::ecs::Entity>();
		const auto entityId = entity->getIdentifier();

		registry.registerComponentsForEntity<guillaume::components::Transform,
											 guillaume::components::Bound>(
			entityId);
		registry.getComponent<guillaume::components::Bound>(entityId)
			.setWidth(100.0f)
			.setHeight(50.0f);

		scene.addEntity(entity);

		const utility::graphic::ViewF view;
		scene.placeEntitiesInFrontOfView(view);

		const auto &pose =
			registry.getComponent<guillaume::components::Transform>(entityId)
				.getPose();

		const auto anchor =
			view.centerRay().origin() + view.centerRay().direction() * 300.0f;
		// A single 100x50 entity must sit with its left edge half a width
		// behind the anchor and its top edge half a height above it, so it is
		// centred on the anchor (its position is its top-left corner).
		const auto expected =
			anchor + view.getRight() * (-50.0f) + view.getUp() * 25.0f;

		EXPECT_NEAR(pose.getPosition()[0], expected[0], 1e-3f);
		EXPECT_NEAR(pose.getPosition()[1], expected[1], 1e-3f);
		EXPECT_NEAR(pose.getPosition()[2], expected[2], 1e-3f);

		const float distanceFromAnchor =
			std::abs(pose.getPosition()[0] - anchor[0])
			+ std::abs(pose.getPosition()[1] - anchor[1])
			+ std::abs(pose.getPosition()[2] - anchor[2]);
		EXPECT_GT(distanceFromAnchor, 1e-3f);
	}

	TEST_F(TestApplication, PlacesEntitiesBuiltInOnEnter)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		guillaume::Application<PlacementRecordingScene> application(
			ressourceProvider, std::make_unique<EngineMock>());

		ASSERT_NE(PlacementRecordingScene::instance, nullptr);

		auto &registry =
			PlacementRecordingScene::instance->getComponentRegistry();
		const auto &pose = registry
							   .getComponent<guillaume::components::Transform>(
								   PlacementRecordingScene::entityIdentifier)
							   .getPose();

		const utility::graphic::ViewF view;
		const auto anchor =
			view.centerRay().origin() + view.centerRay().direction() * 300.0f;
		// Top-left corner of the 100x50 entity centred on the anchor.
		const auto expected =
			anchor + view.getRight() * (-50.0f) + view.getUp() * 25.0f;

		// The entity built in onEnter must have been placed, not left at the
		// default origin.
		EXPECT_NEAR(pose.getPosition()[0], expected[0], 1e-3f);
		EXPECT_NEAR(pose.getPosition()[1], expected[1], 1e-3f);
		EXPECT_NEAR(pose.getPosition()[2], expected[2], 1e-3f);
		EXPECT_NE(pose.getPosition().getX(), 0.0f);
	}

	TEST_F(TestApplication, DoesNotMoveEntitiesOnViewChange)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		auto engine		= std::make_unique<EngineMock>();
		auto *enginePtr = engine.get();

		guillaume::Application<PlacementRecordingScene> application(
			ressourceProvider, std::move(engine));

		ASSERT_NE(PlacementRecordingScene::instance, nullptr);

		auto &registry =
			PlacementRecordingScene::instance->getComponentRegistry();
		auto &transform =
			registry.getComponent<guillaume::components::Transform>(
				PlacementRecordingScene::entityIdentifier);

		const auto placedPose = transform.getPose();

		// Flip the view ~180 degrees about Y: the entity must not move or
		// rotate.
		enginePtr->view.setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(),
			utility::graphic::OrientationF(0.0f, 1.0f, 0.0f, 0.0f)));

		application.routine();

		const auto &pose = transform.getPose();
		EXPECT_FLOAT_EQ(pose.getPosition()[0], placedPose.getPosition()[0]);
		EXPECT_FLOAT_EQ(pose.getPosition()[1], placedPose.getPosition()[1]);
		EXPECT_FLOAT_EQ(pose.getPosition()[2], placedPose.getPosition()[2]);
		EXPECT_EQ(pose.getOrientation(), placedPose.getOrientation());
	}
	TEST_F(TestApplication, EntersOnlyTheDefaultSceneOnConstruction)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);
		lifecycleLog.clear();

		guillaume::Application<FirstScene, SecondScene> application(
			ressourceProvider, std::make_unique<EngineMock>());

		EXPECT_EQ(lifecycleLog, std::vector<std::string> { "first.enter" });
	}

	TEST_F(TestApplication, FrameWithoutRequestDoesNotReenterTheScene)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);
		lifecycleLog.clear();

		guillaume::Application<FirstScene, SecondScene> application(
			ressourceProvider, std::make_unique<EngineMock>());
		application.routine();

		EXPECT_EQ(lifecycleLog, std::vector<std::string> { "first.enter" });
	}

	TEST_F(TestApplication, SceneTransitionExitsThenEntersAtEndOfFrame)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);
		lifecycleLog.clear();

		guillaume::Application<FirstScene, SecondScene> application(
			ressourceProvider, std::make_unique<EngineMock>());
		ASSERT_NE(FirstScene::instance, nullptr);

		FirstScene::instance->goToScene<SecondScene>();

		// The request is only honored by the next frame.
		EXPECT_EQ(lifecycleLog, std::vector<std::string> { "first.enter" });

		application.routine();

		EXPECT_EQ(lifecycleLog,
				  (std::vector<std::string> { "first.enter", "first.exit",
											  "second.enter" }));

		// The request is consumed: later frames do not transition again.
		application.routine();

		EXPECT_EQ(lifecycleLog.size(), 3U);
	}

	TEST_F(TestApplication, TransitionToUnregisteredSceneFailsOnce)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);
		lifecycleLog.clear();

		guillaume::Application<FirstScene, SecondScene> application(
			ressourceProvider, std::make_unique<EngineMock>());
		ASSERT_NE(FirstScene::instance, nullptr);

		FirstScene::instance->goToScene<StubScene>();

		EXPECT_THROW(application.routine(), std::runtime_error);

		// The failed request is dropped and the scene was never exited.
		EXPECT_NO_THROW(application.routine());
		EXPECT_EQ(lifecycleLog, std::vector<std::string> { "first.enter" });
	}

}	 // namespace guillaume::tests
