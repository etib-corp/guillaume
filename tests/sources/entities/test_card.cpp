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

#include "entities/test_card.hpp"

#include <memory>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/elevation.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<Card>
			makeCard(ecs::ComponentRegistry &registry,
					 Card::Variant variant = Card::Variant::Elevated)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Card::Builder builder(registry, entityRegistry);
			Card::Director director;

			return director.makeCard(builder, nullptr, variant);
		}
	}	 // namespace

	TEST_F(TestCard, DefaultSizeAndRadius)
	{
		ecs::ComponentRegistry registry;
		auto card = makeCard(registry);

		card->initialize();
		card->update();

		const auto &bound =
			registry.getComponent<components::Bound>(card->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 240.0f);

		const auto &borders =
			registry.getComponent<components::Borders>(card->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 12.0f);
	}

	TEST_F(TestCard, ElevatedHasElevationAndSurfaceColor)
	{
		ecs::ComponentRegistry registry;
		auto card = makeCard(registry, Card::Variant::Elevated);

		card->initialize();
		card->update();

		const auto &elevation =
			registry.getComponent<components::Elevation>(card->getIdentifier());
		EXPECT_GT(elevation.getLevel(), 0.0f);
	}

	TEST_F(TestCard, FilledHasNoElevation)
	{
		ecs::ComponentRegistry registry;
		auto card = makeCard(registry, Card::Variant::Filled);

		card->initialize();
		card->update();

		const auto &elevation =
			registry.getComponent<components::Elevation>(card->getIdentifier());
		EXPECT_FLOAT_EQ(elevation.getLevel(), 0.0f);
	}

	TEST_F(TestCard, OutlinedUsesTransparentFillAndOutlineBorder)
	{
		ecs::ComponentRegistry registry;
		auto card = makeCard(registry, Card::Variant::Outlined);

		card->initialize();
		card->update();

		const auto &color =
			registry.getComponent<components::Color>(card->getIdentifier());
		EXPECT_EQ(color.getColor().getAlpha(), 0);

		const auto &borders =
			registry.getComponent<components::Borders>(card->getIdentifier());
		EXPECT_GT(borders.getColor().getAlpha(), 0);
	}

	TEST_F(TestCard, VariantSwitchUpdatesStyle)
	{
		ecs::ComponentRegistry registry;
		auto card = makeCard(registry, Card::Variant::Filled);

		card->setVariant(Card::Variant::Outlined);

		EXPECT_EQ(card->getVariant(), Card::Variant::Outlined);
		const auto &color =
			registry.getComponent<components::Color>(card->getIdentifier());
		EXPECT_EQ(color.getColor().getAlpha(), 0);
	}

	TEST_F(TestCard, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Card::Builder builder(registry, entityRegistry);
		Card::Director director;

		auto card = director.makeCard(builder, parent, Card::Variant::Filled);

		ASSERT_NE(card, nullptr);
		EXPECT_EQ(card->getParent(), parent);
		EXPECT_EQ(card->getVariant(), Card::Variant::Filled);
	}

}	 // namespace guillaume::entities::tests
