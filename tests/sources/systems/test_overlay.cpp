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

#include "systems/test_overlay.hpp"

#include <utility/event/keyboard_event.hpp>

#include "guillaume/components/text.hpp"
#include "guillaume/components/text_field.hpp"

#include "guillaume/systems/text_field.hpp"

namespace guillaume::systems::tests
{

	TEST_F(TestOverlay, ShowPushesAndCapturesInput)
	{
		const auto first  = createOverlayEntity();
		const auto second = createOverlayEntity();

		_overlaySystem->show(first);
		_overlaySystem->show(second);

		EXPECT_EQ(_overlaySystem->getDepth(), 2u);
		EXPECT_EQ(_overlaySystem->getTop(), second);
		EXPECT_TRUE(_overlaySystem->isOpen(first));
		EXPECT_FALSE(_engine->shouldCaptureViewportInput());
	}

	TEST_F(TestOverlay, DismissPopsAndRestoresInput)
	{
		const auto first  = createOverlayEntity();
		const auto second = createOverlayEntity();

		bool dismissed = false;
		_componentRegistry.getComponent<components::Overlay>(second)
			.setOnDismissHandler([&dismissed]() {
				dismissed = true;
			});

		_overlaySystem->show(first);
		_overlaySystem->show(second);
		_overlaySystem->dismiss();

		EXPECT_TRUE(dismissed);
		EXPECT_EQ(_overlaySystem->getDepth(), 1u);
		EXPECT_EQ(_overlaySystem->getTop(), first);
		EXPECT_FALSE(_engine->shouldCaptureViewportInput());

		_overlaySystem->dismiss();

		EXPECT_EQ(_overlaySystem->getDepth(), 0u);
		EXPECT_TRUE(_engine->shouldCaptureViewportInput());
	}

	TEST_F(TestOverlay, EscapeDismissesTop)
	{
		const auto entity = createOverlayEntity();
		_overlaySystem->show(entity);

		auto key = std::make_shared<utility::event::KeyboardEvent>();
		key->setScancode(utility::event::KeyboardEvent::ScanCode::Escape);
		key->setIsDownEvent(true);
		_eventBus.publish(key);

		_overlaySystem->prepare();

		EXPECT_EQ(_overlaySystem->getDepth(), 0u);
	}

	// #176: one Escape dismisses one overlay, not one per frame.
	TEST_F(TestOverlay, EscapeDismissesOnlyOneOverlay)
	{
		const auto first  = createOverlayEntity();
		const auto second = createOverlayEntity();
		_overlaySystem->show(first);
		_overlaySystem->show(second);

		auto key = std::make_shared<utility::event::KeyboardEvent>();
		key->setScancode(utility::event::KeyboardEvent::ScanCode::Escape);
		key->setIsDownEvent(true);
		_eventBus.publish(key);

		_overlaySystem->prepare();
		_overlaySystem->prepare();

		EXPECT_EQ(_overlaySystem->getDepth(), 1u);
		EXPECT_EQ(_overlaySystem->getTop(), first);
	}

	TEST_F(TestOverlay, OutsideClickDismisses)
	{
		const auto entity = createOverlayEntity(0.0f, 0.0f, 100.0f, 50.0f);
		_overlaySystem->show(entity);

		_overlaySystem->handleOutsideClick(
			utility::math::Vector2F({ 200.0f, 200.0f }));

		EXPECT_EQ(_overlaySystem->getDepth(), 0u);
	}

	TEST_F(TestOverlay, InsideClickDoesNotDismiss)
	{
		const auto entity = createOverlayEntity(0.0f, 0.0f, 100.0f, 50.0f);
		_overlaySystem->show(entity);

		// The overlay extends down from its top-left corner (y points up).
		_overlaySystem->handleOutsideClick(
			utility::math::Vector2F({ 10.0f, -10.0f }));

		EXPECT_EQ(_overlaySystem->getDepth(), 1u);
	}

	TEST_F(TestOverlay, AutoDismissAfterDelay)
	{
		const auto entity = createOverlayEntity();

		_componentRegistry.getComponent<components::Overlay>(entity)
			.setAutoDismiss(1.0f);

		_overlaySystem->show(entity);
		_overlaySystem->advance(0.5f);
		EXPECT_EQ(_overlaySystem->getDepth(), 1u);

		_overlaySystem->advance(0.6f);
		EXPECT_EQ(_overlaySystem->getDepth(), 0u);
	}

	TEST_F(TestOverlay, MousePressOutsideDismissesTheTopOverlayOnce)
	{
		setUpView();
		const auto entity = createCenteredOverlayEntity();
		int dismissals	  = 0;
		countDismissals(entity, dismissals);
		_overlaySystem->show(entity);

		publishMouseButton(10.0f, 10.0f);
		_overlaySystem->prepare();

		EXPECT_EQ(_overlaySystem->getDepth(), 0u);
		EXPECT_EQ(dismissals, 1);

		// The press stays the last event: it must not close the overlay
		// again once it is reopened.
		_overlaySystem->show(entity);
		_overlaySystem->prepare();

		EXPECT_EQ(_overlaySystem->getDepth(), 1u);
		EXPECT_EQ(dismissals, 1);
	}

	TEST_F(TestOverlay, MousePressInsideDoesNotDismiss)
	{
		setUpView();
		const auto entity = createCenteredOverlayEntity();
		_overlaySystem->show(entity);

		publishMouseButton(400.0f, 300.0f);
		_overlaySystem->prepare();

		EXPECT_EQ(_overlaySystem->getDepth(), 1u);
	}

	TEST_F(TestOverlay, MouseReleaseOutsideDoesNotDismiss)
	{
		setUpView();
		const auto entity = createCenteredOverlayEntity();
		_overlaySystem->show(entity);

		publishMouseButton(10.0f, 10.0f, false);
		_overlaySystem->prepare();

		EXPECT_EQ(_overlaySystem->getDepth(), 1u);
	}

	TEST_F(TestOverlay, MousePressOutsideRespectsTheOverlayFlag)
	{
		setUpView();
		const auto entity = createCenteredOverlayEntity();
		_componentRegistry.getComponent<components::Overlay>(entity)
			.setDismissOnOutsideClick(false);
		_overlaySystem->show(entity);

		publishMouseButton(10.0f, 10.0f);
		_overlaySystem->prepare();

		EXPECT_EQ(_overlaySystem->getDepth(), 1u);
	}

	TEST_F(TestOverlay, MousePressBeforeOpeningDoesNotDismiss)
	{
		setUpView();
		const auto entity = createCenteredOverlayEntity();

		publishMouseButton(10.0f, 10.0f);
		_overlaySystem->prepare();

		_overlaySystem->show(entity);
		_overlaySystem->prepare();

		EXPECT_EQ(_overlaySystem->getDepth(), 1u);
	}

	TEST_F(TestOverlay, HidingAnOpenOverlayDoesNotCallTheDismissHandler)
	{
		const auto entity = createOverlayEntity();
		int dismissals	  = 0;
		countDismissals(entity, dismissals);
		_overlaySystem->show(entity);

		// What Dialog::hide and Menu::hide do.
		_componentRegistry.getComponent<components::Overlay>(entity).setVisible(
			false);
		_overlaySystem->update(entity);

		EXPECT_EQ(dismissals, 0);
		EXPECT_FALSE(_overlaySystem->isOpen(entity));
		EXPECT_TRUE(_engine->shouldCaptureViewportInput());
	}

	TEST_F(TestOverlay, AnExplicitDismissIsReportedOnce)
	{
		const auto entity = createOverlayEntity();
		int dismissals	  = 0;
		countDismissals(entity, dismissals);
		_overlaySystem->show(entity);

		// What Dialog::dismiss and Menu::dismiss do: hide, then report.
		auto &overlay =
			_componentRegistry.getComponent<components::Overlay>(entity);
		overlay.setVisible(false);
		overlay.getOnDismissHandler()();
		_overlaySystem->update(entity);

		EXPECT_EQ(dismissals, 1);
		EXPECT_FALSE(_overlaySystem->isOpen(entity));
	}

	TEST_F(TestOverlay, AutoDismissAdvancesUnderAnotherOverlay)
	{
		const auto snackbar = createOverlayEntity();
		const auto dialog	= createOverlayEntity();
		int dismissals		= 0;
		countDismissals(snackbar, dismissals);

		_componentRegistry.getComponent<components::Overlay>(snackbar)
			.setAutoDismiss(1.0f);

		_overlaySystem->show(snackbar);
		_overlaySystem->show(dialog);

		_overlaySystem->advance(0.5f);
		EXPECT_EQ(_overlaySystem->getDepth(), 2u);

		_overlaySystem->advance(0.6f);
		EXPECT_FALSE(_overlaySystem->isOpen(snackbar));
		EXPECT_TRUE(_overlaySystem->isOpen(dialog));
		EXPECT_EQ(dismissals, 1);
		EXPECT_FALSE(
			_componentRegistry.getComponent<components::Overlay>(snackbar)
				.isVisible());
		EXPECT_FALSE(_engine->shouldCaptureViewportInput());
	}

	TEST_F(TestOverlay, ChangingTheRegistryForgetsTheStack)
	{
		const auto entity = createOverlayEntity();
		int dismissals	  = 0;
		countDismissals(entity, dismissals);

		_overlaySystem->prepare();
		_overlaySystem->show(entity);
		_overlaySystem->prepare();
		EXPECT_EQ(_overlaySystem->getDepth(), 1u);

		// Another scene: another registry is bound to the same system.
		ecs::ComponentRegistry otherRegistry;
		_overlaySystem->bindComponentRegistry(otherRegistry);
		_overlaySystem->prepare();

		EXPECT_EQ(_overlaySystem->getDepth(), 0u);
		EXPECT_FALSE(_overlaySystem->isOpen(entity));
		EXPECT_TRUE(_engine->shouldCaptureViewportInput());
		EXPECT_EQ(dismissals, 0);

		// Back in the first scene, the overlay is still visible there and
		// is pushed again when it is visited.
		_overlaySystem->bindComponentRegistry(_componentRegistry);
		_overlaySystem->prepare();
		_overlaySystem->update(entity);

		EXPECT_EQ(_overlaySystem->getTop(), entity);
	}

	TEST_F(TestOverlay, TextFieldErrorAndContentFloatLabel)
	{
		components::TextField textField;

		EXPECT_FALSE(TextField::reconcile(textField, false));
		EXPECT_FALSE(textField.isLabelFloating());

		EXPECT_TRUE(TextField::reconcile(textField, true));
		EXPECT_TRUE(textField.isLabelFloating());

		components::TextField emptyField;
		emptyField.setError(true);
		EXPECT_TRUE(TextField::reconcile(emptyField, false));
		EXPECT_TRUE(emptyField.isLabelFloating());
	}

	TEST_F(TestOverlay, TextFieldSystemFloatsLabelWithContent)
	{
		ecs::Entity entity;
		const auto id = entity.getIdentifier();

		_componentRegistry.addComponent<components::TextField>(id);
		_componentRegistry.addComponent<components::Text>(id);

		auto textFieldSystem = TextField();
		textFieldSystem.bindComponentRegistry(_componentRegistry);

		textFieldSystem.update(id);
		EXPECT_FALSE(_componentRegistry.getComponent<components::TextField>(id)
						 .isLabelFloating());

		_componentRegistry.getComponent<components::Text>(id).setContent("hi");

		textFieldSystem.update(id);
		EXPECT_TRUE(_componentRegistry.getComponent<components::TextField>(id)
						.isLabelFloating());

		textFieldSystem.unbindComponentRegistry();
	}

}	 // namespace guillaume::systems::tests
