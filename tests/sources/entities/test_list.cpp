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
			List::Director director;

			return director.makeList(builder, nullptr, variant, items);
		}
	}	 // namespace

	TEST_F(TestList, DefaultWidthIsThreeHundredSixty)
	{
		ecs::ComponentRegistry registry;
		auto list = makeList(registry);

		list->initialize();
		list->update();

		const auto &bound =
			registry.getComponent<components::Bound>(list->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 360.0f);
	}

	TEST_F(TestList, SingleLineItemsStackVertically)
	{
		ecs::ComponentRegistry registry;
		auto list = makeList(registry, List::Variant::SingleLine, makeItems(2));

		list->initialize();
		list->update();

		const auto &bound =
			registry.getComponent<components::Bound>(list->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 96.0f);
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
		list->update();

		const auto first  = list->getItemIdentifier(0);
		const auto second = list->getItemIdentifier(1);

		EXPECT_NE(first, ecs::Entity::InvalidIdentifier);
		EXPECT_NE(second, ecs::Entity::InvalidIdentifier);
		EXPECT_NE(first, second);
		EXPECT_EQ(list->getItemIdentifier(10), ecs::Entity::InvalidIdentifier);
	}

	TEST_F(TestList, VariantRoundTripChangesRowHeight)
	{
		ecs::ComponentRegistry registry;
		auto list = makeList(registry, List::Variant::SingleLine, makeItems(2));

		list->setVariant(List::Variant::ThreeLine);
		list->initialize();
		list->update();

		EXPECT_EQ(list->getVariant(), List::Variant::ThreeLine);

		const auto &bound =
			registry.getComponent<components::Bound>(list->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 176.0f);
	}

	TEST_F(TestList, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		List::Builder builder(registry, entityRegistry);
		List::Director director;

		auto list = director.makeList(builder, parent, List::Variant::TwoLine,
									  makeItems(4));

		ASSERT_NE(list, nullptr);
		EXPECT_EQ(list->getParent(), parent);
		EXPECT_EQ(list->getVariant(), List::Variant::TwoLine);
		EXPECT_EQ(list->getItemCount(), 4U);
	}

}	 // namespace guillaume::entities::tests
