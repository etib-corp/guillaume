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
#include "systems/test_button.hpp"

#include <chrono>

#include <guillaume/components/common_button.hpp>
#include <guillaume/components/extended_floating_action_button.hpp>
#include <guillaume/components/floating_action_button.hpp>
#include <guillaume/components/icon_button.hpp>
#include <guillaume/entities/fab_style.hpp>
#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/systems/common_button.hpp>
#include <guillaume/systems/extended_floating_action_button.hpp>
#include <guillaume/systems/floating_action_button.hpp>
#include <guillaume/systems/icon_button.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	using SharedButton = guillaume::systems::Button;

	TEST_F(TestButtonSystem, RunsInTheEventPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Event);
	}

	TEST_F(TestButtonSystem, ReleaseCallsTheClickHandler)
	{
		int clicks = 0;
		button().setOnClick([&clicks]() {
			++clicks;
		});

		SharedButton::press(_registry, _entity);
		SharedButton::release(_registry, _entity);

		EXPECT_EQ(clicks, 1);
	}

	TEST_F(TestButtonSystem, WiredHandlersReachTheSystem)
	{
		int clicks = 0;
		button().setOnClick([&clicks]() {
			++clicks;
		});
		SharedButton::install(_registry, _entity);

		_registry.getComponent<components::MouseButtonInteraction>(_entity)
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_EQ(clicks, 1);
	}

	TEST_F(TestButtonSystem, ALongPressReplacesTheClick)
	{
		int clicks		= 0;
		int longPresses = 0;
		button()
			.setOnClick([&clicks]() {
				++clicks;
			})
			.setOnLongPress([&longPresses]() {
				++longPresses;
			});

		button().setPressStart(std::chrono::steady_clock::now()
							   - 2 * SharedButton::LongPressDuration);
		SharedButton::release(_registry, _entity);

		EXPECT_EQ(clicks, 0);
		EXPECT_EQ(longPresses, 1);
	}

	TEST_F(TestButtonSystem, DisabledOrHiddenButtonsIgnoreRelease)
	{
		int clicks = 0;
		button().setOnClick([&clicks]() {
			++clicks;
		});

		_registry.getComponent<components::StyleState>(_entity).setEnabled(
			false);
		SharedButton::release(_registry, _entity);
		_registry.getComponent<components::StyleState>(_entity).setEnabled(
			true);
		button().setVisible(false);
		SharedButton::release(_registry, _entity);

		EXPECT_EQ(clicks, 0);
	}

	TEST_F(TestButtonSystem, ToggleButtonsFlipSelectionAndGlyph)
	{
		button().setToggle(true).setSelectedIconGlyphName("check");

		SharedButton::release(_registry, _entity);

		EXPECT_TRUE(button().isSelected());
		EXPECT_EQ(_registry.getComponent<components::Glyph>(_icon).getName(),
				  "check");
		EXPECT_TRUE(_registry.getComponent<components::Accessibility>(_entity)
						.isSelected());
	}

	TEST_F(TestButtonSystem, SelectionIsIgnoredWithoutToggle)
	{
		SharedButton::setSelected(_registry, _entity, true);

		EXPECT_FALSE(button().isSelected());
	}

	TEST_F(TestButtonSystem, LoadingShowsTheProgressGlyph)
	{
		button().setLoading(true);

		_system.update(_entity);

		EXPECT_EQ(_registry.getComponent<components::Glyph>(_icon).getName(),
				  SharedButton::LoadingGlyph);
	}

	TEST_F(TestButtonSystem, RestyleRunsTheVariantFunction)
	{
		static int calls = 0;
		calls			 = 0;
		button().setRestyle(
			[](ecs::ComponentRegistry &, const ecs::Entity::Identifier &) {
				++calls;
			});

		SharedButton::press(_registry, _entity);

		EXPECT_EQ(calls, 1);
	}

	TEST_F(TestButtonSystem, CommonButtonSizesAndPlacesIconThenLabel)
	{
		_registry.addComponent<components::CommonButton>(_entity);
		guillaume::systems::CommonButton::install(_registry, _entity);

		guillaume::systems::CommonButton::apply(_registry, _entity);

		const auto &bound = _registry.getComponent<components::Bound>(_entity);
		EXPECT_GE(bound.getWidth(), components::CommonButton::MinWidth);
		EXPECT_GE(bound.getHeight(), components::CommonButton::MinHeight);
		EXPECT_LT(offsetX(_icon), offsetX(_label));
		EXPECT_EQ(_registry.getComponent<components::Text>(_label).getContent(),
				  "Save");
	}

	TEST_F(TestButtonSystem, CommonButtonTrailingIconAndUppercase)
	{
		_registry.addComponent<components::CommonButton>(_entity).setUppercase(
			true);
		button().setTrailingIcon(true);
		guillaume::systems::CommonButton::install(_registry, _entity);

		SharedButton::restyle(_registry, _entity);

		EXPECT_GT(offsetX(_icon), offsetX(_label));
		EXPECT_EQ(_registry.getComponent<components::Text>(_label).getContent(),
				  "SAVE");
	}

	TEST_F(TestButtonSystem, CommonButtonDisabledUsesDisabledColors)
	{
		_registry.addComponent<components::CommonButton>(_entity).setColorStyle(
			components::CommonButton::Color::Filled);
		_registry.getComponent<components::StyleState>(_entity).setEnabled(
			false);
		guillaume::systems::CommonButton::install(_registry, _entity);

		guillaume::systems::CommonButton::apply(_registry, _entity);

		EXPECT_EQ(_registry.getComponent<components::Color>(_entity).getColor(),
				  entities::disabledContainerColor());
		EXPECT_EQ(_registry.getComponent<components::Color>(_label).getColor(),
				  entities::disabledContentColor());
	}

	TEST_F(TestButtonSystem, AnUnchangedCommonButtonIsNotMarkedChanged)
	{
		_registry.addComponent<components::CommonButton>(_entity);
		guillaume::systems::CommonButton::install(_registry, _entity);
		guillaume::systems::CommonButton::apply(_registry, _entity);
		_registry.resetChangedFlags();

		guillaume::systems::CommonButton::apply(_registry, _entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
		EXPECT_FALSE(_registry.hasChanged(_icon));
		EXPECT_FALSE(_registry.hasChanged(_label));
	}

	TEST_F(TestButtonSystem, IconButtonIsSquareWithACenteredIcon)
	{
		_registry.addComponent<components::IconButton>(_entity);
		guillaume::systems::IconButton::install(_registry, _entity);

		guillaume::systems::IconButton::apply(_registry, _entity);

		const auto &bound = _registry.getComponent<components::Bound>(_entity);
		EXPECT_FLOAT_EQ(bound.getWidth(), 40.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 40.0f);
		EXPECT_FLOAT_EQ(offsetX(_icon), (40.0f - 18.0f) / 2.0f);
	}

	TEST_F(TestButtonSystem, FloatingActionButtonShadowFollowsTheMode)
	{
		using Config = components::FloatingActionButton;
		auto &config = _registry.addComponent<Config>(_entity);
		guillaume::systems::FloatingActionButton::install(_registry, _entity);

		guillaume::systems::FloatingActionButton::apply(_registry, _entity);
		const auto &elevation =
			_registry.getComponent<components::Elevation>(_entity);
		EXPECT_EQ(elevation.getElevationLevel(), theme::ElevationLevel::Level3);
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getHeight(),
			entities::fab_style::getContainerSize(config.getSize()));

		config.setMode(Config::Mode::Flat);
		SharedButton::restyle(_registry, _entity);
		EXPECT_EQ(elevation.getElevationLevel(), theme::ElevationLevel::Level0);
	}

	TEST_F(TestButtonSystem, HiddenFloatingActionButtonIsTransparent)
	{
		_registry.addComponent<components::FloatingActionButton>(_entity);
		button().setVisible(false);
		guillaume::systems::FloatingActionButton::install(_registry, _entity);

		guillaume::systems::FloatingActionButton::apply(_registry, _entity);

		EXPECT_EQ(_registry.getComponent<components::Color>(_entity).getColor(),
				  entities::transparentColor());
		EXPECT_EQ(_registry.getComponent<components::Color>(_icon).getColor(),
				  entities::transparentColor());
	}

	TEST_F(TestButtonSystem, CollapsingAnExtendedFabShrinksIt)
	{
		using Config = components::ExtendedFloatingActionButton;
		auto &config = _registry.addComponent<Config>(_entity);
		guillaume::systems::ExtendedFloatingActionButton::install(_registry,
																  _entity);

		guillaume::systems::ExtendedFloatingActionButton::apply(_registry,
																_entity);
		const float extended =
			_registry.getComponent<components::Bound>(_entity).getWidth();

		config.setExtended(false);
		SharedButton::restyle(_registry, _entity);

		EXPECT_LT(_registry.getComponent<components::Bound>(_entity).getWidth(),
				  extended);
	}

}	 // namespace guillaume::systems::tests
