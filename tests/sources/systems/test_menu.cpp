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

#include "systems/test_menu.hpp"

#include <vector>

#include <guillaume/components/mouse_button_interaction.hpp>
#include <guillaume/components/mouse_hover_interaction.hpp>
#include <guillaume/components/style_state.hpp>
#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Mode			 = components::Menu::Mode;
		using AnchorPosition = components::Menu::AnchorPosition;
		using MenuSystem	 = guillaume::systems::Menu;

		/// Horizontal padding of a row and gap between its children, in dp.
		constexpr float ItemPadding = 12.0f;
	}	 // namespace

	TEST_F(TestMenuSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestMenuSystem, InstallSetsSemanticsAndMakesTheRowsMenuItems)
	{
		const auto &accessibility =
			_registry.getComponent<components::Accessibility>(_entity);
		EXPECT_EQ(accessibility.getRole(), components::AccessibilityRole::Menu);
		EXPECT_FALSE(accessibility.isFocusable());

		const auto &row =
			_registry.getComponent<components::Accessibility>(_pasteRow);
		EXPECT_EQ(row.getRole(), components::AccessibilityRole::MenuItem);
		EXPECT_EQ(row.getLabel(), "Paste");
	}

	TEST_F(TestMenuSystem, ModeControlsElevationAndColor)
	{
		const auto &elevation =
			_registry.getComponent<components::Elevation>(_entity);

		_system.update(_entity);
		EXPECT_EQ(elevation.getElevationLevel(), theme::ElevationLevel::Level2);
		EXPECT_TRUE(elevation.hasShadow());
		EXPECT_EQ(colorOf(_entity),
				  getElevatedSurfaceColor(theme::ElevationLevel::Level2));
		EXPECT_FLOAT_EQ(_registry.getComponent<components::Borders>(_entity)
							.getTopLeftRadius(),
						getShapeRadius(theme::ShapeRole::ExtraSmall));

		menu().setMode(Mode::Flat);
		_system.update(_entity);
		EXPECT_FALSE(elevation.hasShadow());
		EXPECT_EQ(colorOf(_entity),
				  getActiveColor(SchemeColorRole::SurfaceContainer));
	}

	TEST_F(TestMenuSystem, OverlayDismissesOnOutsideClickAndEscape)
	{
		_system.update(_entity);

		EXPECT_FALSE(overlay().isModal());
		EXPECT_TRUE(overlay().dismissesOnOutsideClick());
		EXPECT_TRUE(overlay().dismissesOnEscape());
		EXPECT_FALSE(overlay().isVisible());
	}

	TEST_F(TestMenuSystem, RowsAreStackedWithRegularAndDenseHeights)
	{
		_system.update(_entity);

		EXPECT_EQ(
			_registry.getComponent<components::Layout>(_entity).getChildren(),
			(std::vector<ecs::Entity::Identifier> { _copyRow, _pasteRow }));
		EXPECT_FLOAT_EQ(boundOf(_copyRow).getHeight(), 48.0f);
		EXPECT_FLOAT_EQ(boundOf(_pasteRow).getHeight(), 32.0f);
		EXPECT_FLOAT_EQ(boundOf(_entity).getHeight(), 80.0f);
		EXPECT_FLOAT_EQ(positionOf(_copyRow).getY(), 0.0f);
		EXPECT_FLOAT_EQ(positionOf(_pasteRow).getY(), 48.0f);
	}

	TEST_F(TestMenuSystem, RowContentIsLaidOutFromThePadding)
	{
		_system.update(_entity);

		EXPECT_EQ(
			_registry.getComponent<components::Layout>(_copyRow).getChildren(),
			(std::vector<ecs::Entity::Identifier> { _copyIcon, _copyLabel }));
		EXPECT_FLOAT_EQ(positionOf(_copyIcon).getX(), ItemPadding);
		EXPECT_FLOAT_EQ(positionOf(_copyLabel).getX(),
						ItemPadding + 24.0f + ItemPadding);
		// The second row sits below the first: its content follows it.
		EXPECT_GT(positionOf(_pasteLabel).getY(), 48.0f);
	}

	TEST_F(TestMenuSystem, WidthFollowsTheWidestRowWithinTheMaterialRange)
	{
		// 2 * 12 + 24 + 12 + 44 = 104 dp of content: below the minimum.
		_system.update(_entity);
		EXPECT_FLOAT_EQ(boundOf(_entity).getWidth(),
						components::Menu::MinWidth);
		EXPECT_FLOAT_EQ(boundOf(_copyRow).getWidth(),
						components::Menu::MinWidth);

		boundOf(_pasteLabel).setWidth(150.0f);
		_system.update(_entity);
		EXPECT_FLOAT_EQ(boundOf(_entity).getWidth(), 210.0f);
		EXPECT_FLOAT_EQ(boundOf(_copyRow).getWidth(), 210.0f);
		EXPECT_FLOAT_EQ(boundOf(_pasteRow).getWidth(), 210.0f);

		boundOf(_pasteLabel).setWidth(900.0f);
		_system.update(_entity);
		EXPECT_FLOAT_EQ(boundOf(_entity).getWidth(),
						components::Menu::MaxWidth);
	}

	TEST_F(TestMenuSystem, RowsAndContentUseTheMenuColors)
	{
		_system.update(_entity);

		EXPECT_EQ(colorOf(_copyRow), entities::transparentColor());
		EXPECT_EQ(colorOf(_copyLabel),
				  getActiveColor(SchemeColorRole::OnSurface));
		EXPECT_EQ(colorOf(_copyIcon),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
		EXPECT_EQ(colorOf(_pasteIcon),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
	}

	TEST_F(TestMenuSystem, HoveringARowShowsAStateLayer)
	{
		_system.update(_entity);
		auto &hover =
			_registry.getComponent<components::MouseHoverInteraction>(_copyRow);

		hover.setHovered(true);
		hover.getOnHoverHandler()();

		EXPECT_NE(colorOf(_copyRow), entities::transparentColor());
		EXPECT_EQ(colorOf(_pasteRow), entities::transparentColor());

		hover.setHovered(false);
		hover.getOnUnhoverHandler()();
		EXPECT_EQ(colorOf(_copyRow), entities::transparentColor());
	}

	TEST_F(TestMenuSystem, ReleasingOnARowPressesItsItem)
	{
		_system.update(_entity);

		_registry.getComponent<components::MouseButtonInteraction>(_pasteRow)
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_EQ(_copies, 0);
		EXPECT_EQ(_pastes, 1);
		// Pressing an item leaves the visibility alone.
		EXPECT_EQ(_dismissals, 0);
	}

	TEST_F(TestMenuSystem, PressItemIgnoresDisabledAndUnknownItems)
	{
		MenuSystem::pressItem(_registry, _entity, 0);
		EXPECT_EQ(_copies, 1);

		MenuSystem::setItemDisabled(_registry, _entity, 0, true);
		MenuSystem::pressItem(_registry, _entity, 0);
		MenuSystem::pressItem(_registry, _entity, 9);
		EXPECT_EQ(_copies, 1);
		EXPECT_EQ(_pastes, 0);
	}

	TEST_F(TestMenuSystem, DisablingAnItemDisablesAndDimsItsRow)
	{
		_system.update(_entity);

		MenuSystem::setItemDisabled(_registry, _entity, 0, true);

		EXPECT_TRUE(menu().getItems()[0].disabled);
		EXPECT_FALSE(_registry.getComponent<components::StyleState>(_copyRow)
						 .isEnabled());
		EXPECT_EQ(colorOf(_copyLabel), entities::disabledContentColor());
		EXPECT_EQ(colorOf(_copyIcon), entities::disabledContentColor());
		EXPECT_EQ(colorOf(_pasteLabel),
				  getActiveColor(SchemeColorRole::OnSurface));

		// A release on the disabled row is ignored.
		_registry.getComponent<components::MouseButtonInteraction>(_copyRow)
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_EQ(_copies, 0);

		MenuSystem::setItemDisabled(_registry, _entity, 0, false);
		EXPECT_TRUE(_registry.getComponent<components::StyleState>(_copyRow)
						.isEnabled());
		EXPECT_EQ(colorOf(_copyLabel),
				  getActiveColor(SchemeColorRole::OnSurface));

		// Out of range: ignored.
		MenuSystem::setItemDisabled(_registry, _entity, 9, true);
	}

	TEST_F(TestMenuSystem, AnItemDisabledFromTheStartHasADisabledRow)
	{
		auto items		  = menu().getItems();
		items[1].disabled = true;
		menu().setItems(items);

		_system.update(_entity);

		EXPECT_FALSE(_registry.getComponent<components::StyleState>(_pasteRow)
						 .isEnabled());
		EXPECT_EQ(colorOf(_pasteLabel), entities::disabledContentColor());
	}

	TEST_F(TestMenuSystem, AnAnchoredMenuOpensBelowOrAboveItsAnchor)
	{
		menu().setAnchor(_anchor);

		_system.update(_entity);
		// Anchor at y = 100, 40 dp high, plus the 4 dp gap.
		EXPECT_FLOAT_EQ(positionOf(_entity).getX(), 10.0f);
		EXPECT_FLOAT_EQ(positionOf(_entity).getY(), 144.0f);
		EXPECT_FLOAT_EQ(positionOf(_copyRow).getY(), 144.0f);
		EXPECT_FLOAT_EQ(positionOf(_copyIcon).getX(), 10.0f + ItemPadding);

		menu().setAnchorPosition(AnchorPosition::Top);
		_system.update(_entity);
		// The 80 dp high menu ends 4 dp above the anchor.
		EXPECT_FLOAT_EQ(positionOf(_entity).getY(), 16.0f);
		EXPECT_FLOAT_EQ(positionOf(_pasteRow).getY(), 64.0f);
	}

	TEST_F(TestMenuSystem, AMenuWithoutAnchorKeepsItsPose)
	{
		_registry.getComponent<components::Transform>(_entity).setPose(
			utility::graphic::PoseF(
				utility::graphic::PositionF(5.0f, 6.0f, 0.0f),
				utility::graphic::OrientationF()));

		_system.update(_entity);

		EXPECT_FLOAT_EQ(positionOf(_entity).getX(), 5.0f);
		EXPECT_FLOAT_EQ(positionOf(_entity).getY(), 6.0f);
	}

	TEST_F(TestMenuSystem, ShowAnchorsThenShowsAndHideHides)
	{
		menu().setAnchor(_anchor);

		MenuSystem::show(_registry, _entity);
		EXPECT_TRUE(overlay().isVisible());
		EXPECT_FLOAT_EQ(positionOf(_entity).getY(), 144.0f);

		MenuSystem::hide(_registry, _entity);
		EXPECT_FALSE(overlay().isVisible());
		EXPECT_EQ(_dismissals, 0);
	}

	TEST_F(TestMenuSystem, DismissHidesThenCallsTheHandlerOnce)
	{
		MenuSystem::show(_registry, _entity);

		MenuSystem::dismiss(_registry, _entity);

		EXPECT_EQ(_dismissals, 1);
		EXPECT_FALSE(overlay().isVisible());

		// Already hidden: nothing more to report.
		MenuSystem::dismiss(_registry, _entity);
		EXPECT_EQ(_dismissals, 1);
	}

	TEST_F(TestMenuSystem, RowsWithoutItemsAreStillArranged)
	{
		// The rows are rebuilt by whoever owns the entities; until then the
		// system must cope with rows that outnumber the items.
		menu().setItems({});

		_system.update(_entity);

		EXPECT_FLOAT_EQ(boundOf(_copyRow).getHeight(), 48.0f);
		MenuSystem::pressItem(_registry, _entity, 0);
		EXPECT_EQ(_copies, 0);
	}

	TEST_F(TestMenuSystem, AnUnchangedMenuIsNotMarkedChanged)
	{
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);
		_system.update(_entity);

		EXPECT_FALSE(anythingChanged());
	}

	TEST_F(TestMenuSystem, AnUnchangedAnchoredMenuIsNotMarkedChanged)
	{
		menu().setAnchor(_anchor).setAnchorPosition(AnchorPosition::Top);
		MenuSystem::show(_registry, _entity);
		_registry.resetChangedFlags();

		MenuSystem::apply(_registry, _entity);
		MenuSystem::show(_registry, _entity);

		EXPECT_FALSE(anythingChanged());
	}

}	 // namespace guillaume::systems::tests
