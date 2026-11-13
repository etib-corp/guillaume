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

#include "entities/test_chip.hpp"

#include <guillaume/entities/style_helpers.hpp>

#include <guillaume/components/mouse_hover_interaction.hpp>

#include <guillaume/components/mouse_button_interaction.hpp>

#include <guillaume/components/elevation.hpp>

#include <guillaume/components/accessibility.hpp>

#include <guillaume/theme.hpp>

#include <memory>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/selection.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>
#include <guillaume/systems/chip.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<Chip>
			makeChip(ecs::ComponentRegistry &registry,
					 Chip::Variant variant		  = Chip::Variant::Assist,
					 const std::string &label	  = "Label",
					 const std::string &iconGlyph = "", bool selected = false)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Chip::Builder builder(registry, entityRegistry);

			return builder.makeChip(nullptr, variant, label, iconGlyph,
									selected);
		}
	}	 // namespace

	TEST_F(TestChip, DefaultGeometryUsesSmallShape)
	{
		ecs::ComponentRegistry registry;
		auto chip = makeChip(registry);

		const auto &bound =
			registry.getComponent<components::Bound>(chip->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 32.0f);

		const auto &borders =
			registry.getComponent<components::Borders>(chip->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(),
						getShapeRadius(theme::ShapeRole::Small));
	}

	TEST_F(TestChip, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto chip = makeChip(registry);

		chip->setVariant(Chip::Variant::Filter);
		EXPECT_EQ(chip->getVariant(), Chip::Variant::Filter);

		chip->setVariant(Chip::Variant::Input);
		EXPECT_EQ(chip->getVariant(), Chip::Variant::Input);
	}

	TEST_F(TestChip, SelectedRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto chip = makeChip(registry);

		EXPECT_FALSE(chip->isSelected());
		chip->setSelected(true);
		EXPECT_TRUE(chip->isSelected());

		chip->setSelected(false);
		EXPECT_FALSE(chip->isSelected());
	}

	TEST_F(TestChip, FilterSelectedUsesContainerFill)
	{
		ecs::ComponentRegistry registry;
		auto chip = makeChip(registry, Chip::Variant::Filter, "Tag");

		chip->setSelected(true);

		const auto &color =
			registry.getComponent<components::Color>(chip->getIdentifier());
		EXPECT_GT(color.getColor().getAlpha(), 0);
	}

	TEST_F(TestChip, InputHasTrailingRemoveIcon)
	{
		ecs::ComponentRegistry registry;
		auto chip = makeChip(registry, Chip::Variant::Input, "Email");

		EXPECT_EQ(chip->getChildren().size(), 2U);
	}

	TEST_F(TestChip, BuilderPresetBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Chip::Builder builder(registry, entityRegistry);

		auto chip = builder.makeChip(parent, Chip::Variant::Filter, "Tag",
									 "check", true);

		ASSERT_NE(chip, nullptr);
		EXPECT_EQ(chip->getParent(), parent);
		EXPECT_EQ(chip->getVariant(), Chip::Variant::Filter);
		EXPECT_TRUE(chip->isSelected());
	}

	TEST_F(TestChip, DefaultsToFlatSecondaryContainer)
	{
		ecs::ComponentRegistry registry;
		auto chip = makeChip(registry);

		EXPECT_EQ(chip->getMode(), Chip::Mode::Flat);
		EXPECT_EQ(
			registry.getComponent<components::Color>(chip->getIdentifier())
				.getColor(),
			schemeColor(SchemeColorRole::SecondaryContainer));
		EXPECT_FALSE(
			registry.getComponent<components::Borders>(chip->getIdentifier())
				.hasStroke());

		chip->setMode(Chip::Mode::Outlined);
		EXPECT_EQ(
			registry.getComponent<components::Color>(chip->getIdentifier())
				.getColor()
				.getAlpha(),
			0);
		EXPECT_EQ(
			registry.getComponent<components::Borders>(chip->getIdentifier())
				.getColor(),
			schemeColor(SchemeColorRole::Outline));
	}

	TEST_F(TestChip, FilterCheckOnlyWhenSelected)
	{
		ecs::ComponentRegistry registry;
		auto chip = makeChip(registry, Chip::Variant::Filter, "Tag");

		// The chip owns the check and the label; the check is only laid out
		// while selected, so the Layout component is what tracks it.
		const auto &layout =
			registry.getComponent<components::Layout>(chip->getIdentifier());
		EXPECT_EQ(chip->getChildren().size(), 2U);

		// Unselected filter chip: label only.
		EXPECT_EQ(layout.getChildren().size(), 1U);

		registry
			.getComponent<components::MouseButtonInteraction>(
				chip->getIdentifier())
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_TRUE(chip->isSelected());
		EXPECT_EQ(layout.getChildren().size(), 2U);
		EXPECT_TRUE(
			registry
				.getComponent<components::Accessibility>(chip->getIdentifier())
				.isSelected());
	}

	TEST_F(TestChip, PressCloseAndDisabled)
	{
		ecs::ComponentRegistry registry;
		auto chip	= makeChip(registry, Chip::Variant::Input, "Email");
		int presses = 0;
		int closes	= 0;
		chip->setOnPress([&presses]() {
			++presses;
		});
		chip->setOnClose([&closes]() {
			++closes;
		});

		registry
			.getComponent<components::MouseButtonInteraction>(
				chip->getIdentifier())
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_EQ(presses, 1);
		ASSERT_NE(chip->getCloseIconIdentifier(),
				  ecs::Entity::InvalidIdentifier);
		registry
			.getComponent<components::MouseButtonInteraction>(
				chip->getCloseIconIdentifier())
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_EQ(closes, 1);
		EXPECT_EQ(
			registry
				.getComponent<components::Accessibility>(chip->getIdentifier())
				.getRole(),
			components::AccessibilityRole::Button);

		chip->setDisabled(true);
		registry
			.getComponent<components::MouseButtonInteraction>(
				chip->getIdentifier())
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();
		registry
			.getComponent<components::MouseButtonInteraction>(
				chip->getCloseIconIdentifier())
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_EQ(presses, 1);
		EXPECT_EQ(closes, 1);
		EXPECT_EQ(
			registry.getComponent<components::Color>(chip->getIdentifier())
				.getColor(),
			disabledContainerColor());
	}

	TEST_F(TestChip, HoverAddsStateLayerAndElevatedCastsShadow)
	{
		ecs::ComponentRegistry registry;
		auto chip = makeChip(registry);

		const auto rest =
			registry.getComponent<components::Color>(chip->getIdentifier())
				.getColor();
		registry
			.getComponent<components::MouseHoverInteraction>(
				chip->getIdentifier())
			.setHovered(true);
		systems::Chip::apply(registry, chip->getIdentifier());
		EXPECT_NE(
			registry.getComponent<components::Color>(chip->getIdentifier())
				.getColor(),
			rest);

		chip->setElevated(true);
		EXPECT_TRUE(
			registry.getComponent<components::Elevation>(chip->getIdentifier())
				.hasShadow());
	}

}	 // namespace guillaume::entities::tests
