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

#include "entities/test_badge.hpp"

#include <memory>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<Badge> makeBadge(ecs::ComponentRegistry &registry,
										 const std::string &label,
										 Badge::Variant variant)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Badge::Builder builder(registry, entityRegistry);
			Badge::Director director;

			return director.makeBadge(builder, nullptr, label, variant);
		}
	}	 // namespace

	TEST_F(TestBadge, DotIsSmallCircle)
	{
		ecs::ComponentRegistry registry;
		auto badge = makeBadge(registry, "", Badge::Variant::Dot);

		badge->initialize();
		badge->update();

		const auto &bound =
			registry.getComponent<components::Bound>(badge->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 6.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 6.0f);

		const auto &borders =
			registry.getComponent<components::Borders>(badge->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 3.0f);
	}

	TEST_F(TestBadge, SmallIsPill)
	{
		ecs::ComponentRegistry registry;
		auto badge = makeBadge(registry, "99+", Badge::Variant::Small);

		badge->initialize();
		badge->update();

		const auto &bound =
			registry.getComponent<components::Bound>(badge->getIdentifier());
		EXPECT_GT(bound.getWidth(), bound.getHeight());
		EXPECT_FLOAT_EQ(bound.getHeight(), 16.0f);
	}

	TEST_F(TestBadge, LargeIsTallerThanSmall)
	{
		ecs::ComponentRegistry registry;
		auto smallEntity = makeBadge(registry, "9", Badge::Variant::Small);
		smallEntity->initialize();
		smallEntity->update();
		const float smallHeight =
			registry
				.getComponent<components::Bound>(smallEntity->getIdentifier())
				.getHeight();

		auto largeEntity = makeBadge(registry, "9", Badge::Variant::Large);
		largeEntity->initialize();
		largeEntity->update();
		const float largeHeight =
			registry
				.getComponent<components::Bound>(largeEntity->getIdentifier())
				.getHeight();

		EXPECT_GT(largeHeight, smallHeight);
	}

	TEST_F(TestBadge, NonDotUsesErrorColor)
	{
		ecs::ComponentRegistry registry;
		auto badge = makeBadge(registry, "1", Badge::Variant::Small);

		badge->initialize();
		badge->update();

		const auto &color =
			registry.getComponent<components::Color>(badge->getIdentifier());
		EXPECT_GT(color.getColor().getAlpha(), 0);
	}

	TEST_F(TestBadge, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto badge = makeBadge(registry, "1", Badge::Variant::Dot);

		badge->setVariant(Badge::Variant::Large);
		EXPECT_EQ(badge->getVariant(), Badge::Variant::Large);
	}

	TEST_F(TestBadge, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Badge::Builder builder(registry, entityRegistry);
		Badge::Director director;

		auto badge =
			director.makeBadge(builder, parent, "5", Badge::Variant::Small);

		ASSERT_NE(badge, nullptr);
		EXPECT_EQ(badge->getParent(), parent);
		EXPECT_EQ(badge->getVariant(), Badge::Variant::Small);
	}

}	 // namespace guillaume::entities::tests
