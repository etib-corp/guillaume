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

#include "entities/test_floating_action_button.hpp"

#include <guillaume/components/transform.hpp>

namespace guillaume::entities::tests
{
	TEST_F(TestFloatingActionButton, CreatesIconAndLabelChildren)
	{
		ecs::ComponentRegistry registry;
		auto fab = std::make_shared<FloatingActionButton>(
			registry, "edit", components::Glyph::Style::Outlined, "Edit",
			FloatingActionButton::Variant::Surface,
			FloatingActionButton::Size::Medium, false, false, "",
			std::function<void(void)>());

		fab->initialize();
		fab->update();

		EXPECT_NE(fab->getIconIdentifier(), ecs::Entity::InvalidIdentifier);
		EXPECT_NE(fab->getLabelIdentifier(), ecs::Entity::InvalidIdentifier);
	}

	TEST_F(TestFloatingActionButton, DisabledFabIgnoresRelease)
	{
		ecs::ComponentRegistry registry;
		bool clicked = false;
		auto fab	 = std::make_shared<FloatingActionButton>(
			registry, "edit", components::Glyph::Style::Outlined, "Edit",
			FloatingActionButton::Variant::Surface,
			FloatingActionButton::Size::Medium, false, false, "", [&clicked]() {
				clicked = true;
			});

		fab->initialize();
		fab->update();

		fab->setDisabled(true);
		EXPECT_TRUE(fab->isDisabled());

		auto &mouseInteraction =
			registry.getComponent<components::MouseButtonInteraction>(
				fab->getIdentifier());

		mouseInteraction.getOnButtonReleaseHandler(
			utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_FALSE(clicked);
	}

	TEST_F(TestFloatingActionButton, AccessibilityLabelRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto fab = std::make_shared<FloatingActionButton>(
			registry, "edit", components::Glyph::Style::Outlined, "Edit",
			FloatingActionButton::Variant::Surface,
			FloatingActionButton::Size::Medium, false, false, "",
			std::function<void(void)>());

		fab->initialize();
		fab->update();

		fab->setAccessibilityLabel("Edit comment");
		EXPECT_EQ(fab->getAccessibilityLabel(), "Edit comment");
	}
}	 // namespace guillaume::entities::tests
