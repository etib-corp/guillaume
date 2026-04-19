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

#include <utility/event/hand_button_event.hpp>
#include <utility/event/hand_motion_event.hpp>
#include <utility/event/event.hpp>
#include <utility/event/hand_pinch_event.hpp>
#include <utility/event/hand_poke_event.hpp>
#include <utility/event/mouse_button_event.hpp>
#include <utility/event/mouse_motion_event.hpp>

namespace guillaume::systems
{

	/**
	 * @brief System handling pointer interactions (hover and click).
	 * @see components::Interaction
	 */
	class Interaction:
		public ecs::SystemFiller<components::Interaction, components::Transform,
								 components::Bound>
	{
		private:
		event::EventSubscriber<utility::event::MouseButtonEvent>
			_mouseButtonSubscriber;	   ///< Subscriber for mouse button events
		std::unique_ptr<utility::event::MouseButtonEvent>
			_lastMouseButtonEvent;	  ///< Last received mouse button event for
									  ///< click processing

		event::EventSubscriber<utility::event::MouseMotionEvent>
			_mouseMotionSubscriber;	   ///< Subscriber for mouse motion events
		std::unique_ptr<utility::event::MouseMotionEvent>
			_lastMouseMotionEvent;	  ///< Last received mouse motion event for
									  ///< hover processing

		event::EventSubscriber<utility::event::HandButtonEvent>
			_handButtonSubscriber;	///< Subscriber for XR hand
											///< button events
		std::unique_ptr<utility::event::HandButtonEvent>
			_lastHandButtonEvent;	   ///< Last received hand button
										   ///< event for click processing

		event::EventSubscriber<utility::event::HandMotionEvent>
			_handMotionSubscriber;	///< Subscriber for XR hand
											///< motion events
		std::unique_ptr<utility::event::HandMotionEvent>
			_lastHandMotionEvent;	   ///< Last received hand motion
										   ///< event for hover processing

		event::EventSubscriber<utility::event::HandPinchEvent>
			_handPinchSubscriber;	 ///< Subscriber for XR hand pinch events
		std::unique_ptr<utility::event::HandPinchEvent>
			_lastHandPinchEvent;	///< Last received hand pinch event for
									///< click processing

		event::EventSubscriber<utility::event::HandPokeEvent>
			_handPokeSubscriber;	///< Subscriber for XR hand poke events
		std::unique_ptr<utility::event::HandPokeEvent>
			_lastHandPokeEvent;	   ///< Last received hand poke event for click
								   ///< processing

		Renderer &_renderer;	///< Reference to the renderer for view and
								///< viewport information

		/**
		 * @brief Update the last input events for click and hover processing.
		 * This method should be called at the beginning of each update cycle
		 * to ensure that the latest input events are used for interaction
		 * processing.
		 *
		 */
		void updateLastInputEvents(void);

		/**
		 * @brief Update the mouse hovered state for the specified entity.
		 * @param entityIdentifier The identifier of the entity to update.
		 * @param isInside Whether the pointer is currently inside the entity
		 * bounds.
		 */
		void processMouseHover(const ecs::Entity::Identifier &entityIdentifier,
							   bool isInside);

		/**
		 * @brief Update the mouse clicked state for the specified entity based
		 * on the latest input events and whether the pointer is inside the
		 * entity bounds.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void processMouseButtonClick(
			const ecs::Entity::Identifier &entityIdentifier, bool isInside);

		/**
		 * @brief Update the hand hovered state for the specified entity.
		 * @param entityIdentifier The identifier of the entity to update.
		 * @param isInside Whether the pointer is currently inside the entity
		 * bounds.
		 */
		void processHandHover(
			const ecs::Entity::Identifier &entityIdentifier, bool isInside);

		/**
		 * @brief Update the hand clicked state for the specified entity
		 * based on the latest hand button events and whether the pointer
		 * is inside the entity bounds.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void processHandButtonClick(
			const ecs::Entity::Identifier &entityIdentifier, bool isInside);

		/**
		 * @brief Update the state for the specified entity based on the
		 * latest hand pinch events and whether the pointer is inside the entity
		 * bounds.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void processHandPinch(const ecs::Entity::Identifier &entityIdentifier,
							  bool isInside);

		/**
		 * @brief Update the hand poke state for the specified entity based on
		 * the latest hand poke events and whether the pointer is inside the
		 * entity bounds.
		 * @param entityIdentifier The identifier of the entity to update.
		 * @param isInside Whether the pointer is currently inside the entity
		 * bounds.
		 */
		void processHandPoke(const ecs::Entity::Identifier &entityIdentifier,
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