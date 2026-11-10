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

#include "entities/test_text.hpp"

#include <memory>

#include <utility/graphic/color.hpp>
#include <utility/graphic/text/text.hpp>
#include <utility/system_io/default_system_io.hpp>

#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity.hpp"
#include "guillaume/ecs/entity_registry_container.hpp"
#include "guillaume/ecs/level_order_traveler.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/systems/measure_text.hpp"
#include "guillaume/systems/text_render.hpp"
#include "guillaume/theme.hpp"

#include "mocks/engine_mock.hpp"

namespace guillaume::entities::tests
{
	namespace
	{
		/**
		 * @brief Drives a `Text` entity through the text measure/render
		 * systems.
		 */
		class TextEntityFixture: public TestText
		{
			protected:
			std::unique_ptr<guillaume::tests::EngineMock> engineMock =
				std::make_unique<guillaume::tests::EngineMock>();
			guillaume::tests::EngineMock *enginePtr = engineMock.get();
			std::unique_ptr<utility::Engine> engine { std::move(engineMock) };
			utility::DefaultSystemIO systemIo;
			std::shared_ptr<utility::RessourceProvider> ressourceProvider;
			ecs::ComponentRegistry componentRegistry;
			ecs::EntityRegistryContainer entityRegistry;
			std::shared_ptr<entities::Text> text;
			std::unique_ptr<guillaume::systems::MeasureText> measureText;
			std::unique_ptr<guillaume::systems::TextRender> textRender;

			void SetUp(void) override
			{
				ressourceProvider =
					std::make_shared<utility::RessourceProvider>(systemIo);
				measureText = std::make_unique<guillaume::systems::MeasureText>(
					ressourceProvider, *engine);
				textRender = std::make_unique<guillaume::systems::TextRender>(
					ressourceProvider, *engine);

				text = std::make_shared<entities::Text>(
					componentRegistry, "Hello", 24.0f,
					utility::graphic::Color32Bit(255, 255, 255, 255));
				text->update();
				entityRegistry.addEntity(text);

				textRender->bindComponentRegistry(componentRegistry);
			}

			void TearDown(void) override
			{
				textRender->unbindComponentRegistry();
			}
		};

	}	 // namespace

	TEST_F(TextEntityFixture, MeasureTextSynchronizesBoundWithEngine)
	{
		enginePtr->textMeasurement = { 120.0f, 30.0f };
		ecs::LevelOrderTraveler traveler;

		measureText->routine(componentRegistry, entityRegistry, traveler);

		const auto &bound = componentRegistry.getComponent<components::Bound>(
			text->getIdentifier());
		EXPECT_EQ(bound.getWidth(), 120U);
		EXPECT_EQ(bound.getHeight(), 30U);
		EXPECT_EQ(enginePtr->measureTextCallCount, 1);
		EXPECT_EQ(enginePtr->lastMeasuredContent, "Hello");
	}

	TEST_F(TextEntityFixture, TextRenderCreatesTextObject)
	{
		textRender->update(text->getIdentifier());

		ASSERT_EQ(enginePtr->createObjectCallCount, 1);
		const auto created = std::dynamic_pointer_cast<utility::graphic::Text>(
			enginePtr->lastCreated());
		ASSERT_NE(created, nullptr);
		EXPECT_EQ(created->getContent(), "Hello");
	}

	TEST_F(TextEntityFixture, SettersUpdateComponents)
	{
		const utility::graphic::Color32Bit color(1, 2, 3, 255);

		text->setContent("World");
		text->setFontSize(40.0f);
		text->setColor(color);

		const auto &textComponent =
			componentRegistry.getComponent<components::Text>(
				text->getIdentifier());
		EXPECT_EQ(textComponent.getContent(), "World");
		EXPECT_FLOAT_EQ(textComponent.getFontSize(), 40.0f);

		const auto &colorComponent =
			componentRegistry.getComponent<components::Color>(
				text->getIdentifier());
		EXPECT_EQ(colorComponent.getColor(), color);
	}

	TEST_F(TestText, BuilderPresetBuildsAndRegistersText)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		entities::Text::Builder builder(registry, entityRegistry);

		auto text =
			builder.makeText(parent, "Hello", 18.0f,
							 utility::graphic::Color32Bit(255, 255, 255, 255));
		text->update();

		ASSERT_NE(text, nullptr);
		EXPECT_EQ(text->getParent(), parent);
		EXPECT_EQ(registry.getComponent<components::Text>(text->getIdentifier())
					  .getContent(),
				  "Hello");
	}

	TEST_F(TestText, BuilderDefaultsToBodyMediumOnSurface)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		entities::Text::Builder builder(registry, entityRegistry);
		auto text = builder.withContent("Hello").registerEntity(parent);
		text->update();

		const auto &token = getTypeToken(theme::TypeRole::BodyMedium);
		const auto &component =
			registry.getComponent<components::Text>(text->getIdentifier());
		EXPECT_EQ(text->getTypeRole(), theme::TypeRole::BodyMedium);
		EXPECT_FLOAT_EQ(component.getFontSize(), token.fontSize);
		EXPECT_FLOAT_EQ(component.getLineHeight(), token.lineHeight);
		EXPECT_FLOAT_EQ(component.getLetterSpacing(), token.letterSpacing);
		EXPECT_EQ(
			registry.getComponent<components::Color>(text->getIdentifier())
				.getColor(),
			getActiveScheme().getColor(SchemeColorRole::OnSurface).getColor());
	}

	TEST_F(TestText, TypeRoleSelectsWeightAndSize)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		entities::Text::Builder builder(registry, entityRegistry);
		auto text =
			builder.makeText(parent, "Save", theme::TypeRole::LabelLarge,
							 utility::graphic::Color32Bit(0, 0, 0, 255));
		text->update();

		const auto &component =
			registry.getComponent<components::Text>(text->getIdentifier());
		EXPECT_FLOAT_EQ(component.getFontSize(),
						getTypeToken(theme::TypeRole::LabelLarge).fontSize);
		EXPECT_EQ(component.getFontWeight(), theme::FontWeight::Medium);

		text->setTypeRole(theme::TypeRole::HeadlineSmall);
		EXPECT_FLOAT_EQ(component.getFontSize(),
						getTypeToken(theme::TypeRole::HeadlineSmall).fontSize);
		EXPECT_EQ(component.getFontWeight(), theme::FontWeight::Regular);
	}

	TEST_F(TestText, ExplicitSizeOverridesRole)
	{
		ecs::ComponentRegistry registry;
		auto text = std::make_shared<entities::Text>(
			registry, "Hi", theme::TypeRole::TitleMedium,
			utility::graphic::Color32Bit(0, 0, 0, 255), 30.0f);
		text->update();

		const auto &component =
			registry.getComponent<components::Text>(text->getIdentifier());
		EXPECT_FLOAT_EQ(component.getFontSize(), 30.0f);
		EXPECT_EQ(component.getFontWeight(), theme::FontWeight::Medium);
	}

	TEST_F(TextEntityFixture, MeasureTextHonoursRoleLineHeight)
	{
		text->setTypeRole(theme::TypeRole::DisplayLarge);
		measureText->bindComponentRegistry(componentRegistry);
		measureText->update(text->getIdentifier());

		EXPECT_GE(componentRegistry
					  .getComponent<components::Bound>(text->getIdentifier())
					  .getHeight(),
				  64.0f);
	}

}	 // namespace guillaume::entities::tests
