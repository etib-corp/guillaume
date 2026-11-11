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

#include "entities/test_icon_button.hpp"

#include <guillaume/entities/style_helpers.hpp>

#include <guillaume/components/color.hpp>

#include <guillaume/theme.hpp>

#include <guillaume/components/mouse_hover_interaction.hpp>

#include <guillaume/components/bound.hpp>

#include <guillaume/components/borders.hpp>

#include <functional>
#include <memory>

#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>
#include <guillaume/systems/layout.hpp>

namespace guillaume::entities::tests
{
	TEST_F(TestIconButton, ToggleFlipsSelectedOnRelease)
	{
		ecs::ComponentRegistry registry;
		auto button = IconButton::create(
			registry, "favorite", "favorite_border",
			components::Glyph::Style::Outlined, true,
			IconButton::Variant::Standard, IconButton::Size::Medium, false, "",
			std::function<void(void)>());

		EXPECT_FALSE(button->isSelected());

		auto &mouseInteraction =
			registry.getComponent<components::MouseButtonInteraction>(
				button->getIdentifier());

		mouseInteraction.getOnButtonReleaseHandler(
			utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_TRUE(button->isSelected());
	}

	TEST_F(TestIconButton, IconIsCenteredAndLiftedByTheContentLayer)
	{
		ecs::ComponentRegistry registry;
		auto button = IconButton::create(
			registry, "favorite", "favorite_border",
			components::Glyph::Style::Outlined, false,
			IconButton::Variant::Standard, IconButton::Size::Medium, false, "",
			std::function<void(void)>());

		const auto iconId = button->getIconIdentifier();
		ASSERT_NE(iconId, ecs::Entity::InvalidIdentifier);

		const auto &buttonPose =
			registry
				.getComponent<components::Transform>(button->getIdentifier())
				.getPose();
		const auto &iconPose =
			registry.getComponent<components::Transform>(iconId).getPose();

		EXPECT_FLOAT_EQ(
			iconPose.getPosition().getZ(),
			buttonPose.getPosition().getZ()
				+ static_cast<float>(systems::Layout::ContentLayerOffset));
		EXPECT_GT(iconPose.getPosition().getX(),
				  buttonPose.getPosition().getX());
		EXPECT_GT(iconPose.getPosition().getY(),
				  buttonPose.getPosition().getY());
	}

	TEST_F(TestIconButton, SetSelectedUpdatesState)
	{
		ecs::ComponentRegistry registry;
		auto button = IconButton::create(
			registry, "favorite", "favorite_border",
			components::Glyph::Style::Outlined, true,
			IconButton::Variant::Standard, IconButton::Size::Medium, false, "",
			std::function<void(void)>());

		button->setSelected(true);
		EXPECT_TRUE(button->isSelected());

		button->setSelected(false);
		EXPECT_FALSE(button->isSelected());
	}

	TEST_F(TestIconButton, SetSelectedIgnoredWhenNotToggle)
	{
		ecs::ComponentRegistry registry;
		auto button = IconButton::create(
			registry, "favorite", "favorite_border",
			components::Glyph::Style::Outlined, false,
			IconButton::Variant::Standard, IconButton::Size::Medium, false, "",
			std::function<void(void)>());

		button->setSelected(true);
		EXPECT_FALSE(button->isSelected());
	}

	TEST_F(TestIconButton, DisabledButtonIgnoresRelease)
	{
		ecs::ComponentRegistry registry;
		bool clicked = false;
		auto button	 = IconButton::create(
			 registry, "favorite", "favorite_border",
			 components::Glyph::Style::Outlined, true,
			 IconButton::Variant::Standard, IconButton::Size::Medium, false, "",
			 [&clicked]() {
				 clicked = true;
			 });

		button->setDisabled(true);
		EXPECT_TRUE(button->isDisabled());

		auto &mouseInteraction =
			registry.getComponent<components::MouseButtonInteraction>(
				button->getIdentifier());

		mouseInteraction.getOnButtonReleaseHandler(
			utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_FALSE(button->isSelected());
		EXPECT_FALSE(clicked);
	}

	TEST_F(TestIconButton, AccessibilityLabelRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto button = IconButton::create(
			registry, "favorite", "favorite_border",
			components::Glyph::Style::Outlined, false,
			IconButton::Variant::Standard, IconButton::Size::Medium, false, "",
			std::function<void(void)>());

		button->setAccessibilityLabel("Favorite");
		EXPECT_EQ(button->getAccessibilityLabel(), "Favorite");
	}

	TEST_F(TestIconButton, BuilderPresetBuildsAndRegistersIconButton)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		IconButton::Builder builder(registry, entityRegistry);

		auto button = builder.makeIconButton(
			parent, "favorite", std::function<void(void)>(),
			IconButton::Variant::Filled, IconButton::Size::Large);

		ASSERT_NE(button, nullptr);
		EXPECT_EQ(button->getParent(), parent);
		EXPECT_EQ(button->getVariant(), IconButton::Variant::Filled);
		EXPECT_EQ(button->getSize(), IconButton::Size::Large);
	}
	TEST_F(TestIconButton, SmallIconButtonReachesTouchTarget)
	{
		ecs::ComponentRegistry registry;
		auto button = IconButton::create(
			registry, "favorite", "", components::Glyph::Style::Outlined, false,
			IconButton::Variant::Standard, IconButton::Size::Small, false, "",
			std::function<void(void)>());

		const auto &bound =
			registry.getComponent<components::Bound>(button->getIdentifier());
		const auto margin =
			registry
				.getComponent<components::MouseHoverInteraction>(
					button->getIdentifier())
				.getAccessibilityMargin();
		EXPECT_GE(bound.getWidth() + 2.0f * margin.getWidth(),
				  theme::SpacingScale::MinTouchTarget);
		EXPECT_FLOAT_EQ(
			registry.getComponent<components::Borders>(button->getIdentifier())
				.getTopLeftRadius(),
			getShapeRadius(theme::ShapeRole::Full));
	}

	namespace
	{
		std::shared_ptr<IconButton>
			makeIconButtonFor(ecs::ComponentRegistry &registry,
							  IconButton::Variant variant, bool toggle,
							  bool disabled = false)
		{
			auto button = IconButton::create(
				registry, "favorite", "", components::Glyph::Style::Outlined,
				toggle, variant, IconButton::Size::Medium, disabled, "Like",
				std::function<void(void)>());
			return button;
		}

		utility::graphic::Color32Bit
			containerOf(ecs::ComponentRegistry &registry,
						const std::shared_ptr<IconButton> &button)
		{
			return registry
				.getComponent<components::Color>(button->getIdentifier())
				.getColor();
		}

		utility::graphic::Color32Bit
			iconOf(ecs::ComponentRegistry &registry,
				   const std::shared_ptr<IconButton> &button)
		{
			return registry
				.getComponent<components::Color>(button->getIconIdentifier())
				.getColor();
		}
	}	 // namespace

	TEST_F(TestIconButton, ColorRolesFollowPaper)
	{
		ecs::ComponentRegistry registry;

		auto filled =
			makeIconButtonFor(registry, IconButton::Variant::Filled, false);
		EXPECT_EQ(containerOf(registry, filled),
				  schemeColor(SchemeColorRole::Primary));
		EXPECT_EQ(iconOf(registry, filled),
				  schemeColor(SchemeColorRole::OnPrimary));

		auto filledToggle =
			makeIconButtonFor(registry, IconButton::Variant::Filled, true);
		EXPECT_EQ(containerOf(registry, filledToggle),
				  schemeColor(SchemeColorRole::SurfaceVariant));
		EXPECT_EQ(iconOf(registry, filledToggle),
				  schemeColor(SchemeColorRole::Primary));

		auto tonal =
			makeIconButtonFor(registry, IconButton::Variant::Tonal, false);
		EXPECT_EQ(containerOf(registry, tonal),
				  schemeColor(SchemeColorRole::SecondaryContainer));
		EXPECT_EQ(iconOf(registry, tonal),
				  schemeColor(SchemeColorRole::OnSecondaryContainer));

		auto standard =
			makeIconButtonFor(registry, IconButton::Variant::Standard, true);
		EXPECT_EQ(iconOf(registry, standard),
				  schemeColor(SchemeColorRole::OnSurfaceVariant));
		standard->setSelected(true);
		EXPECT_EQ(containerOf(registry, standard).getAlpha(), 0);
		EXPECT_EQ(iconOf(registry, standard),
				  schemeColor(SchemeColorRole::Primary));
	}

	TEST_F(TestIconButton, DisabledStandardHasNoContainerOrBorder)
	{
		ecs::ComponentRegistry registry;
		auto button = makeIconButtonFor(registry, IconButton::Variant::Standard,
										false, true);
		EXPECT_EQ(containerOf(registry, button).getAlpha(), 0);
		EXPECT_FALSE(
			registry.getComponent<components::Borders>(button->getIdentifier())
				.hasStroke());
		EXPECT_EQ(iconOf(registry, button), disabledContentColor());
	}

	TEST_F(TestIconButton, OverridesAndHint)
	{
		ecs::ComponentRegistry registry;
		auto button =
			makeIconButtonFor(registry, IconButton::Variant::Filled, false);
		const utility::graphic::Color32Bit icon(9, 9, 9, 255);
		button->setContentColorOverride(icon);
		button->setAccessibilityHint("Adds to favorites");
		EXPECT_EQ(iconOf(registry, button), icon);
		EXPECT_EQ(button->getAccessibilityLabel(), "Like");
		EXPECT_EQ(button->getAccessibilityHint(), "Adds to favorites");
	}

}	 // namespace guillaume::entities::tests
