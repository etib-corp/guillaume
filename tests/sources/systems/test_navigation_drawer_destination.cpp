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

#include "systems/test_navigation_drawer_destination.hpp"

#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Destination = components::NavigationDrawerDestination;
	}	 // namespace

	TEST_F(TestNavigationDrawerDestinationSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestNavigationDrawerDestinationSystem, InstallGivesTheButtonRole)
	{
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_entity)
					  .getRole(),
				  components::AccessibilityRole::Button);
	}

	TEST_F(TestNavigationDrawerDestinationSystem,
		   RegularItemIsARowWithTheTrailingTextAtTheFarEdge)
	{
		_system.update(_entity);

		const auto &layout =
			_registry.getComponent<components::Layout>(_entity);
		const float padding = theme::SpacingScale::get(4);

		EXPECT_EQ(layout.getAxis(), components::Layout::Axis::Horizontal);
		EXPECT_EQ(layout.getMainAxisAlignment(),
				  components::Layout::MainAxisAlignment::SpaceBetween);
		EXPECT_EQ(layout.getChildren(),
				  (std::vector<ecs::Entity::Identifier> { _lead, _right }));
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getHeight(),
			Destination::Height);
		EXPECT_FLOAT_EQ(positionOf(_lead).getX(), padding);
		EXPECT_FLOAT_EQ(positionOf(_right).getX(), 296.0f - padding - 16.0f);
		// The icon and the label follow the lead group they are in.
		EXPECT_FLOAT_EQ(positionOf(_icon).getX(), positionOf(_lead).getX());
		EXPECT_FLOAT_EQ(positionOf(_label).getX(),
						positionOf(_lead).getX() + 24.0f + 12.0f);
	}

	TEST_F(TestNavigationDrawerDestinationSystem,
		   WithoutTrailingTextTheRowStartsAtTheLeadingEdge)
	{
		_registry.getComponent<Destination>(_entity)
			.setRight("")
			.setRightEntity(ecs::Entity::InvalidIdentifier);
		_system.update(_entity);

		const auto &layout =
			_registry.getComponent<components::Layout>(_entity);
		EXPECT_EQ(layout.getMainAxisAlignment(),
				  components::Layout::MainAxisAlignment::Start);
		EXPECT_EQ(layout.getChildren(),
				  (std::vector<ecs::Entity::Identifier> { _lead }));
	}

	TEST_F(TestNavigationDrawerDestinationSystem,
		   CollapsedItemStacksItsIndicatorAboveItsLabel)
	{
		// A collapsed item has no trailing text, and its lead group only
		// holds the icon.
		_registry.getComponent<Destination>(_entity)
			.setCollapsed(true)
			.setRightEntity(ecs::Entity::InvalidIdentifier);
		_registry.getComponent<components::Layout>(_lead)
			.setFixedWidth(Destination::CollapsedIndicatorWidth)
			.setFixedHeight(Destination::CollapsedIndicatorHeight)
			.setChildren({ _icon });
		_system.update(_entity);

		const auto &layout =
			_registry.getComponent<components::Layout>(_entity);
		EXPECT_EQ(layout.getAxis(), components::Layout::Axis::Vertical);
		EXPECT_EQ(layout.getChildren(),
				  (std::vector<ecs::Entity::Identifier> { _lead, _label }));
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getHeight(),
			Destination::Height + theme::SpacingScale::get(2));
		EXPECT_LT(positionOf(_lead).getY(), positionOf(_label).getY());
	}

	TEST_F(TestNavigationDrawerDestinationSystem,
		   SelectedRegularItemIsTheActivePill)
	{
		_registry.getComponent<components::Selectable>(_entity).setSelected(
			true);
		_system.update(_entity);

		const auto content =
			getActiveColor(SchemeColorRole::OnSecondaryContainer);
		EXPECT_EQ(colorOf(_entity),
				  getActiveColor(SchemeColorRole::SecondaryContainer));
		EXPECT_EQ(colorOf(_lead).getAlpha(), 0);
		EXPECT_EQ(colorOf(_icon), content);
		EXPECT_EQ(colorOf(_label), content);
		EXPECT_EQ(colorOf(_right), content);

		const auto &accessibility =
			_registry.getComponent<components::Accessibility>(_entity);
		EXPECT_EQ(accessibility.getLabel(), "Inbox");
		EXPECT_TRUE(accessibility.isSelected());
	}

	TEST_F(TestNavigationDrawerDestinationSystem,
		   SelectedCollapsedItemShowsThePillOnItsIndicator)
	{
		_registry.getComponent<Destination>(_entity).setCollapsed(true);
		_registry.getComponent<components::Selectable>(_entity).setSelected(
			true);
		_system.update(_entity);

		EXPECT_EQ(colorOf(_entity).getAlpha(), 0);
		EXPECT_EQ(colorOf(_lead),
				  getActiveColor(SchemeColorRole::SecondaryContainer));
	}

	TEST_F(TestNavigationDrawerDestinationSystem,
		   UnselectedItemUsesTheOnSurfaceVariantColor)
	{
		_system.update(_entity);

		EXPECT_EQ(colorOf(_entity).getAlpha(), 0);
		EXPECT_EQ(colorOf(_icon),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
		EXPECT_FALSE(_registry.getComponent<components::Accessibility>(_entity)
						 .isSelected());
	}

	TEST_F(TestNavigationDrawerDestinationSystem, PressCallsTheHandler)
	{
		int presses = 0;
		_registry.getComponent<Destination>(_entity).setOnPress([&presses]() {
			++presses;
		});

		click(_registry, _entity);

		EXPECT_EQ(presses, 1);
	}

	TEST_F(TestNavigationDrawerDestinationSystem,
		   DisabledItemIsDimmedAndIgnoresPresses)
	{
		int presses = 0;
		_registry.getComponent<Destination>(_entity).setOnPress([&presses]() {
			++presses;
		});
		_registry.getComponent<components::Selectable>(_entity).setSelected(
			true);

		guillaume::systems::NavigationDrawerDestination::setDisabled(
			_registry, _entity, true);
		click(_registry, _entity);
		guillaume::systems::NavigationDrawerDestination::press(_registry,
															   _entity);

		EXPECT_TRUE(guillaume::systems::NavigationDrawerDestination::isDisabled(
			_registry, _entity));
		EXPECT_FALSE(_registry.getComponent<components::StyleState>(_entity)
						 .isEnabled());
		EXPECT_EQ(presses, 0);
		// A disabled item shows no pill, even when it is selected.
		EXPECT_EQ(colorOf(_entity).getAlpha(), 0);
		EXPECT_EQ(colorOf(_icon), entities::disabledContentColor());
		EXPECT_EQ(colorOf(_label), entities::disabledContentColor());

		guillaume::systems::NavigationDrawerDestination::setDisabled(
			_registry, _entity, false);
		click(_registry, _entity);

		EXPECT_EQ(presses, 1);
		EXPECT_EQ(colorOf(_entity),
				  getActiveColor(SchemeColorRole::SecondaryContainer));
	}

	TEST_F(TestNavigationDrawerDestinationSystem,
		   AnUnchangedDestinationIsNotMarkedChanged)
	{
		_system.update(_entity);
		_registry.resetChangedFlags();
		_system.update(_entity);

		for (const auto identifier: { _entity, _lead, _icon, _label, _right }) {
			EXPECT_FALSE(_registry.hasChanged(identifier)) << identifier;
		}
	}
}	 // namespace guillaume::systems::tests
