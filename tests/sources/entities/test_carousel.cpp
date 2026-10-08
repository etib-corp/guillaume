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

#include "entities/test_carousel.hpp"

#include <cstddef>
#include <memory>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/scrollable.hpp>
#include <guillaume/components/value.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<Carousel>
			makeCarousel(ecs::ComponentRegistry &registry,
						 Carousel::Variant variant = Carousel::Variant::Basic,
						 std::size_t itemCount	   = 3)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Carousel::Builder builder(registry, entityRegistry);
			Carousel::Director director;

			return director.makeCarousel(builder, nullptr, variant, itemCount);
		}
	}	 // namespace

	TEST_F(TestCarousel, DefaultGeometry)
	{
		ecs::ComponentRegistry registry;
		auto carousel = makeCarousel(registry);

		carousel->initialize();
		carousel->update();

		const auto &bound =
			registry.getComponent<components::Bound>(carousel->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 176.0f);
	}

	TEST_F(TestCarousel, ItemCountAndIdentifiers)
	{
		ecs::ComponentRegistry registry;
		auto carousel = makeCarousel(registry, Carousel::Variant::Basic, 4);

		carousel->initialize();
		carousel->update();

		EXPECT_EQ(carousel->getItemCount(), 4U);
		EXPECT_NE(carousel->getItemIdentifier(0),
				  ecs::Entity::InvalidIdentifier);
		EXPECT_NE(carousel->getItemIdentifier(3),
				  ecs::Entity::InvalidIdentifier);
		EXPECT_EQ(carousel->getItemIdentifier(10),
				  ecs::Entity::InvalidIdentifier);
	}

	TEST_F(TestCarousel, ValueClampsToItemCount)
	{
		ecs::ComponentRegistry registry;
		auto carousel = makeCarousel(registry, Carousel::Variant::Basic, 3);

		carousel->initialize();

		const auto &value =
			registry.getComponent<components::Value>(carousel->getIdentifier());
		EXPECT_FLOAT_EQ(value.getMin(), 0.0f);
		EXPECT_FLOAT_EQ(value.getMax(), 2.0f);
		EXPECT_FLOAT_EQ(value.getStep(), 1.0f);

		carousel->setValue(10);
		EXPECT_EQ(carousel->getValue(), 2U);
	}

	TEST_F(TestCarousel, ScrollableContentSizeMatchesItems)
	{
		ecs::ComponentRegistry registry;
		auto carousel = makeCarousel(registry, Carousel::Variant::Basic, 3);

		carousel->initialize();
		carousel->update();

		const auto &scrollable = registry.getComponent<components::Scrollable>(
			carousel->getIdentifier());
		EXPECT_GT(scrollable.getContentSize().x, 0.0f);
	}

	TEST_F(TestCarousel, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto carousel = makeCarousel(registry);

		carousel->setVariant(Carousel::Variant::Centered);
		EXPECT_EQ(carousel->getVariant(), Carousel::Variant::Centered);

		carousel->setVariant(Carousel::Variant::Uncontained);
		EXPECT_EQ(carousel->getVariant(), Carousel::Variant::Uncontained);
	}

	TEST_F(TestCarousel, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Carousel::Builder builder(registry, entityRegistry);
		Carousel::Director director;

		auto carousel = director.makeCarousel(builder, parent,
											  Carousel::Variant::Centered, 5);

		ASSERT_NE(carousel, nullptr);
		EXPECT_EQ(carousel->getParent(), parent);
		EXPECT_EQ(carousel->getVariant(), Carousel::Variant::Centered);
		EXPECT_EQ(carousel->getItemCount(), 5U);
	}

}	 // namespace guillaume::entities::tests
