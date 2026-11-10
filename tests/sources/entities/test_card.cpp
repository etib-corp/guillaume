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

#include "entities/test_card.hpp"

#include <guillaume/entities/text.hpp>

#include <guillaume/entities/image.hpp>

#include <guillaume/entities/button.hpp>

#include <guillaume/components/text.hpp>

#include <guillaume/components/style_state.hpp>

#include <guillaume/components/mouse_button_interaction.hpp>

#include <guillaume/components/animation.hpp>

#include <guillaume/components/accessibility.hpp>

#include <memory>

#include <guillaume/components/borders.hpp>
#include <guillaume/theme.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/elevation.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<Card>
			makeCard(ecs::ComponentRegistry &registry,
					 Card::Variant variant = Card::Variant::Elevated)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Card::Builder builder(registry, entityRegistry);
			Card::Director director;

			return director.makeCard(builder, nullptr, variant);
		}
	}	 // namespace

	TEST_F(TestCard, DefaultSizeAndRadius)
	{
		ecs::ComponentRegistry registry;
		auto card = makeCard(registry);

		card->initialize();
		card->update();

		const auto &bound =
			registry.getComponent<components::Bound>(card->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 240.0f);

		const auto &borders =
			registry.getComponent<components::Borders>(card->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(),
						getShapeRadius(theme::ShapeRole::Medium));
	}

	TEST_F(TestCard, ElevatedHasElevationAndSurfaceColor)
	{
		ecs::ComponentRegistry registry;
		auto card = makeCard(registry, Card::Variant::Elevated);

		card->initialize();
		card->update();

		const auto &elevation =
			registry.getComponent<components::Elevation>(card->getIdentifier());
		EXPECT_GT(elevation.getLevel(), 0.0f);
	}

	TEST_F(TestCard, FilledHasNoElevation)
	{
		ecs::ComponentRegistry registry;
		auto card = makeCard(registry, Card::Variant::Filled);

		card->initialize();
		card->update();

		const auto &elevation =
			registry.getComponent<components::Elevation>(card->getIdentifier());
		EXPECT_FLOAT_EQ(elevation.getLevel(), 0.0f);
	}

	TEST_F(TestCard, OutlinedUsesSurfaceFillAndOutlineBorder)
	{
		ecs::ComponentRegistry registry;
		auto card = makeCard(registry, Card::Variant::Outlined);

		card->initialize();
		card->update();

		const auto &color =
			registry.getComponent<components::Color>(card->getIdentifier());
		EXPECT_EQ(color.getColor(),
				  getActiveScheme().getColor(SchemeColorRole::Surface).getColor());

		const auto &borders =
			registry.getComponent<components::Borders>(card->getIdentifier());
		EXPECT_TRUE(borders.hasStroke());
		EXPECT_FLOAT_EQ(borders.getWidth(), 1.0f);
		EXPECT_EQ(borders.getColor(),
				  getActiveScheme()
					  .getColor(SchemeColorRole::OutlineVariant)
					  .getColor());
	}

	TEST_F(TestCard, VariantSwitchUpdatesStyle)
	{
		ecs::ComponentRegistry registry;
		auto card = makeCard(registry, Card::Variant::Filled);

		card->setVariant(Card::Variant::Outlined);

		EXPECT_EQ(card->getVariant(), Card::Variant::Outlined);
		const auto &color =
			registry.getComponent<components::Color>(card->getIdentifier());
		EXPECT_EQ(color.getColor(),
				  getActiveScheme().getColor(SchemeColorRole::Surface).getColor());
	}

	TEST_F(TestCard, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Card::Builder builder(registry, entityRegistry);
		Card::Director director;

		auto card = director.makeCard(builder, parent, Card::Variant::Filled);

		ASSERT_NE(card, nullptr);
		EXPECT_EQ(card->getParent(), parent);
		EXPECT_EQ(card->getVariant(), Card::Variant::Filled);
	}

	TEST_F(TestCard, VariantColorRolesMatchPaper)
	{
		ecs::ComponentRegistry registry;
		auto elevated = makeCard(registry, Card::Variant::Elevated);
		auto filled	  = makeCard(registry, Card::Variant::Filled);

		EXPECT_EQ(
			registry.getComponent<components::Color>(elevated->getIdentifier())
				.getColor(),
			getElevatedSurfaceColor(theme::ElevationLevel::Level1));
		EXPECT_EQ(
			registry.getComponent<components::Color>(filled->getIdentifier())
				.getColor(),
			getActiveScheme()
				.getColor(SchemeColorRole::SurfaceVariant)
				.getColor());
	}

	TEST_F(TestCard, PressRaisesElevationAndCallsHandler)
	{
		ecs::ComponentRegistry registry;
		auto card	= makeCard(registry);
		int presses = 0;
		card->setOnPress([&presses]() {
			++presses;
		});
		card->initialize();

		EXPECT_TRUE(card->isInteractive());
		EXPECT_EQ(
			registry.getComponent<components::Accessibility>(card->getIdentifier())
				.getRole(),
			components::AccessibilityRole::Button);

		auto &mouse = registry.getComponent<components::MouseButtonInteraction>(
			card->getIdentifier());
		mouse.setButtonPressed(utility::event::MouseButtonEvent::Button::Left,
							   true);
		mouse.getOnButtonPressHandler(
			utility::event::MouseButtonEvent::Button::Left)();

		auto &animation =
			registry.getComponent<components::Animation>(card->getIdentifier());
		EXPECT_TRUE(animation.isPlaying());
		EXPECT_FLOAT_EQ(animation.getDuration(),
						getMotionDuration(theme::MotionDuration::Short3));
		animation.advance(1.0f);
		EXPECT_FLOAT_EQ(card->getPressProgress(), 1.0f);
		const auto &elevation =
			registry.getComponent<components::Elevation>(card->getIdentifier());
		EXPECT_EQ(elevation.getElevationLevel(), theme::ElevationLevel::Level2);
		EXPECT_FLOAT_EQ(
			elevation.getSpread(),
			getElevationToken(theme::ElevationLevel::Level2).shadowSpread);

		mouse.setButtonPressed(utility::event::MouseButtonEvent::Button::Left,
							   false);
		mouse.getOnButtonReleaseHandler(
			utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_EQ(presses, 1);
		animation.advance(1.0f);
		EXPECT_FLOAT_EQ(card->getPressProgress(), 0.0f);
	}

	TEST_F(TestCard, DisabledCardIgnoresPress)
	{
		ecs::ComponentRegistry registry;
		auto card	= makeCard(registry);
		int presses = 0;
		card->setOnPress([&presses]() {
			++presses;
		});
		card->setDisabled(true);
		card->initialize();

		EXPECT_TRUE(card->isDisabled());
		registry
			.getComponent<components::MouseButtonInteraction>(
				card->getIdentifier())
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_EQ(presses, 0);
		EXPECT_FALSE(
			registry.getComponent<components::StyleState>(card->getIdentifier())
				.isEnabled());
	}

	TEST_F(TestCard, ComposesCoverTitleContentActionsInOrder)
	{
		ecs::ComponentRegistry registry;
		auto card = makeCard(registry);
		card->initialize();

		int actionPresses = 0;
		card->addAction("Ok", [&actionPresses]() {
			++actionPresses;
		});
		card->setContent("Body");
		card->setTitle("Title", "Subtitle");
		card->setCover("images/cover.png");

		const auto &children = card->getChildren();
		ASSERT_EQ(children.size(), 4u);
		EXPECT_EQ(children[0], card->getCover());
		EXPECT_EQ(children[1], card->getTitleSection());
		EXPECT_EQ(children[2], card->getContent());
		EXPECT_EQ(children[3], card->getActionsSection());

		EXPECT_FLOAT_EQ(
			registry
				.getComponent<components::Bound>(card->getCover()->getIdentifier())
				.getHeight(),
			Card::CoverHeight);
		EXPECT_EQ(registry
					  .getComponent<components::Accessibility>(
						  card->getTitleSection()->getIdentifier())
					  .getRole(),
				  components::AccessibilityRole::Header);
		// Text entities push their content on update (as the systems do).
		card->getContent()->update();
		EXPECT_EQ(registry
					  .getComponent<components::Text>(
						  card->getContent()->getIdentifier())
					  .getContent(),
				  "Body");

		ASSERT_EQ(card->getActions().size(), 1u);
		EXPECT_EQ(card->getActions().front()->getAccessibilityLabel(), "Ok");
		registry
			.getComponent<components::MouseButtonInteraction>(
				card->getActions().front()->getIdentifier())
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_EQ(actionPresses, 1);
	}

}	 // namespace guillaume::entities::tests
