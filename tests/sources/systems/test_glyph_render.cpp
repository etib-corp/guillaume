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
#include <string>

#include <utility/system_io/default_system_io.hpp>

#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/glyph.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/ecs/component_registry.hpp"

#include "mocks/engine_mock.hpp"
#include "mocks/fake_system_io.hpp"
#include "systems/test_glyph_render.hpp"

#include "guillaume/systems/measure_glyph.hpp"

namespace
{
	const std::string GLYPH_FONT_PATH =
		"fonts/Material_Symbols_Outlined/"
		"MaterialSymbolsOutlined-VariableFont_FILL,GRAD,opsz,wght.ttf";

	const std::string GLYPH_CODEPOINTS_PATH =
		"fonts/Material_Symbols_Outlined/"
		"MaterialSymbolsOutlined[FILL,GRAD,opsz,wght].codepoints";

	class GlyphRenderFixture: public guillaume::systems::tests::TestGlyphRender
	{
		protected:
		std::unique_ptr<guillaume::tests::EngineMock> engine =
			std::make_unique<guillaume::tests::EngineMock>();
		guillaume::tests::FakeSystemIO systemIo;
		std::shared_ptr<utility::RessourceProvider> ressourceProvider;
		std::unique_ptr<guillaume::systems::GlyphRender> glyphRender;
		guillaume::ecs::ComponentRegistry componentRegistry;
		guillaume::ecs::Entity::Identifier entityIdentifier { 1 };

		void SetUp(void) override
		{
			// The library does not bundle Material Symbols: fake the code
			// point table and stand Roboto in for the icon font.
			systemIo.files[GLYPH_CODEPOINTS_PATH] = "home 41\n";
			systemIo.addFileFromDisk(GLYPH_FONT_PATH,
									 "fonts/Roboto-Regular.ttf");
			ressourceProvider =
				std::make_shared<utility::RessourceProvider>(systemIo);
			glyphRender = std::make_unique<guillaume::systems::GlyphRender>(
				ressourceProvider, *engine);

			componentRegistry.addComponent<guillaume::components::Transform>(
				entityIdentifier);
			componentRegistry.addComponent<guillaume::components::Bound>(
				entityIdentifier);
			componentRegistry.addComponent<guillaume::components::Glyph>(
				entityIdentifier);
			componentRegistry.addComponent<guillaume::components::Color>(
				entityIdentifier);

			componentRegistry
				.getComponent<guillaume::components::Glyph>(entityIdentifier)
				.setName("home")
				.setFontSize(24.0f);

			glyphRender->bindComponentRegistry(componentRegistry);
		}

		void TearDown(void) override
		{
			glyphRender->unbindComponentRegistry();
		}
	};

}	 // namespace

TEST_F(GlyphRenderFixture, CreatesOneRenderObjectForAnEntity)
{
	glyphRender->update(entityIdentifier);

	EXPECT_EQ(engine->createObjectCallCount, 1);
	EXPECT_EQ(engine->removeObjectCallCount, 0);
	EXPECT_EQ(glyphRender->size(), 1U);
}

TEST_F(GlyphRenderFixture, UpdatesTheSameRenderObjectOnLaterFrames)
{
	glyphRender->update(entityIdentifier);
	glyphRender->update(entityIdentifier);

	EXPECT_EQ(engine->createObjectCallCount, 1);
	EXPECT_EQ(engine->updateObjectCallCount, 1);
	EXPECT_EQ(glyphRender->size(), 1U);
}

TEST_F(GlyphRenderFixture, UnknownGlyphNameStillRenders)
{
	componentRegistry
		.getComponent<guillaume::components::Glyph>(entityIdentifier)
		.setName("definitely-not-a-material-symbol");

	EXPECT_NO_THROW(glyphRender->update(entityIdentifier));
	EXPECT_EQ(engine->createObjectCallCount, 1);
}

TEST_F(GlyphRenderFixture, SkipsEntityMissingARequiredComponent)
{
	const guillaume::ecs::Entity::Identifier incomplete { 2 };
	componentRegistry.addComponent<guillaume::components::Glyph>(incomplete);

	glyphRender->update(incomplete);

	EXPECT_EQ(engine->createObjectCallCount, 0);
	EXPECT_EQ(glyphRender->size(), 0U);
}

TEST_F(GlyphRenderFixture, CleanupRemovesEntitiesNotUpdatedThisFrame)
{
	glyphRender->update(entityIdentifier);

	// Next frame: the entity is not visited between prepare and cleanup.
	glyphRender->prepare();
	glyphRender->cleanup();

	EXPECT_EQ(engine->removeObjectCallCount, 1);
	EXPECT_EQ(glyphRender->size(), 0U);
}

TEST_F(GlyphRenderFixture, CleanupKeepsEntitiesUpdatedThisFrame)
{
	glyphRender->update(entityIdentifier);

	glyphRender->prepare();
	glyphRender->update(entityIdentifier);
	glyphRender->cleanup();

	EXPECT_EQ(engine->removeObjectCallCount, 0);
	EXPECT_EQ(glyphRender->size(), 1U);
}

TEST_F(GlyphRenderFixture, RendersNothingWhenTheIconFontIsMissing)
{
	// A resource root without the Material Symbols assets.
	guillaume::tests::FakeSystemIO emptyIo;
	auto emptyProvider = std::make_shared<utility::RessourceProvider>(emptyIo);
	guillaume::systems::GlyphRender fontless { emptyProvider, *engine };
	fontless.bindComponentRegistry(componentRegistry);

	EXPECT_NO_THROW(fontless.update(entityIdentifier));

	fontless.unbindComponentRegistry();

	EXPECT_EQ(engine->createObjectCallCount, 0);
	EXPECT_EQ(fontless.size(), 0U);
}

TEST_F(GlyphRenderFixture, MeasuresNothingWhenTheIconFontIsMissing)
{
	guillaume::tests::FakeSystemIO emptyIo;
	auto emptyProvider = std::make_shared<utility::RessourceProvider>(emptyIo);
	guillaume::systems::MeasureGlyph fontless { emptyProvider, *engine };
	fontless.bindComponentRegistry(componentRegistry);

	auto &bound = componentRegistry.getComponent<guillaume::components::Bound>(
		entityIdentifier);
	bound.setWidth(12.0f).setHeight(34.0f);

	EXPECT_NO_THROW(fontless.update(entityIdentifier));

	fontless.unbindComponentRegistry();

	// The bound is left as it was.
	EXPECT_FLOAT_EQ(bound.getWidth(), 12.0f);
	EXPECT_FLOAT_EQ(bound.getHeight(), 34.0f);
}
