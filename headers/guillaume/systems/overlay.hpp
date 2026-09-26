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

#include <chrono>
#include <vector>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/event/event_bus.hpp"
#include "guillaume/event/event_manager.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/overlay.hpp"
#include "guillaume/components/transform.hpp"

#include <utility/engine.hpp>
#include <utility/event/keyboard_event.hpp>
#include <utility/math/vector.hpp>

namespace guillaume::systems
{

	/**
	 * @brief Stack-based overlay/modal manager.
	 *
	 * Tracks the visible `components::Overlay` entities as a stack, assigns
	 * modal input capture, and dismisses the top overlay on Escape, on an
	 * outside click, or when its auto-dismiss delay elapses.
	 *
	 * @see components::Overlay
	 */
	class Overlay:
		public ecs::SystemFiller<components::Overlay, components::Transform,
								 components::Bound>,
		public event::EventManager<utility::event::KeyboardEvent>
	{
		private:
		std::unique_ptr<utility::Engine> &_engine;		///< Engine instance.
		std::vector<ecs::Entity::Identifier> _stack;	///< Overlay stack.
		std::chrono::steady_clock::time_point
			_lastTime;	  ///< Last frame time for the fallback clock.

		public:
		/**
		 * @brief Construct an overlay system running in the Event phase.
		 * @param eventBus The event bus to subscribe to.
		 * @param engine The engine used to capture viewport input.
		 */
		Overlay(event::EventBus &eventBus,
				std::unique_ptr<utility::Engine> &engine);

		/**
		 * @brief Default destructor.
		 */
		~Overlay(void) override = default;

		/**
		 * @brief Consume input events and advance auto-dismiss timing.
		 */
		void prepare(void) override;

		/**
		 * @brief Synchronize the stack with an overlay entity visibility.
		 * @param entityIdentifier The target entity identifier.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		/**
		 * @brief Show an overlay and push it on top of the stack.
		 * @param entityIdentifier The overlay entity to show.
		 */
		void show(const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Dismiss the top overlay.
		 */
		void dismiss(void);

		/**
		 * @brief Dismiss a specific overlay wherever it is in the stack.
		 * @param entityIdentifier The overlay entity to dismiss.
		 */
		void dismiss(const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Check whether an overlay is in the stack.
		 * @param entityIdentifier The overlay entity to query.
		 * @return True when the overlay is open.
		 */
		bool isOpen(const ecs::Entity::Identifier &entityIdentifier) const;

		/**
		 * @brief Get the top overlay identifier.
		 * @return The top overlay, or InvalidIdentifier when empty.
		 */
		ecs::Entity::Identifier getTop(void) const;

		/**
		 * @brief Get the number of open overlays.
		 * @return The stack depth.
		 */
		std::size_t getDepth(void) const;

		/**
		 * @brief Dismiss the top overlay when it dismisses on Escape.
		 */
		void handleEscape(void);

		/**
		 * @brief Dismiss the top overlay when a click lands outside it.
		 * @param point The click position.
		 */
		void handleOutsideClick(const utility::math::Vector2F &point);

		/**
		 * @brief Advance the auto-dismiss timer of the top overlay.
		 * @param deltaTime The elapsed time, in seconds.
		 */
		void advance(float deltaTime);
	};

}	 // namespace guillaume::systems
