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

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/overlay.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

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
			Menu::Director director;

			return director.makeMenu(builder, nullptr, variant, items);
		}
	}	 // namespace

	TEST_F(TestMenu, DefaultWidthIsTwoHundred)
	{
		ecs::ComponentRegistry registry;
		auto menu = makeMenu(registry, makeItems(2));

		menu->initialize();
		menu->update();

		const auto &bound =
			registry.getComponent<components::Bound>(menu->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 200.0f);
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

		menu->initialize();
		menu->update();

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

		menu->initialize();
		menu->update();

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

		menu->initialize();

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

	TEST_F(TestMenu, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Menu::Builder builder(registry, entityRegistry);
		Menu::Director director;

		auto menu = director.makeMenu(builder, parent, Menu::Variant::Cascading,
									  makeItems(4));

		ASSERT_NE(menu, nullptr);
		EXPECT_EQ(menu->getParent(), parent);
		EXPECT_EQ(menu->getVariant(), Menu::Variant::Cascading);
		EXPECT_EQ(menu->getItemCount(), 4U);
	}

}	 // namespace guillaume::entities::tests
