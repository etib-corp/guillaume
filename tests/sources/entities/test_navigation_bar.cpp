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

#include "entities/test_navigation_bar.hpp"

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

		std::shared_ptr<NavigationBar> makeNavigationBar(
			ecs::ComponentRegistry &registry,
			const std::vector<std::pair<std::string, std::string>>
				&destinations			   = {},
			NavigationBar::Variant variant = NavigationBar::Variant::WithLabels)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			NavigationBar::Builder builder(registry, entityRegistry);
			NavigationBar::Director director;

			return director.makeNavigationBar(builder, nullptr, variant,
											  destinations);
		}
	}	 // namespace

	TEST_F(TestNavigationBar, GeometryIsBottomBar)
	{
		ecs::ComponentRegistry registry;
		auto bar = makeNavigationBar(registry, makeDestinations(3));

		bar->initialize();
		bar->update();

		const auto &bound =
			registry.getComponent<components::Bound>(bar->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 360.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 80.0f);
	}

	TEST_F(TestNavigationBar, DestinationCountMatchesInput)
	{
		ecs::ComponentRegistry registry;
		auto bar = makeNavigationBar(registry, makeDestinations(3));

		EXPECT_EQ(bar->getDestinationCount(), 3U);
	}

	TEST_F(TestNavigationBar, SelectUpdatesIndex)
	{
		ecs::ComponentRegistry registry;
		auto bar = makeNavigationBar(registry, makeDestinations(3));

		bar->initialize();

		EXPECT_EQ(bar->getSelectedIndex(), 0U);
		bar->select(2);
		EXPECT_EQ(bar->getSelectedIndex(), 2U);
	}

	TEST_F(TestNavigationBar, SelectionIsExclusive)
	{
		ecs::ComponentRegistry registry;
		auto bar = makeNavigationBar(registry, makeDestinations(3));

		bar->initialize();
		bar->select(1);

		const auto first = registry.getComponent<components::Selectable>(
			bar->getDestinationIdentifier(0));
		const auto second = registry.getComponent<components::Selectable>(
			bar->getDestinationIdentifier(1));

		EXPECT_FALSE(first.isSelected());
		EXPECT_TRUE(second.isSelected());
	}

	TEST_F(TestNavigationBar, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto bar = makeNavigationBar(registry, makeDestinations(2));

		bar->setVariant(NavigationBar::Variant::WithoutLabels);

		EXPECT_EQ(bar->getVariant(), NavigationBar::Variant::WithoutLabels);
	}

	TEST_F(TestNavigationBar, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		NavigationBar::Builder builder(registry, entityRegistry);
		NavigationBar::Director director;

		auto bar = director.makeNavigationBar(
			builder, parent, NavigationBar::Variant::WithoutLabels,
			makeDestinations(4));

		ASSERT_NE(bar, nullptr);
		EXPECT_EQ(bar->getParent(), parent);
		EXPECT_EQ(bar->getVariant(), NavigationBar::Variant::WithoutLabels);
		EXPECT_EQ(bar->getDestinationCount(), 4U);
	}

}	 // namespace guillaume::entities::tests
