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

#include "systems/test_value_interactions.hpp"

#include <utility/event/mouse_motion_event.hpp>
#include <utility/event/mouse_wheel_event.hpp>

#include "guillaume/components/bound.hpp"
#include "guillaume/components/drag_interaction.hpp"
#include "guillaume/components/mouse_button_interaction.hpp"
#include "guillaume/components/scrollable.hpp"
#include "guillaume/components/selection.hpp"

#include "guillaume/systems/selection.hpp"

namespace guillaume::systems::tests
{

	namespace
	{
		ecs::Entity::Identifier nextEntityId(void)
		{
			ecs::Entity entity;
			return entity.getIdentifier();
		}

		void publishMotion(event::EventBus &bus, float x, float y)
		{
			auto motion = std::make_shared<utility::event::MouseMotionEvent>();
			motion->setPosition(utility::math::Vector2F({ x, y }));
			bus.publish(motion);
		}

		void publishWheel(event::EventBus &bus, float x, float y)
		{
			auto wheel = std::make_shared<utility::event::MouseWheelEvent>();
			wheel->setOffset(utility::math::Vector2F({ x, y }));
			bus.publish(wheel);
		}
	}	 // namespace

	TEST_F(TestValueInteractions, DragStartsAndAccumulatesDelta)
	{
		const auto entity = nextEntityId();

		_componentRegistry.addComponent<components::DragInteraction>(entity);
		_componentRegistry.addComponent<components::MouseButtonInteraction>(
			entity);

		auto &drag =
			_componentRegistry.getComponent<components::DragInteraction>(
				entity);
		auto &mouse =
			_componentRegistry.getComponent<components::MouseButtonInteraction>(
				entity);

		bool started = false;
		bool dragged = false;
		float deltaX = 0.0f;
		float deltaY = 0.0f;

		drag.setOnDragStartHandler([&started]() {
			started = true;
		});
		drag.setOnDragHandler(
			[&dragged, &deltaX, &deltaY](const utility::math::Vector2F &delta) {
				dragged = true;
				deltaX	= delta.x;
				deltaY	= delta.y;
			});

		mouse.setButtonPressed(utility::event::MouseButtonEvent::Button::Left,
							   true);

		publishMotion(_eventBus, 10.0f, 0.0f);
		_dragSystem->prepare();
		_dragSystem->update(entity);

		EXPECT_TRUE(drag.isDragging());
		EXPECT_TRUE(started);

		publishMotion(_eventBus, 15.0f, 5.0f);
		_dragSystem->prepare();
		_dragSystem->update(entity);

		EXPECT_TRUE(dragged);
		EXPECT_FLOAT_EQ(deltaX, 5.0f);
		EXPECT_FLOAT_EQ(deltaY, 5.0f);
	}

	TEST_F(TestValueInteractions, DragEndsOnRelease)
	{
		const auto entity = nextEntityId();

		_componentRegistry.addComponent<components::DragInteraction>(entity);
		_componentRegistry.addComponent<components::MouseButtonInteraction>(
			entity);

		auto &drag =
			_componentRegistry.getComponent<components::DragInteraction>(
				entity);
		auto &mouse =
			_componentRegistry.getComponent<components::MouseButtonInteraction>(
				entity);

		bool ended = false;
		drag.setOnDragEndHandler([&ended]() {
			ended = true;
		});

		mouse.setButtonPressed(utility::event::MouseButtonEvent::Button::Left,
							   true);
		publishMotion(_eventBus, 10.0f, 0.0f);
		_dragSystem->prepare();
		_dragSystem->update(entity);
		EXPECT_TRUE(drag.isDragging());

		mouse.setButtonPressed(utility::event::MouseButtonEvent::Button::Left,
							   false);
		_dragSystem->prepare();
		_dragSystem->update(entity);

		EXPECT_FALSE(drag.isDragging());
		EXPECT_TRUE(ended);
	}

	TEST_F(TestValueInteractions, ScrollAdvancesAndClampsOffset)
	{
		const auto entity = nextEntityId();

		_componentRegistry.addComponent<components::Scrollable>(entity);
		_componentRegistry.addComponent<components::Bound>(entity);

		auto &bound =
			_componentRegistry.getComponent<components::Bound>(entity);
		bound.setWidth(100.0f).setHeight(100.0f);

		auto &scrollable =
			_componentRegistry.getComponent<components::Scrollable>(entity);
		scrollable.setContentSize(utility::math::Vector2F({ 200.0f, 300.0f }));

		publishWheel(_eventBus, 10.0f, 20.0f);
		_scrollSystem->prepare();
		_scrollSystem->update(entity);

		auto offset = scrollable.getContentOffset();
		EXPECT_FLOAT_EQ(offset.x, 10.0f);
		EXPECT_FLOAT_EQ(offset.y, 20.0f);

		publishWheel(_eventBus, 1000.0f, 1000.0f);
		_scrollSystem->prepare();
		_scrollSystem->update(entity);

		offset = scrollable.getContentOffset();
		EXPECT_FLOAT_EQ(offset.x, 100.0f);
		EXPECT_FLOAT_EQ(offset.y, 200.0f);
	}

	TEST_F(TestValueInteractions, ScrollClampsToZeroWhenContentFits)
	{
		const auto entity = nextEntityId();

		_componentRegistry.addComponent<components::Scrollable>(entity);
		_componentRegistry.addComponent<components::Bound>(entity);

		auto &bound =
			_componentRegistry.getComponent<components::Bound>(entity);
		bound.setWidth(200.0f).setHeight(200.0f);

		auto &scrollable =
			_componentRegistry.getComponent<components::Scrollable>(entity);
		scrollable.setContentSize(utility::math::Vector2F({ 50.0f, 50.0f }));

		publishWheel(_eventBus, 30.0f, 30.0f);
		_scrollSystem->prepare();
		_scrollSystem->update(entity);

		const auto offset = scrollable.getContentOffset();
		EXPECT_FLOAT_EQ(offset.x, 0.0f);
		EXPECT_FLOAT_EQ(offset.y, 0.0f);
	}

	TEST_F(TestValueInteractions, SelectionKeepsSingleSelectedChild)
	{
		const auto group	   = nextEntityId();
		const auto firstChild  = nextEntityId();
		const auto secondChild = nextEntityId();

		_componentRegistry.addComponent<components::SelectionGroup>(group);
		_componentRegistry.addComponent<components::Selectable>(firstChild);
		_componentRegistry.addComponent<components::Selectable>(secondChild);

		_componentRegistry.getComponent<components::Selectable>(firstChild)
			.setSelected(true);
		_componentRegistry.getComponent<components::Selectable>(secondChild)
			.setSelected(true);

		const auto selected = Selection::reconcile(_componentRegistry, group,
												   { firstChild, secondChild });

		EXPECT_EQ(selected, firstChild);
		EXPECT_TRUE(
			_componentRegistry.getComponent<components::Selectable>(firstChild)
				.isSelected());
		EXPECT_FALSE(
			_componentRegistry.getComponent<components::Selectable>(secondChild)
				.isSelected());
		EXPECT_EQ(
			_componentRegistry.getComponent<components::SelectionGroup>(group)
				.getSelected(),
			firstChild);
	}

	TEST_F(TestValueInteractions, SelectionClearsWhenEmptyAllowed)
	{
		const auto group = nextEntityId();
		const auto child = nextEntityId();

		_componentRegistry.addComponent<components::SelectionGroup>(group);
		_componentRegistry.addComponent<components::Selectable>(child);

		const auto selected =
			Selection::reconcile(_componentRegistry, group, { child });

		EXPECT_EQ(selected, ecs::Entity::InvalidIdentifier);
		EXPECT_FALSE(
			_componentRegistry.getComponent<components::SelectionGroup>(group)
				.hasSelection());
	}

	TEST_F(TestValueInteractions, SelectionSelectsFirstWhenEmptyDisallowed)
	{
		const auto group = nextEntityId();
		const auto child = nextEntityId();

		_componentRegistry.addComponent<components::SelectionGroup>(group)
			.setAllowEmpty(false);
		_componentRegistry.addComponent<components::Selectable>(child);

		const auto selected =
			Selection::reconcile(_componentRegistry, group, { child });

		EXPECT_EQ(selected, child);
		EXPECT_TRUE(
			_componentRegistry.getComponent<components::Selectable>(child)
				.isSelected());
	}

}	 // namespace guillaume::systems::tests
