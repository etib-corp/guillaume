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

#include "entities/test_floating_action_button_menu.hpp"

#include <functional>
#include <vector>

namespace guillaume::entities::tests
{
	TEST_F(TestFloatingActionButtonMenu, CreatesTriggerAndActions)
	{
		ecs::ComponentRegistry registry;
		std::vector<FloatingActionButtonMenu::Action> actions {
			FloatingActionButtonMenu::Action { "Add", "add",
											   std::function<void(void)>() },
			FloatingActionButtonMenu::Action { "Edit", "edit",
											   std::function<void(void)>() },
		};
		auto menu = std::make_shared<FloatingActionButtonMenu>(registry, "menu",
															   actions, "Menu");

		menu->initialize();
		menu->update();

		EXPECT_EQ(menu->getActionCount(), 2);
		EXPECT_NE(menu->getTriggerIdentifier(), ecs::Entity::InvalidIdentifier);
		EXPECT_NE(menu->getActionIdentifier(0), ecs::Entity::InvalidIdentifier);
		EXPECT_NE(menu->getActionIdentifier(1), ecs::Entity::InvalidIdentifier);
	}

	TEST_F(TestFloatingActionButtonMenu, StartsClosedAndOpens)
	{
		ecs::ComponentRegistry registry;
		auto menu = std::make_shared<FloatingActionButtonMenu>(
			registry, "menu", std::vector<FloatingActionButtonMenu::Action>(),
			"Menu");

		menu->initialize();
		menu->update();

		EXPECT_FALSE(menu->isOpen());

		menu->open();
		EXPECT_TRUE(menu->isOpen());

		menu->close();
		EXPECT_FALSE(menu->isOpen());
	}

	TEST_F(TestFloatingActionButtonMenu, ToggleFlipsOpenState)
	{
		ecs::ComponentRegistry registry;
		auto menu = std::make_shared<FloatingActionButtonMenu>(
			registry, "menu", std::vector<FloatingActionButtonMenu::Action>(),
			"Menu");

		menu->initialize();
		menu->update();

		menu->toggle();
		EXPECT_TRUE(menu->isOpen());

		menu->toggle();
		EXPECT_FALSE(menu->isOpen());
	}
}	 // namespace guillaume::entities::tests
