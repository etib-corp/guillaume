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

#include "systems/test_dialog.hpp"

#include <vector>

#include <guillaume/systems/backdrop.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Variant	   = components::Dialog::Variant;
		using DialogSystem = guillaume::systems::Dialog;
	}	 // namespace

	TEST_F(TestDialogSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestDialogSystem, InstallSetsSemanticsAndMarksTheTitleAsHeader)
	{
		const auto &accessibility =
			_registry.getComponent<components::Accessibility>(_entity);
		EXPECT_EQ(accessibility.getRole(),
				  components::AccessibilityRole::Dialog);
		EXPECT_FALSE(accessibility.isFocusable());

		const auto &title =
			_registry.getComponent<components::Accessibility>(_title);
		EXPECT_EQ(title.getRole(), components::AccessibilityRole::Header);
		EXPECT_EQ(title.getLabel(), "Title");
	}

	TEST_F(TestDialogSystem, AlertIsARoundedElevatedPanelOfFixedWidth)
	{
		_system.update(_entity);

		const auto &bound = _registry.getComponent<components::Bound>(_entity);
		EXPECT_FLOAT_EQ(bound.getWidth(), components::Dialog::Width);
		EXPECT_FALSE(layout().hasFixedHeight());
		EXPECT_FLOAT_EQ(layout().getPadding(), DialogSystem::Padding);
		EXPECT_FLOAT_EQ(layout().getSpacing(), DialogSystem::Spacing);
		EXPECT_FLOAT_EQ(_registry.getComponent<components::Borders>(_entity)
							.getTopLeftRadius(),
						getShapeRadius(theme::ShapeRole::ExtraLarge));
		EXPECT_EQ(colorOf(_entity),
				  getElevatedSurfaceColor(theme::ElevationLevel::Level3));
		EXPECT_EQ(_registry.getComponent<components::Elevation>(_entity)
					  .getElevationLevel(),
				  theme::ElevationLevel::Level3);
	}

	TEST_F(TestDialogSystem, FullScreenIsTallUnroundedAndKeepsOutsideClicks)
	{
		dialog().setVariant(Variant::FullScreen);

		_system.update(_entity);

		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getHeight(),
			components::Dialog::FullScreenHeight);
		EXPECT_FLOAT_EQ(_registry.getComponent<components::Borders>(_entity)
							.getTopLeftRadius(),
						0.0f);
		EXPECT_FALSE(overlay().dismissesOnOutsideClick());

		dialog().setVariant(Variant::Alert);
		_system.update(_entity);
		EXPECT_FALSE(layout().hasFixedHeight());
		EXPECT_TRUE(overlay().dismissesOnOutsideClick());
	}

	TEST_F(TestDialogSystem, OverlayIsModalAndEscapable)
	{
		_system.update(_entity);

		EXPECT_TRUE(overlay().isModal());
		EXPECT_TRUE(overlay().dismissesOnOutsideClick());
		EXPECT_TRUE(overlay().dismissesOnEscape());
		EXPECT_FALSE(overlay().isVisible());
	}

	TEST_F(TestDialogSystem, ChildrenAreStackedInOrder)
	{
		_system.update(_entity);

		EXPECT_EQ(layout().getChildren(),
				  (std::vector<ecs::Entity::Identifier> {
					  _icon, _title, _message, _actionRow }));
		const auto y = [this](ecs::Entity::Identifier identifier) {
			return _registry.getComponent<components::Transform>(identifier)
				.getPose()
				.getPosition()
				.getY();
		};
		EXPECT_FLOAT_EQ(y(_icon), DialogSystem::Padding);
		EXPECT_LT(y(_icon), y(_title));
		EXPECT_LT(y(_title), y(_message));
		EXPECT_LT(y(_message), y(_actionRow));
	}

	TEST_F(TestDialogSystem, MissingChildrenAreLeftOut)
	{
		dialog()
			.setIconEntity(ecs::Entity::InvalidIdentifier)
			.setActionRow(ecs::Entity::InvalidIdentifier);

		_system.update(_entity);

		EXPECT_EQ(layout().getChildren(),
				  (std::vector<ecs::Entity::Identifier> { _title, _message }));
	}

	TEST_F(TestDialogSystem, AHeroIconCentersTheContent)
	{
		_system.update(_entity);
		EXPECT_EQ(layout().getCrossAxisAlignment(),
				  components::Layout::CrossAxisAlignment::Center);
		EXPECT_EQ(colorOf(_icon), getActiveColor(SchemeColorRole::Secondary));

		dialog().setIconEntity(ecs::Entity::InvalidIdentifier);
		_system.update(_entity);
		EXPECT_EQ(layout().getCrossAxisAlignment(),
				  components::Layout::CrossAxisAlignment::Start);
	}

	TEST_F(TestDialogSystem, DefaultIconOnlyExistsForConfirmation)
	{
		EXPECT_TRUE(dialog().getEffectiveIcon().empty());

		dialog().setVariant(Variant::Confirmation);
		EXPECT_EQ(dialog().getEffectiveIcon(), "info");

		dialog().setIcon("warning");
		EXPECT_EQ(dialog().getEffectiveIcon(), "warning");
	}

	TEST_F(TestDialogSystem, TitleAndMessageAreMirroredOnTheChildren)
	{
		dialog().setTitle("Delete?").setMessage("This cannot be undone");

		_system.update(_entity);

		EXPECT_EQ(_registry.getComponent<components::Text>(_title).getContent(),
				  "Delete?");
		EXPECT_EQ(
			_registry.getComponent<components::Text>(_message).getContent(),
			"This cannot be undone");
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_entity)
					  .getLabel(),
				  "Delete?");
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_title)
					  .getLabel(),
				  "Delete?");
		EXPECT_EQ(colorOf(_title), getActiveColor(SchemeColorRole::OnSurface));
		EXPECT_EQ(colorOf(_message),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
	}

	TEST_F(TestDialogSystem, ShowingFadesTheBackdropInBehindTheDialog)
	{
		_system.update(_entity);
		EXPECT_FALSE(backdrop().isVisible());

		DialogSystem::show(_registry, _entity);

		EXPECT_TRUE(overlay().isVisible());
		EXPECT_TRUE(backdrop().isVisible());
		EXPECT_TRUE(_registry.getComponent<components::Animation>(_backdrop)
						.isPlaying());

		// The backdrop is centred on the dialog.
		const auto &bound = _registry.getComponent<components::Bound>(_entity);
		const auto position =
			_registry.getComponent<components::Transform>(_backdrop)
				.getPose()
				.getPosition();
		EXPECT_FLOAT_EQ(position.getX(),
						-(Backdrop::Extent - bound.getWidth()) / 2.0f);
		EXPECT_FLOAT_EQ(position.getY(),
						-(Backdrop::Extent - bound.getHeight()) / 2.0f);
	}

	TEST_F(TestDialogSystem, HidingHidesTheBackdropWithoutDismissal)
	{
		DialogSystem::show(_registry, _entity);

		DialogSystem::hide(_registry, _entity);

		EXPECT_FALSE(overlay().isVisible());
		EXPECT_FALSE(backdrop().isVisible());
		EXPECT_EQ(_dismissals, 0);
	}

	TEST_F(TestDialogSystem, AFullScreenDialogShowsNoBackdrop)
	{
		dialog().setVariant(Variant::FullScreen);

		DialogSystem::show(_registry, _entity);

		EXPECT_TRUE(overlay().isVisible());
		EXPECT_FALSE(backdrop().isVisible());
	}

	TEST_F(TestDialogSystem, AnOverlayDismissalHidesTheBackdropOnTheNextUpdate)
	{
		DialogSystem::show(_registry, _entity);

		// systems::Overlay hides the overlay on Escape or an outside click.
		overlay().setVisible(false);
		_system.update(_entity);

		EXPECT_FALSE(backdrop().isVisible());
	}

	TEST_F(TestDialogSystem, DismissHidesThenCallsTheHandlerOnce)
	{
		DialogSystem::show(_registry, _entity);

		DialogSystem::dismiss(_registry, _entity);

		EXPECT_EQ(_dismissals, 1);
		EXPECT_FALSE(overlay().isVisible());
		EXPECT_FALSE(backdrop().isVisible());

		// Already hidden: nothing more to report.
		DialogSystem::dismiss(_registry, _entity);
		EXPECT_EQ(_dismissals, 1);
	}

	TEST_F(TestDialogSystem, PressingAnActionCallsItsHandler)
	{
		DialogSystem::pressAction(_registry, _entity, 0);
		EXPECT_EQ(_confirmations, 0);

		DialogSystem::pressAction(_registry, _entity, 1);
		EXPECT_EQ(_confirmations, 1);

		DialogSystem::pressAction(_registry, _entity, 7);
		EXPECT_EQ(_confirmations, 1);
	}

	TEST_F(TestDialogSystem, AnActionMayReplaceTheActions)
	{
		dialog().setActions({ { "Once", [this]() {
								   ++_confirmations;
								   dialog().setActions({});
							   } } });

		DialogSystem::pressAction(_registry, _entity, 0);
		DialogSystem::pressAction(_registry, _entity, 0);

		EXPECT_EQ(_confirmations, 1);
	}

	TEST_F(TestDialogSystem, AnUnchangedDialogIsNotMarkedChanged)
	{
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);
		_system.update(_entity);

		EXPECT_FALSE(anythingChanged());
	}

	TEST_F(TestDialogSystem, AnUnchangedShownDialogIsNotMarkedChanged)
	{
		DialogSystem::show(_registry, _entity);
		_registry.resetChangedFlags();

		DialogSystem::apply(_registry, _entity);
		DialogSystem::show(_registry, _entity);

		EXPECT_FALSE(anythingChanged());
	}

}	 // namespace guillaume::systems::tests
