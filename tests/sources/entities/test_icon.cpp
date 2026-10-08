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

#include "entities/test_icon.hpp"

#include <memory>
#include <string>

#include <utility/graphic/color.hpp>
#include <utility/graphic/text/text.hpp>

#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/glyph.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity.hpp"
#include "guillaume/ecs/entity_registry_container.hpp"
#include "guillaume/ecs/level_order_traveler.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/systems/glyph_render.hpp"
#include "guillaume/systems/measure_glyph.hpp"

#include "mocks/engine_mock.hpp"
#include "mocks/fake_system_io.hpp"

namespace guillaume::entities::tests
{
	namespace
	{
		/**
		 * @brief Path the glyph systems use to resolve the code point asset.
		 */
		const std::string GLYPH_CODEPOINTS_PATH =
			"fonts/Material_Symbols_Outlined/"
			"MaterialSymbolsOutlined[FILL,GRAD,opsz,wght].codepoints";

		/**
		 * @brief Path the glyph systems use to resolve the icon font.
		 */
		const std::string GLYPH_FONT_PATH =
			"fonts/Material_Symbols_Outlined/"
			"MaterialSymbolsOutlined-VariableFont_FILL,GRAD,opsz,wght.ttf";

		/**
		 * @brief Drives an `Icon` entity through the glyph measure/render
		 * systems, faking the Material Symbols code point asset.
		 */
		class IconEntityFixture: public TestIcon
		{
			protected:
			std::unique_ptr<guillaume::tests::EngineMock> engineMock =
				std::make_unique<guillaume::tests::EngineMock>();
			guillaume::tests::EngineMock *enginePtr = engineMock.get();
			std::unique_ptr<utility::Engine> engine { std::move(engineMock) };
			guillaume::tests::FakeSystemIO systemIo;
			std::shared_ptr<utility::RessourceProvider> ressourceProvider;
			ecs::ComponentRegistry componentRegistry;
			ecs::EntityRegistryContainer entityRegistry;
			std::shared_ptr<entities::Icon> icon;
			std::unique_ptr<guillaume::systems::MeasureGlyph> measureGlyph;
			std::unique_ptr<guillaume::systems::GlyphRender> glyphRender;

			void SetUp(void) override
			{
				systemIo.files[GLYPH_CODEPOINTS_PATH] = "home 41\n";
				systemIo.addFileFromDisk(GLYPH_FONT_PATH,
										 "fonts/Roboto-Regular.ttf");
				ressourceProvider =
					std::make_shared<utility::RessourceProvider>(systemIo);
				measureGlyph =
					std::make_unique<guillaume::systems::MeasureGlyph>(
						ressourceProvider, engine);
				glyphRender = std::make_unique<guillaume::systems::GlyphRender>(
					ressourceProvider, engine);

				icon = std::make_shared<entities::Icon>(
					componentRegistry, "home", 24.0f,
					utility::graphic::Color32Bit(255, 255, 255, 255),
					components::Glyph::Style::Outlined);
				icon->update();
				entityRegistry.addEntity(icon);

				glyphRender->bindComponentRegistry(componentRegistry);
			}

			void TearDown(void) override
			{
				glyphRender->unbindComponentRegistry();
			}
		};

	}	 // namespace

	TEST_F(IconEntityFixture, MeasureGlyphSynchronizesBoundWithEngine)
	{
		enginePtr->textMeasurement = { 40.0f, 40.0f };
		ecs::LevelOrderTraveler traveler;

		measureGlyph->routine(componentRegistry, entityRegistry, traveler);

		const auto &bound = componentRegistry.getComponent<components::Bound>(
			icon->getIdentifier());
		EXPECT_EQ(bound.getWidth(), 40U);
		EXPECT_EQ(bound.getHeight(), 40U);
		EXPECT_EQ(enginePtr->measureTextCallCount, 1);
	}

	TEST_F(IconEntityFixture, GlyphRenderCreatesTextObject)
	{
		glyphRender->update(icon->getIdentifier());

		ASSERT_EQ(enginePtr->createObjectCallCount, 1);
		EXPECT_NE(std::dynamic_pointer_cast<utility::graphic::Text>(
					  enginePtr->lastCreated()),
				  nullptr);
	}

	TEST_F(IconEntityFixture, SettersUpdateComponents)
	{
		const utility::graphic::Color32Bit color(10, 20, 30, 255);

		icon->setGlyphName("search");
		icon->setFontSize(48.0f);
		icon->setColor(color);
		icon->setStyle(components::Glyph::Style::Sharp);

		const auto &glyph = componentRegistry.getComponent<components::Glyph>(
			icon->getIdentifier());
		EXPECT_EQ(glyph.getName(), "search");
		EXPECT_FLOAT_EQ(glyph.getFontSize(), 48.0f);
		EXPECT_EQ(glyph.getStyle(), components::Glyph::Style::Sharp);

		const auto &colorComponent =
			componentRegistry.getComponent<components::Color>(
				icon->getIdentifier());
		EXPECT_EQ(colorComponent.getColor(), color);
	}

	TEST_F(TestIcon, DirectorBuildsAndRegistersIcon)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		entities::Icon::Builder builder(registry, entityRegistry);
		entities::Icon::Director director;

		auto icon =
			director.makeIcon(builder, parent, "home", 32.0f,
							  utility::graphic::Color32Bit(255, 255, 255, 255),
							  components::Glyph::Style::Outlined);
		icon->update();

		ASSERT_NE(icon, nullptr);
		EXPECT_EQ(icon->getParent(), parent);
		EXPECT_EQ(
			registry.getComponent<components::Glyph>(icon->getIdentifier())
				.getName(),
			"home");
	}

}	 // namespace guillaume::entities::tests
