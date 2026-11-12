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

#include "systems/test_floating_action_button_menu.hpp"

#include <guillaume/entities/style_helpers.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Menu		   = guillaume::systems::FloatingActionButtonMenu;
		using SharedButton = guillaume::systems::Button;
	}	 // namespace

	TEST_F(TestFloatingActionButtonMenuSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestFloatingActionButtonMenuSystem, InstallConfiguresTheColumn)
	{
		const auto &layout =
			_registry.getComponent<components::Layout>(_entity);

		EXPECT_EQ(layout.getAxis(), components::Layout::Axis::Vertical);
		EXPECT_FLOAT_EQ(layout.getSpacing(),
						components::FloatingActionButtonMenu::Spacing);
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_trigger)
					  .getLabel(),
				  "Create");
	}

	TEST_F(TestFloatingActionButtonMenuSystem, AClosedMenuShowsOnlyItsTrigger)
	{
		_system.update(_entity);

		EXPECT_EQ(layoutChildren(),
				  std::vector<ecs::Entity::Identifier> { _trigger });
		for (const auto action: _actions) {
			EXPECT_FALSE(
				_registry.getComponent<components::Button>(action).isVisible());
			EXPECT_FALSE(
				_registry.getComponent<components::Accessibility>(action)
					.isFocusable());
			EXPECT_EQ(
				_registry.getComponent<components::Color>(action).getColor(),
				entities::transparentColor());
		}
		const auto &accessibility =
			_registry.getComponent<components::Accessibility>(_trigger);
		EXPECT_EQ(accessibility.getExpanded(), false);
		EXPECT_EQ(
			_registry.getComponent<components::Glyph>(_triggerIcon).getName(),
			"add");
	}

	TEST_F(TestFloatingActionButtonMenuSystem, OpeningStacksTheActionsAboveIt)
	{
		_system.update(_entity);

		Menu::open(_registry, _entity);

		EXPECT_TRUE(menu().isOpen());
		EXPECT_EQ(layoutChildren(),
				  (std::vector<ecs::Entity::Identifier> {
					  _actions[0], _actions[1], _trigger }));
		EXPECT_LT(positionY(_actions[0]), positionY(_actions[1]));
		EXPECT_LT(positionY(_actions[1]), positionY(_trigger));
		for (const auto action: _actions) {
			EXPECT_TRUE(
				_registry.getComponent<components::Button>(action).isVisible());
			EXPECT_TRUE(
				_registry.getComponent<components::Accessibility>(action)
					.isFocusable());
			EXPECT_NE(
				_registry.getComponent<components::Color>(action).getColor(),
				entities::transparentColor());
		}
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_trigger)
					  .getExpanded(),
				  true);
		EXPECT_EQ(
			_registry.getComponent<components::Glyph>(_triggerIcon).getName(),
			Menu::CloseGlyph);
	}

	TEST_F(TestFloatingActionButtonMenuSystem, ClosingRestoresTheTrigger)
	{
		Menu::open(_registry, _entity);

		Menu::close(_registry, _entity);

		EXPECT_FALSE(menu().isOpen());
		EXPECT_EQ(layoutChildren(),
				  std::vector<ecs::Entity::Identifier> { _trigger });
		EXPECT_EQ(
			_registry.getComponent<components::Glyph>(_triggerIcon).getName(),
			"add");
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_trigger)
					  .getExpanded(),
				  false);
	}

	TEST_F(TestFloatingActionButtonMenuSystem, TheHandlerSeesEveryRealChange)
	{
		std::vector<bool> states;
		menu().setOnStateChange([&states](bool open) {
			states.push_back(open);
		});

		Menu::close(_registry, _entity);
		Menu::open(_registry, _entity);
		Menu::open(_registry, _entity);
		Menu::toggle(_registry, _entity);
		Menu::toggle(_registry, _entity);

		EXPECT_EQ(states, (std::vector<bool> { true, false, true }));
	}

	TEST_F(TestFloatingActionButtonMenuSystem, ClickingTheTriggerToggles)
	{
		_system.update(_entity);

		// The click handler wired by install() toggles the menu.
		SharedButton::release(_registry, _trigger);
		EXPECT_TRUE(menu().isOpen());

		SharedButton::release(_registry, _trigger);
		EXPECT_FALSE(menu().isOpen());
	}

	TEST_F(TestFloatingActionButtonMenuSystem, ActionsRunOnlyWhileOpen)
	{
		_system.update(_entity);

		SharedButton::release(_registry, _actions[1]);
		EXPECT_EQ(_clicks[1], 0);

		Menu::open(_registry, _entity);
		SharedButton::release(_registry, _actions[1]);
		EXPECT_EQ(_clicks[0], 0);
		EXPECT_EQ(_clicks[1], 1);
	}

	TEST_F(TestFloatingActionButtonMenuSystem,
		   AnUnchangedMenuIsNotMarkedChanged)
	{
		for (const bool open: { false, true }) {
			menu().setOpen(open);
			_system.update(_entity);
			_registry.resetChangedFlags();

			_system.update(_entity);

			EXPECT_FALSE(_registry.hasChanged(_entity));
			EXPECT_FALSE(_registry.hasChanged(_trigger));
			EXPECT_FALSE(_registry.hasChanged(_triggerIcon));
			for (const auto action: _actions) {
				EXPECT_FALSE(_registry.hasChanged(action));
				EXPECT_FALSE(_registry.hasChanged(action + 1));
			}
		}
	}

}	 // namespace guillaume::systems::tests
