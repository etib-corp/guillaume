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

#include "entities/test_button.hpp"

#include <guillaume/components/transform.hpp>

namespace guillaume::entities::tests
{
	TEST_F(TestButton, ChildrenHaveDistinctLayersAndDepthOffsets)
	{
		ecs::ComponentRegistry registry;
		auto button = std::make_shared<Button>(
			registry, "home", components::Glyph::Style::Outlined, "Save", false,
			Button::Color::Filled, Button::Shape::Round, Button::Size::Medium,
			false, false, false, "", std::function<void(void)>());

		auto &buttonTransform = registry.getComponent<components::Transform>(
			button->getIdentifier());
		buttonTransform.setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(0.0f, 0.0f, -300.0f),
			utility::graphic::OrientationF()));

		button->initialize();
		button->update();

		const auto iconIdentifier  = button->getIconIdentifier();
		const auto labelIdentifier = button->getLabelIdentifier();

		ASSERT_NE(iconIdentifier, ecs::Entity::InvalidIdentifier);
		ASSERT_NE(labelIdentifier, ecs::Entity::InvalidIdentifier);

		const auto iconEntity = button->getEntity<ecs::Entity>(iconIdentifier);
		const auto labelEntity =
			button->getEntity<ecs::Entity>(labelIdentifier);
		ASSERT_NE(iconEntity, nullptr);
		ASSERT_NE(labelEntity, nullptr);

		EXPECT_EQ(iconEntity->getLayer(), 1);
		EXPECT_EQ(labelEntity->getLayer(), 1);

		const float baseZ = buttonTransform.getPose().getPosition().getZ();
		const auto iconZ =
			registry.getComponent<components::Transform>(iconIdentifier)
				.getPose()
				.getPosition()
				.getZ();
		const auto textZ =
			registry.getComponent<components::Transform>(labelIdentifier)
				.getPose()
				.getPosition()
				.getZ();

		EXPECT_FLOAT_EQ(iconZ, baseZ + 1.0f);
		EXPECT_FLOAT_EQ(textZ, baseZ + 1.0f);
		EXPECT_LT(baseZ, iconZ);
		EXPECT_LT(baseZ, textZ);
	}

	TEST_F(TestButton, EmptyContentStillCreatesBothChildren)
	{
		ecs::ComponentRegistry registry;
		auto button = std::make_shared<Button>(
			registry, "", components::Glyph::Style::Outlined, "", false,
			Button::Color::Filled, Button::Shape::Round, Button::Size::Medium,
			false, false, false, "", std::function<void(void)>());

		auto &buttonTransform = registry.getComponent<components::Transform>(
			button->getIdentifier());
		buttonTransform.setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(0.0f, 0.0f, -300.0f),
			utility::graphic::OrientationF()));

		button->initialize();
		button->update();

		const auto iconIdentifier  = button->getIconIdentifier();
		const auto labelIdentifier = button->getLabelIdentifier();

		ASSERT_NE(iconIdentifier, ecs::Entity::InvalidIdentifier);
		ASSERT_NE(labelIdentifier, ecs::Entity::InvalidIdentifier);

		const auto iconEntity = button->getEntity<ecs::Entity>(iconIdentifier);
		const auto labelEntity =
			button->getEntity<ecs::Entity>(labelIdentifier);
		ASSERT_NE(iconEntity, nullptr);
		ASSERT_NE(labelEntity, nullptr);
		EXPECT_EQ(iconEntity->getLayer(), 1);
		EXPECT_EQ(labelEntity->getLayer(), 1);

		const float baseZ = buttonTransform.getPose().getPosition().getZ();
		const auto iconZ =
			registry.getComponent<components::Transform>(iconIdentifier)
				.getPose()
				.getPosition()
				.getZ();
		const auto textZ =
			registry.getComponent<components::Transform>(labelIdentifier)
				.getPose()
				.getPosition()
				.getZ();

		EXPECT_FLOAT_EQ(iconZ, baseZ + 1.0f);
		EXPECT_FLOAT_EQ(textZ, baseZ + 1.0f);
	}

	TEST_F(TestButton, ToggleFlipsSelectedOnRelease)
	{
		ecs::ComponentRegistry registry;
		auto button = std::make_shared<Button>(
			registry, "", components::Glyph::Style::Outlined, "Save", true,
			Button::Color::Filled, Button::Shape::Round, Button::Size::Medium,
			false, false, false, "", std::function<void(void)>());

		button->initialize();
		button->update();

		EXPECT_FALSE(button->isSelected());

		auto &mouseInteraction =
			registry.getComponent<components::MouseButtonInteraction>(
				button->getIdentifier());

		mouseInteraction.getOnButtonReleaseHandler(
			utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_TRUE(button->isSelected());

		mouseInteraction.getOnButtonReleaseHandler(
			utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_FALSE(button->isSelected());
	}

	TEST_F(TestButton, SetSelectedUpdatesState)
	{
		ecs::ComponentRegistry registry;
		auto button = std::make_shared<Button>(
			registry, "", components::Glyph::Style::Outlined, "Save", true,
			Button::Color::Filled, Button::Shape::Round, Button::Size::Medium,
			false, false, false, "", std::function<void(void)>());

		button->initialize();
		button->update();

		button->setSelected(true);
		EXPECT_TRUE(button->isSelected());

		button->setSelected(false);
		EXPECT_FALSE(button->isSelected());
	}

	TEST_F(TestButton, SetSelectedIgnoredWhenNotToggle)
	{
		ecs::ComponentRegistry registry;
		auto button = std::make_shared<Button>(
			registry, "", components::Glyph::Style::Outlined, "Save", false,
			Button::Color::Filled, Button::Shape::Round, Button::Size::Medium,
			false, false, false, "", std::function<void(void)>());

		button->initialize();
		button->update();

		button->setSelected(true);
		EXPECT_FALSE(button->isSelected());
	}

	TEST_F(TestButton, SetIsToggleFalseResetsSelected)
	{
		ecs::ComponentRegistry registry;
		auto button = std::make_shared<Button>(
			registry, "", components::Glyph::Style::Outlined, "Save", true,
			Button::Color::Filled, Button::Shape::Round, Button::Size::Medium,
			false, false, false, "", std::function<void(void)>());

		button->initialize();
		button->update();

		button->setSelected(true);
		EXPECT_TRUE(button->isSelected());

		button->setIsToggle(false);
		EXPECT_FALSE(button->isSelected());
	}

	TEST_F(TestButton, MorphGatesShapeFlip)
	{
		ecs::ComponentRegistry registry;
		auto button = std::make_shared<Button>(
			registry, "", components::Glyph::Style::Outlined, "Save", true,
			Button::Color::Filled, Button::Shape::Round, Button::Size::Medium,
			false, false, false, "", std::function<void(void)>());

		button->initialize();
		button->update();

		auto &borders =
			registry.getComponent<components::Borders>(button->getIdentifier());

		button->setSelected(true);
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 100.0f);

		button->setMorph(true);
		button->setSelected(true);
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 16.0f);
	}

	TEST_F(TestButton, IconOnlyButtonHasNoLabelSpacing)
	{
		ecs::ComponentRegistry registry;
		auto button = std::make_shared<Button>(
			registry, "home", components::Glyph::Style::Outlined, "", false,
			Button::Color::Filled, Button::Shape::Round, Button::Size::Medium,
			false, false, false, "", std::function<void(void)>());

		button->initialize();
		button->update();

		const auto &bound =
			registry.getComponent<components::Bound>(button->getIdentifier());
		EXPECT_GT(bound.getWidth(), 0.0f);
	}

	TEST_F(TestButton, DisabledButtonIgnoresRelease)
	{
		ecs::ComponentRegistry registry;
		bool clicked = false;
		auto button	 = std::make_shared<Button>(
			registry, "", components::Glyph::Style::Outlined, "Save", true,
			Button::Color::Filled, Button::Shape::Round, Button::Size::Medium,
			false, false, false, "", [&clicked]() {
				clicked = true;
			});

		button->initialize();
		button->update();

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

	TEST_F(TestButton, DisabledButtonAppliesDisabledColors)
	{
		ecs::ComponentRegistry registry;
		auto button = std::make_shared<Button>(
			registry, "", components::Glyph::Style::Outlined, "Save", false,
			Button::Color::Filled, Button::Shape::Round, Button::Size::Medium,
			false, false, false, "", std::function<void(void)>());

		button->initialize();
		button->update();

		button->setDisabled(true);

		const auto &color =
			registry.getComponent<components::Color>(button->getIdentifier());
		EXPECT_EQ(color.getColor().getAlpha(), 31);
	}

	TEST_F(TestButton, TrailingIconPlacesIconAfterLabel)
	{
		ecs::ComponentRegistry registry;
		auto button = std::make_shared<Button>(
			registry, "home", components::Glyph::Style::Outlined, "Save", false,
			Button::Color::Filled, Button::Shape::Round, Button::Size::Medium,
			false, false, true, "", std::function<void(void)>());

		button->initialize();
		button->update();

		registry.getComponent<components::Bound>(button->getLabelIdentifier())
			.setWidth(50.0f);

		button->setTrailingIcon(true);

		const auto iconX  = registry
								.getComponent<components::Transform>(
									button->getIconIdentifier())
								.getPose()
								.getPosition()
								.getX();
		const auto labelX = registry
								.getComponent<components::Transform>(
									button->getLabelIdentifier())
								.getPose()
								.getPosition()
								.getX();

		EXPECT_GT(iconX, labelX);
	}

	TEST_F(TestButton, AccessibilityLabelRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto button = std::make_shared<Button>(
			registry, "", components::Glyph::Style::Outlined, "Save", false,
			Button::Color::Filled, Button::Shape::Round, Button::Size::Medium,
			false, false, false, "", std::function<void(void)>());

		button->initialize();
		button->update();

		button->setAccessibilityLabel("Save changes");
		EXPECT_EQ(button->getAccessibilityLabel(), "Save changes");
	}
}	 // namespace guillaume::entities::tests
