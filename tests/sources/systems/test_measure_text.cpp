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

#include <vector>

#include <utility/graphic/text/text.hpp>
#include <utility/system_io/default_system_io.hpp>

#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_registry.hpp"
#include "guillaume/ecs/entity_registry_container.hpp"
#include "guillaume/ecs/level_order_traveler.hpp"

#include "systems/test_measure_text.hpp"

namespace
{
	class EngineStub: public guillaume::Engine
	{
		public:
		mutable utility::math::Vector2F measurement = { 0.0f, 0.0f };
		mutable std::size_t measureCallCount		= 0;
		mutable std::string lastContent;

		ViewportSize getViewportSize(void) const override
		{
			return { 800.0f, 600.0f };
		}
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
			(void)objectID;
			return true;
		}
		utility::math::Vector2F
			measureText(const utility::graphic::Text &text) const override
		{
			++measureCallCount;
			lastContent = text.getContent();
			return measurement;
		}
		size_t addText(const utility::graphic::Text &) override
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

	class MeasureTextFixture: public guillaume::systems::tests::TestMeasureText
	{
		protected:
		std::unique_ptr<EngineStub> engineStub = std::make_unique<EngineStub>();
		EngineStub *enginePtr				   = engineStub.get();
		std::unique_ptr<guillaume::Engine> engine { std::move(engineStub) };
		utility::DefaultSystemIO systemIo;
		std::shared_ptr<utility::RessourceProvider> ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);
		guillaume::systems::MeasureText measureTextSystem { ressourceProvider,
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
					guillaume::components::Text, guillaume::components::Bound,
					guillaume::components::Color>());
			entityRegistry.addEntity(std::move(entity));

			componentRegistry.addComponent<guillaume::components::Transform>(
				entityIdentifier);
			componentRegistry.addComponent<guillaume::components::Text>(
				entityIdentifier);
			componentRegistry.addComponent<guillaume::components::Bound>(
				entityIdentifier);
		}
	};

}	 // namespace

TEST_F(MeasureTextFixture, SynchronizesBoundSizeWithMeasuredText)
{
	componentRegistry
		.getComponent<guillaume::components::Text>(entityIdentifier)
		.setContent("Measure me")
		.setFontSize(32);
	enginePtr->measurement = { 140.0f, 28.0f };
	guillaume::ecs::LevelOrderTraveler traveler;

	measureTextSystem.routine(componentRegistry, entityRegistry, traveler);

	const auto &bound =
		componentRegistry.getComponent<guillaume::components::Bound>(
			entityIdentifier);
	EXPECT_EQ(bound.getWidth(), 140U);
	EXPECT_EQ(bound.getHeight(), 28U);
	EXPECT_EQ(enginePtr->measureCallCount, 1);
	EXPECT_EQ(enginePtr->lastContent, "Measure me");
}

TEST_F(MeasureTextFixture, SkipsMeasurementWhenRequiredComponentIsMissing)
{
	componentRegistry.removeComponent<guillaume::components::Bound>(
		entityIdentifier);
	guillaume::ecs::LevelOrderTraveler traveler;

	measureTextSystem.routine(componentRegistry, entityRegistry, traveler);

	EXPECT_EQ(enginePtr->measureCallCount, 0);
}

namespace guillaume::systems::tests
{
}	 // namespace guillaume::systems::tests
