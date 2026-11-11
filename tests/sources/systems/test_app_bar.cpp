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

#include "systems/test_app_bar.hpp"

#include <optional>
#include <utility>

#include <guillaume/components/accessibility.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Variant = components::AppBar::Variant;
		using Ids	  = std::vector<ecs::Entity::Identifier>;
		using Main	  = components::Layout::MainAxisAlignment;
	}	 // namespace

	TEST_F(TestAppBarSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestAppBarSystem, HeightsAndTitleRolesFollowTheVariant)
	{
		const std::pair<Variant, std::pair<float, theme::TypeRole>>
			expected[] = {
				{ Variant::Small, { 64.0f, theme::TypeRole::TitleLarge } },
				{ Variant::CenterAligned,
				  { 64.0f, theme::TypeRole::TitleLarge } },
				{ Variant::Medium, { 112.0f, theme::TypeRole::HeadlineSmall } },
				{ Variant::Large, { 152.0f, theme::TypeRole::HeadlineMedium } },
				{ Variant::Bottom, { 80.0f, theme::TypeRole::TitleLarge } },
			};

		for (const auto &[variant, values]: expected) {
			appBar().setVariant(variant);
			_system.update(_entity);

			EXPECT_FLOAT_EQ(
				_registry.getComponent<components::Bound>(_entity).getHeight(),
				values.first);
			EXPECT_FLOAT_EQ(guillaume::systems::AppBar::height(variant),
							values.first);
			EXPECT_EQ(_registry.getComponent<components::Typography>(_title)
						  .getRole(),
					  values.second);
			EXPECT_FLOAT_EQ(
				_registry.getComponent<components::Text>(_title).getFontSize(),
				getTypeToken(values.second).fontSize);
		}
	}

	TEST_F(TestAppBarSystem, ChildrenAreLaidOutNavigationTitleActions)
	{
		_system.update(_entity);

		EXPECT_EQ(layout().getChildren(),
				  (Ids { _navigation, _title, _search, _more }));
		EXPECT_EQ(layout().getAxis(), components::Layout::Axis::Horizontal);
		EXPECT_EQ(layout().getMainAxisAlignment(), Main::Start);
		EXPECT_FLOAT_EQ(layout().getPadding(),
						components::AppBar::HorizontalPadding);
		EXPECT_FLOAT_EQ(layout().getSpacing(),
						components::AppBar::HorizontalPadding);

		// 4 dp padding, then each child followed by a 4 dp gap.
		EXPECT_FLOAT_EQ(xOf(_navigation), 4.0f);
		EXPECT_FLOAT_EQ(xOf(_title), 4.0f + 48.0f + 4.0f);
		EXPECT_FLOAT_EQ(xOf(_search), 4.0f + 48.0f + 4.0f + 60.0f + 4.0f);
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getWidth(),
			360.0f);
	}

	TEST_F(TestAppBarSystem, MissingChildrenAreLeftOutOfTheLayout)
	{
		appBar()
			.setNavigationButton(ecs::Entity::InvalidIdentifier)
			.setActionButtons({ ecs::Entity::InvalidIdentifier, _more });
		_system.update(_entity);

		EXPECT_EQ(layout().getChildren(), (Ids { _title, _more }));
	}

	TEST_F(TestAppBarSystem, CenterAlignedSpreadsOrCentersItsChildren)
	{
		appBar().setVariant(Variant::CenterAligned);
		_system.update(_entity);
		EXPECT_EQ(layout().getMainAxisAlignment(), Main::SpaceBetween);

		appBar().setNavigation({}).setActions({});
		_system.update(_entity);
		EXPECT_EQ(layout().getMainAxisAlignment(), Main::Center);
	}

	TEST_F(TestAppBarSystem, ColorAndShadowFollowElevatedAndBottom)
	{
		const auto &color = _registry.getComponent<components::Color>(_entity);
		const auto &elevation =
			_registry.getComponent<components::Elevation>(_entity);

		_system.update(_entity);
		EXPECT_EQ(color.getColor(), getActiveColor(SchemeColorRole::Surface));
		EXPECT_EQ(elevation.getElevationLevel(), theme::ElevationLevel::Level0);

		appBar().setElevated(true);
		_system.update(_entity);
		EXPECT_EQ(color.getColor(),
				  getElevatedSurfaceColor(theme::ElevationLevel::Level2));
		EXPECT_EQ(elevation.getElevationLevel(), theme::ElevationLevel::Level2);

		// The bottom bar is tinted but casts no shadow of its own.
		appBar().setElevated(false).setVariant(Variant::Bottom);
		_system.update(_entity);
		EXPECT_EQ(color.getColor(),
				  getElevatedSurfaceColor(theme::ElevationLevel::Level2));
		EXPECT_EQ(elevation.getElevationLevel(), theme::ElevationLevel::Level0);
	}

	TEST_F(TestAppBarSystem, TitleChildFollowsTheTitleAndIsAHeader)
	{
		_system.update(_entity);

		const auto &text = _registry.getComponent<components::Text>(_title);
		const auto &accessibility =
			_registry.getComponent<components::Accessibility>(_title);
		EXPECT_EQ(text.getContent(), "Inbox");
		EXPECT_EQ(accessibility.getRole(),
				  components::AccessibilityRole::Header);
		EXPECT_EQ(accessibility.getLabel(), "Inbox");
		EXPECT_FALSE(accessibility.isFocusable());

		appBar().setTitle("Sent");
		_system.update(_entity);
		EXPECT_EQ(text.getContent(), "Sent");
		EXPECT_EQ(accessibility.getLabel(), "Sent");
	}

	TEST_F(TestAppBarSystem, OnlyTheNavigationButtonUsesOnSurface)
	{
		_system.update(_entity);

		EXPECT_EQ(_registry.getComponent<components::Button>(_navigation)
					  .getContentColorOverride(),
				  std::optional(getActiveColor(SchemeColorRole::OnSurface)));
		EXPECT_FALSE(_registry.getComponent<components::Button>(_search)
						 .getContentColorOverride()
						 .has_value());
	}

	TEST_F(TestAppBarSystem, ConfigurationDecidesWhichChildrenAreMissing)
	{
		EXPECT_FALSE(guillaume::systems::AppBar::needsNavigationButton(
			_registry, _entity));
		EXPECT_FALSE(
			guillaume::systems::AppBar::needsTitle(_registry, _entity));
		EXPECT_FALSE(
			guillaume::systems::AppBar::needsActionButtons(_registry, _entity));

		appBar()
			.setNavigationButton(ecs::Entity::InvalidIdentifier)
			.setTitleEntity(ecs::Entity::InvalidIdentifier)
			.setActionButtons({});
		EXPECT_TRUE(guillaume::systems::AppBar::needsNavigationButton(_registry,
																	  _entity));
		EXPECT_TRUE(guillaume::systems::AppBar::needsTitle(_registry, _entity));
		EXPECT_TRUE(
			guillaume::systems::AppBar::needsActionButtons(_registry, _entity));

		// Nothing to build without an icon, a title or actions.
		appBar().setNavigation({}).setTitle("").setActions({});
		EXPECT_FALSE(guillaume::systems::AppBar::needsNavigationButton(
			_registry, _entity));
		EXPECT_FALSE(
			guillaume::systems::AppBar::needsTitle(_registry, _entity));
		EXPECT_FALSE(
			guillaume::systems::AppBar::needsActionButtons(_registry, _entity));
	}

	TEST_F(TestAppBarSystem, AnUnchangedAppBarIsNotMarkedChanged)
	{
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
		for (const auto child: { _navigation, _title, _search, _more }) {
			EXPECT_FALSE(_registry.hasChanged(child));
		}
	}

}	 // namespace guillaume::systems::tests
