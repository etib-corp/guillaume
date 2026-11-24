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

#include "test_scene.hpp"

#include <memory>
#include <stdexcept>

#include <utility/system_io/default_system_io.hpp>

#include <utility/graphic/view.hpp>

#include "guillaume/components/bound.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/ecs/entity.hpp"
#include "guillaume/local_storage.hpp"
#include "guillaume/session_storage.hpp"

namespace guillaume::tests
{
	namespace
	{
		/**
		 * @brief Scene that names an entity in `onEnter` and takes the name
		 * back in `onExit`, and opens the protected members under test.
		 */
		class NamingScene: public guillaume::Scene
		{
			public:
			using guillaume::Scene::Scene;

			using guillaume::Scene::addRootEntity;
			using guillaume::Scene::getLocalStorage;
			using guillaume::Scene::getRootEntity;
			using guillaume::Scene::getSessionStorage;
			using guillaume::Scene::removeRootEntity;

			void onEnter(void) override
			{
				addRootEntity("root", std::make_shared<ecs::Entity>());
			}

			void onExit(void) override
			{
				removeRootEntity("root");
			}
		};

		/**
		 * @brief Add an entity with a Transform and a Bound to a scene.
		 * @param scene The scene that registers the entity.
		 * @param width The width of the Bound.
		 * @param height The height of the Bound.
		 * @param parent The parent of the entity, or nullptr for a root.
		 * @return The entity.
		 */
		std::shared_ptr<ecs::Entity>
			addPlaceable(guillaume::Scene &scene, float width, float height,
						 std::shared_ptr<ecs::Entity> parent = nullptr)
		{
			auto entity	  = std::make_shared<ecs::Entity>();
			const auto id = entity->getIdentifier();

			auto &registry = scene.getComponentRegistry();
			registry.registerComponentsForEntity<components::Transform,
												 components::Bound>(id);
			registry.getComponent<components::Bound>(id)
				.setWidth(width)
				.setHeight(height);

			// Same two steps as EntityBuilderBase::registerEntity.
			entity->setParent(parent);
			scene.addEntity(entity);
			return entity;
		}
	}	 // namespace

	TEST_F(TestScene, AddRootEntityRejectsAUsedNameAndKeepsTheFirstEntity)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);
		guillaume::LocalStorage localStorage;
		guillaume::SessionStorage sessionStorage;
		NamingScene scene(ressourceProvider, localStorage, sessionStorage);

		auto first	= std::make_shared<ecs::Entity>();
		auto second = std::make_shared<ecs::Entity>();

		scene.addRootEntity("name", first);
		EXPECT_THROW(scene.addRootEntity("name", second), std::runtime_error);
		EXPECT_EQ(scene.getRootEntity<ecs::Entity>("name"), first);
	}

	TEST_F(TestScene, RemoveRootEntityFreesTheName)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);
		guillaume::LocalStorage localStorage;
		guillaume::SessionStorage sessionStorage;
		NamingScene scene(ressourceProvider, localStorage, sessionStorage);

		auto first	= std::make_shared<ecs::Entity>();
		auto second = std::make_shared<ecs::Entity>();

		scene.addRootEntity("name", first);

		EXPECT_TRUE(scene.removeRootEntity("name"));
		EXPECT_EQ(scene.getRootEntity<ecs::Entity>("name"), nullptr);
		// The name table no longer keeps the entity alive.
		EXPECT_EQ(first.use_count(), 1);

		EXPECT_NO_THROW(scene.addRootEntity("name", second));
		EXPECT_EQ(scene.getRootEntity<ecs::Entity>("name"), second);
	}

	TEST_F(TestScene, RemoveRootEntityReportsAnUnknownName)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);
		guillaume::LocalStorage localStorage;
		guillaume::SessionStorage sessionStorage;
		NamingScene scene(ressourceProvider, localStorage, sessionStorage);

		EXPECT_FALSE(scene.removeRootEntity("unknown"));
	}

	TEST_F(TestScene, SceneNamingAnEntityInOnEnterCanBeEnteredTwice)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);
		guillaume::LocalStorage localStorage;
		guillaume::SessionStorage sessionStorage;
		NamingScene scene(ressourceProvider, localStorage, sessionStorage);

		scene.onEnter();
		scene.onExit();
		EXPECT_NO_THROW(scene.onEnter());
		EXPECT_NE(scene.getRootEntity<ecs::Entity>("root"), nullptr);
	}

	TEST_F(TestScene, PlacementSkipsEntitiesThatHaveAParent)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);
		guillaume::LocalStorage localStorage;
		guillaume::SessionStorage sessionStorage;
		NamingScene scene(ressourceProvider, localStorage, sessionStorage);

		auto root  = addPlaceable(scene, 100.0f, 50.0f);
		auto child = addPlaceable(scene, 40.0f, 20.0f, root);

		auto &registry = scene.getComponentRegistry();
		const auto childPoseBefore =
			registry.getComponent<components::Transform>(child->getIdentifier())
				.getPose();

		const utility::graphic::ViewF view;
		scene.placeEntitiesInFrontOfView(view);

		const auto anchor =
			view.centerRay().origin() + view.centerRay().direction() * 300.0f;
		// The root is alone in the row: the child does not widen it.
		const auto expected =
			anchor + view.getRight() * (-50.0f) + view.getUp() * 25.0f;

		const auto &rootPose =
			registry.getComponent<components::Transform>(root->getIdentifier())
				.getPose();
		EXPECT_NEAR(rootPose.getPosition()[0], expected[0], 1e-3f);
		EXPECT_NEAR(rootPose.getPosition()[1], expected[1], 1e-3f);
		EXPECT_NEAR(rootPose.getPosition()[2], expected[2], 1e-3f);

		// The child keeps the pose it had: it is not laid out as a root.
		const auto &childPose =
			registry.getComponent<components::Transform>(child->getIdentifier())
				.getPose();
		EXPECT_FLOAT_EQ(childPose.getPosition()[0],
						childPoseBefore.getPosition()[0]);
		EXPECT_FLOAT_EQ(childPose.getPosition()[1],
						childPoseBefore.getPosition()[1]);
		EXPECT_FLOAT_EQ(childPose.getPosition()[2],
						childPoseBefore.getPosition()[2]);
	}

	TEST_F(TestScene, DerivedSceneReachesTheStoragesItWasGiven)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);
		guillaume::LocalStorage localStorage;
		guillaume::SessionStorage sessionStorage;
		NamingScene scene(ressourceProvider, localStorage, sessionStorage);

		EXPECT_EQ(&scene.getLocalStorage(), &localStorage);
		EXPECT_EQ(&scene.getSessionStorage(), &sessionStorage);

		scene.getSessionStorage().setItem("key", "value");
		EXPECT_EQ(sessionStorage.getItem("key"), "value");
	}

}	 // namespace guillaume::tests
