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

#include "entities/test_image.hpp"

#include <guillaume/components/bound.hpp>

#include <guillaume/components/accessibility.hpp>

#include <stdexcept>

#include <memory>
#include <string>

#include <utility/ressource_provider.hpp>

#include "guillaume/components/bound.hpp"
#include "guillaume/components/image.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_registry_container.hpp"
#include "guillaume/entities/image.hpp"
#include "guillaume/mesh_renderable.hpp"
#include "guillaume/systems/image_render.hpp"

#include "mocks/engine_mock.hpp"
#include "mocks/fake_system_io.hpp"

namespace guillaume::entities::tests
{
	namespace
	{
		/**
		 * @brief Path of the committed texture used by the render tests.
		 */
		const std::string DEFAULT_TEXTURE_ASSET =
			"examples/evan/assets/textures/default_texture.png";

	}	 // namespace

	TEST_F(TestImage, ConstructorConfiguresImageComponent)
	{
		ecs::ComponentRegistry registry;
		auto image = std::make_shared<entities::Image>(registry, "texture.png");
		image->update();

		const auto &component =
			registry.getComponent<components::Image>(image->getIdentifier());

		EXPECT_EQ(component.getTexturePath(), "texture.png");
	}

	TEST_F(TestImage, SetTexturePathUpdatesComponent)
	{
		ecs::ComponentRegistry registry;
		auto image = std::make_shared<entities::Image>(registry, "texture.png");

		image->setTexturePath("other.png");

		const auto &component =
			registry.getComponent<components::Image>(image->getIdentifier());

		EXPECT_EQ(component.getTexturePath(), "other.png");
	}

	TEST_F(TestImage, ComponentIsTheSourceOfTruthForTheTexturePath)
	{
		ecs::ComponentRegistry registry;
		auto image = std::make_shared<entities::Image>(registry, "texture.png");

		// The entity keeps no copy: the path is in the component from
		// construction, and a direct component change is what it reports.
		const auto &component =
			registry.getComponent<components::Image>(image->getIdentifier());
		EXPECT_EQ(component.getTexturePath(), "texture.png");

		registry.getComponent<components::Image>(image->getIdentifier())
			.setTexturePath("mutated.png");

		EXPECT_EQ(component.getTexturePath(), "mutated.png");
		EXPECT_EQ(image->getTexturePath(), "mutated.png");
	}

	TEST_F(TestImage, BuilderPresetBuildsAndRegistersImage)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;

		entities::Image::Builder builder(registry, entityRegistry);

		auto image = builder.makeImage(nullptr, "menu.png");

		ASSERT_NE(image, nullptr);
		EXPECT_NE(
			entityRegistry.getEntity<entities::Image>(image->getIdentifier()),
			nullptr);

		const auto &component =
			registry.getComponent<components::Image>(image->getIdentifier());

		EXPECT_EQ(component.getTexturePath(), "menu.png");
	}

	TEST_F(TestImage, ImageRenderCreatesMeshRenderable)
	{
		auto engineMock = std::make_unique<guillaume::tests::EngineMock>();
		guillaume::tests::EngineMock *enginePtr = engineMock.get();
		std::unique_ptr<utility::Engine> engine = std::move(engineMock);

		guillaume::tests::FakeSystemIO systemIo;
		systemIo.addFileFromDisk("texture.png", DEFAULT_TEXTURE_ASSET);
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		guillaume::systems::ImageRender imageRender(ressourceProvider, *engine);

		ecs::ComponentRegistry registry;
		auto image = std::make_shared<entities::Image>(registry, "texture.png");
		registry.getComponent<components::Bound>(image->getIdentifier())
			.setWidth(64)
			.setHeight(64);
		image->update();

		imageRender.bindComponentRegistry(registry);
		imageRender.update(image->getIdentifier());

		EXPECT_EQ(enginePtr->createObjectCallCount, 1);
		EXPECT_EQ(enginePtr->countCreated<guillaume::MeshRenderable>(), 1);
	}

	TEST_F(TestImage, ImageRenderSkipsWhenTextureCannotBeLoaded)
	{
		auto engineMock = std::make_unique<guillaume::tests::EngineMock>();
		guillaume::tests::EngineMock *enginePtr = engineMock.get();
		std::unique_ptr<utility::Engine> engine = std::move(engineMock);

		guillaume::tests::FakeSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		guillaume::systems::ImageRender imageRender(ressourceProvider, *engine);

		ecs::ComponentRegistry registry;
		auto image = std::make_shared<entities::Image>(registry, "missing.png");

		imageRender.bindComponentRegistry(registry);
		imageRender.update(image->getIdentifier());

		EXPECT_EQ(enginePtr->createObjectCallCount, 0);
	}

	TEST_F(TestImage, SizeAspectAndSemantics)
	{
		ecs::ComponentRegistry registry;
		auto image = std::make_shared<Image>(registry, "images/a.png");

		image->setSize(120.0f, 80.0f);
		const auto &bound =
			registry.getComponent<components::Bound>(image->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 120.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 80.0f);

		image->setAspectRatio(160.0f, 2.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 80.0f);
		EXPECT_FLOAT_EQ(bound.getWidth(), 160.0f);
		EXPECT_THROW(image->setSize(0.0f, 10.0f), std::invalid_argument);

		EXPECT_EQ(image->getTexturePath(), "images/a.png");
		image->setAccessibilityLabel("Avatar");
		const auto &accessibility =
			registry.getComponent<components::Accessibility>(
				image->getIdentifier());
		EXPECT_EQ(accessibility.getRole(),
				  components::AccessibilityRole::Image);
		EXPECT_EQ(accessibility.getLabel(), "Avatar");
		EXPECT_EQ(image->getLoadState(), components::Image::LoadState::Pending);
	}

}	 // namespace guillaume::entities::tests
