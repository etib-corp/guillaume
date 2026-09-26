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

		_overlaySystem->handleOutsideClick(
			utility::math::Vector2F({ 10.0f, 10.0f }));

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
