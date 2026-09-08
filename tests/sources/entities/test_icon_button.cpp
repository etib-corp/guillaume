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

#include <guillaume/components/transform.hpp>

namespace guillaume::entities::tests
{
	TEST_F(TestIconButton, ToggleFlipsSelectedOnRelease)
	{
		ecs::ComponentRegistry registry;
		auto button = std::make_shared<IconButton>(
			registry, "favorite", "favorite_border",
			components::Glyph::Style::Outlined, true,
			IconButton::Variant::Standard, IconButton::Size::Medium, false, "",
			std::function<void(void)>());

		button->initialize();
		button->update();

		EXPECT_FALSE(button->isSelected());

		auto &mouseInteraction =
			registry.getComponent<components::MouseButtonInteraction>(
				button->getIdentifier());

		mouseInteraction.getOnButtonReleaseHandler(
			utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_TRUE(button->isSelected());
	}

	TEST_F(TestIconButton, SetSelectedUpdatesState)
	{
		ecs::ComponentRegistry registry;
		auto button = std::make_shared<IconButton>(
			registry, "favorite", "favorite_border",
			components::Glyph::Style::Outlined, true,
			IconButton::Variant::Standard, IconButton::Size::Medium, false, "",
			std::function<void(void)>());

		button->initialize();
		button->update();

		button->setSelected(true);
		EXPECT_TRUE(button->isSelected());

		button->setSelected(false);
		EXPECT_FALSE(button->isSelected());
	}

	TEST_F(TestIconButton, SetSelectedIgnoredWhenNotToggle)
	{
		ecs::ComponentRegistry registry;
		auto button = std::make_shared<IconButton>(
			registry, "favorite", "favorite_border",
			components::Glyph::Style::Outlined, false,
			IconButton::Variant::Standard, IconButton::Size::Medium, false, "",
			std::function<void(void)>());

		button->initialize();
		button->update();

		button->setSelected(true);
		EXPECT_FALSE(button->isSelected());
	}

	TEST_F(TestIconButton, DisabledButtonIgnoresRelease)
	{
		ecs::ComponentRegistry registry;
		bool clicked = false;
		auto button	 = std::make_shared<IconButton>(
			registry, "favorite", "favorite_border",
			components::Glyph::Style::Outlined, true,
			IconButton::Variant::Standard, IconButton::Size::Medium, false, "",
			[&clicked]() {
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

	TEST_F(TestIconButton, AccessibilityLabelRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto button = std::make_shared<IconButton>(
			registry, "favorite", "favorite_border",
			components::Glyph::Style::Outlined, false,
			IconButton::Variant::Standard, IconButton::Size::Medium, false, "",
			std::function<void(void)>());

		button->initialize();
		button->update();

		button->setAccessibilityLabel("Favorite");
		EXPECT_EQ(button->getAccessibilityLabel(), "Favorite");
	}
}	 // namespace guillaume::entities::tests
