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

#include "entities/test_snackbar.hpp"

#include <guillaume/components/mouse_button_interaction.hpp>

#include <guillaume/components/elevation.hpp>

#include <guillaume/components/accessibility.hpp>

#include <stdexcept>

#include <memory>
#include <string>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/overlay.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>
#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<Snackbar> makeSnackbar(
			ecs::ComponentRegistry &registry,
			Snackbar::Variant variant  = Snackbar::Variant::SingleLine,
			const std::string &message = "Message",
			const std::string &action  = "")
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Snackbar::Builder builder(registry, entityRegistry);
			Snackbar::Director director;

			return director.makeSnackbar(builder, nullptr, variant, message,
										 action);
		}
	}	 // namespace

	TEST_F(TestSnackbar, SingleLineHeightIsFortyEight)
	{
		ecs::ComponentRegistry registry;
		auto snackbar = makeSnackbar(registry, Snackbar::Variant::SingleLine);

		snackbar->initialize();
		snackbar->update();

		const auto &bound =
			registry.getComponent<components::Bound>(snackbar->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 48.0f);
	}

	TEST_F(TestSnackbar, MultiLineHeightIsSixtyEight)
	{
		ecs::ComponentRegistry registry;
		auto snackbar = makeSnackbar(registry, Snackbar::Variant::MultiLine);

		snackbar->initialize();
		snackbar->update();

		const auto &bound =
			registry.getComponent<components::Bound>(snackbar->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 68.0f);
	}

	TEST_F(TestSnackbar, UsesInverseSurfaceColor)
	{
		ecs::ComponentRegistry registry;
		auto snackbar = makeSnackbar(registry, Snackbar::Variant::SingleLine);

		snackbar->initialize();
		snackbar->update();

		const auto &color =
			registry.getComponent<components::Color>(snackbar->getIdentifier());
		EXPECT_EQ(color.getColor(),
				  schemeColor(SchemeColorRole::InverseSurface));
	}

	TEST_F(TestSnackbar, OverlayAutoDismisses)
	{
		ecs::ComponentRegistry registry;
		auto snackbar = makeSnackbar(registry, Snackbar::Variant::SingleLine);

		snackbar->initialize();
		snackbar->update();

		const auto &overlay = registry.getComponent<components::Overlay>(
			snackbar->getIdentifier());
		EXPECT_FALSE(overlay.isModal());
		EXPECT_TRUE(overlay.hasAutoDismiss());
		EXPECT_FLOAT_EQ(overlay.getAutoDismiss(), Snackbar::DurationMedium);
	}

	TEST_F(TestSnackbar, WithActionBuildsTwoChildren)
	{
		ecs::ComponentRegistry registry;
		auto snackbar = makeSnackbar(registry, Snackbar::Variant::WithAction,
									 "Message", "Undo");

		snackbar->initialize();
		snackbar->update();

		EXPECT_EQ(snackbar->getChildren().size(), 2U);
	}

	TEST_F(TestSnackbar, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto snackbar = makeSnackbar(registry, Snackbar::Variant::SingleLine);

		snackbar->setVariant(Snackbar::Variant::MultiLine);

		EXPECT_EQ(snackbar->getVariant(), Snackbar::Variant::MultiLine);

		const auto &bound =
			registry.getComponent<components::Bound>(snackbar->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 68.0f);
	}

	TEST_F(TestSnackbar, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Snackbar::Builder builder(registry, entityRegistry);
		Snackbar::Director director;

		auto snackbar = director.makeSnackbar(
			builder, parent, Snackbar::Variant::WithAction, "Message", "Undo");

		ASSERT_NE(snackbar, nullptr);
		EXPECT_EQ(snackbar->getParent(), parent);
		EXPECT_EQ(snackbar->getVariant(), Snackbar::Variant::WithAction);
	}

	TEST_F(TestSnackbar, VisibilityDrivesOverlayAndVisuals)
	{
		ecs::ComponentRegistry registry;
		auto snackbar = makeSnackbar(registry);
		snackbar->initialize();

		auto &overlay =
			registry.getComponent<components::Overlay>(snackbar->getIdentifier());
		EXPECT_TRUE(snackbar->isVisible());
		EXPECT_TRUE(overlay.isVisible());
		EXPECT_FLOAT_EQ(overlay.getAutoDismiss(), Snackbar::DurationMedium);

		snackbar->hide();
		EXPECT_FALSE(overlay.isVisible());
		EXPECT_EQ(
			registry.getComponent<components::Color>(snackbar->getIdentifier())
				.getColor()
				.getAlpha(),
			0);
		EXPECT_FALSE(
			registry.getComponent<components::Elevation>(snackbar->getIdentifier())
				.hasShadow());

		snackbar->show();
		EXPECT_TRUE(overlay.isVisible());
		EXPECT_EQ(
			registry.getComponent<components::Color>(snackbar->getIdentifier())
				.getColor(),
			schemeColor(SchemeColorRole::InverseSurface));
	}

	TEST_F(TestSnackbar, OverlayDismissCallsOnDismissAndHides)
	{
		ecs::ComponentRegistry registry;
		auto snackbar = makeSnackbar(registry);
		snackbar->initialize();
		int dismissals = 0;
		snackbar->setOnDismiss([&dismissals]() {
			++dismissals;
		});
		snackbar->setDuration(Snackbar::DurationShort);
		EXPECT_FLOAT_EQ(
			registry.getComponent<components::Overlay>(snackbar->getIdentifier())
				.getAutoDismiss(),
			Snackbar::DurationShort);
		EXPECT_THROW(snackbar->setDuration(-1.0f), std::invalid_argument);

		// systems::Overlay calls the dismiss handler when the timer elapses.
		registry.getComponent<components::Overlay>(snackbar->getIdentifier())
			.getOnDismissHandler()();
		EXPECT_EQ(dismissals, 1);
		EXPECT_FALSE(snackbar->isVisible());

		// Hiding programmatically does not report a dismissal.
		snackbar->show();
		snackbar->hide();
		EXPECT_EQ(dismissals, 1);
	}

	TEST_F(TestSnackbar, ActionIsPressableAndDismisses)
	{
		ecs::ComponentRegistry registry;
		auto snackbar =
			makeSnackbar(registry, Snackbar::Variant::WithAction, "Saved", "Undo");
		snackbar->initialize();
		int actions	   = 0;
		int dismissals = 0;
		snackbar->setOnAction([&actions]() {
			++actions;
		});
		snackbar->setOnDismiss([&dismissals]() {
			++dismissals;
		});

		auto action = snackbar->getActionEntity();
		ASSERT_NE(action, nullptr);
		EXPECT_EQ(registry
					  .getComponent<components::Accessibility>(
						  action->getIdentifier())
					  .getRole(),
				  components::AccessibilityRole::Button);
		registry
			.getComponent<components::MouseButtonInteraction>(
				action->getIdentifier())
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_EQ(actions, 1);
		EXPECT_EQ(dismissals, 1);
		EXPECT_FALSE(snackbar->isVisible());
	}

	TEST_F(TestSnackbar, AnnouncesAsPoliteAlert)
	{
		ecs::ComponentRegistry registry;
		auto snackbar = makeSnackbar(registry);
		const auto &accessibility =
			registry.getComponent<components::Accessibility>(
				snackbar->getIdentifier());
		EXPECT_EQ(accessibility.getRole(), components::AccessibilityRole::Alert);
		EXPECT_EQ(accessibility.getLiveRegion(), components::LiveRegion::Polite);
		EXPECT_EQ(accessibility.getLabel(), "Message");
		EXPECT_EQ(
			registry.getComponent<components::Elevation>(snackbar->getIdentifier())
				.getElevationLevel(),
			theme::ElevationLevel::Level3);
	}

}	 // namespace guillaume::entities::tests
