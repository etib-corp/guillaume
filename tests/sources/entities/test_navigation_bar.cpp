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

#include <guillaume/components/glyph.hpp>

#include <guillaume/theme.hpp>

#include <guillaume/components/style_state.hpp>

#include <guillaume/components/mouse_hover_interaction.hpp>

#include <guillaume/components/mouse_button_interaction.hpp>

#include <guillaume/components/color.hpp>

#include <guillaume/components/accessibility.hpp>

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

			return builder.makeNavigationBar(nullptr, variant, destinations);
		}
	}	 // namespace

	TEST_F(TestNavigationBar, GeometryIsBottomBar)
	{
		ecs::ComponentRegistry registry;
		auto bar = makeNavigationBar(registry, makeDestinations(3));

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

		EXPECT_EQ(bar->getSelectedIndex(), 0U);
		bar->select(2);
		EXPECT_EQ(bar->getSelectedIndex(), 2U);
	}

	TEST_F(TestNavigationBar, SelectionIsExclusive)
	{
		ecs::ComponentRegistry registry;
		auto bar = makeNavigationBar(registry, makeDestinations(3));

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

	TEST_F(TestNavigationBar, BuilderPresetBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		NavigationBar::Builder builder(registry, entityRegistry);

		auto bar = builder.makeNavigationBar(
			parent, NavigationBar::Variant::WithoutLabels, makeDestinations(4));

		ASSERT_NE(bar, nullptr);
		EXPECT_EQ(bar->getParent(), parent);
		EXPECT_EQ(bar->getVariant(), NavigationBar::Variant::WithoutLabels);
		EXPECT_EQ(bar->getDestinationCount(), 4U);
	}

	TEST_F(TestNavigationBar, ContainerAndActivePillUseMaterialRoles)
	{
		ecs::ComponentRegistry registry;
		auto bar = makeNavigationBar(registry, makeDestinations(3));

		const auto role = [](SchemeColorRole value) {
			return getActiveScheme().getColor(value).getColor();
		};
		EXPECT_EQ(registry.getComponent<components::Color>(bar->getIdentifier())
					  .getColor(),
				  role(SchemeColorRole::SurfaceContainer));
		EXPECT_EQ(
			registry
				.getComponent<components::Color>(bar->getIndicatorIdentifier(0))
				.getColor(),
			role(SchemeColorRole::SecondaryContainer));
		EXPECT_EQ(
			registry
				.getComponent<components::Color>(bar->getIndicatorIdentifier(1))
				.getColor()
				.getAlpha(),
			0);
		EXPECT_EQ(
			registry.getComponent<components::Color>(bar->getLabelIdentifier(0))
				.getColor(),
			role(SchemeColorRole::OnSurface));
		EXPECT_EQ(
			registry.getComponent<components::Color>(bar->getIconIdentifier(0))
				.getColor(),
			role(SchemeColorRole::OnSecondaryContainer));
		EXPECT_EQ(
			registry.getComponent<components::Color>(bar->getLabelIdentifier(1))
				.getColor(),
			role(SchemeColorRole::OnSurfaceVariant));
	}

	TEST_F(TestNavigationBar, PressSelectsAndReportsTheIndex)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer &entityRegistry = scratchEntityRegistry();
		NavigationBar::Builder builder(registry, entityRegistry);
		std::size_t reported = 99;
		auto bar = builder.addDestination("home_outline", "Home", "home")
					   .addDestination("mail_outline", "Mail", "mail", "3")
					   .withOnIndexChange([&reported](std::size_t index) {
						   reported = index;
					   })
					   .registerEntity(nullptr);

		std::size_t presses = 0;
		bar->setOnTabPress([&presses](std::size_t) {
			++presses;
		});

		registry
			.getComponent<components::MouseButtonInteraction>(
				bar->getDestinationIdentifier(1))
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_EQ(bar->getSelectedIndex(), 1U);
		EXPECT_EQ(reported, 1U);
		EXPECT_EQ(presses, 1U);
		EXPECT_EQ(
			registry.getComponent<components::Glyph>(bar->getIconIdentifier(1))
				.getName(),
			"mail");
		EXPECT_EQ(
			registry.getComponent<components::Glyph>(bar->getIconIdentifier(0))
				.getName(),
			"home_outline");
		EXPECT_NE(bar->getBadgeIdentifier(1), ecs::Entity::InvalidIdentifier);
		EXPECT_EQ(bar->getBadgeIdentifier(0), ecs::Entity::InvalidIdentifier);
	}

	TEST_F(TestNavigationBar, ExposesTabSemantics)
	{
		ecs::ComponentRegistry registry;
		auto bar = makeNavigationBar(registry, makeDestinations(2));

		EXPECT_EQ(
			registry
				.getComponent<components::Accessibility>(bar->getIdentifier())
				.getRole(),
			components::AccessibilityRole::TabList);
		const auto &first = registry.getComponent<components::Accessibility>(
			bar->getDestinationIdentifier(0));
		EXPECT_EQ(first.getRole(), components::AccessibilityRole::Tab);
		EXPECT_TRUE(first.isSelected());
		EXPECT_EQ(first.getLabel(), "Dest 0");
		EXPECT_FALSE(registry
						 .getComponent<components::Accessibility>(
							 bar->getDestinationIdentifier(1))
						 .isSelected());
	}

}	 // namespace guillaume::entities::tests
