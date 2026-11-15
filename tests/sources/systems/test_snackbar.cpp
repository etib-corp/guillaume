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

#include "systems/test_snackbar.hpp"

#include <stdexcept>
#include <vector>

#include <guillaume/components/mouse_button_interaction.hpp>
#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Variant		 = components::Snackbar::Variant;
		using SnackbarSystem = guillaume::systems::Snackbar;
	}	 // namespace

	TEST_F(TestSnackbarSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestSnackbarSystem, InstallSetsSemanticsAndMakesTheActionAButton)
	{
		const auto &accessibility =
			_registry.getComponent<components::Accessibility>(_entity);
		EXPECT_EQ(accessibility.getRole(),
				  components::AccessibilityRole::Alert);
		EXPECT_EQ(accessibility.getLiveRegion(),
				  components::LiveRegion::Polite);

		const auto &action =
			_registry.getComponent<components::Accessibility>(_action);
		EXPECT_EQ(action.getRole(), components::AccessibilityRole::Button);
		EXPECT_EQ(action.getLabel(), "Undo");
	}

	TEST_F(TestSnackbarSystem, ShownSnackbarIsAnElevatedInverseSurface)
	{
		_system.update(_entity);

		EXPECT_TRUE(overlay().isVisible());
		EXPECT_FALSE(overlay().isModal());
		EXPECT_FALSE(overlay().dismissesOnOutsideClick());
		EXPECT_EQ(colorOf(_entity),
				  getActiveColor(SchemeColorRole::InverseSurface));
		EXPECT_EQ(colorOf(_message),
				  getActiveColor(SchemeColorRole::InverseOnSurface));
		EXPECT_EQ(colorOf(_action),
				  getActiveColor(SchemeColorRole::InversePrimary));
		EXPECT_EQ(_registry.getComponent<components::Elevation>(_entity)
					  .getElevationLevel(),
				  theme::ElevationLevel::Level3);
		EXPECT_FLOAT_EQ(_registry.getComponent<components::Borders>(_entity)
							.getTopLeftRadius(),
						getShapeRadius(theme::ShapeRole::ExtraSmall));
	}

	TEST_F(TestSnackbarSystem, HeightFollowsTheVariant)
	{
		const auto &bound = _registry.getComponent<components::Bound>(_entity);

		_system.update(_entity);
		EXPECT_FLOAT_EQ(bound.getHeight(), 48.0f);

		snackbar().setVariant(Variant::MultiLine);
		_system.update(_entity);
		EXPECT_FLOAT_EQ(bound.getHeight(), 68.0f);

		snackbar().setVariant(Variant::SingleLine);
		_system.update(_entity);
		EXPECT_FLOAT_EQ(bound.getHeight(), 48.0f);
	}

	TEST_F(TestSnackbarSystem, MessageAndActionAreSpreadAcrossTheBar)
	{
		_system.update(_entity);

		EXPECT_EQ(
			_registry.getComponent<components::Layout>(_entity).getChildren(),
			(std::vector<ecs::Entity::Identifier> { _message, _action }));
		const auto x = [this](ecs::Entity::Identifier identifier) {
			return _registry.getComponent<components::Transform>(identifier)
				.getPose()
				.getPosition()
				.getX();
		};
		// 16 dp padding on both sides of the 344 dp bar.
		EXPECT_FLOAT_EQ(x(_message), 16.0f);
		EXPECT_FLOAT_EQ(x(_action), 344.0f - 16.0f - 40.0f);
	}

	TEST_F(TestSnackbarSystem, DurationDrivesTheOverlayAutoDismiss)
	{
		_system.update(_entity);
		EXPECT_FLOAT_EQ(overlay().getAutoDismiss(),
						components::Snackbar::DurationMedium);

		snackbar().setDuration(components::Snackbar::DurationShort);
		_system.update(_entity);
		EXPECT_FLOAT_EQ(overlay().getAutoDismiss(),
						components::Snackbar::DurationShort);

		snackbar().setDuration(0.0f);
		_system.update(_entity);
		EXPECT_FALSE(overlay().hasAutoDismiss());

		EXPECT_THROW(snackbar().setDuration(-1.0f), std::invalid_argument);
	}

	TEST_F(TestSnackbarSystem, MessageIsMirroredOnTheLabelAndTheChild)
	{
		snackbar().setMessage("Deleted");

		_system.update(_entity);

		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_entity)
					  .getLabel(),
				  "Deleted");
		EXPECT_EQ(
			_registry.getComponent<components::Text>(_message).getContent(),
			"Deleted");
	}

	TEST_F(TestSnackbarSystem, HidePaintsNothingAndReportsNoDismissal)
	{
		_system.update(_entity);

		SnackbarSystem::hide(_registry, _entity);

		EXPECT_FALSE(snackbar().isVisible());
		EXPECT_FALSE(overlay().isVisible());
		EXPECT_EQ(colorOf(_entity), entities::transparentColor());
		EXPECT_EQ(colorOf(_message), entities::transparentColor());
		EXPECT_EQ(colorOf(_action), entities::transparentColor());
		EXPECT_FALSE(
			_registry.getComponent<components::Elevation>(_entity).hasShadow());
		EXPECT_EQ(_dismissals, 0);
	}

	TEST_F(TestSnackbarSystem, ShowRestartsTheCountdown)
	{
		_system.update(_entity);
		SnackbarSystem::hide(_registry, _entity);
		overlay().setElapsed(3.0f);

		SnackbarSystem::show(_registry, _entity);

		EXPECT_TRUE(snackbar().isVisible());
		EXPECT_TRUE(overlay().isVisible());
		EXPECT_FLOAT_EQ(overlay().getElapsed(), 0.0f);
		EXPECT_EQ(colorOf(_entity),
				  getActiveColor(SchemeColorRole::InverseSurface));
	}

	TEST_F(TestSnackbarSystem, ShowingAShownSnackbarKeepsTheCountdown)
	{
		_system.update(_entity);
		overlay().setElapsed(3.0f);

		SnackbarSystem::show(_registry, _entity);

		EXPECT_FLOAT_EQ(overlay().getElapsed(), 3.0f);
	}

	TEST_F(TestSnackbarSystem, OverlayTimeoutDismissesOnce)
	{
		_system.update(_entity);

		// systems::Overlay hides the overlay, then reports the dismissal.
		overlay().setVisible(false);
		overlay().getOnDismissHandler()();

		EXPECT_EQ(_dismissals, 1);
		EXPECT_FALSE(snackbar().isVisible());
		EXPECT_EQ(colorOf(_entity), entities::transparentColor());

		// Already hidden: nothing more to report.
		overlay().getOnDismissHandler()();
		SnackbarSystem::dismiss(_registry, _entity);
		EXPECT_EQ(_dismissals, 1);
	}

	TEST_F(TestSnackbarSystem, PressingTheActionRunsItThenDismisses)
	{
		_system.update(_entity);

		_registry.getComponent<components::MouseButtonInteraction>(_action)
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_EQ(_actions, 1);
		EXPECT_EQ(_dismissals, 1);
		EXPECT_FALSE(snackbar().isVisible());
		EXPECT_FALSE(overlay().isVisible());
	}

	TEST_F(TestSnackbarSystem, AHiddenSnackbarIgnoresItsAction)
	{
		SnackbarSystem::hide(_registry, _entity);

		SnackbarSystem::pressAction(_registry, _entity);

		EXPECT_EQ(_actions, 0);
		EXPECT_EQ(_dismissals, 0);
	}

	TEST_F(TestSnackbarSystem, ACallbackMayReplaceItself)
	{
		_system.update(_entity);
		snackbar().setOnDismiss([this]() {
			++_dismissals;
			snackbar().setOnDismiss({});
		});

		SnackbarSystem::dismiss(_registry, _entity);

		EXPECT_EQ(_dismissals, 1);
		EXPECT_FALSE(snackbar().getOnDismiss());
	}

	TEST_F(TestSnackbarSystem, AnUnchangedSnackbarIsNotMarkedChanged)
	{
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);
		_system.update(_entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
		EXPECT_FALSE(_registry.hasChanged(_message));
		EXPECT_FALSE(_registry.hasChanged(_action));
	}

	TEST_F(TestSnackbarSystem, AnUnchangedHiddenSnackbarIsNotMarkedChanged)
	{
		SnackbarSystem::hide(_registry, _entity);
		_registry.resetChangedFlags();

		SnackbarSystem::apply(_registry, _entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
		EXPECT_FALSE(_registry.hasChanged(_message));
		EXPECT_FALSE(_registry.hasChanged(_action));
	}

}	 // namespace guillaume::systems::tests
