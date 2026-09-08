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

#include "entities/test_split_button.hpp"

#include <guillaume/components/mouse_button_interaction.hpp>

namespace guillaume::entities::tests
{
	TEST_F(TestSplitButton, CreatesActionAndChevron)
	{
		ecs::ComponentRegistry registry;
		auto split = std::make_shared<SplitButton>(registry, "Save",
												   std::function<void(void)>(),
												   std::function<void(void)>());

		split->initialize();
		split->update();

		EXPECT_NE(split->getActionIdentifier(), ecs::Entity::InvalidIdentifier);
		EXPECT_NE(split->getChevronIdentifier(),
				  ecs::Entity::InvalidIdentifier);
	}

	TEST_F(TestSplitButton, MainActionFiresOnClick)
	{
		ecs::ComponentRegistry registry;
		bool clicked = false;
		auto split	 = std::make_shared<SplitButton>(
			registry, "Save",
			[&clicked]() {
				clicked = true;
			},
			std::function<void(void)>());

		split->initialize();
		split->update();

		const auto actionId = split->getActionIdentifier();

		auto &actionInteraction =
			registry.getComponent<components::MouseButtonInteraction>(actionId);

		actionInteraction.setOnButtonReleaseHandler(
			utility::event::MouseButtonEvent::Button::Left, [&clicked]() {
				clicked = true;
			});
		actionInteraction.getOnButtonReleaseHandler(
			utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_TRUE(clicked);
	}

	TEST_F(TestSplitButton, ChevronFiresMenuOpen)
	{
		ecs::ComponentRegistry registry;
		bool opened = false;
		auto split	= std::make_shared<SplitButton>(
			registry, "Save", std::function<void(void)>(), [&opened]() {
				opened = true;
			});

		split->initialize();
		split->update();

		const auto chevronId = split->getChevronIdentifier();

		auto &chevronInteraction =
			registry.getComponent<components::MouseButtonInteraction>(
				chevronId);

		chevronInteraction.setOnButtonReleaseHandler(
			utility::event::MouseButtonEvent::Button::Left, [&opened]() {
				opened = true;
			});
		chevronInteraction.getOnButtonReleaseHandler(
			utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_TRUE(opened);
	}
}	 // namespace guillaume::entities::tests
