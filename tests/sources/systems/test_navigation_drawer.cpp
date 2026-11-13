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

#include "systems/test_navigation_drawer.hpp"

#include <cstddef>

#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/systems/backdrop.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Variant = components::NavigationDrawer::Variant;
	}	 // namespace

	TEST_F(TestNavigationDrawerSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestNavigationDrawerSystem, InstallLabelsTheSectionHeaders)
	{
		const auto &header =
			_registry.getComponent<components::Accessibility>(_section);

		EXPECT_EQ(header.getRole(), components::AccessibilityRole::Header);
		EXPECT_EQ(header.getLabel(), "Mail");
		EXPECT_FALSE(_registry.getComponent<components::SelectionGroup>(_entity)
						 .isEmptyAllowed());
	}

	TEST_F(TestNavigationDrawerSystem,
		   StandardVariantIsASheetAttachedToTheLeadingEdge)
	{
		_system.update(_entity);

		const auto &bound = _registry.getComponent<components::Bound>(_entity);
		const auto &borders =
			_registry.getComponent<components::Borders>(_entity);
		const auto &overlay =
			_registry.getComponent<components::Overlay>(_entity);

		EXPECT_FLOAT_EQ(bound.getWidth(), components::NavigationDrawer::Width);
		EXPECT_FLOAT_EQ(bound.getHeight(),
						components::NavigationDrawer::Height);
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 0.0f);
		EXPECT_FLOAT_EQ(borders.getBottomLeftRadius(), 0.0f);
		EXPECT_FLOAT_EQ(borders.getTopRightRadius(),
						getShapeRadius(theme::ShapeRole::Large));
		EXPECT_FLOAT_EQ(borders.getBottomRightRadius(),
						getShapeRadius(theme::ShapeRole::Large));
		EXPECT_EQ(colorOf(_entity), getActiveColor(SchemeColorRole::Surface));
		EXPECT_FALSE(overlay.isModal());
		EXPECT_FALSE(overlay.dismissesOnOutsideClick());
		EXPECT_TRUE(overlay.dismissesOnEscape());
	}

	TEST_F(TestNavigationDrawerSystem, ModalVariantCapturesOutsideClicks)
	{
		drawer().setVariant(Variant::Modal);
		_system.update(_entity);

		const auto &overlay =
			_registry.getComponent<components::Overlay>(_entity);

		EXPECT_EQ(colorOf(_entity),
				  getActiveColor(SchemeColorRole::SurfaceContainerLow));
		EXPECT_TRUE(overlay.isModal());
		EXPECT_TRUE(overlay.dismissesOnOutsideClick());
	}

	TEST_F(TestNavigationDrawerSystem, CollapsedVariantIsASquareRail)
	{
		drawer().setVariant(Variant::Collapsed);
		_system.update(_entity);

		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getWidth(),
			components::NavigationDrawer::CollapsedWidth);
		EXPECT_FLOAT_EQ(_registry.getComponent<components::Borders>(_entity)
							.getTopRightRadius(),
						getShapeRadius(theme::ShapeRole::None));
		EXPECT_EQ(_registry.getComponent<components::Layout>(_entity)
					  .getCrossAxisAlignment(),
				  components::Layout::CrossAxisAlignment::Center);
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Layout>(_destinations[0])
				.getFixedWidth(),
			components::NavigationDrawer::CollapsedWidth
				- 2.0f * guillaume::systems::NavigationDrawer::Padding);
	}

	TEST_F(TestNavigationDrawerSystem, EntriesArePlacedInAColumn)
	{
		_system.update(_entity);

		const std::vector<ecs::Entity::Identifier> entries {
			_section, _destinations[0], _destinations[1], _divider,
			_destinations[2]
		};

		EXPECT_EQ(
			_registry.getComponent<components::Layout>(_entity).getChildren(),
			entries);
		// Entries stack downward from the top edge (y points up).
		EXPECT_FLOAT_EQ(yOf(_section),
						-guillaume::systems::NavigationDrawer::Padding);
		for (std::size_t index = 1; index < entries.size(); ++index) {
			EXPECT_GT(yOf(entries[index - 1]), yOf(entries[index])) << index;
		}
		for (const auto destination: _destinations) {
			EXPECT_FLOAT_EQ(
				_registry.getComponent<components::Bound>(destination)
					.getWidth(),
				components::NavigationDrawer::Width
					- 2.0f * guillaume::systems::NavigationDrawer::Padding);
			// The content of a destination follows it, below its top edge.
			EXPECT_LT(yOf(destination + 1), yOf(destination));
		}
	}

	TEST_F(TestNavigationDrawerSystem, SelectIsExclusiveAndShowsThePill)
	{
		guillaume::systems::NavigationDrawer::select(_registry, _entity, 0);
		guillaume::systems::NavigationDrawer::select(_registry, _entity, 1);

		EXPECT_EQ(selected(), 1U);
		EXPECT_FALSE(
			_registry.getComponent<components::Selectable>(_destinations[0])
				.isSelected());
		EXPECT_EQ(colorOf(_destinations[1]),
				  getActiveColor(SchemeColorRole::SecondaryContainer));
		EXPECT_EQ(colorOf(_destinations[0]).getAlpha(), 0);
		EXPECT_FALSE(drawer().isUpdatingSelection());

		guillaume::systems::NavigationDrawer::select(_registry, _entity, 9);

		EXPECT_EQ(selected(), 1U);
	}

	TEST_F(TestNavigationDrawerSystem, SelectFirstEnabledSkipsDisabledOnes)
	{
		guillaume::systems::NavigationDrawer::setDestinationDisabled(
			_registry, _entity, 0, true);
		guillaume::systems::NavigationDrawer::selectFirstEnabled(_registry,
																 _entity);

		EXPECT_EQ(selected(), 1U);
	}

	TEST_F(TestNavigationDrawerSystem, PressSelectsAndDisabledIsInert)
	{
		std::size_t reported = 99;
		int changes			 = 0;
		drawer().setOnIndexChange([&reported, &changes](std::size_t index) {
			reported = index;
			++changes;
		});
		guillaume::systems::NavigationDrawer::select(_registry, _entity, 0);

		TestNavigationDrawerDestinationSystem::click(_registry,
													 _destinations[1]);

		EXPECT_EQ(selected(), 1U);
		EXPECT_EQ(reported, 1U);
		EXPECT_EQ(changes, 1);

		// Pressing the selected destination changes nothing.
		TestNavigationDrawerDestinationSystem::click(_registry,
													 _destinations[1]);
		// The third destination is disabled.
		TestNavigationDrawerDestinationSystem::click(_registry,
													 _destinations[2]);
		guillaume::systems::NavigationDrawer::press(_registry, _entity, 2);

		EXPECT_EQ(selected(), 1U);
		EXPECT_EQ(changes, 1);
		EXPECT_FALSE(
			_registry.getComponent<components::StyleState>(_destinations[2])
				.isEnabled());
	}

	TEST_F(TestNavigationDrawerSystem, SelectingADestinationDeselectsTheOthers)
	{
		guillaume::systems::NavigationDrawer::select(_registry, _entity, 0);

		// Something other than the drawer selects a destination.
		_registry.getComponent<components::Selectable>(_destinations[1])
			.setSelected(true);

		EXPECT_EQ(selected(), 1U);
		EXPECT_FALSE(
			_registry.getComponent<components::Selectable>(_destinations[0])
				.isSelected());
		EXPECT_EQ(colorOf(_destinations[0]).getAlpha(), 0);
	}

	TEST_F(TestNavigationDrawerSystem, SetDestinationDisabledSkipsOtherEntries)
	{
		guillaume::systems::NavigationDrawer::setDestinationDisabled(
			_registry, _entity, 1, true);

		// The second destination is the third entry, after the section.
		EXPECT_TRUE(drawer().getEntries()[2].disabled);
		EXPECT_FALSE(drawer().getEntries()[1].disabled);
		EXPECT_TRUE(guillaume::systems::NavigationDrawerDestination::isDisabled(
			_registry, _destinations[1]));
		EXPECT_EQ(colorOf(_destinations[1] + 2),
				  entities::disabledContentColor());

		guillaume::systems::NavigationDrawer::setDestinationDisabled(
			_registry, _entity, 2, false);

		EXPECT_FALSE(drawer().getEntries()[4].disabled);
		EXPECT_TRUE(
			_registry.getComponent<components::StyleState>(_destinations[2])
				.isEnabled());
	}

	TEST_F(TestNavigationDrawerSystem, OnlyAModalDrawerShowsTheBackdrop)
	{
		const auto &backdrop =
			_registry.getComponent<components::Backdrop>(_backdrop);

		guillaume::systems::NavigationDrawer::show(_registry, _entity);

		EXPECT_TRUE(
			_registry.getComponent<components::Overlay>(_entity).isVisible());
		EXPECT_FALSE(backdrop.isVisible());

		drawer().setVariant(Variant::Modal);
		_system.update(_entity);

		EXPECT_TRUE(backdrop.isVisible());

		guillaume::systems::NavigationDrawer::hide(_registry, _entity);

		EXPECT_FALSE(
			_registry.getComponent<components::Overlay>(_entity).isVisible());
		EXPECT_FALSE(backdrop.isVisible());
	}

	TEST_F(TestNavigationDrawerSystem, BackdropIsCenteredOnTheDrawer)
	{
		_system.update(_entity);

		const auto position =
			_registry.getComponent<components::Transform>(_backdrop)
				.getPose()
				.getPosition();

		EXPECT_FLOAT_EQ(position.getX(),
						-(guillaume::systems::Backdrop::Extent
						  - components::NavigationDrawer::Width)
							/ 2.0f);
		EXPECT_FLOAT_EQ(position.getY(),
						-(guillaume::systems::Backdrop::Extent
						  - components::NavigationDrawer::Height)
							/ 2.0f);
	}

	TEST_F(TestNavigationDrawerSystem, AnUnchangedDrawerIsNotMarkedChanged)
	{
		drawer().setVariant(Variant::Modal);
		guillaume::systems::NavigationDrawer::show(_registry, _entity);
		guillaume::systems::NavigationDrawer::select(_registry, _entity, 1);
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);
		for (const auto destination: _destinations) {
			_destinationSystem.update(destination);
		}

		for (const auto identifier:
			 { _entity, _section, _divider, _backdrop }) {
			EXPECT_FALSE(_registry.hasChanged(identifier)) << identifier;
		}
		for (const auto destination: _destinations) {
			for (ecs::Entity::Identifier offset = 0; offset < 5; ++offset) {
				EXPECT_FALSE(_registry.hasChanged(destination + offset))
					<< destination + offset;
			}
		}
	}
}	 // namespace guillaume::systems::tests
