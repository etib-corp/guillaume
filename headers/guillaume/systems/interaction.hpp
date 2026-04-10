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

#include "guillaume/components/bound.hpp"
#include "guillaume/components/interaction.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/event/event_subscriber.hpp"
#include "guillaume/renderer.hpp"

#include <memory>
#include <unordered_set>

#include <utility/event/event.hpp>
#include <utility/event/mouse_button_event.hpp>
#include <utility/event/mouse_motion_event.hpp>

namespace guillaume::systems
{

	/**
	 * @brief System handling pointer interactions (hover and click).
	 * @see components::Interaction
	 */
	class Interaction:
		public ecs::SystemFiller<components::Transform, components::Bound>
	{
		private:
		event::EventSubscriber<utility::event::MouseButtonEvent>
			_mouseButtonSubscriber;	   ///< Subscriber for mouse button events
		event::EventSubscriber<utility::event::MouseMotionEvent>
			_mouseMotionSubscriber;	   ///< Subscriber for mouse motion events
		Renderer &_renderer;	///< Reference to the renderer for view and
								///< viewport information

		std::unique_ptr<utility::event::MouseButtonEvent>
			_pendingClickEvent;	   ///< Pending mouse button event to process in
								   ///< the next update
		std::unordered_set<ecs::Entity::Identifier>
			_evaluatedClickEntities;	///< Set of entities evaluated for click
										///< interactions in the current update
		utility::event::MouseButtonEvent::MouseButtonsState _buttonStates {
			0
		};	  ///< Last known state of mouse buttons

		std::unique_ptr<utility::event::MouseMotionEvent>
			_pendingMotionEvent;	///< Pending mouse motion event to process
									///< in the next update
		std::unordered_set<ecs::Entity::Identifier>
			_evaluatedMotionEntities;	 ///< Set of entities evaluated for
										 ///< motion interactions in the current
										 ///< update
		utility::event::MouseMotionEvent::MousePosition _lastMousePosition {
			0.0f, 0.0f
		};	  ///< Last known mouse position in screen coordinates
		bool _hasMousePosition {
			false
		};	  ///< Flag indicating if a valid mouse position has been received

		/**
		 * @brief Update the mouse motion state for the specified entity.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void updateMouseMotionState(
			const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Update the hovered state for the specified entity.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void processHover(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier,
						  bool isInside);

		/**
		 * @brief Update the clicked state for the specified entity.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void processClick(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier,
						  bool isInside);

		public:
		/**
		 * @brief Default constructor for the Interaction system.
		 * @param eventBus The event bus to subscribe to.
		 * @param renderer The renderer instance for view and viewport
		 * information.
		 */
		Interaction(event::EventBus &eventBus, Renderer &renderer);

		/**
		 * @brief Default destructor for the Interaction system.
		 */
		~Interaction(void) = default;

		/**
		 * @brief Update the Interaction system for the specified entity.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems