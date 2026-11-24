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

#include "systems/test_input_events.hpp"

#include <utility/event/hand_button_event.hpp>
#include <utility/event/hand_motion_event.hpp>
#include <utility/event/mouse_motion_event.hpp>
#include <utility/event/mouse_wheel_event.hpp>
#include <utility/ressource_provider.hpp>

#include <guillaume/components/drag_interaction.hpp>
#include <guillaume/components/hand_button_interaction.hpp>
#include <guillaume/components/hand_hover_interaction.hpp>
#include <guillaume/components/mouse_button_interaction.hpp>
#include <guillaume/components/mouse_hover_interaction.hpp>
#include <guillaume/components/scrollable.hpp>
#include <guillaume/ecs/system_registry.hpp>
#include <guillaume/systems/core_systems.hpp>
#include <guillaume/systems/drag.hpp>
#include <guillaume/systems/hand_button.hpp>
#include <guillaume/systems/hand_motion.hpp>
#include <guillaume/systems/hand_pinch.hpp>
#include <guillaume/systems/hand_squeeze.hpp>
#include <guillaume/systems/hand_thumb_rest.hpp>
#include <guillaume/systems/hand_thumb_stick.hpp>
#include <guillaume/systems/hand_trigger.hpp>
#include <guillaume/systems/mouse_button.hpp>
#include <guillaume/systems/scroll.hpp>

#include "mocks/engine_mock.hpp"
#include "mocks/fake_system_io.hpp"

namespace guillaume::systems::tests
{
	// #176 (care note): hover is a state. The pointer keeps its pose on the
	// frames that bring no motion event, and is still tested.
	TEST_F(TestInputEvents, HoverKeepsThePointerOnFramesWithoutMotion)
	{
		HandMotion system(_eventBus);
		const auto entity = addEntity<components::HandHoverInteraction>();
		auto &hover =
			_componentRegistry.getComponent<components::HandHoverInteraction>(
				entity);
		int hovers = 0;
		hover.setOnHoverHandler([&hovers]() {
			++hovers;
		});

		_eventBus.publish(
			makeHandEvent<utility::event::HandMotionEvent>(50.0f, -25.0f));
		run(system);
		EXPECT_TRUE(hover.isHovered());

		run(system);
		run(system);
		EXPECT_TRUE(hover.isHovered());
		EXPECT_EQ(hovers, 1);

		// The entity leaves the motionless pointer: the hover ends.
		_componentRegistry.getComponent<components::Transform>(entity).setPose(
			utility::graphic::PoseF(
				utility::graphic::PositionF(1000.0f, 0.0f, 0.0f),
				utility::graphic::OrientationF()));
		run(system);
		EXPECT_FALSE(hover.isHovered());
	}

	// #176: a press is applied once; the button stays marked pressed.
	TEST_F(TestInputEvents, HeldButtonCallsThePressHandlerOnce)
	{
		HandButton system(_eventBus);
		const auto entity = addEntity<components::HandButtonInteraction>();
		auto &interaction =
			_componentRegistry.getComponent<components::HandButtonInteraction>(
				entity);
		int presses = 0;
		interaction.setOnButtonPressHandler(
			utility::event::HandButtonEvent::Button::A, [&presses]() {
				++presses;
			});

		auto event =
			makeHandEvent<utility::event::HandButtonEvent>(50.0f, -25.0f);
		event->setHandType(utility::event::HandEvent::HandType::Right);
		event->setButton(utility::event::HandButtonEvent::Button::A);
		event->setButtonPressed(true);
		_eventBus.publish(event);

		run(system);
		run(system);
		run(system);

		EXPECT_EQ(presses, 1);
		EXPECT_TRUE(interaction.isButtonPressed(
			utility::event::HandButtonEvent::Button::A));
	}

	// #176: the pinch flag does not stay set after the pinch event.
	TEST_F(TestInputEvents, PinchFlagIsClearedOnAFrameWithoutEvent)
	{
		HandPinch system(_eventBus);
		const auto entity = addEntity<components::HandPinchInteraction>();
		auto &interaction =
			_componentRegistry.getComponent<components::HandPinchInteraction>(
				entity);
		int pinches = 0;
		interaction.setOnEventHandler([&pinches]() {
			++pinches;
		});

		_eventBus.publish(
			makeHandEvent<utility::event::HandPinchEvent>(50.0f, -25.0f));
		run(system);
		EXPECT_TRUE(interaction.isPinch());
		EXPECT_EQ(pinches, 1);

		run(system);
		EXPECT_FALSE(interaction.isPinch());
		EXPECT_EQ(pinches, 1);
	}

	// #185: a squeeze reaches the entity the hand points at, once.
	TEST_F(TestInputEvents, SqueezeReachesTheEntityTheHandPointsAt)
	{
		HandSqueeze system(_eventBus);
		const auto hit = addEntity<components::HandSqueezeInteraction>();
		const auto missed =
			addEntity<components::HandSqueezeInteraction>(1000.0f, 0.0f);
		auto &hitInteraction =
			_componentRegistry.getComponent<components::HandSqueezeInteraction>(
				hit);
		auto &missedInteraction =
			_componentRegistry.getComponent<components::HandSqueezeInteraction>(
				missed);
		int hitCalls	= 0;
		int missedCalls = 0;
		hitInteraction.setOnEventHandler([&hitCalls]() {
			++hitCalls;
		});
		missedInteraction.setOnEventHandler([&missedCalls]() {
			++missedCalls;
		});

		auto event =
			makeHandEvent<utility::event::HandSqueezeEvent>(50.0f, -25.0f);
		event->setValue(0.75f);
		event->setButtonPressed(true);
		_eventBus.publish(event);

		run(system);
		EXPECT_EQ(hitCalls, 1);
		EXPECT_FLOAT_EQ(hitInteraction.getValue(), 0.75f);
		EXPECT_TRUE(hitInteraction.isButtonPressed());
		EXPECT_EQ(missedCalls, 0);
		EXPECT_FLOAT_EQ(missedInteraction.getValue(), 0.0f);

		// The handler set on the component survives the copy of the event,
		// and the event is not applied again.
		run(system);
		EXPECT_EQ(hitCalls, 1);
	}

	// #185: a disabled entity receives nothing.
	TEST_F(TestInputEvents, SqueezeSkipsADisabledEntity)
	{
		HandSqueeze system(_eventBus);
		const auto entity = addEntity<components::HandSqueezeInteraction>();
		_componentRegistry.addComponent<components::StyleState>(entity)
			.setEnabled(false);
		int calls = 0;
		_componentRegistry
			.getComponent<components::HandSqueezeInteraction>(entity)
			.setOnEventHandler([&calls]() {
				++calls;
			});

		_eventBus.publish(
			makeHandEvent<utility::event::HandSqueezeEvent>(50.0f, -25.0f));
		run(system);

		EXPECT_EQ(calls, 0);
	}

	// #185
	TEST_F(TestInputEvents, TriggerReachesTheEntityTheHandPointsAt)
	{
		HandTrigger system(_eventBus);
		const auto entity = addEntity<components::HandTriggerInteraction>();
		auto &interaction =
			_componentRegistry.getComponent<components::HandTriggerInteraction>(
				entity);
		int calls = 0;
		interaction.setOnEventHandler([&calls]() {
			++calls;
		});

		auto event =
			makeHandEvent<utility::event::HandTriggerEvent>(50.0f, -25.0f);
		event->setValue(0.5f);
		_eventBus.publish(event);
		run(system);

		EXPECT_EQ(calls, 1);
		EXPECT_FLOAT_EQ(interaction.getValue(), 0.5f);
	}

	// #185
	TEST_F(TestInputEvents, ThumbRestReachesTheEntityTheHandPointsAt)
	{
		HandThumbRest system(_eventBus);
		const auto entity = addEntity<components::HandThumbRestInteraction>();
		auto &interaction =
			_componentRegistry
				.getComponent<components::HandThumbRestInteraction>(entity);
		int calls = 0;
		interaction.setOnEventHandler([&calls]() {
			++calls;
		});

		auto event =
			makeHandEvent<utility::event::HandThumbRestEvent>(50.0f, -25.0f);
		event->setTouched(true);
		_eventBus.publish(event);
		run(system);

		EXPECT_EQ(calls, 1);
		EXPECT_TRUE(interaction.isTouched());
	}

	// #185
	TEST_F(TestInputEvents, ThumbStickReachesTheEntityTheHandPointsAt)
	{
		HandThumbStick system(_eventBus);
		const auto entity = addEntity<components::HandThumbStickInteraction>();
		auto &interaction =
			_componentRegistry
				.getComponent<components::HandThumbStickInteraction>(entity);
		int calls = 0;
		interaction.setOnEventHandler([&calls]() {
			++calls;
		});

		auto event =
			makeHandEvent<utility::event::HandThumbStickEvent>(50.0f, -25.0f);
		event->setAxis(0.25f, -0.5f);
		_eventBus.publish(event);
		run(system);

		EXPECT_EQ(calls, 1);
		EXPECT_FLOAT_EQ(interaction.getX(), 0.25f);
		EXPECT_FLOAT_EQ(interaction.getY(), -0.5f);
	}

	// #180: Drag visits an entity that has only the mouse interaction.
	TEST_F(TestInputEvents, DragVisitsEntityWithMouseInteractionOnly)
	{
		Drag system(_eventBus);
		const auto entity = addEntity<components::DragInteraction,
									  components::MouseButtonInteraction>();
		auto &drag =
			_componentRegistry.getComponent<components::DragInteraction>(
				entity);
		utility::math::Vector2F moved({ 0.0f, 0.0f });
		drag.setOnDragHandler([&moved](const utility::math::Vector2F &delta) {
			moved = delta;
		});
		_componentRegistry
			.getComponent<components::MouseButtonInteraction>(entity)
			.setButtonPressed(utility::event::MouseButtonEvent::Button::Left,
							  true);

		auto first = std::make_shared<utility::event::MouseMotionEvent>();
		first->setPosition(utility::math::Vector2F({ 10.0f, 10.0f }));
		_eventBus.publish(first);
		run(system);
		EXPECT_TRUE(drag.isDragging());

		auto second = std::make_shared<utility::event::MouseMotionEvent>();
		second->setPosition(utility::math::Vector2F({ 25.0f, 14.0f }));
		_eventBus.publish(second);
		run(system);
		EXPECT_FLOAT_EQ(moved.x, 15.0f);
		EXPECT_FLOAT_EQ(moved.y, 4.0f);
	}

	// #183: a drag started with a hand button follows the hand, with no
	// mouse event at all.
	TEST_F(TestInputEvents, HandDragFollowsTheHand)
	{
		Drag system(_eventBus);
		const auto entity = addEntity<components::DragInteraction,
									  components::HandButtonInteraction>();
		auto &drag =
			_componentRegistry.getComponent<components::DragInteraction>(
				entity);
		int starts = 0;
		utility::math::Vector2F moved({ 0.0f, 0.0f });
		drag.setOnDragStartHandler([&starts]() {
			++starts;
		});
		drag.setOnDragHandler([&moved](const utility::math::Vector2F &delta) {
			moved = delta;
		});
		_componentRegistry
			.getComponent<components::HandButtonInteraction>(entity)
			.setButtonPressed(utility::event::HandButtonEvent::Button::A, true);

		_eventBus.publish(
			makeHandEvent<utility::event::HandMotionEvent>(10.0f, -10.0f));
		run(system);
		EXPECT_TRUE(drag.isDragging());
		EXPECT_EQ(starts, 1);

		_eventBus.publish(
			makeHandEvent<utility::event::HandMotionEvent>(25.0f, -14.0f));
		run(system);
		EXPECT_FLOAT_EQ(moved.x, 15.0f);
		EXPECT_FLOAT_EQ(moved.y, -4.0f);
	}

	// #183: the release ends the drag on its own frame, without waiting
	// for a motion event. #176: a motionless pointer is not a move.
	TEST_F(TestInputEvents, DragEndsOnReleaseWithoutMotion)
	{
		Drag system(_eventBus);
		const auto entity = addEntity<components::DragInteraction,
									  components::HandButtonInteraction>();
		auto &drag =
			_componentRegistry.getComponent<components::DragInteraction>(
				entity);
		int moves = 0;
		int ends  = 0;
		drag.setOnDragHandler([&moves](const utility::math::Vector2F &) {
			++moves;
		});
		drag.setOnDragEndHandler([&ends]() {
			++ends;
		});
		auto &button =
			_componentRegistry.getComponent<components::HandButtonInteraction>(
				entity);
		button.setButtonPressed(utility::event::HandButtonEvent::Button::A,
								true);

		_eventBus.publish(
			makeHandEvent<utility::event::HandMotionEvent>(10.0f, -10.0f));
		run(system);
		run(system);
		run(system);
		EXPECT_TRUE(drag.isDragging());
		EXPECT_EQ(moves, 0);

		button.setButtonPressed(utility::event::HandButtonEvent::Button::A,
								false);
		run(system);
		EXPECT_FALSE(drag.isDragging());
		EXPECT_EQ(ends, 1);
	}

	// #183: Drag must run after the systems that record the presses, or a
	// grab is seen one frame late.
	TEST_F(TestInputEvents, DragIsRegisteredAfterTheButtonSystems)
	{
		guillaume::tests::EngineMock engine;
		guillaume::tests::FakeSystemIO systemIo;
		ecs::SystemRegistry systemRegistry;
		registerCoreSystems(
			systemRegistry, _eventBus, engine,
			std::make_shared<utility::RessourceProvider>(systemIo));

		const auto &systems =
			systemRegistry.getSystemsByPhase(ecs::Phase::Event);
		std::ptrdiff_t drag		   = -1;
		std::ptrdiff_t mouseButton = -1;
		std::ptrdiff_t handButton  = -1;
		for (std::size_t index = 0; index < systems.size(); ++index) {
			const auto position = static_cast<std::ptrdiff_t>(index);
			if (dynamic_cast<Drag *>(systems[index].get())) {
				drag = position;
			} else if (dynamic_cast<MouseButton *>(systems[index].get())) {
				mouseButton = position;
			} else if (dynamic_cast<HandButton *>(systems[index].get())) {
				handButton = position;
			}
		}

		ASSERT_GE(drag, 0);
		ASSERT_GE(mouseButton, 0);
		ASSERT_GE(handButton, 0);
		EXPECT_GT(drag, mouseButton);
		EXPECT_GT(drag, handButton);
	}

	// #176: one wheel event scrolls once.
	TEST_F(TestInputEvents, WheelEventScrollsOnce)
	{
		Scroll system(_eventBus);
		const auto entity = addEntity<components::Scrollable>();
		auto &scrollable =
			_componentRegistry.getComponent<components::Scrollable>(entity);
		scrollable.setContentSize(utility::math::Vector2F({ 100.0f, 1000.0f }));

		auto wheel = std::make_shared<utility::event::MouseWheelEvent>();
		wheel->setOffset(utility::math::Vector2F({ 0.0f, 5.0f }));
		_eventBus.publish(wheel);

		run(system);
		run(system);
		run(system);

		EXPECT_FLOAT_EQ(scrollable.getContentOffset().y, 5.0f);
	}

	// #178 (hover part): of two scrollable entities that track hovering,
	// only the hovered one scrolls.
	TEST_F(TestInputEvents, OnlyTheHoveredScrollableScrolls)
	{
		Scroll system(_eventBus);
		const auto hovered = addEntity<components::Scrollable,
									   components::MouseHoverInteraction>();
		const auto other =
			addEntity<components::Scrollable,
					  components::MouseHoverInteraction>(1000.0f, 0.0f);
		for (const auto entity: { hovered, other }) {
			_componentRegistry.getComponent<components::Scrollable>(entity)
				.setContentSize(utility::math::Vector2F({ 100.0f, 1000.0f }));
		}
		_componentRegistry
			.getComponent<components::MouseHoverInteraction>(hovered)
			.setHovered(true);

		auto wheel = std::make_shared<utility::event::MouseWheelEvent>();
		wheel->setOffset(utility::math::Vector2F({ 0.0f, 5.0f }));
		_eventBus.publish(wheel);
		run(system);

		EXPECT_FLOAT_EQ(
			_componentRegistry.getComponent<components::Scrollable>(hovered)
				.getContentOffset()
				.y,
			5.0f);
		EXPECT_FLOAT_EQ(
			_componentRegistry.getComponent<components::Scrollable>(other)
				.getContentOffset()
				.y,
			0.0f);
	}
}	 // namespace guillaume::systems::tests
