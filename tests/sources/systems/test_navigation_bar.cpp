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

#include "systems/test_navigation_bar.hpp"

#include <cstddef>

#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Variant = components::NavigationBar::Variant;
	}	 // namespace

	TEST_F(TestNavigationBarSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestNavigationBarSystem, InstallMakesTheBarAnExclusiveTabList)
	{
		const auto &accessibility =
			_registry.getComponent<components::Accessibility>(_entity);

		EXPECT_EQ(accessibility.getRole(),
				  components::AccessibilityRole::TabList);
		EXPECT_FALSE(_registry.getComponent<components::SelectionGroup>(_entity)
						 .isEmptyAllowed());
	}

	TEST_F(TestNavigationBarSystem, BarUsesTheSurfaceContainerColor)
	{
		_system.update(_entity);

		EXPECT_EQ(colorOf(_entity),
				  getActiveColor(SchemeColorRole::SurfaceContainer));
	}

	TEST_F(TestNavigationBarSystem, DestinationsShareTheWidthInARow)
	{
		_system.update(_entity);

		const float width = (360.0f - 2.0f * 8.0f) / 3.0f;

		EXPECT_EQ(
			_registry.getComponent<components::Layout>(_entity).getChildren(),
			_destinations);
		for (std::size_t index = 0; index < _destinations.size(); ++index) {
			const auto &bound =
				_registry.getComponent<components::Bound>(_destinations[index]);
			EXPECT_FLOAT_EQ(bound.getWidth(), width);
			EXPECT_FLOAT_EQ(bound.getHeight(),
							components::NavigationBar::Height - 2.0f * 8.0f);
			EXPECT_NEAR(xOf(_destinations[index]),
						8.0f + static_cast<float>(index) * width, 0.001f);
			// The content of a destination follows it.
			EXPECT_NEAR(
				xOf(indicator(index)),
				xOf(_destinations[index])
					+ (width
					   - components::NavigationBarDestination::IndicatorWidth)
						/ 2.0f,
				0.001f);
		}
	}

	TEST_F(TestNavigationBarSystem, SelectIsExclusive)
	{
		guillaume::systems::NavigationBar::select(_registry, _entity, 0);
		guillaume::systems::NavigationBar::select(_registry, _entity, 1);

		EXPECT_FALSE(isSelected(0));
		EXPECT_TRUE(isSelected(1));
		EXPECT_FALSE(isSelected(2));
		EXPECT_EQ(guillaume::systems::NavigationBar::selectedIndex(_registry,
																   _entity),
				  1U);
		EXPECT_EQ(_registry.getComponent<components::SelectionGroup>(_entity)
					  .getSelected(),
				  _destinations[1]);
		EXPECT_FALSE(_registry.getComponent<components::NavigationBar>(_entity)
						 .isUpdatingSelection());
	}

	TEST_F(TestNavigationBarSystem, SelectOutOfRangeIsIgnored)
	{
		guillaume::systems::NavigationBar::select(_registry, _entity, 1);
		guillaume::systems::NavigationBar::select(_registry, _entity, 7);

		EXPECT_EQ(guillaume::systems::NavigationBar::selectedIndex(_registry,
																   _entity),
				  1U);
	}

	TEST_F(TestNavigationBarSystem, SelectRestylesTheDestinations)
	{
		guillaume::systems::NavigationBar::select(_registry, _entity, 1);

		EXPECT_EQ(colorOf(indicator(1)),
				  getActiveColor(SchemeColorRole::SecondaryContainer));
		EXPECT_EQ(colorOf(indicator(0)).getAlpha(), 0);
		EXPECT_EQ(colorOf(label(1)),
				  getActiveColor(SchemeColorRole::OnSurface));
		EXPECT_EQ(colorOf(label(0)),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
		EXPECT_EQ(_registry.getComponent<components::Glyph>(icon(1)).getName(),
				  "mail");
		EXPECT_EQ(_registry.getComponent<components::Glyph>(icon(0)).getName(),
				  "home_outline");
	}

	TEST_F(TestNavigationBarSystem, WithoutLabelsOnlyTheSelectedLabelIsShown)
	{
		_registry.getComponent<components::NavigationBar>(_entity).setVariant(
			Variant::WithoutLabels);
		guillaume::systems::NavigationBar::select(_registry, _entity, 2);

		EXPECT_EQ(colorOf(label(0)).getAlpha(), 0);
		EXPECT_EQ(colorOf(label(1)).getAlpha(), 0);
		EXPECT_EQ(colorOf(label(2)),
				  getActiveColor(SchemeColorRole::OnSurface));

		_registry.getComponent<components::NavigationBar>(_entity).setVariant(
			Variant::WithLabels);
		_system.update(_entity);

		EXPECT_EQ(colorOf(label(0)),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
	}

	TEST_F(TestNavigationBarSystem, PressSelectsAndReportsTheIndex)
	{
		std::size_t reported = 99;
		int changes			 = 0;
		int presses			 = 0;
		_registry.getComponent<components::NavigationBar>(_entity)
			.setOnIndexChange([&reported, &changes](std::size_t index) {
				reported = index;
				++changes;
			})
			.setOnTabPress([&presses](std::size_t) {
				++presses;
			});
		guillaume::systems::NavigationBar::select(_registry, _entity, 0);

		TestNavigationBarDestinationSystem::click(_registry, _destinations[2]);

		EXPECT_TRUE(isSelected(2));
		EXPECT_EQ(reported, 2U);
		EXPECT_EQ(changes, 1);
		EXPECT_EQ(presses, 1);

		// Pressing the selected destination is reported as a press only.
		TestNavigationBarDestinationSystem::click(_registry, _destinations[2]);

		EXPECT_EQ(changes, 1);
		EXPECT_EQ(presses, 2);
	}

	TEST_F(TestNavigationBarSystem, SelectingADestinationDeselectsTheOthers)
	{
		guillaume::systems::NavigationBar::select(_registry, _entity, 0);

		// Something other than the bar selects a destination.
		_registry.getComponent<components::Selectable>(_destinations[1])
			.setSelected(true);

		EXPECT_FALSE(isSelected(0));
		EXPECT_TRUE(isSelected(1));
		EXPECT_EQ(colorOf(indicator(1)),
				  getActiveColor(SchemeColorRole::SecondaryContainer));
		EXPECT_EQ(colorOf(indicator(0)).getAlpha(), 0);
	}

	TEST_F(TestNavigationBarSystem, SetBadgeDrivesTheBadgeOfTheDestination)
	{
		_system.update(_entity);

		const auto &badge = _registry.getComponent<components::Badge>(_badge);
		EXPECT_EQ(badge.getLabel(), "3");

		guillaume::systems::NavigationBar::setBadge(_registry, _entity, 1, "");

		EXPECT_EQ(_registry.getComponent<components::NavigationBar>(_entity)
					  .getDestinations()[1]
					  .badge,
				  "");
		EXPECT_EQ(badge.getVariant(), components::Badge::Variant::Dot);

		guillaume::systems::NavigationBar::setBadge(_registry, _entity, 1,
													"12");

		EXPECT_EQ(badge.getVariant(), components::Badge::Variant::Large);
		EXPECT_EQ(badge.getLabel(), "12");
	}

	TEST_F(TestNavigationBarSystem, AnUnchangedBarIsNotMarkedChanged)
	{
		guillaume::systems::NavigationBar::select(_registry, _entity, 1);
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);
		for (const auto destination: _destinations) {
			_destinationSystem.update(destination);
		}

		EXPECT_FALSE(_registry.hasChanged(_entity));
		EXPECT_FALSE(_registry.hasChanged(_badge));
		for (std::size_t index = 0; index < _destinations.size(); ++index) {
			EXPECT_FALSE(_registry.hasChanged(_destinations[index])) << index;
			EXPECT_FALSE(_registry.hasChanged(indicator(index))) << index;
			EXPECT_FALSE(_registry.hasChanged(icon(index))) << index;
			EXPECT_FALSE(_registry.hasChanged(label(index))) << index;
		}
	}
}	 // namespace guillaume::systems::tests
