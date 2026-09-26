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

#pragma once

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/event/event_bus.hpp"
#include "guillaume/event/event_manager.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/drag_interaction.hpp"
#include "guillaume/components/hand_button_interaction.hpp"
#include "guillaume/components/mouse_button_interaction.hpp"
#include "guillaume/components/transform.hpp"

#include <utility/event/mouse_motion_event.hpp>

namespace guillaume::systems
{

	/**
	 * @brief System handling continuous pointer/hand drag interactions.
	 *
	 * Consumes mouse motion events and, while the primary pointer button is
	 * held, accumulates the delta and drives the `components::DragInteraction`
	 * handlers. The entity is considered grabbed while the left mouse button
	 * (or the hand A button) is pressed.
	 *
	 * @see components::DragInteraction
	 */
	class Drag:
		public ecs::SystemFiller<components::DragInteraction,
								 components::Transform, components::Bound,
								 components::MouseButtonInteraction,
								 components::HandButtonInteraction>,
		public event::EventManager<utility::event::MouseMotionEvent>
	{
		public:
		/**
		 * @brief Construct the Drag system and subscribe to mouse motion
		 * events.
		 * @param eventBus The event bus to subscribe to.
		 */
		Drag(event::EventBus &eventBus);

		/**
		 * @brief Default destructor for the Drag system.
		 */
		~Drag(void);

		/**
		 * @brief Consume the pending mouse motion event before the pass.
		 */
		void prepare(void) override;

		/**
		 * @brief Update the Drag system for one entity.
		 * @param entityIdentifier The target entity identifier.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
