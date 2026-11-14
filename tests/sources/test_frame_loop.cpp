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
#include "guillaume/components/hierarchy.hpp"
#include "test_frame_loop.hpp"

#include <guillaume/entities/app_bar.hpp>
#include <guillaume/entities/badge.hpp>
#include <guillaume/entities/button.hpp>
#include <guillaume/entities/card.hpp>
#include <guillaume/entities/checkbox.hpp>
#include <guillaume/entities/chip.hpp>
#include <guillaume/entities/dialog.hpp>
#include <guillaume/entities/divider.hpp>
#include <guillaume/entities/extended_floating_action_button.hpp>
#include <guillaume/entities/floating_action_button.hpp>
#include <guillaume/entities/floating_action_button_menu.hpp>
#include <guillaume/entities/icon.hpp>
#include <guillaume/entities/icon_button.hpp>
#include <guillaume/entities/list.hpp>
#include <guillaume/entities/menu.hpp>
#include <guillaume/entities/navigation_bar.hpp>
#include <guillaume/entities/navigation_drawer.hpp>
#include <guillaume/entities/progress_indicator.hpp>
#include <guillaume/entities/radio_button.hpp>
#include <guillaume/entities/radio_group.hpp>
#include <guillaume/entities/search.hpp>
#include <guillaume/entities/segmented_button.hpp>
#include <guillaume/entities/snackbar.hpp>
#include <guillaume/entities/surface.hpp>
#include <guillaume/entities/switch.hpp>
#include <guillaume/entities/text.hpp>
#include <guillaume/entities/text_field.hpp>
#include <guillaume/entities/tooltip.hpp>
#include <guillaume/entities/view.hpp>

namespace guillaume::tests
{
	namespace
	{
		/**
		 * @brief Call a function once per widget class of the library.
		 * @tparam Function A callable template taking the class as argument.
		 * @param function The callable.
		 */
		template<typename Function> void forEachWidget(Function &&function)
		{
			function.template operator()<entities::AppBar>("AppBar");
			function.template operator()<entities::Badge>("Badge");
			function.template operator()<entities::Button>("Button");
			function.template operator()<entities::Card>("Card");
			function.template operator()<entities::Checkbox>("Checkbox");
			function.template operator()<entities::Chip>("Chip");
			function.template operator()<entities::Dialog>("Dialog");
			function.template operator()<entities::Divider>("Divider");
			function
				.template operator()<entities::ExtendedFloatingActionButton>(
					"ExtendedFloatingActionButton");
			function.template operator()<entities::FloatingActionButton>(
				"FloatingActionButton");
			function.template operator()<entities::FloatingActionButtonMenu>(
				"FloatingActionButtonMenu");
			function.template operator()<entities::IconButton>("IconButton");
			function.template operator()<entities::List>("List");
			function.template operator()<entities::Menu>("Menu");
			function.template operator()<entities::NavigationBar>(
				"NavigationBar");
			function.template operator()<entities::NavigationDrawer>(
				"NavigationDrawer");
			function.template operator()<entities::ProgressIndicator>(
				"ProgressIndicator");
			function.template operator()<entities::RadioButton>("RadioButton");
			function.template operator()<entities::RadioGroup>("RadioGroup");
			function.template operator()<entities::Search>("Search");
			function.template operator()<entities::SegmentedButton>(
				"SegmentedButton");
			function.template operator()<entities::Snackbar>("Snackbar");
			function.template operator()<entities::Surface>("Surface");
			function.template operator()<entities::Switch>("Switch");
			function.template operator()<entities::Text>("Text");
			function.template operator()<entities::TextField>("TextField");
			function.template operator()<entities::Tooltip>("Tooltip");
			function.template operator()<entities::View>("View");
		}
	}	 // namespace

	TEST_F(TestFrameLoop, EveryWidgetRunsThroughFrames)
	{
		forEachWidget([this]<typename EntityType>(const char *name) {
			EXPECT_NE(build<EntityType>(), nullptr) << name;
		});

		for (int frame = 0; frame < 3; ++frame) {
			EXPECT_NO_THROW(runFrame()) << "frame " << frame;
		}
	}

	TEST_F(TestFrameLoop, EachWidgetSettlesAfterAFewFrames)
	{
		// One widget per run, so a widget that keeps marking itself changed
		// is named in the failure.
		forEachWidget([this]<typename EntityType>(const char *name) {
			_entities.clearEntities();
			const auto widget = build<EntityType>();
			ASSERT_NE(widget, nullptr) << name;

			for (int frame = 0; frame < 4; ++frame) {
				runFrame();
			}
			runPhases();

			EXPECT_TRUE(changedEntities().empty())
				<< name << " still changes " << changedEntities().size()
				<< " entities on every frame";
			_registry.resetChangedFlags();
		});
	}

	TEST_F(TestFrameLoop, ReplacedChildrenLeaveTheFrame)
	{
		auto menu = build<entities::Menu>();
		std::vector<entities::Menu::Item> items(3);
		for (auto &item: items) {
			item.label = "Item";
		}
		menu->setItems(items);
		// Open it, so its rows are drawn.
		menu->show();
		runFrame();
		runFrame();
		const auto removedBefore = _engine.removeObjectCallCount;

		items.resize(1);
		menu->setItems(items);

		EXPECT_NO_THROW(runFrame());
		EXPECT_NO_THROW(runFrame());
		// The render systems dropped the graphics of the removed rows.
		EXPECT_GT(_engine.removeObjectCallCount, removedBefore);
	}

	TEST_F(TestFrameLoop, WidgetsWithContentSettleToo)
	{
		auto button = build<entities::Button>();
		button->setLabelContent("Save");
		button->setIconGlyphName("add");

		auto extended = build<entities::ExtendedFloatingActionButton>();
		extended->setLabelContent("Compose");

		auto chip = build<entities::Chip>();
		chip->setLabel("Filter");
		chip->setSelected(true);

		std::vector<entities::Menu::Item> menuItems(3);
		for (auto &item: menuItems) {
			item.label		 = "Item";
			item.leadingIcon = "star";
		}
		build<entities::Menu>()->setItems(menuItems);

		auto checkbox = build<entities::Checkbox>();
		checkbox->setState(entities::Checkbox::State::Checked);

		auto field = build<entities::TextField>();
		field->setText("Hello");

		for (int frame = 0; frame < 4; ++frame) {
			runFrame();
		}
		runPhases();

		EXPECT_TRUE(changedEntities().empty())
			<< changedEntities().size()
			<< " entities still change on every frame";
	}

}	 // namespace guillaume::tests
