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

		PlacementRecordingScene *PlacementRecordingScene::instance = nullptr;
		ecs::Entity::Identifier PlacementRecordingScene::entityIdentifier =
			ecs::Entity::InvalidIdentifier;
	}	 // namespace

	TEST_F(TestApplication, ConstructsWithRessourceProvider)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		guillaume::Application<StubScene, StubScene> application(
			ressourceProvider);

		EXPECT_FALSE(application.shouldQuit());
	}

	TEST_F(TestApplication, SetEngineAcceptsAnEngine)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		guillaume::Application<StubScene, StubScene> application(
			ressourceProvider);

		auto engine = std::make_unique<EngineMock>();
		EXPECT_NO_THROW(application.setEngine(std::move(engine)));
	}

	TEST_F(TestApplication, LifecycleMethodsRunAfterEngineAttached)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		guillaume::Application<StubScene, StubScene> application(
			ressourceProvider);

		application.setEngine(std::make_unique<EngineMock>());

		EXPECT_NO_THROW(application.update());
		EXPECT_NO_THROW(application.clear());
		EXPECT_NO_THROW(application.present());
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
		// A single 100px-wide entity must sit with its left edge half a width
		// behind the anchor, not centred on it.
		const auto expected = anchor + view.getRight() * (-50.0f);

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

		guillaume::Application<PlacementRecordingScene, PlacementRecordingScene>
			application(ressourceProvider);

		application.setEngine(std::make_unique<EngineMock>());

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
		const auto expected = anchor + view.getRight() * (-50.0f);

		// The entity built in onEnter must have been placed, not left at the
		// default origin.
		EXPECT_NEAR(pose.getPosition()[0], expected[0], 1e-3f);
		EXPECT_NEAR(pose.getPosition()[1], expected[1], 1e-3f);
		EXPECT_NEAR(pose.getPosition()[2], expected[2], 1e-3f);
		EXPECT_NE(pose.getPosition().getX(), 0.0f);
	}
}	 // namespace guillaume::tests
