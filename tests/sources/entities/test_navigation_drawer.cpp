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

#include "entities/test_navigation_drawer.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/overlay.hpp>
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

		std::shared_ptr<NavigationDrawer> makeNavigationDrawer(
			ecs::ComponentRegistry &registry,
			const std::vector<std::pair<std::string, std::string>>
				&destinations = {},
			NavigationDrawer::Variant variant =
				NavigationDrawer::Variant::Standard)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			NavigationDrawer::Builder builder(registry, entityRegistry);
			NavigationDrawer::Director director;

			return director.makeNavigationDrawer(builder, nullptr, variant,
												 destinations);
		}
	}	 // namespace

	TEST_F(TestNavigationDrawer, GeometryIsFullHeight)
	{
		ecs::ComponentRegistry registry;
		auto drawer = makeNavigationDrawer(registry, makeDestinations(3));

		drawer->initialize();
		drawer->update();

		const auto &bound =
			registry.getComponent<components::Bound>(drawer->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 320.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 640.0f);
	}

	TEST_F(TestNavigationDrawer, LeadingEdgeIsFlat)
	{
		ecs::ComponentRegistry registry;
		auto drawer = makeNavigationDrawer(registry, makeDestinations(2));

		drawer->initialize();
		drawer->update();

		const auto &borders =
			registry.getComponent<components::Borders>(drawer->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 0.0f);
		EXPECT_FLOAT_EQ(borders.getTopRightRadius(), 16.0f);
	}

	TEST_F(TestNavigationDrawer, DestinationCountMatchesInput)
	{
		ecs::ComponentRegistry registry;
		auto drawer = makeNavigationDrawer(registry, makeDestinations(4));

		EXPECT_EQ(drawer->getDestinationCount(), 4U);
	}

	TEST_F(TestNavigationDrawer, SelectUpdatesIndex)
	{
		ecs::ComponentRegistry registry;
		auto drawer = makeNavigationDrawer(registry, makeDestinations(3));

		drawer->initialize();

		EXPECT_EQ(drawer->getSelectedIndex(), 0U);
		drawer->select(2);
		EXPECT_EQ(drawer->getSelectedIndex(), 2U);
	}

	TEST_F(TestNavigationDrawer, ModalCapturesOutsideClicks)
	{
		ecs::ComponentRegistry registry;
		auto drawer = makeNavigationDrawer(registry, makeDestinations(2),
										   NavigationDrawer::Variant::Modal);

		drawer->initialize();
		drawer->update();

		const auto &overlay =
			registry.getComponent<components::Overlay>(drawer->getIdentifier());
		EXPECT_TRUE(overlay.isModal());
		EXPECT_TRUE(overlay.dismissesOnOutsideClick());
	}

	TEST_F(TestNavigationDrawer, OpenCloseTogglesState)
	{
		ecs::ComponentRegistry registry;
		auto drawer = makeNavigationDrawer(registry, makeDestinations(2));

		drawer->initialize();

		EXPECT_FALSE(drawer->isOpen());
		drawer->open();
		EXPECT_TRUE(drawer->isOpen());
		drawer->close();
		EXPECT_FALSE(drawer->isOpen());
	}

	TEST_F(TestNavigationDrawer, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		NavigationDrawer::Builder builder(registry, entityRegistry);
		NavigationDrawer::Director director;

		auto drawer = director.makeNavigationDrawer(
			builder, parent, NavigationDrawer::Variant::Modal,
			makeDestinations(3));

		ASSERT_NE(drawer, nullptr);
		EXPECT_EQ(drawer->getParent(), parent);
		EXPECT_EQ(drawer->getDestinationCount(), 3U);
	}

}	 // namespace guillaume::entities::tests
