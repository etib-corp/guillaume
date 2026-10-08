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

#include "entities/test_search.hpp"

#include <memory>
#include <string>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/overlay.hpp>
#include <guillaume/components/text_field.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<Search>
			makeSearch(ecs::ComponentRegistry &registry,
					   Search::Variant variant = Search::Variant::SearchBar,
					   const std::string &text = "query")
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Search::Builder builder(registry, entityRegistry);
			Search::Director director;

			return director.makeSearch(builder, nullptr, variant, "Search",
									   text);
		}
	}	 // namespace

	TEST_F(TestSearch, SearchBarIsPill)
	{
		ecs::ComponentRegistry registry;
		auto search = makeSearch(registry, Search::Variant::SearchBar);

		search->initialize();
		search->update();

		const auto &bound =
			registry.getComponent<components::Bound>(search->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 56.0f);

		const auto &borders =
			registry.getComponent<components::Borders>(search->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 28.0f);
	}

	TEST_F(TestSearch, FullScreenIsTallAndUnrounded)
	{
		ecs::ComponentRegistry registry;
		auto search = makeSearch(registry, Search::Variant::FullScreen);

		search->initialize();
		search->update();

		const auto &bound =
			registry.getComponent<components::Bound>(search->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 640.0f);

		const auto &borders =
			registry.getComponent<components::Borders>(search->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 0.0f);
	}

	TEST_F(TestSearch, TextFieldComponentCarriesPlaceholder)
	{
		ecs::ComponentRegistry registry;
		auto search = makeSearch(registry);

		search->initialize();
		search->setPlaceholder("Find");

		const auto &textField = registry.getComponent<components::TextField>(
			search->getIdentifier());
		EXPECT_EQ(textField.getPlaceholder(), "Find");
	}

	TEST_F(TestSearch, TextRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto search = makeSearch(registry);

		search->initialize();
		search->setText("hello");

		EXPECT_EQ(search->getText(), "hello");
	}

	TEST_F(TestSearch, FullScreenOverlayIsModal)
	{
		ecs::ComponentRegistry registry;
		auto search = makeSearch(registry, Search::Variant::FullScreen);

		search->initialize();
		search->update();

		const auto &overlay =
			registry.getComponent<components::Overlay>(search->getIdentifier());
		EXPECT_TRUE(overlay.isModal());
		EXPECT_TRUE(overlay.dismissesOnOutsideClick());
	}

	TEST_F(TestSearch, OpenCloseTogglesState)
	{
		ecs::ComponentRegistry registry;
		auto search = makeSearch(registry);

		search->initialize();

		EXPECT_FALSE(search->isOpen());
		search->open();
		EXPECT_TRUE(search->isOpen());
		search->close();
		EXPECT_FALSE(search->isOpen());
	}

	TEST_F(TestSearch, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Search::Builder builder(registry, entityRegistry);
		Search::Director director;

		auto search = director.makeSearch(
			builder, parent, Search::Variant::FullScreen, "Search", "query");

		ASSERT_NE(search, nullptr);
		EXPECT_EQ(search->getParent(), parent);
		EXPECT_EQ(search->getVariant(), Search::Variant::FullScreen);
	}

}	 // namespace guillaume::entities::tests
