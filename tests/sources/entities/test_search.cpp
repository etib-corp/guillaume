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

#include <guillaume/components/line.hpp>

#include <guillaume/components/layout.hpp>

#include <guillaume/components/glyph.hpp>

#include <guillaume/components/elevation.hpp>

#include <algorithm>

#include <guillaume/theme.hpp>

#include <guillaume/components/style_state.hpp>

#include <guillaume/components/mouse_hover_interaction.hpp>

#include <guillaume/components/mouse_button_interaction.hpp>

#include <guillaume/components/color.hpp>

#include <guillaume/components/accessibility.hpp>

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

			return builder.makeSearch(nullptr, variant, "Search", text);
		}
	}	 // namespace

	TEST_F(TestSearch, SearchBarIsPill)
	{
		ecs::ComponentRegistry registry;
		auto search = makeSearch(registry, Search::Variant::SearchBar);

		search->initialize();

		const auto &bound =
			registry.getComponent<components::Bound>(search->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 56.0f);

		const auto &borders =
			registry.getComponent<components::Borders>(search->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(),
						getShapeRadius(theme::ShapeRole::ExtraLarge));
	}

	TEST_F(TestSearch, ViewModeIsInlineWithDivider)
	{
		ecs::ComponentRegistry registry;
		auto search = makeSearch(registry, Search::Variant::FullScreen);

		search->initialize();

		const auto &bound =
			registry.getComponent<components::Bound>(search->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 72.0f);

		const auto &borders =
			registry.getComponent<components::Borders>(search->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(),
						getShapeRadius(theme::ShapeRole::None));

		const auto &line =
			registry.getComponent<components::Line>(search->getIdentifier());
		EXPECT_FLOAT_EQ(line.getThickness(), 1.0f);
		EXPECT_EQ(line.getPlacement(), components::Line::Placement::Bottom);
		EXPECT_EQ(line.getColor(),
				  getActiveScheme()
					  .getColor(SchemeColorRole::OutlineVariant)
					  .getColor());
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

	TEST_F(TestSearch, ViewModeIsNotModal)
	{
		ecs::ComponentRegistry registry;
		auto search = makeSearch(registry, Search::Variant::FullScreen);

		search->initialize();

		const auto &overlay =
			registry.getComponent<components::Overlay>(search->getIdentifier());
		EXPECT_FALSE(overlay.isModal());
		EXPECT_FALSE(overlay.dismissesOnOutsideClick());
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

	TEST_F(TestSearch, BuilderPresetBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Search::Builder builder(registry, entityRegistry);

		auto search = builder.makeSearch(parent, Search::Variant::FullScreen,
										 "Search", "query");

		ASSERT_NE(search, nullptr);
		EXPECT_EQ(search->getParent(), parent);
		EXPECT_EQ(search->getVariant(), Search::Variant::FullScreen);
	}

	TEST_F(TestSearch, ClearIconFollowsTheQuery)
	{
		ecs::ComponentRegistry registry;
		auto search = makeSearch(registry);
		search->initialize();
		search->setText("query");

		bool cleared = false;
		search->setOnClearIconPress([&cleared]() {
			cleared = true;
		});

		const auto clearIdentifier = search->getClearIdentifier();
		ASSERT_NE(clearIdentifier, ecs::Entity::InvalidIdentifier);
		EXPECT_GT(registry.getComponent<components::Color>(clearIdentifier)
					  .getColor()
					  .getAlpha(),
				  0);

		registry
			.getComponent<components::MouseButtonInteraction>(clearIdentifier)
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_TRUE(cleared);
		EXPECT_EQ(search->getText(), "");
		EXPECT_EQ(registry.getComponent<components::Color>(clearIdentifier)
					  .getColor()
					  .getAlpha(),
				  0);
		EXPECT_FALSE(
			registry.getComponent<components::StyleState>(clearIdentifier)
				.isEnabled());
	}

	TEST_F(TestSearch, LeadingIconIsReplaceableAndPressable)
	{
		ecs::ComponentRegistry registry;
		auto search = makeSearch(registry);
		search->initialize();

		int presses = 0;
		search->setOnIconPress([&presses]() {
			++presses;
		});
		search->setIcon("arrow_back");
		const auto icon = search->getIconIdentifier();
		registry.getComponent<components::MouseButtonInteraction>(icon)
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_EQ(presses, 1);
		EXPECT_EQ(registry.getComponent<components::Glyph>(icon).getName(),
				  "arrow_back");
		EXPECT_EQ(
			registry.getComponent<components::Accessibility>(icon).getRole(),
			components::AccessibilityRole::Button);
	}

	TEST_F(TestSearch, PlaceholderShowsWhileEmpty)
	{
		ecs::ComponentRegistry registry;
		auto search = makeSearch(registry);
		search->initialize();
		search->setText("");

		const auto &children =
			registry.getComponent<components::Layout>(search->getIdentifier())
				.getChildren();
		EXPECT_NE(std::find(children.begin(), children.end(),
							search->getPlaceholderIdentifier()),
				  children.end());
		EXPECT_EQ(registry
					  .getComponent<components::Color>(
						  search->getPlaceholderIdentifier())
					  .getColor(),
				  getActiveScheme()
					  .getColor(SchemeColorRole::OnSurfaceVariant)
					  .getColor());
	}

	TEST_F(TestSearch, LoadingAndElevation)
	{
		ecs::ComponentRegistry registry;
		auto search = makeSearch(registry);
		search->initialize();
		search->setLoading(true);
		search->setElevation(theme::ElevationLevel::Level3);

		EXPECT_TRUE(search->isLoading());
		EXPECT_EQ(
			registry
				.getComponent<components::Elevation>(search->getIdentifier())
				.getElevationLevel(),
			theme::ElevationLevel::Level3);
		EXPECT_EQ(
			registry.getComponent<components::Color>(search->getIdentifier())
				.getColor(),
			getElevatedSurfaceColor(theme::ElevationLevel::Level3));
	}

	TEST_F(TestSearch, ExposesSearchSemantics)
	{
		ecs::ComponentRegistry registry;
		auto search = makeSearch(registry);
		search->initialize();

		EXPECT_EQ(registry
					  .getComponent<components::Accessibility>(
						  search->getIdentifier())
					  .getRole(),
				  components::AccessibilityRole::Search);
		EXPECT_EQ(registry
					  .getComponent<components::Accessibility>(
						  search->getClearIdentifier())
					  .getLabel(),
				  "clear search");
	}

}	 // namespace guillaume::entities::tests
