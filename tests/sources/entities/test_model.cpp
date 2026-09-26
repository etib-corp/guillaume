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

#include "entities/test_model.hpp"

#include <memory>
#include <string>

#include <utility/ressource_provider.hpp>

#include "guillaume/components/bound.hpp"
#include "guillaume/components/model.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_registry_container.hpp"
#include "guillaume/ecs/level_order_traveler.hpp"
#include "guillaume/entities/model.hpp"
#include "guillaume/mesh_renderable.hpp"
#include "guillaume/systems/measure_model.hpp"
#include "guillaume/systems/model_render.hpp"

#include "mocks/engine_mock.hpp"
#include "mocks/fake_system_io.hpp"

namespace guillaume::entities::tests
{
	namespace
	{
		/**
		 * @brief Path of the committed model used by the render tests.
		 */
		const std::string DEFAULT_MODEL_ASSET =
			"examples/evan/assets/models/teapot.obj";

		/**
		 * @brief Path of the committed texture used by the render tests.
		 */
		const std::string DEFAULT_TEXTURE_ASSET =
			"examples/evan/assets/textures/default_texture.png";

		/**
		 * @brief Default texture path a loaded model refers to.
		 */
		const std::string MODEL_TEXTURE_PATH = "textures/default_texture.png";

	}	 // namespace

	TEST_F(TestModel, ConstructorConfiguresModelComponent)
	{
		ecs::ComponentRegistry registry;
		auto model = std::make_shared<entities::Model>(registry, "teapot.obj");
		model->update();

		const auto &component =
			registry.getComponent<components::Model>(model->getIdentifier());

		EXPECT_EQ(component.getModelPath(), "teapot.obj");
	}

	TEST_F(TestModel, SetPathsUpdateComponent)
	{
		ecs::ComponentRegistry registry;
		auto model = std::make_shared<entities::Model>(registry, "teapot.obj");

		model->setModelPath("other.obj");
		model->setTexturePath("skin.png");

		const auto &component =
			registry.getComponent<components::Model>(model->getIdentifier());

		EXPECT_EQ(component.getModelPath(), "other.obj");
		EXPECT_EQ(component.getTexturePath(), "skin.png");
	}

	TEST_F(TestModel, UpdateReappliesStoredPaths)
	{
		ecs::ComponentRegistry registry;
		auto model = std::make_shared<entities::Model>(registry, "teapot.obj");
		model->setTexturePath("skin.png");

		registry.getComponent<components::Model>(model->getIdentifier())
			.setModelPath("mutated.obj")
			.setTexturePath("mutated.png");

		model->update();

		const auto &component =
			registry.getComponent<components::Model>(model->getIdentifier());

		EXPECT_EQ(component.getModelPath(), "teapot.obj");
		EXPECT_EQ(component.getTexturePath(), "skin.png");
	}

	TEST_F(TestModel, DirectorBuildsAndRegistersModel)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;

		entities::Model::Builder builder(registry, entityRegistry);
		entities::Model::Director director;

		auto model =
			director.makeModel(builder, nullptr, "teapot.obj", "skin.png");

		ASSERT_NE(model, nullptr);
		EXPECT_NE(
			entityRegistry.getEntity<entities::Model>(model->getIdentifier()),
			nullptr);

		const auto &component =
			registry.getComponent<components::Model>(model->getIdentifier());

		EXPECT_EQ(component.getModelPath(), "teapot.obj");
		EXPECT_EQ(component.getTexturePath(), "skin.png");
	}

	TEST_F(TestModel, MeasureModelSynchronizesBoundWithModelSize)
	{
		auto engineMock = std::make_unique<guillaume::tests::EngineMock>();
		std::unique_ptr<utility::Engine> engine = std::move(engineMock);

		guillaume::tests::FakeSystemIO systemIo;
		systemIo.addFileFromDisk("teapot.obj", DEFAULT_MODEL_ASSET);
		systemIo.addFileFromDisk(MODEL_TEXTURE_PATH, DEFAULT_TEXTURE_ASSET);
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		guillaume::systems::MeasureModel measureModel(ressourceProvider,
													  engine);

		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto model = std::make_shared<entities::Model>(registry, "teapot.obj");
		model->update();
		entityRegistry.addEntity(model);

		ecs::LevelOrderTraveler traveler;
		measureModel.routine(registry, entityRegistry, traveler);

		const auto &bound =
			registry.getComponent<components::Bound>(model->getIdentifier());

		EXPECT_GT(bound.getWidth(), 0U);
		EXPECT_GT(bound.getHeight(), 0U);
	}

	TEST_F(TestModel, ModelRenderCreatesMeshRenderable)
	{
		auto engineMock = std::make_unique<guillaume::tests::EngineMock>();
		guillaume::tests::EngineMock *enginePtr = engineMock.get();
		std::unique_ptr<utility::Engine> engine = std::move(engineMock);

		guillaume::tests::FakeSystemIO systemIo;
		systemIo.addFileFromDisk("teapot.obj", DEFAULT_MODEL_ASSET);
		systemIo.addFileFromDisk(MODEL_TEXTURE_PATH, DEFAULT_TEXTURE_ASSET);
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		guillaume::systems::ModelRender modelRender(ressourceProvider, engine);

		ecs::ComponentRegistry registry;
		auto model = std::make_shared<entities::Model>(registry, "teapot.obj");
		model->update();

		modelRender.bindComponentRegistry(registry);
		modelRender.update(model->getIdentifier());

		EXPECT_EQ(enginePtr->createObjectCallCount, 1);
		EXPECT_EQ(enginePtr->countCreated<guillaume::MeshRenderable>(), 1);
	}

	TEST_F(TestModel, ModelRenderSkipsWhenModelCannotBeLoaded)
	{
		auto engineMock = std::make_unique<guillaume::tests::EngineMock>();
		guillaume::tests::EngineMock *enginePtr = engineMock.get();
		std::unique_ptr<utility::Engine> engine = std::move(engineMock);

		guillaume::tests::FakeSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		guillaume::systems::ModelRender modelRender(ressourceProvider, engine);

		ecs::ComponentRegistry registry;
		auto model = std::make_shared<entities::Model>(registry, "missing.obj");

		modelRender.bindComponentRegistry(registry);
		modelRender.update(model->getIdentifier());

		EXPECT_EQ(enginePtr->createObjectCallCount, 0);
	}

}	 // namespace guillaume::entities::tests
