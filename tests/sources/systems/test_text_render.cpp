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

#include <memory>

#include <utility/system_io/default_system_io.hpp>

#include "guillaume/components/color.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_registry.hpp"
#include "guillaume/ecs/entity_registry_container.hpp"

#include "systems/test_text_render.hpp"

namespace
{
	/**
	 * @brief Engine stub recording the text object lifecycle.
	 */
	class EngineStub: public guillaume::Engine
	{
		public:
		std::size_t addTextCallCount		 = 0;
		std::size_t removeObjectCallCount	 = 0;
		std::string lastAddedContent;
		size_t lastRemovedObject { 0 };
		size_t nextObjectID { 1 };

		void clear(void) override
		{
		}
		void present(void) override
		{
		}
		size_t addMesh(const utility::graphic::Mesh &,
					   const std::string &) override
		{
			return 0;
		}
		bool removeObject(size_t objectID) override
		{
			++removeObjectCallCount;
			lastRemovedObject = objectID;
			return true;
		}
		utility::graphic::SizeF
			measureText(const utility::graphic::Text &) const override
		{
			return { 0.0f, 0.0f };
		}
		size_t addText(utility::graphic::Text text) override
		{
			++addTextCallCount;
			lastAddedContent = text.getContent();
			return nextObjectID++;
		}
		size_t addModel(std::shared_ptr<utility::graphic::Model>) override
		{
			return 0;
		}
		utility::graphic::ViewF getView(void) const override
		{
			return utility::graphic::ViewF();
		}
		void addScene(size_t sceneIndex) override
		{
			(void)sceneIndex;
		}
		void pollEvents(void) override
		{
		}
		void update(void) override
		{
		}
	};

	class TextRenderFixture: public guillaume::systems::tests::TestTextRender
	{
		protected:
		std::unique_ptr<EngineStub> engineStub = std::make_unique<EngineStub>();
		EngineStub *enginePtr				   = engineStub.get();
		std::unique_ptr<guillaume::Engine> engine { std::move(engineStub) };
		utility::DefaultSystemIO systemIo;
		std::shared_ptr<utility::RessourceProvider> ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);
		guillaume::systems::TextRender textRenderSystem { ressourceProvider,
														  engine };
		guillaume::ecs::ComponentRegistry componentRegistry;
		guillaume::ecs::EntityRegistryContainer entityRegistry;
		guillaume::ecs::Entity::Identifier entityIdentifier {
			guillaume::ecs::Entity::InvalidIdentifier
		};

		void SetUp(void) override
		{
			auto entity		 = std::make_shared<guillaume::ecs::Entity>();
			entityIdentifier = entity->getIdentifier();
			entity->setSignature(
				guillaume::ecs::Entity::getSignatureFromTypes<
					guillaume::components::Transform,
					guillaume::components::Text,
					guillaume::components::Color>());
			entityRegistry.addEntity(std::move(entity));

			componentRegistry.addComponent<guillaume::components::Transform>(
				entityIdentifier);
			componentRegistry.addComponent<guillaume::components::Text>(
				entityIdentifier);
			componentRegistry.addComponent<guillaume::components::Color>(
				entityIdentifier);

			componentRegistry
				.getComponent<guillaume::components::Text>(entityIdentifier)
				.setContent("Hello");

			textRenderSystem.bindComponentRegistry(componentRegistry);
		}

		void TearDown(void) override
		{
			textRenderSystem.unbindComponentRegistry();
		}
	};

}	 // namespace

TEST_F(TextRenderFixture, ReusesRenderObjectWhenTextStateIsUnchanged)
{
	textRenderSystem.update(entityIdentifier);
	textRenderSystem.update(entityIdentifier);

	EXPECT_EQ(enginePtr->addTextCallCount, 1);
	EXPECT_EQ(enginePtr->removeObjectCallCount, 0);
	EXPECT_EQ(textRenderSystem.size(), 1);
}

TEST_F(TextRenderFixture, RegeneratesRenderObjectWhenContentChanges)
{
	textRenderSystem.update(entityIdentifier);

	componentRegistry
		.getComponent<guillaume::components::Text>(entityIdentifier)
		.setContent("New content");

	textRenderSystem.update(entityIdentifier);

	EXPECT_EQ(enginePtr->addTextCallCount, 2);
	EXPECT_EQ(enginePtr->removeObjectCallCount, 1);
	EXPECT_EQ(enginePtr->lastRemovedObject, 1);
	EXPECT_EQ(enginePtr->lastAddedContent, "New content");
	EXPECT_EQ(textRenderSystem.size(), 1);
}

TEST_F(TextRenderFixture, RegeneratesRenderObjectWhenPoseChanges)
{
	textRenderSystem.update(entityIdentifier);

	componentRegistry
		.getComponent<guillaume::components::Transform>(entityIdentifier)
		.setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(10.0f, 20.0f, 30.0f),
			utility::graphic::OrientationF()));

	textRenderSystem.update(entityIdentifier);

	EXPECT_EQ(enginePtr->addTextCallCount, 2);
	EXPECT_EQ(enginePtr->removeObjectCallCount, 1);
	EXPECT_EQ(enginePtr->lastRemovedObject, 1);
	EXPECT_EQ(textRenderSystem.size(), 1);
}

TEST_F(TextRenderFixture, RemovesRenderObjectWhenNoSceneUsesIt)
{
	// First frame: the active scene renders its text.
	textRenderSystem.prepare();
	textRenderSystem.update(entityIdentifier);
	textRenderSystem.cleanup();

	EXPECT_EQ(enginePtr->addTextCallCount, 1);
	EXPECT_EQ(enginePtr->removeObjectCallCount, 0);

	// Next frame: another scene is active and this text is not rendered.
	textRenderSystem.prepare();
	textRenderSystem.cleanup();

	EXPECT_EQ(enginePtr->removeObjectCallCount, 1);
	EXPECT_EQ(enginePtr->lastRemovedObject, 1);
	EXPECT_EQ(textRenderSystem.size(), 0);
}

TEST_F(TextRenderFixture, RecreatesRenderObjectAfterCleanup)
{
	// The entry is gone after a frame without the entity, so rendering it
	// again (scene switched back) creates a fresh object.
	textRenderSystem.prepare();
	textRenderSystem.update(entityIdentifier);
	textRenderSystem.cleanup();
	textRenderSystem.prepare();
	textRenderSystem.cleanup();

	textRenderSystem.update(entityIdentifier);

	EXPECT_EQ(enginePtr->addTextCallCount, 2);
	EXPECT_EQ(enginePtr->removeObjectCallCount, 1);
	EXPECT_EQ(textRenderSystem.size(), 1);
}
