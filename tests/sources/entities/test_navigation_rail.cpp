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

#include "entities/test_navigation_rail.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/selection.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::vector<std::pair<std::string, std::string>>
			makeDestinations(std::size_t count)
		{
			std::vector<std::pair<std::string, std::string>> destinations;

			for (std::size_t index = 0; index < count; ++index) {
				destinations.emplace_back("home",
										  "Dest " + std::to_string(index));
			}

			return destinations;
		}

		std::shared_ptr<NavigationRail> makeNavigationRail(
			ecs::ComponentRegistry &registry,
			const std::vector<std::pair<std::string, std::string>>
				&destinations = {},
			NavigationRail::Variant variant =
				NavigationRail::Variant::Collapsed,
			const std::string &fabGlyph = "")
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			NavigationRail::Builder builder(registry, entityRegistry);
			NavigationRail::Director director;

			return director.makeNavigationRail(builder, nullptr, variant,
											   destinations, fabGlyph);
		}
	}	 // namespace

	TEST_F(TestNavigationRail, CollapsedWidthIsEighty)
	{
		ecs::ComponentRegistry registry;
		auto rail = makeNavigationRail(registry, makeDestinations(3));

		rail->initialize();
		rail->update();

		const auto &bound =
			registry.getComponent<components::Bound>(rail->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 80.0f);
	}

	TEST_F(TestNavigationRail, ExpandedWidthIsTwoHundredFiftySix)
	{
		ecs::ComponentRegistry registry;
		auto rail = makeNavigationRail(registry, makeDestinations(3),
									   NavigationRail::Variant::Expanded);

		rail->initialize();
		rail->update();

		const auto &bound =
			registry.getComponent<components::Bound>(rail->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 256.0f);
	}

	TEST_F(TestNavigationRail, DestinationCountMatchesInput)
	{
		ecs::ComponentRegistry registry;
		auto rail = makeNavigationRail(registry, makeDestinations(4));

		EXPECT_EQ(rail->getDestinationCount(), 4U);
	}

	TEST_F(TestNavigationRail, SelectUpdatesIndex)
	{
		ecs::ComponentRegistry registry;
		auto rail = makeNavigationRail(registry, makeDestinations(3));

		rail->initialize();

		EXPECT_EQ(rail->getSelectedIndex(), 0U);
		rail->select(2);
		EXPECT_EQ(rail->getSelectedIndex(), 2U);
	}

	TEST_F(TestNavigationRail, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto rail = makeNavigationRail(registry, makeDestinations(2));

		rail->setVariant(NavigationRail::Variant::Expanded);

		EXPECT_EQ(rail->getVariant(), NavigationRail::Variant::Expanded);
	}

	TEST_F(TestNavigationRail, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		NavigationRail::Builder builder(registry, entityRegistry);
		NavigationRail::Director director;

		auto rail = director.makeNavigationRail(
			builder, parent, NavigationRail::Variant::Expanded,
			makeDestinations(3), "add");

		ASSERT_NE(rail, nullptr);
		EXPECT_EQ(rail->getParent(), parent);
		EXPECT_EQ(rail->getVariant(), NavigationRail::Variant::Expanded);
		EXPECT_EQ(rail->getDestinationCount(), 3U);
	}

}	 // namespace guillaume::entities::tests
