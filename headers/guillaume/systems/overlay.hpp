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
	 * @brief Keeps the stack of open overlays (dialogs, menus, snackbars)
	 * and closes the top one on Escape or when its delay runs out.
	 *
	 * Runs in the Event phase on entities that have a components::Overlay,
	 * a Transform and a Bound. An overlay is *open* while it is in the
	 * stack; the last one opened is the *top*. Each frame the system:
	 * - pushes an overlay whose component became visible, and dismisses one
	 *   whose component became hidden, so writing
	 *   components::Overlay::setVisible is enough to open or close one;
	 * - adds the frame time to the elapsed time of the top overlay and
	 *   dismisses it once its auto-dismiss delay is reached (only the top
	 *   overlay counts down);
	 * - dismisses the top overlay on an Escape key-down event, if it
	 *   dismisses on Escape.
	 *
	 * Dismissing writes `visible = false` and `elapsed = 0` on the
	 * component and calls its dismiss handler; widget systems such as
	 * systems::Snackbar rely on that handler to learn about a timeout.
	 * While at least one overlay is open, the engine is told not to
	 * capture viewport input; capture is given back when the stack empties.
	 *
	 * @note Outside clicks are not detected by the system itself: the code
	 * that owns the pointer position calls handleOutsideClick().
	 * @note The modal flag of components::Overlay is not read here.
	 * @note show(), dismiss(), handleEscape(), handleOutsideClick() and
	 * advance() read components through the registry bound to the system.
	 * Outside the frame loop, call ecs::System::bindComponentRegistry
	 * first; without it only the stack changes (the component is not
	 * written and the dismiss handler is not called).
	 *
	 * @see components::Overlay
	 * @see systems::Snackbar
	 */
	class Overlay:
		public ecs::SystemFiller<components::Overlay, components::Transform,
								 components::Bound>,
		public event::EventManager<utility::event::KeyboardEvent>
	{
		private:
		utility::Engine &_engine;	 ///< Engine told whether the viewport
									 ///< may capture input.
		std::vector<ecs::Entity::Identifier>
			_stack;	   ///< Open overlays, oldest first; the top is the back.
		std::chrono::steady_clock::time_point
			_lastTime;	  ///< When prepare() last ran, to measure the frame
						  ///< time for auto-dismiss.

		public:
		/**
		 * @brief Create the system, in the Event phase, and subscribe it to
		 * keyboard events (for Escape).
		 * @param eventBus The event bus delivering the keyboard events.
		 * @param engine The engine told whether the viewport may capture
		 * input. It must outlive the system.
		 */
		Overlay(event::EventBus &eventBus, utility::Engine &engine);

		/**
		 * @brief Default destructor.
		 */
		~Overlay(void) override = default;

		/**
		 * @brief Once per frame: take the next pending keyboard event, count
		 * down the top overlay, and handle Escape.
		 *
		 * The time passed to advance() is measured on a steady clock since
		 * the previous call. Escape is recognized by its scan code, on a
		 * key-down event.
		 */
		void prepare(void) override;

		/**
		 * @brief Make the stack agree with the visibility of one overlay.
		 *
		 * A visible overlay that is not open is pushed on top and viewport
		 * input capture is turned off; a hidden overlay that is open is
		 * dismissed (its dismiss handler is called).
		 * @param entityIdentifier The entity holding the
		 * components::Overlay.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		/**
		 * @brief Open an overlay at once: push it on top of the stack, make
		 * its component visible and turn viewport input capture off.
		 *
		 * Does nothing when the overlay is already open.
		 * @param entityIdentifier The overlay entity to open.
		 */
		void show(const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Close the top overlay.
		 *
		 * Removes it from the stack, hides its component, resets its
		 * elapsed time to 0 and calls its dismiss handler. Viewport input
		 * capture is turned back on when the stack becomes empty. Does
		 * nothing when no overlay is open.
		 */
		void dismiss(void);

		/**
		 * @brief Close one overlay, wherever it is in the stack.
		 *
		 * Same effects as dismiss(void), on that overlay. Does nothing when
		 * it is not open.
		 * @param entityIdentifier The overlay entity to close.
		 */
		void dismiss(const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether an overlay is open, that is, in the stack.
		 * @param entityIdentifier The overlay entity to look for.
		 * @return True when the overlay is in the stack.
		 */
		bool isOpen(const ecs::Entity::Identifier &entityIdentifier) const;

		/**
		 * @brief The overlay opened last, the one Escape and the countdown
		 * apply to.
		 * @return Its identifier, or `ecs::Entity::InvalidIdentifier` when
		 * no overlay is open.
		 */
		ecs::Entity::Identifier getTop(void) const;

		/**
		 * @brief The number of open overlays.
		 * @return The size of the stack, 0 when none is open.
		 */
		std::size_t getDepth(void) const;

		/**
		 * @brief Close the top overlay if its component says it dismisses
		 * on Escape. Called by prepare() on an Escape key-down.
		 */
		void handleEscape(void);

		/**
		 * @brief Close the top overlay if a click landed outside it and its
		 * component says it dismisses on outside clicks.
		 *
		 * "Inside" is the axis-aligned rectangle starting at the x and y of
		 * the overlay's Transform position, with the size of its Bound.
		 * The system does not call this itself.
		 * @param point The click position, in the coordinate space of the
		 * overlay's Transform position.
		 */
		void handleOutsideClick(const utility::math::Vector2F &point);

		/**
		 * @brief Count down the top overlay and close it when its time is
		 * up.
		 *
		 * Adds @p deltaTime to the elapsed time of the top overlay when it
		 * has an auto-dismiss delay, and dismisses it once the elapsed time
		 * reaches the delay. Called by prepare() each frame.
		 * @param deltaTime The time to add, in seconds.
		 */
		void advance(float deltaTime);
	};

}	 // namespace guillaume::systems
