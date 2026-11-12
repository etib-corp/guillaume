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

#include "systems/test_tooltip.hpp"

#include <stdexcept>
#include <vector>

#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Variant		= components::Tooltip::Variant;
		using TooltipSystem = guillaume::systems::Tooltip;
	}	 // namespace

	TEST_F(TestTooltipSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestTooltipSystem, InstallSetsSemanticsAndTheTicker)
	{
		const auto &accessibility =
			_registry.getComponent<components::Accessibility>(_entity);
		const auto &ticker =
			_registry.getComponent<components::Animation>(_entity);

		EXPECT_EQ(accessibility.getRole(), components::AccessibilityRole::Text);
		EXPECT_EQ(accessibility.getLiveRegion(),
				  components::LiveRegion::Polite);
		EXPECT_TRUE(ticker.isLooping());
		EXPECT_FLOAT_EQ(ticker.getDuration(), 1.0f);
		EXPECT_FALSE(ticker.isPlaying());
	}

	TEST_F(TestTooltipSystem, HiddenTooltipPaintsNothing)
	{
		_system.update(_entity);

		const auto &overlay =
			_registry.getComponent<components::Overlay>(_entity);
		EXPECT_FALSE(overlay.isVisible());
		EXPECT_FALSE(overlay.isModal());
		EXPECT_FALSE(overlay.dismissesOnOutsideClick());
		EXPECT_TRUE(overlay.dismissesOnEscape());
		EXPECT_EQ(_registry.getComponent<components::Color>(_entity).getColor(),
				  entities::transparentColor());
		EXPECT_EQ(_registry.getComponent<components::Color>(_text).getColor(),
				  entities::transparentColor());
	}

	TEST_F(TestTooltipSystem, ShownPlainTooltipUsesTheInverseSurface)
	{
		TooltipSystem::show(_registry, _entity);

		EXPECT_TRUE(
			_registry.getComponent<components::Overlay>(_entity).isVisible());
		EXPECT_EQ(_registry.getComponent<components::Color>(_entity).getColor(),
				  getActiveColor(SchemeColorRole::InverseSurface));
		EXPECT_EQ(_registry.getComponent<components::Color>(_text).getColor(),
				  getActiveColor(SchemeColorRole::InverseOnSurface));
		EXPECT_FLOAT_EQ(_registry.getComponent<components::Borders>(_entity)
							.getTopLeftRadius(),
						getShapeRadius(theme::ShapeRole::ExtraSmall));
		EXPECT_FALSE(
			_registry.getComponent<components::Elevation>(_entity).hasShadow());
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getHeight(),
			32.0f);
	}

	TEST_F(TestTooltipSystem, ShownRichTooltipIsATallerElevatedContainer)
	{
		tooltip().setVariant(Variant::Rich);

		TooltipSystem::show(_registry, _entity);

		EXPECT_EQ(_registry.getComponent<components::Color>(_entity).getColor(),
				  getActiveColor(SchemeColorRole::SurfaceContainer));
		EXPECT_EQ(_registry.getComponent<components::Color>(_title).getColor(),
				  getActiveColor(SchemeColorRole::OnSurface));
		EXPECT_EQ(_registry.getComponent<components::Color>(_text).getColor(),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
		EXPECT_FLOAT_EQ(_registry.getComponent<components::Borders>(_entity)
							.getTopLeftRadius(),
						getShapeRadius(theme::ShapeRole::Medium));
		EXPECT_TRUE(
			_registry.getComponent<components::Elevation>(_entity).hasShadow());
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getHeight(),
			64.0f);
	}

	TEST_F(TestTooltipSystem, TitleIsOnlyLaidOutInARichTooltip)
	{
		const auto &layout =
			_registry.getComponent<components::Layout>(_entity);

		_system.update(_entity);
		EXPECT_EQ(layout.getChildren(),
				  std::vector<ecs::Entity::Identifier> { _text });

		tooltip().setVariant(Variant::Rich);
		_system.update(_entity);
		EXPECT_EQ(layout.getChildren(),
				  (std::vector<ecs::Entity::Identifier> { _title, _text }));
		// Stacked vertically: the body sits under the title.
		EXPECT_GT(positionOf(_text).getY(), positionOf(_title).getY());
	}

	TEST_F(TestTooltipSystem, TextIsMirroredOnTheLabelAndTheBody)
	{
		tooltip().setText("Updated");

		_system.update(_entity);

		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_entity)
					  .getLabel(),
				  "Updated");
		EXPECT_EQ(_registry.getComponent<components::Text>(_text).getContent(),
				  "Updated");
	}

	TEST_F(TestTooltipSystem, SetAnchorStartsAndStopsTheTicker)
	{
		const auto &ticker =
			_registry.getComponent<components::Animation>(_entity);

		TooltipSystem::setAnchor(_registry, _entity, _anchor);
		EXPECT_TRUE(ticker.isPlaying());
		EXPECT_TRUE(TooltipSystem::hasAnchor(_registry, _entity));

		TooltipSystem::setAnchor(_registry, _entity,
								 ecs::Entity::InvalidIdentifier);
		EXPECT_FALSE(ticker.isPlaying());
		EXPECT_FALSE(TooltipSystem::hasAnchor(_registry, _entity));
	}

	TEST_F(TestTooltipSystem, HoverHonoursTheEnterAndLeaveDelays)
	{
		TooltipSystem::setAnchor(_registry, _entity, _anchor);
		hoverAnchor(true);

		TooltipSystem::advance(_registry, _entity,
							   components::Tooltip::DefaultEnterDelay * 0.5f);
		EXPECT_FALSE(tooltip().isVisible());
		TooltipSystem::advance(_registry, _entity,
							   components::Tooltip::DefaultEnterDelay);
		EXPECT_TRUE(tooltip().isVisible());
		EXPECT_TRUE(
			_registry.getComponent<components::Overlay>(_entity).isVisible());

		hoverAnchor(false);
		TooltipSystem::advance(_registry, _entity,
							   components::Tooltip::DefaultLeaveDelay * 0.5f);
		EXPECT_TRUE(tooltip().isVisible());
		TooltipSystem::advance(_registry, _entity,
							   components::Tooltip::DefaultLeaveDelay);
		EXPECT_FALSE(tooltip().isVisible());
		EXPECT_FLOAT_EQ(tooltip().getHoverTime(), 0.0f);
		EXPECT_FLOAT_EQ(tooltip().getLeaveTime(), 0.0f);
	}

	TEST_F(TestTooltipSystem, CustomDelaysAreUsed)
	{
		tooltip().setEnterDelay(2.0f);
		TooltipSystem::setAnchor(_registry, _entity, _anchor);
		hoverAnchor(true);

		TooltipSystem::advance(_registry, _entity, 1.0f);
		EXPECT_FALSE(tooltip().isVisible());
		TooltipSystem::advance(_registry, _entity, 1.0f);
		EXPECT_TRUE(tooltip().isVisible());
	}

	TEST_F(TestTooltipSystem, TickerProgressAdvancesTheDelays)
	{
		TooltipSystem::setAnchor(_registry, _entity, _anchor);
		hoverAnchor(true);
		const auto sample =
			_registry.getComponent<components::Animation>(_entity)
				.getOnSampleHandler();
		ASSERT_TRUE(sample);

		sample(0.4f);
		EXPECT_FLOAT_EQ(tooltip().getHoverTime(), 0.4f);
		EXPECT_FALSE(tooltip().isVisible());

		sample(0.9f);
		EXPECT_TRUE(tooltip().isVisible());

		// The looping ticker wraps around: 0.9 -> 0.1 is 0.2 s.
		sample(0.1f);
		EXPECT_FLOAT_EQ(tooltip().getLastProgress(), 0.1f);
		EXPECT_NEAR(tooltip().getHoverTime(), 1.1f, 1e-5f);
	}

	TEST_F(TestTooltipSystem, AGoneAnchorIsIgnored)
	{
		// No anchor at all.
		TooltipSystem::advance(_registry, _entity, 10.0f);
		EXPECT_FALSE(tooltip().isVisible());

		// An anchor whose components are gone.
		const ecs::Entity::Identifier gone { 99 };
		TooltipSystem::setAnchor(_registry, _entity, gone);
		EXPECT_FALSE(TooltipSystem::hasAnchor(_registry, _entity));
		TooltipSystem::advance(_registry, _entity, 10.0f);
		EXPECT_FALSE(tooltip().isVisible());

		const auto before = positionOf(_entity);
		TooltipSystem::place(_registry, _entity);
		EXPECT_EQ(positionOf(_entity), before);
	}

	TEST_F(TestTooltipSystem, ShowPlacesTheTooltipCenteredUnderItsAnchor)
	{
		TooltipSystem::setAnchor(_registry, _entity, _anchor);
		_system.update(_entity);

		TooltipSystem::show(_registry, _entity);

		// The tooltip is 66 wide (50 + 2 * 8 padding), the anchor 40.
		const float width =
			_registry.getComponent<components::Bound>(_entity).getWidth();
		EXPECT_FLOAT_EQ(width, 66.0f);
		EXPECT_FLOAT_EQ(positionOf(_entity).getX(),
						100.0f + (40.0f - width) / 2.0f);
		EXPECT_FLOAT_EQ(positionOf(_entity).getY(), 70.0f);
		// The body child follows the tooltip.
		EXPECT_FLOAT_EQ(positionOf(_text).getX(),
						positionOf(_entity).getX() + 8.0f);
	}

	TEST_F(TestTooltipSystem, PlacementFlipsAndClampsInsideTheViewport)
	{
		_registry.getComponent<components::Transform>(_anchor).setPose(
			utility::graphic::PoseF(
				utility::graphic::PositionF(-30.0f, 280.0f, 0.0f),
				utility::graphic::OrientationF()));
		TooltipSystem::setAnchor(_registry, _entity, _anchor);
		_system.update(_entity);
		TooltipSystem::setViewport(_registry, _entity, 400.0f, 300.0f);

		TooltipSystem::show(_registry, _entity);

		EXPECT_FLOAT_EQ(positionOf(_entity).getX(), 0.0f);
		EXPECT_FLOAT_EQ(positionOf(_entity).getY(), 280.0f - 32.0f);
	}

	TEST_F(TestTooltipSystem, ChangingTheViewportPlacesAShownTooltipAgain)
	{
		TooltipSystem::setAnchor(_registry, _entity, _anchor);
		_system.update(_entity);
		TooltipSystem::show(_registry, _entity);
		ASSERT_FLOAT_EQ(positionOf(_entity).getY(), 70.0f);

		TooltipSystem::setViewport(_registry, _entity, 400.0f, 80.0f);

		// No room under the anchor: flipped above it.
		EXPECT_FLOAT_EQ(positionOf(_entity).getY(), 50.0f - 32.0f);
	}

	TEST_F(TestTooltipSystem, OverlayDismissalHidesTheTooltip)
	{
		TooltipSystem::show(_registry, _entity);
		auto &overlay = _registry.getComponent<components::Overlay>(_entity);

		// systems::Overlay hides the overlay, then reports the dismissal.
		overlay.setVisible(false);
		overlay.getOnDismissHandler()();

		EXPECT_FALSE(tooltip().isVisible());
		EXPECT_EQ(_registry.getComponent<components::Color>(_entity).getColor(),
				  entities::transparentColor());
	}

	TEST_F(TestTooltipSystem, NegativeDelaysAreRejected)
	{
		EXPECT_THROW(tooltip().setEnterDelay(-1.0f), std::invalid_argument);
		EXPECT_THROW(tooltip().setLeaveDelay(-1.0f), std::invalid_argument);
	}

	TEST_F(TestTooltipSystem, AnUnchangedTooltipIsNotMarkedChanged)
	{
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
		EXPECT_FALSE(_registry.hasChanged(_title));
		EXPECT_FALSE(_registry.hasChanged(_text));
	}

	TEST_F(TestTooltipSystem, AnUnchangedShownTooltipIsNotMarkedChanged)
	{
		tooltip().setVariant(Variant::Rich);
		TooltipSystem::setAnchor(_registry, _entity, _anchor);
		TooltipSystem::show(_registry, _entity);
		_registry.resetChangedFlags();

		TooltipSystem::apply(_registry, _entity);
		TooltipSystem::apply(_registry, _entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
		EXPECT_FALSE(_registry.hasChanged(_title));
		EXPECT_FALSE(_registry.hasChanged(_text));
	}

}	 // namespace guillaume::systems::tests
