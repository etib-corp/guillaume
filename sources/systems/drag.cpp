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

#include "guillaume/systems/drag.hpp"

namespace guillaume::systems
{
	Drag::Drag(event::EventBus &eventBus)
		: ecs::SystemFiller<components::DragInteraction, components::Transform,
							components::Bound,
							components::MouseButtonInteraction,
							components::HandButtonInteraction>(
			  ecs::Phase::Event)
		, event::EventManager<utility::event::MouseMotionEvent>(eventBus)
	{
	}

	Drag::~Drag(void)
	{
	}

	void Drag::prepare(void)
	{
		consumeNextEvent();
	}

	void Drag::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto motionEvent = getLastEvent();
		if (!motionEvent) {
			return;
		}

		auto &drag =
			getComponent<components::DragInteraction>(entityIdentifier);

		bool pressed = false;

		if (hasComponent<components::MouseButtonInteraction>(
				entityIdentifier)) {
			pressed = getComponent<components::MouseButtonInteraction>(
						  entityIdentifier)
						  .isButtonPressed(
							  utility::event::MouseButtonEvent::Button::Left);
		}

		if (!pressed
			&& hasComponent<components::HandButtonInteraction>(
				entityIdentifier)) {
			pressed = getComponent<components::HandButtonInteraction>(
						  entityIdentifier)
						  .isButtonPressed(
							  utility::event::HandButtonEvent::Button::A);
		}

		const auto position = motionEvent->getPosition();

		if (pressed) {
			if (!drag.isDragging()) {
				drag.setLastPosition(position);
				drag.setDragging(true);
				if (const auto handler = drag.getOnDragStartHandler()) {
					handler();
				}
				return;
			}

			const auto delta = position - drag.getLastPosition();
			drag.setLastPosition(position);
			drag.setDelta(delta);
			if (const auto handler = drag.getOnDragHandler()) {
				handler(delta);
			}
			return;
		}

		if (drag.isDragging()) {
			drag.setDragging(false);
			if (const auto handler = drag.getOnDragEndHandler()) {
				handler();
			}
		}
	}

}	 // namespace guillaume::systems
