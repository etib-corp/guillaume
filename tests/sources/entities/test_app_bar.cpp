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

#include "entities/test_app_bar.hpp"

#include <guillaume/components/mouse_button_interaction.hpp>

#include <guillaume/components/elevation.hpp>

#include <guillaume/components/accessibility.hpp>

#include <utility>

#include <memory>
#include <string>

#include <utility/graphic/color.hpp>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>
#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/systems/app_bar.hpp>
#include <guillaume/systems/layout.hpp>
#include <guillaume/theme.hpp>

#include <utility/graphic/pose.hpp>
#include <utility/graphic/position.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<AppBar>
			makeAppBar(ecs::ComponentRegistry &registry,
					   AppBar::Variant variant = AppBar::Variant::Small)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			AppBar::Builder builder(registry, entityRegistry);

			return builder.makeAppBar(nullptr, variant);
		}
	}	 // namespace

	TEST_F(TestAppBar, SmallVariantGeometry)
	{
		ecs::ComponentRegistry registry;
		auto appBar = makeAppBar(registry, AppBar::Variant::Small);

		const auto &bound =
			registry.getComponent<components::Bound>(appBar->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 360.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 64.0f);
	}

	TEST_F(TestAppBar, TitleChildIsPositionedByLayout)
	{
		ecs::ComponentRegistry registry;
		auto appBar = makeAppBar(registry, AppBar::Variant::Small);
		appBar->setTitle("Title");

		registry.getComponent<components::Transform>(appBar->getIdentifier())
			.setPose(utility::graphic::PoseF(
				utility::graphic::PositionF(100.0f, 50.0f, 0.0f),
				utility::graphic::OrientationF()));

		// The pose was written behind the facade: run the system as a frame
		// would.
		systems::AppBar::apply(registry, appBar->getIdentifier());

		const auto positions = directChildPositions(registry, appBar);
		ASSERT_EQ(positions.size(), 1u);

		// Paper 4 dp padding on the main axis; the title is the only child
		// and its bound is unmeasured (0x0) so it is centered in the 64 dp bar,
		// 32 dp below the bar's top edge (y points up).
		EXPECT_FLOAT_EQ(positions[0].getX(),
						100.0f + AppBar::HorizontalPadding);
		EXPECT_FLOAT_EQ(positions[0].getY(), 18.0f);
		EXPECT_FLOAT_EQ(
			positions[0].getZ(),
			static_cast<float>(systems::Layout::ContentLayerOffset));
	}

	TEST_F(TestAppBar, LargeVariantIsTallerThanSmall)
	{
		ecs::ComponentRegistry registry;
		auto smallEntity = makeAppBar(registry, AppBar::Variant::Small);

		auto largeEntity = makeAppBar(registry, AppBar::Variant::Large);

		const float smallHeight =
			registry
				.getComponent<components::Bound>(smallEntity->getIdentifier())
				.getHeight();
		const float largeHeight =
			registry
				.getComponent<components::Bound>(largeEntity->getIdentifier())
				.getHeight();

		EXPECT_FLOAT_EQ(largeHeight, 152.0f);
		EXPECT_GT(largeHeight, smallHeight);
	}

	TEST_F(TestAppBar, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto appBar = makeAppBar(registry, AppBar::Variant::Small);

		appBar->setVariant(AppBar::Variant::Bottom);

		EXPECT_EQ(appBar->getVariant(), AppBar::Variant::Bottom);

		const auto &bound =
			registry.getComponent<components::Bound>(appBar->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 80.0f);
	}

	TEST_F(TestAppBar, TitleOnlyBuildsTextChild)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;

		AppBar::Builder builder(registry, entityRegistry);

		auto appBar = builder.makeAppBar(nullptr, AppBar::Variant::Small);
		appBar->setTitle("Title");

		EXPECT_EQ(appBar->getChildren().size(), 1U);
		EXPECT_NE(appBar->getChildren()[0], ecs::Entity::InvalidIdentifier);
	}

	TEST_F(TestAppBar, BuilderAttachesNavigationAndActions)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;

		AppBar::Builder builder(registry, entityRegistry);
		builder.withVariant(AppBar::Variant::Small)
			.withTitle("Title")
			.withNavigationIcon("menu")
			.addAction("search")
			.addAction("more_vert");

		auto appBar = builder.registerEntity(nullptr);

		EXPECT_EQ(appBar->getChildren().size(), 4U);

		for (const auto &child: appBar->getChildren()) {
			EXPECT_NE(child, ecs::Entity::InvalidIdentifier);
		}
	}

	TEST_F(TestAppBar, UsesSurfaceColor)
	{
		ecs::ComponentRegistry registry;
		auto appBar = makeAppBar(registry, AppBar::Variant::Small);

		const auto &color =
			registry.getComponent<components::Color>(appBar->getIdentifier());
		EXPECT_EQ(color.getColor(), schemeColor(SchemeColorRole::Surface));
	}

	TEST_F(TestAppBar, BuilderPresetBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		AppBar::Builder builder(registry, entityRegistry);

		auto appBar = builder.makeAppBar(parent, AppBar::Variant::Medium);

		ASSERT_NE(appBar, nullptr);
		EXPECT_EQ(appBar->getParent(), parent);
		EXPECT_EQ(appBar->getVariant(), AppBar::Variant::Medium);
	}

	TEST_F(TestAppBar, HeightsAndTitleRolesFollowPaper)
	{
		ecs::ComponentRegistry registry;
		const std::pair<AppBar::Variant, std::pair<float, theme::TypeRole>>
			expected[] = {
				{ AppBar::Variant::Small,
				  { 64.0f, theme::TypeRole::TitleLarge } },
				{ AppBar::Variant::CenterAligned,
				  { 64.0f, theme::TypeRole::TitleLarge } },
				{ AppBar::Variant::Medium,
				  { 112.0f, theme::TypeRole::HeadlineSmall } },
				{ AppBar::Variant::Large,
				  { 152.0f, theme::TypeRole::HeadlineMedium } },
			};

		for (const auto &[variant, values]: expected) {
			ecs::EntityRegistryContainer entityRegistry;
			AppBar::Builder builder(registry, entityRegistry);
			auto appBar =
				builder.withVariant(variant).withTitle("Title").registerEntity(
					nullptr);

			EXPECT_FLOAT_EQ(
				registry
					.getComponent<components::Bound>(appBar->getIdentifier())
					.getHeight(),
				values.first);
			ASSERT_NE(appBar->getTitle(), nullptr);
			EXPECT_EQ(appBar->getTitle()->getTypeRole(), values.second);
			EXPECT_EQ(registry
						  .getComponent<components::Accessibility>(
							  appBar->getTitle()->getIdentifier())
						  .getRole(),
					  components::AccessibilityRole::Header);
		}
	}

	TEST_F(TestAppBar, ActionsArePressableIconButtons)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		AppBar::Builder builder(registry, entityRegistry);
		int searches   = 0;
		int navigation = 0;
		auto appBar	   = builder.withTitle("Inbox")
						  .withNavigationIcon("arrow_back")
						  .withOnNavigation([&navigation]() {
							  ++navigation;
						  })
						  .addAction(
							  "search",
							  [&searches]() {
								  ++searches;
							  },
							  "Search")
						  .registerEntity(nullptr);

		auto action = appBar->getActionButton(0);
		ASSERT_NE(action, nullptr);
		registry
			.getComponent<components::MouseButtonInteraction>(
				action->getIdentifier())
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_EQ(searches, 1);
		EXPECT_EQ(action->getAccessibilityLabel(), "Search");
		EXPECT_EQ(registry
					  .getComponent<components::Accessibility>(
						  action->getIdentifier())
					  .getRole(),
				  components::AccessibilityRole::Button);
		EXPECT_EQ(
			registry
				.getComponent<components::Color>(action->getIconIdentifier())
				.getColor(),
			schemeColor(SchemeColorRole::OnSurfaceVariant));

		auto back = appBar->getNavigationButton();
		ASSERT_NE(back, nullptr);
		registry
			.getComponent<components::MouseButtonInteraction>(
				back->getIdentifier())
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_EQ(navigation, 1);
		EXPECT_EQ(
			registry.getComponent<components::Color>(back->getIconIdentifier())
				.getColor(),
			schemeColor(SchemeColorRole::OnSurface));
	}

	TEST_F(TestAppBar, ElevatedUsesLevelTwoSurface)
	{
		ecs::ComponentRegistry registry;
		auto appBar = makeAppBar(registry);
		appBar->setElevated(true);

		EXPECT_TRUE(appBar->isElevated());
		EXPECT_EQ(
			registry.getComponent<components::Color>(appBar->getIdentifier())
				.getColor(),
			getElevatedSurfaceColor(theme::ElevationLevel::Level2));
		EXPECT_EQ(
			registry
				.getComponent<components::Elevation>(appBar->getIdentifier())
				.getElevationLevel(),
			theme::ElevationLevel::Level2);
	}

}	 // namespace guillaume::entities::tests
