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

#include "entities/test_tooltip.hpp"

#include <guillaume/components/transform.hpp>

#include <guillaume/components/mouse_hover_interaction.hpp>

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
		std::shared_ptr<Tooltip>
			makeTooltip(ecs::ComponentRegistry &registry,
						Tooltip::Variant variant = Tooltip::Variant::Plain,
						const std::string &text	 = "Tooltip")
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Tooltip::Builder builder(registry, entityRegistry);
			Tooltip::Director director;

			return director.makeTooltip(builder, nullptr, variant, text, "");
		}
	}	 // namespace

	TEST_F(TestTooltip, PlainIsPillWithRadiusFour)
	{
		ecs::ComponentRegistry registry;
		auto tooltip = makeTooltip(registry, Tooltip::Variant::Plain);

		tooltip->initialize();
		tooltip->update();

		const auto &borders = registry.getComponent<components::Borders>(
			tooltip->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 4.0f);
	}

	TEST_F(TestTooltip, RichUsesRadiusTwelve)
	{
		ecs::ComponentRegistry registry;
		auto tooltip = makeTooltip(registry, Tooltip::Variant::Rich);

		tooltip->initialize();
		tooltip->update();

		const auto &borders = registry.getComponent<components::Borders>(
			tooltip->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 12.0f);
	}

	TEST_F(TestTooltip, PlainUsesInverseSurfaceColor)
	{
		ecs::ComponentRegistry registry;
		auto tooltip = makeTooltip(registry, Tooltip::Variant::Plain);

		tooltip->initialize();
		tooltip->update();

		const auto &color =
			registry.getComponent<components::Color>(tooltip->getIdentifier());
		// Hidden until shown.
		EXPECT_EQ(color.getColor().getAlpha(), 0);

		tooltip->show();
		EXPECT_EQ(color.getColor(),
				  schemeColor(SchemeColorRole::InverseSurface));
	}

	TEST_F(TestTooltip, OverlayConfiguration)
	{
		ecs::ComponentRegistry registry;
		auto tooltip = makeTooltip(registry, Tooltip::Variant::Plain);

		tooltip->initialize();
		tooltip->update();

		const auto &overlay = registry.getComponent<components::Overlay>(
			tooltip->getIdentifier());
		EXPECT_FALSE(overlay.isModal());
		EXPECT_FALSE(overlay.dismissesOnOutsideClick());
		EXPECT_TRUE(overlay.dismissesOnEscape());
	}

	TEST_F(TestTooltip, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto tooltip = makeTooltip(registry, Tooltip::Variant::Plain);

		tooltip->setVariant(Tooltip::Variant::Rich);

		EXPECT_EQ(tooltip->getVariant(), Tooltip::Variant::Rich);

		const auto &borders = registry.getComponent<components::Borders>(
			tooltip->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 12.0f);
	}

	TEST_F(TestTooltip, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Tooltip::Builder builder(registry, entityRegistry);
		Tooltip::Director director;

		auto tooltip = director.makeTooltip(
			builder, parent, Tooltip::Variant::Rich, "Text", "Title");

		ASSERT_NE(tooltip, nullptr);
		EXPECT_EQ(tooltip->getParent(), parent);
		EXPECT_EQ(tooltip->getVariant(), Tooltip::Variant::Rich);
	}

	TEST_F(TestTooltip, PlainLabelUsesInverseOnSurface)
	{
		ecs::ComponentRegistry registry;
		auto tooltip = makeTooltip(registry, Tooltip::Variant::Plain);
		tooltip->initialize();
		tooltip->show();

		ASSERT_FALSE(tooltip->getChildren().empty());
		EXPECT_EQ(registry
					  .getComponent<components::Color>(
						  tooltip->getChildren().back()->getIdentifier())
					  .getColor(),
				  schemeColor(SchemeColorRole::InverseOnSurface));
		EXPECT_TRUE(
			registry.getComponent<components::Overlay>(tooltip->getIdentifier())
				.isVisible());

		tooltip->hide();
		EXPECT_FALSE(tooltip->isVisible());
		EXPECT_FALSE(
			registry.getComponent<components::Overlay>(tooltip->getIdentifier())
				.isVisible());
	}

	TEST_F(TestTooltip, AnchorHoverHonoursEnterAndLeaveDelays)
	{
		ecs::ComponentRegistry registry;
		auto tooltip = makeTooltip(registry, Tooltip::Variant::Plain);
		tooltip->initialize();

		auto anchor = std::make_shared<ecs::Entity>();
		registry.registerComponentsForEntity<
			components::Transform, components::Bound,
			components::MouseHoverInteraction>(anchor->getIdentifier());
		registry.getComponent<components::Bound>(anchor->getIdentifier())
			.setWidth(40.0f)
			.setHeight(40.0f);
		tooltip->setAnchor(anchor);

		auto &hover = registry.getComponent<components::MouseHoverInteraction>(
			anchor->getIdentifier());
		hover.setHovered(true);

		tooltip->advance(Tooltip::DefaultEnterDelay * 0.5f);
		EXPECT_FALSE(tooltip->isVisible());
		tooltip->advance(Tooltip::DefaultEnterDelay);
		EXPECT_TRUE(tooltip->isVisible());

		// Placed under the anchor.
		const auto pose =
			registry.getComponent<components::Transform>(tooltip->getIdentifier())
				.getPose();
		EXPECT_FLOAT_EQ(pose.getPosition().getY(), 40.0f);

		hover.setHovered(false);
		tooltip->advance(Tooltip::DefaultLeaveDelay * 0.5f);
		EXPECT_TRUE(tooltip->isVisible());
		tooltip->advance(Tooltip::DefaultLeaveDelay);
		EXPECT_FALSE(tooltip->isVisible());
	}

	TEST_F(TestTooltip, PlacementFlipsAndClampsInsideViewport)
	{
		ecs::ComponentRegistry registry;
		auto tooltip = makeTooltip(registry, Tooltip::Variant::Plain);
		tooltip->initialize();

		auto anchor = std::make_shared<ecs::Entity>();
		registry.registerComponentsForEntity<
			components::Transform, components::Bound,
			components::MouseHoverInteraction>(anchor->getIdentifier());
		registry.getComponent<components::Transform>(anchor->getIdentifier())
			.setPose(utility::graphic::PoseF(
				utility::graphic::PositionF(-30.0f, 280.0f, 0.0f),
				utility::graphic::OrientationF()));
		registry.getComponent<components::Bound>(anchor->getIdentifier())
			.setWidth(20.0f)
			.setHeight(20.0f);
		tooltip->setAnchor(anchor);
		tooltip->setViewport(400.0f, 300.0f);
		tooltip->show();

		const auto &bound =
			registry.getComponent<components::Bound>(tooltip->getIdentifier());
		const auto pose =
			registry.getComponent<components::Transform>(tooltip->getIdentifier())
				.getPose();
		EXPECT_FLOAT_EQ(pose.getPosition().getX(), 0.0f);
		EXPECT_FLOAT_EQ(pose.getPosition().getY(), 280.0f - bound.getHeight());
	}

	TEST_F(TestTooltip, ExposesTextSemantics)
	{
		ecs::ComponentRegistry registry;
		auto tooltip = makeTooltip(registry, Tooltip::Variant::Plain);
		const auto &accessibility =
			registry.getComponent<components::Accessibility>(
				tooltip->getIdentifier());
		EXPECT_EQ(accessibility.getRole(), components::AccessibilityRole::Text);
		EXPECT_EQ(accessibility.getLiveRegion(), components::LiveRegion::Polite);
		EXPECT_THROW(tooltip->setEnterDelay(-1.0f), std::invalid_argument);
	}

}	 // namespace guillaume::entities::tests
