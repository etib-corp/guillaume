/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, distribute, sublicense, and/or sell copies
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

#include "systems/test_focus.hpp"

namespace guillaume::systems::tests
{

	TEST_F(TestFocusSystem, Constructor)
	{
		EXPECT_NO_THROW({ auto focusSystem = Focus(*_eventBus, _engine); });
	}

	TEST_F(TestFocusSystem, InitialFocusState)
	{
		EXPECT_FALSE(_focusSystem->getFocusedEntity().has_value());
		EXPECT_FALSE(_focusSystem->getLastFocusedEntity().has_value());
	}

	TEST_F(TestFocusSystem, SetFocusToEntity)
	{
		auto entity1 = createFocusableEntity();

		_focusSystem->setFocus(entity1);

		EXPECT_TRUE(_focusSystem->getFocusedEntity().has_value());
		EXPECT_EQ(_focusSystem->getFocusedEntity().value(), entity1);
		EXPECT_FALSE(_focusSystem->getLastFocusedEntity().has_value());
	}

	TEST_F(TestFocusSystem, SetFocusToEntityWithHandler)
	{
		auto entity1 = createFocusableEntity();

		bool focusGained = false;
		auto &focusComponent =
			_componentRegistry.getComponent<components::Focus>(entity1);
		focusComponent.setOnFocusGainedHandler([&focusGained]() {
			focusGained = true;
		});

		_focusSystem->setFocus(entity1);

		EXPECT_TRUE(focusGained);
	}

	TEST_F(TestFocusSystem, ClearFocus)
	{
		auto entity1 = createFocusableEntity();

		_focusSystem->setFocus(entity1);
		EXPECT_TRUE(_focusSystem->getFocusedEntity().has_value());

		_focusSystem->clearFocus();
		EXPECT_FALSE(_focusSystem->getFocusedEntity().has_value());
	}

	TEST_F(TestFocusSystem, ClearFocusWithHandler)
	{
		auto entity1 = createFocusableEntity();

		bool focusLost = false;
		auto &focusComponent =
			_componentRegistry.getComponent<components::Focus>(entity1);
		focusComponent.setOnFocusLostHandler([&focusLost]() {
			focusLost = true;
		});

		_focusSystem->setFocus(entity1);
		EXPECT_EQ(_focusSystem->getFocusedEntity().value(), entity1);
		EXPECT_FALSE(_focusSystem->getLastFocusedEntity().has_value());

		_focusSystem->clearFocus();
		EXPECT_TRUE(focusLost);
		EXPECT_FALSE(_focusSystem->getFocusedEntity().has_value());
		EXPECT_EQ(_focusSystem->getLastFocusedEntity().value(), entity1);
	}

	TEST_F(TestFocusSystem, SwitchFocusBetweenEntities)
	{
		auto entity1 = createFocusableEntity(
			utility::graphic::PoseF(utility::graphic::PositionF(-100, 0, 0),
									utility::graphic::OrientationF()),
			100, 50);
		auto entity2 = createFocusableEntity(
			utility::graphic::PoseF(utility::graphic::PositionF(100, 0, 0),
									utility::graphic::OrientationF()),
			100, 50);

		bool entity1Lost   = false;
		bool entity2Gained = false;

		auto &focus1 =
			_componentRegistry.getComponent<components::Focus>(entity1);
		focus1.setOnFocusLostHandler([&entity1Lost]() {
			entity1Lost = true;
		});

		auto &focus2 =
			_componentRegistry.getComponent<components::Focus>(entity2);
		focus2.setOnFocusGainedHandler([&entity2Gained]() {
			entity2Gained = true;
		});

		// Set focus to entity1
		_focusSystem->setFocus(entity1);
		EXPECT_EQ(_focusSystem->getFocusedEntity().value(), entity1);
		EXPECT_FALSE(_focusSystem->getLastFocusedEntity().has_value());

		// Switch focus to entity2
		_focusSystem->setFocus(entity2);
		EXPECT_EQ(_focusSystem->getFocusedEntity().value(), entity2);
		EXPECT_EQ(_focusSystem->getLastFocusedEntity().value(), entity1);
		EXPECT_TRUE(entity1Lost);
		EXPECT_TRUE(entity2Gained);
	}

	TEST_F(TestFocusSystem, UpdateWithMouseClick)
	{
		auto entity1 = createFocusableEntity();

		// Simulate mouse click by setting button pressed state
		auto &mouseInteraction =
			_componentRegistry.getComponent<components::MouseButtonInteraction>(
				entity1);
		mouseInteraction.setButtonPressed(
			utility::event::MouseButtonEvent::Button::Left, true);

		_focusSystem->update(entity1);

		EXPECT_TRUE(_focusSystem->getFocusedEntity().has_value());
		EXPECT_EQ(_focusSystem->getFocusedEntity().value(), entity1);
	}

	TEST_F(TestFocusSystem, UpdateWithHandClick)
	{
		auto entity1 = createFocusableEntity();

		// Simulate hand click by setting button pressed state
		auto &handInteraction =
			_componentRegistry.getComponent<components::HandButtonInteraction>(
				entity1);
		handInteraction.setButtonPressed(
			utility::event::HandButtonEvent::Button::A, true);

		_focusSystem->update(entity1);

		EXPECT_TRUE(_focusSystem->getFocusedEntity().has_value());
		EXPECT_EQ(_focusSystem->getFocusedEntity().value(), entity1);
	}

	TEST_F(TestFocusSystem, UpdateWithHandPinch)
	{
		auto entity1 = createFocusableEntity();

		// Simulate hand pinch by setting pinch state
		auto &pinchInteraction =
			_componentRegistry.getComponent<components::HandPinchInteraction>(
				entity1);
		pinchInteraction.setPinching(true);

		_focusSystem->update(entity1);

		EXPECT_TRUE(_focusSystem->getFocusedEntity().has_value());
		EXPECT_EQ(_focusSystem->getFocusedEntity().value(), entity1);
	}

	TEST_F(TestFocusSystem, UpdateWithHandPoke)
	{
		auto entity1 = createFocusableEntity();

		// Simulate hand poke by setting poke state
		auto &pokeInteraction =
			_componentRegistry.getComponent<components::HandPokeInteraction>(
				entity1);
		pokeInteraction.setPoking(true);

		_focusSystem->update(entity1);

		EXPECT_TRUE(_focusSystem->getFocusedEntity().has_value());
		EXPECT_EQ(_focusSystem->getFocusedEntity().value(), entity1);
	}

	TEST_F(TestFocusSystem, UpdateNoClick)
	{
		auto entity1 = createFocusableEntity();

		// No button pressed
		_focusSystem->update(entity1);

		// Focus should remain unchanged (nullopt initially)
		EXPECT_FALSE(_focusSystem->getFocusedEntity().has_value());
	}

	TEST_F(TestFocusSystem, UpdateDoesNotChangeFocusWithoutClick)
	{
		auto entity1 = createFocusableEntity();
		auto entity2 = createFocusableEntity();

		// Set initial focus
		_focusSystem->setFocus(entity1);
		EXPECT_TRUE(_focusSystem->getFocusedEntity().has_value());

		// Update entity2 without click should not change focus
		_focusSystem->update(entity2);

		// Focus should remain on entity1
		EXPECT_TRUE(_focusSystem->getFocusedEntity().has_value());
		EXPECT_EQ(_focusSystem->getFocusedEntity().value(), entity1);
	}

	TEST_F(TestFocusSystem, MultipleEntitiesSameFocus)
	{
		auto entity1 = createFocusableEntity(
			utility::graphic::PoseF(utility::graphic::PositionF(-100, 0, 0),
									utility::graphic::OrientationF()),
			100, 50);
		auto entity2 = createFocusableEntity(
			utility::graphic::PoseF(utility::graphic::PositionF(100, 0, 0),
									utility::graphic::OrientationF()),
			100, 50);

		_focusSystem->setFocus(entity1);
		EXPECT_EQ(_focusSystem->getFocusedEntity().value(), entity1);

		// Setting focus to same entity again should not trigger handler twice
		int focusCount = 0;
		auto &focus1 =
			_componentRegistry.getComponent<components::Focus>(entity1);
		focus1.setOnFocusGainedHandler([&focusCount]() {
			focusCount++;
		});

		_focusSystem->setFocus(entity1);
		EXPECT_EQ(focusCount, 1);
	}

}	 // namespace guillaume::systems::tests
