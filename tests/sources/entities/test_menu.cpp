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

#include "entities/test_menu.hpp"

#include <guillaume/components/transform.hpp>

#include <guillaume/components/elevation.hpp>

#include <guillaume/theme.hpp>

#include <guillaume/components/style_state.hpp>

#include <guillaume/components/mouse_hover_interaction.hpp>

#include <guillaume/components/mouse_button_interaction.hpp>

#include <guillaume/components/color.hpp>

#include <guillaume/components/accessibility.hpp>

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/overlay.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/menu.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::vector<Menu::Item> makeItems(std::size_t count)
		{
			std::vector<Menu::Item> items;

			for (std::size_t index = 0; index < count; ++index) {
				Menu::Item item;
				item.label		 = "Item " + std::to_string(index);
				item.leadingIcon = "star";
				items.push_back(item);
			}

			return items;
		}

		std::shared_ptr<Menu>
			makeMenu(ecs::ComponentRegistry &registry,
					 const std::vector<Menu::Item> &items = {},
					 Menu::Variant variant = Menu::Variant::Dropdown)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Menu::Builder builder(registry, entityRegistry);

			return builder.makeMenu(nullptr, variant, items);
		}
	}	 // namespace

	TEST_F(TestMenu, WidthFollowsContentWithinMaterialRange)
	{
		ecs::ComponentRegistry registry;
		auto menu = makeMenu(registry, makeItems(2));

		const auto &bound =
			registry.getComponent<components::Bound>(menu->getIdentifier());
		EXPECT_GE(bound.getWidth(), Menu::MinWidth);
		EXPECT_LE(bound.getWidth(), Menu::MaxWidth);
		EXPECT_FLOAT_EQ(
			registry.getComponent<components::Bound>(menu->getItemIdentifier(0))
				.getWidth(),
			bound.getWidth());
	}

	TEST_F(TestMenu, ItemCountMatchesInput)
	{
		ecs::ComponentRegistry registry;
		auto menu = makeMenu(registry, makeItems(3));

		EXPECT_EQ(menu->getItemCount(), 3U);
	}

	TEST_F(TestMenu, ItemIdentifiersAreValidAndUnique)
	{
		ecs::ComponentRegistry registry;
		auto menu = makeMenu(registry, makeItems(2));

		const auto first  = menu->getItemIdentifier(0);
		const auto second = menu->getItemIdentifier(1);

		EXPECT_NE(first, ecs::Entity::InvalidIdentifier);
		EXPECT_NE(second, ecs::Entity::InvalidIdentifier);
		EXPECT_NE(first, second);
		EXPECT_EQ(menu->getItemIdentifier(10), ecs::Entity::InvalidIdentifier);
	}

	TEST_F(TestMenu, OverlayDismissesOnOutsideClick)
	{
		ecs::ComponentRegistry registry;
		auto menu = makeMenu(registry, makeItems(2));

		const auto &overlay =
			registry.getComponent<components::Overlay>(menu->getIdentifier());
		EXPECT_FALSE(overlay.isModal());
		EXPECT_TRUE(overlay.dismissesOnOutsideClick());
		EXPECT_TRUE(overlay.dismissesOnEscape());
	}

	TEST_F(TestMenu, OpenCloseTogglesState)
	{
		ecs::ComponentRegistry registry;
		auto menu = makeMenu(registry, makeItems(2));

		EXPECT_FALSE(menu->isOpen());
		menu->open();
		EXPECT_TRUE(menu->isOpen());
		menu->close();
		EXPECT_FALSE(menu->isOpen());
	}

	TEST_F(TestMenu, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto menu = makeMenu(registry, makeItems(1));

		menu->setVariant(Menu::Variant::Context);

		EXPECT_EQ(menu->getVariant(), Menu::Variant::Context);
	}

	TEST_F(TestMenu, BuilderPresetBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Menu::Builder builder(registry, entityRegistry);

		auto menu =
			builder.makeMenu(parent, Menu::Variant::Cascading, makeItems(4));

		ASSERT_NE(menu, nullptr);
		EXPECT_EQ(menu->getParent(), parent);
		EXPECT_EQ(menu->getVariant(), Menu::Variant::Cascading);
		EXPECT_EQ(menu->getItemCount(), 4U);
	}

	TEST_F(TestMenu, ItemsArePressableWithMenuSemantics)
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
		auto menu		  = makeMenu(registry, items);

		registry
			.getComponent<components::MouseButtonInteraction>(
				menu->getItemIdentifier(0))
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		registry
			.getComponent<components::MouseButtonInteraction>(
				menu->getItemIdentifier(1))
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_EQ(presses, 1);
		EXPECT_EQ(
			registry
				.getComponent<components::Accessibility>(menu->getIdentifier())
				.getRole(),
			components::AccessibilityRole::Menu);
		EXPECT_EQ(registry
					  .getComponent<components::Accessibility>(
						  menu->getItemIdentifier(0))
					  .getRole(),
				  components::AccessibilityRole::MenuItem);

		menu->setItemDisabled(0, true);
		registry
			.getComponent<components::MouseButtonInteraction>(
				menu->getItemIdentifier(0))
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_EQ(presses, 1);
	}

	TEST_F(TestMenu, ItemsUseBodyLargeLabelsAndDenseHeight)
	{
		ecs::ComponentRegistry registry;
		auto items	   = makeItems(2);
		items[1].dense = true;
		auto menu	   = makeMenu(registry, items);

		EXPECT_FLOAT_EQ(
			registry.getComponent<components::Bound>(menu->getItemIdentifier(0))
				.getHeight(),
			48.0f);
		EXPECT_FLOAT_EQ(
			registry.getComponent<components::Bound>(menu->getItemIdentifier(1))
				.getHeight(),
			32.0f);
	}

	TEST_F(TestMenu, AnchorPlacesTheMenuBelowOrAbove)
	{
		ecs::ComponentRegistry registry;
		ecs::Entity anchor;
		registry.registerComponentsForEntity<components::Transform,
											 components::Bound>(
			anchor.getIdentifier());
		registry.getComponent<components::Transform>(anchor.getIdentifier())
			.setPose(utility::graphic::PoseF(
				utility::graphic::PositionF(10.0f, 100.0f, 0.0f),
				utility::graphic::OrientationF()));
		registry.getComponent<components::Bound>(anchor.getIdentifier())
			.setWidth(40.0f)
			.setHeight(40.0f);

		auto menu = makeMenu(registry, makeItems(1));
		menu->setAnchor(anchor.getIdentifier());

		auto y = [&]() {
			return registry
				.getComponent<components::Transform>(menu->getIdentifier())
				.getPose()
				.getPosition()
				.getY();
		};
		EXPECT_GT(y(), 140.0f);

		menu->setAnchorPosition(Menu::AnchorPosition::Top);
		EXPECT_LT(y(), 100.0f);
		EXPECT_EQ(menu->getAnchor(), anchor.getIdentifier());
	}

	TEST_F(TestMenu, ModeControlsElevation)
	{
		ecs::ComponentRegistry registry;
		auto menu = makeMenu(registry, makeItems(1));

		const auto &elevation =
			registry.getComponent<components::Elevation>(menu->getIdentifier());
		EXPECT_EQ(elevation.getElevationLevel(), theme::ElevationLevel::Level2);
		EXPECT_TRUE(elevation.hasShadow());

		menu->setMode(Menu::Mode::Flat);
		EXPECT_FALSE(elevation.hasShadow());
	}

	TEST_F(TestMenu, DismissCallsTheHandler)
	{
		ecs::ComponentRegistry registry;
		auto menu = makeMenu(registry, makeItems(1));

		bool dismissed = false;
		menu->setOnDismiss([&dismissed]() {
			dismissed = true;
		});
		menu->show();
		menu->dismiss();

		EXPECT_TRUE(dismissed);
		EXPECT_FALSE(menu->isVisible());
	}

	TEST_F(TestMenu, ReplacingItemsRemovesThePreviousRows)
	{
		ecs::ComponentRegistry registry;
		auto menu = makeMenu(registry, makeItems(3));
		const auto previous =
			registry.getComponent<components::Menu>(menu->getIdentifier())
				.getRows();
		ASSERT_EQ(previous.size(), 3U);

		menu->setItems(makeItems(2));

		for (const auto &row: previous) {
			EXPECT_FALSE(
				registry.hasComponent<components::Transform>(row.view));
			EXPECT_FALSE(
				registry.hasComponent<components::Hierarchy>(row.view));
			for (const auto content: row.content) {
				EXPECT_FALSE(
					registry.hasComponent<components::Transform>(content));
			}
		}
		// Each row owns a container, an icon and a label.
		EXPECT_EQ(menu->getEntities().size(), 6U);
		EXPECT_EQ(
			registry.getComponent<components::Hierarchy>(menu->getIdentifier())
				.getChildIdentifiers()
				.size(),
			2U);
	}

}	 // namespace guillaume::entities::tests
