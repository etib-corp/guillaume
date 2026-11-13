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

#include "systems/test_navigation_bar_destination.hpp"

#include <algorithm>
#include <vector>

#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Destination = components::NavigationBarDestination;
	}	 // namespace

	TEST_F(TestNavigationBarDestinationSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestNavigationBarDestinationSystem, InstallGivesTheTabRole)
	{
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_entity)
					  .getRole(),
				  components::AccessibilityRole::Tab);
	}

	TEST_F(TestNavigationBarDestinationSystem, UnselectedDestinationIsQuiet)
	{
		_system.update(_entity);

		EXPECT_EQ(colorOf(_indicator).getAlpha(), 0);
		EXPECT_EQ(colorOf(_icon),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
		EXPECT_EQ(colorOf(_label),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
		EXPECT_EQ(_registry.getComponent<components::Glyph>(_icon).getName(),
				  "mail_outline");

		const auto &accessibility =
			_registry.getComponent<components::Accessibility>(_entity);
		EXPECT_EQ(accessibility.getLabel(), "Mail");
		EXPECT_FALSE(accessibility.isSelected());
	}

	TEST_F(TestNavigationBarDestinationSystem,
		   SelectedDestinationShowsThePillAndTheFocusedIcon)
	{
		_registry.getComponent<components::Selectable>(_entity).setSelected(
			true);
		_system.update(_entity);

		EXPECT_TRUE(guillaume::systems::NavigationBarDestination::isSelected(
			_registry, _entity));
		EXPECT_EQ(colorOf(_indicator),
				  getActiveColor(SchemeColorRole::SecondaryContainer));
		EXPECT_EQ(colorOf(_icon),
				  getActiveColor(SchemeColorRole::OnSecondaryContainer));
		EXPECT_EQ(colorOf(_label), getActiveColor(SchemeColorRole::OnSurface));
		EXPECT_EQ(_registry.getComponent<components::Glyph>(_icon).getName(),
				  "mail");
		EXPECT_TRUE(_registry.getComponent<components::Accessibility>(_entity)
						.isSelected());
	}

	TEST_F(TestNavigationBarDestinationSystem,
		   SelectedDestinationWithoutFocusedIconKeepsItsIcon)
	{
		_registry.getComponent<Destination>(_entity).setFocusedIcon("");
		_registry.getComponent<components::Selectable>(_entity).setSelected(
			true);
		_system.update(_entity);

		EXPECT_EQ(_registry.getComponent<components::Glyph>(_icon).getName(),
				  "mail_outline");
	}

	TEST_F(TestNavigationBarDestinationSystem,
		   HiddenLabelIsTransparentAndKeepsItsPlace)
	{
		_registry.getComponent<Destination>(_entity).setShowLabel(false);
		_system.update(_entity);

		const auto &children =
			_registry.getComponent<components::Layout>(_entity).getChildren();
		EXPECT_EQ(colorOf(_label).getAlpha(), 0);
		EXPECT_NE(std::find(children.begin(), children.end(), _label),
				  children.end());
	}

	TEST_F(TestNavigationBarDestinationSystem,
		   IndicatorAndLabelArePlacedInACenteredColumn)
	{
		_system.update(_entity);

		const float spacing = theme::SpacingScale::get(1);
		const float top =
			(64.0f - (Destination::IndicatorHeight + spacing + 16.0f)) / 2.0f;

		EXPECT_EQ(
			_registry.getComponent<components::Layout>(_entity).getChildren(),
			(std::vector<ecs::Entity::Identifier> { _indicator, _label }));
		EXPECT_FLOAT_EQ(positionOf(_indicator).getX(),
						(100.0f - Destination::IndicatorWidth) / 2.0f);
		EXPECT_FLOAT_EQ(positionOf(_indicator).getY(), top);
		EXPECT_FLOAT_EQ(positionOf(_label).getX(), (100.0f - 30.0f) / 2.0f);
		EXPECT_FLOAT_EQ(positionOf(_label).getY(),
						top + Destination::IndicatorHeight + spacing);
		// The icon follows the indicator it is centered in.
		EXPECT_FLOAT_EQ(positionOf(_icon).getX(),
						positionOf(_indicator).getX()
							+ (Destination::IndicatorWidth - 24.0f) / 2.0f);
	}

	TEST_F(TestNavigationBarDestinationSystem, BadgeSitsOnTheCornerOfTheIcon)
	{
		_system.update(_entity);

		EXPECT_FLOAT_EQ(positionOf(_badge).getX(),
						positionOf(_indicator).getX()
							+ Destination::BadgeOffsetX);
		EXPECT_FLOAT_EQ(positionOf(_badge).getY(),
						positionOf(_indicator).getY()
							+ Destination::BadgeOffsetY);
	}

	TEST_F(TestNavigationBarDestinationSystem, BadgeShowsTheLabelOrADot)
	{
		_system.update(_entity);

		const auto &badge = _registry.getComponent<components::Badge>(_badge);
		EXPECT_EQ(badge.getVariant(), components::Badge::Variant::Large);
		EXPECT_EQ(badge.getLabel(), "3");

		guillaume::systems::NavigationBarDestination::setBadge(_registry,
															   _entity, "");

		EXPECT_EQ(_registry.getComponent<Destination>(_entity).getBadge(), "");
		EXPECT_EQ(badge.getVariant(), components::Badge::Variant::Dot);
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_badge).getWidth(),
			components::Badge::DotSize);
	}

	TEST_F(TestNavigationBarDestinationSystem, PressCallsTheHandler)
	{
		int presses = 0;
		_registry.getComponent<Destination>(_entity).setOnPress([&presses]() {
			++presses;
		});

		click(_registry, _entity);

		EXPECT_EQ(presses, 1);
	}

	TEST_F(TestNavigationBarDestinationSystem,
		   AnUnchangedDestinationIsNotMarkedChanged)
	{
		_system.update(_entity);
		_registry.resetChangedFlags();
		_system.update(_entity);

		for (const auto identifier:
			 { _entity, _indicator, _icon, _label, _badge }) {
			EXPECT_FALSE(_registry.hasChanged(identifier)) << identifier;
		}
	}
}	 // namespace guillaume::systems::tests
