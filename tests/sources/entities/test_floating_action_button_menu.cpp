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

#include <guillaume/components/mouse_button_interaction.hpp>

#include <guillaume/components/accessibility.hpp>

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

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

	TEST_F(TestFloatingActionButtonMenu, DirectorBuildsAndRegistersMenu)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		FloatingActionButtonMenu::Builder builder(registry, entityRegistry);
		FloatingActionButtonMenu::Director director;

		auto menu =
			director.makeFloatingActionButtonMenu(builder, parent, "add");

		ASSERT_NE(menu, nullptr);
		EXPECT_EQ(menu->getParent(), parent);
		EXPECT_NE(menu->getTriggerIdentifier(), ecs::Entity::InvalidIdentifier);
	}
	TEST_F(TestFloatingActionButtonMenu, ClosedActionsAreHiddenAndInert)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		int edits = 0;
		std::vector<bool> states;
		FloatingActionButtonMenu::Builder builder(registry, entityRegistry);
		auto menu = builder.withIcon("add")
						.withAccessibilityLabel("Create")
						.addAction("edit", "Edit",
								   [&edits]() {
									   ++edits;
								   })
						.registerEntity(nullptr);
		menu->setOnStateChange([&states](bool open) {
			states.push_back(open);
		});
		menu->update();

		const auto action  = menu->getActionIdentifier(0);
		const auto trigger = menu->getTriggerIdentifier();
		registry
			.getComponent<components::MouseButtonInteraction>(action)
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_EQ(edits, 0);

		const auto &accessibility =
			registry.getComponent<components::Accessibility>(trigger);
		EXPECT_EQ(accessibility.getLabel(), "Create");
		EXPECT_EQ(accessibility.getExpanded(), false);

		registry
			.getComponent<components::MouseButtonInteraction>(trigger)
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_TRUE(menu->isOpen());
		EXPECT_EQ(accessibility.getExpanded(), true);
		ASSERT_EQ(states.size(), 1u);
		EXPECT_TRUE(states[0]);

		registry
			.getComponent<components::MouseButtonInteraction>(action)
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_EQ(edits, 1);

		// The labelled action renders its label.
		bool hasLabel = false;
		for (const auto &child: menu->getEntities()) {
			if (child->getIdentifier() == action) {
				hasLabel = true;
			}
		}
		EXPECT_TRUE(hasLabel);
		EXPECT_EQ(registry.getComponent<components::Accessibility>(action)
					  .getLabel(),
				  "Edit");
	}

}	 // namespace guillaume::entities::tests
