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

#include "entities/test_list.hpp"

#include <guillaume/components/text.hpp>

#include <guillaume/components/style_state.hpp>

#include <guillaume/components/mouse_hover_interaction.hpp>

#include <guillaume/components/mouse_button_interaction.hpp>

#include <guillaume/components/accessibility.hpp>

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>
#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/theme.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/list.hpp>
#include <guillaume/components/transform.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::vector<List::Item> makeItems(std::size_t count)
		{
			std::vector<List::Item> items;

			for (std::size_t index = 0; index < count; ++index) {
				List::Item item;
				item.headline = "Item " + std::to_string(index);
				items.push_back(item);
			}

			return items;
		}

		std::shared_ptr<List>
			makeList(ecs::ComponentRegistry &registry,
					 List::Variant variant = List::Variant::SingleLine,
					 const std::vector<List::Item> &items = {})
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			List::Builder builder(registry, entityRegistry);

			return builder.makeList(nullptr, variant, items);
		}
	}	 // namespace

	TEST_F(TestList, DefaultWidthIsThreeHundredSixty)
	{
		ecs::ComponentRegistry registry;
		auto list = makeList(registry);

		list->initialize();

		const auto &bound =
			registry.getComponent<components::Bound>(list->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 360.0f);
	}

	TEST_F(TestList, SingleLineItemsStackVertically)
	{
		ecs::ComponentRegistry registry;
		auto list = makeList(registry, List::Variant::SingleLine, makeItems(2));

		list->initialize();

		// Heights follow the content: each row is its content column plus
		// the 16 dp M3 padding on both edges.
		float rows = 0.0f;
		for (std::size_t index = 0; index < 2; ++index) {
			const auto &row = registry.getComponent<components::Bound>(
				list->getItemIdentifier(index));
			const auto &content = registry.getComponent<components::Bound>(
				list->getContentIdentifier(index));
			EXPECT_FLOAT_EQ(row.getHeight(),
							content.getHeight()
								+ 2.0f * theme::SpacingScale::get(4));
			rows += row.getHeight();
		}

		const auto &bound =
			registry.getComponent<components::Bound>(list->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), rows);
	}

	TEST_F(TestList, ItemCountMatchesInput)
	{
		ecs::ComponentRegistry registry;
		auto list = makeList(registry, List::Variant::SingleLine, makeItems(3));

		EXPECT_EQ(list->getItemCount(), 3U);
	}

	TEST_F(TestList, ItemIdentifiersAreValidAndUnique)
	{
		ecs::ComponentRegistry registry;
		auto list = makeList(registry, List::Variant::SingleLine, makeItems(2));

		list->initialize();

		const auto first  = list->getItemIdentifier(0);
		const auto second = list->getItemIdentifier(1);

		EXPECT_NE(first, ecs::Entity::InvalidIdentifier);
		EXPECT_NE(second, ecs::Entity::InvalidIdentifier);
		EXPECT_NE(first, second);
		EXPECT_EQ(list->getItemIdentifier(10), ecs::Entity::InvalidIdentifier);
	}

	TEST_F(TestList, VariantControlsTheDescription)
	{
		ecs::ComponentRegistry registry;
		auto items			= makeItems(1);
		items[0].supporting = "Details";
		auto list = makeList(registry, List::Variant::SingleLine, items);

		list->initialize();
		EXPECT_EQ(
			registry
				.getComponent<components::Layout>(list->getContentIdentifier(0))
				.getChildren()
				.size(),
			1U);

		list->setVariant(List::Variant::TwoLine);
		EXPECT_EQ(list->getVariant(), List::Variant::TwoLine);
		EXPECT_EQ(
			registry
				.getComponent<components::Layout>(list->getContentIdentifier(0))
				.getChildren()
				.size(),
			2U);
	}

	TEST_F(TestList, BuilderPresetBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		List::Builder builder(registry, entityRegistry);

		auto list =
			builder.makeList(parent, List::Variant::TwoLine, makeItems(4));

		ASSERT_NE(list, nullptr);
		EXPECT_EQ(list->getParent(), parent);
		EXPECT_EQ(list->getVariant(), List::Variant::TwoLine);
		EXPECT_EQ(list->getItemCount(), 4U);
	}

	TEST_F(TestList, RowsArePressableAndDisabledRowsAreInert)
	{
		ecs::ComponentRegistry registry;
		int presses		 = 0;
		auto items		 = makeItems(2);
		items[0].onPress = [&presses]() {
			++presses;
		};
		items[1].onPress = [&presses]() {
			++presses;
		};
		items[1].disabled = true;
		auto list = makeList(registry, List::Variant::SingleLine, items);
		list->initialize();

		registry
			.getComponent<components::MouseButtonInteraction>(
				list->getItemIdentifier(0))
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		registry
			.getComponent<components::MouseButtonInteraction>(
				list->getItemIdentifier(1))
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_EQ(presses, 1);
		EXPECT_EQ(registry
					  .getComponent<components::Accessibility>(
						  list->getItemIdentifier(0))
					  .getRole(),
				  components::AccessibilityRole::ListItem);
		EXPECT_FALSE(registry
						 .getComponent<components::StyleState>(
							 list->getItemIdentifier(1))
						 .isEnabled());
	}

	TEST_F(TestList, HoverPaintsTheStateLayer)
	{
		ecs::ComponentRegistry registry;
		auto list = makeList(registry, List::Variant::SingleLine, makeItems(1));
		list->initialize();

		const auto row = list->getItemIdentifier(0);
		EXPECT_EQ(
			registry.getComponent<components::Color>(row).getColor().getAlpha(),
			0);

		auto &hover =
			registry.getComponent<components::MouseHoverInteraction>(row);
		hover.setHovered(true);
		hover.getOnHoverHandler()();
		EXPECT_EQ(
			registry.getComponent<components::Color>(row).getColor().getAlpha(),
			getStateLayerAlpha(theme::StateLayer::Hovered));
	}

	TEST_F(TestList, SubheadersAndDividersStructureSections)
	{
		ecs::ComponentRegistry registry;
		std::vector<List::Item> items { List::subheader("Recent") };
		auto rows = makeItems(1);
		items.insert(items.end(), rows.begin(), rows.end());
		items.push_back(List::divider());
		auto list = makeList(registry, List::Variant::SingleLine, items);
		list->initialize();

		const auto header = list->getItemIdentifier(0);
		EXPECT_EQ(
			registry.getComponent<components::Accessibility>(header).getRole(),
			components::AccessibilityRole::Header);
		EXPECT_FLOAT_EQ(
			registry.getComponent<components::Text>(header).getFontSize(),
			getTypeToken(theme::TypeRole::TitleSmall).fontSize);
		EXPECT_NE(list->getItemIdentifier(2), ecs::Entity::InvalidIdentifier);
		EXPECT_FALSE(registry.hasComponent<components::Accessibility>(
			list->getItemIdentifier(2)));
	}

	TEST_F(TestList, AccordionTogglesItsChildren)
	{
		ecs::ComponentRegistry registry;
		List::Item accordion;
		accordion.headline = "Folders";
		accordion.children = makeItems(2);
		auto list =
			makeList(registry, List::Variant::SingleLine, { accordion });
		list->initialize();

		const auto row	= list->getItemIdentifier(0);
		auto childCount = [&]() {
			return registry
				.getComponent<components::Layout>(list->getIdentifier())
				.getChildren()
				.size();
		};
		EXPECT_EQ(childCount(), 1U);
		EXPECT_EQ(
			registry.getComponent<components::Accessibility>(row).getExpanded(),
			false);

		registry.getComponent<components::MouseButtonInteraction>(row)
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_TRUE(list->isExpanded(0));
		EXPECT_EQ(childCount(), 3U);
		EXPECT_EQ(
			registry.getComponent<components::Accessibility>(row).getExpanded(),
			true);
		EXPECT_TRUE(registry
						.getComponent<components::StyleState>(
							list->getChildIdentifier(0, 1))
						.isEnabled());

		list->setExpanded(0, false);
		EXPECT_EQ(childCount(), 1U);
		EXPECT_FALSE(registry
						 .getComponent<components::StyleState>(
							 list->getChildIdentifier(0, 1))
						 .isEnabled());
	}

	TEST_F(TestList, ReplacingItemsRemovesThePreviousRows)
	{
		ecs::ComponentRegistry registry;
		auto list = makeList(registry, List::Variant::SingleLine, makeItems(3));
		list->initialize();
		const auto previous =
			registry.getComponent<components::List>(list->getIdentifier())
				.getRows();
		ASSERT_FALSE(previous.empty());

		list->setItems(makeItems(1));

		for (const auto &row: previous) {
			EXPECT_FALSE(
				registry.hasComponent<components::Transform>(row.entity));
		}
		const auto &hierarchy =
			registry.getComponent<components::Hierarchy>(list->getIdentifier());
		for (const auto &row: previous) {
			EXPECT_FALSE(hierarchy.hasChild(row.entity));
		}
	}

}	 // namespace guillaume::entities::tests
