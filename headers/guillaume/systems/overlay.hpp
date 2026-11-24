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
#include <utility/event/mouse_button_event.hpp>
#include <utility/graphic/ray.hpp>
#include <utility/math/vector.hpp>

namespace guillaume::systems
{

	/**
	 * @brief Keeps the stack of open overlays (dialogs, menus, snackbars)
	 * and closes them on Escape, on an outside click or when their delay
	 * runs out.
	 *
	 * Runs in the Event phase on entities that have a components::Overlay,
	 * a Transform and a Bound. An overlay is *open* while it is in the
	 * stack; the last one opened is the *top*. Each frame the system:
	 * - pushes an overlay whose component became visible, and removes one
	 *   whose component became hidden, so writing
	 *   components::Overlay::setVisible is enough to open or close one;
	 * - adds the frame time to the elapsed time of every open overlay that
	 *   has an auto-dismiss delay and dismisses it once the delay is
	 *   reached, wherever it is in the stack;
	 * - dismisses the top overlay on an Escape key-down event, if it
	 *   dismisses on Escape;
	 * - dismisses the top overlay on a mouse button press that lands
	 *   outside it, if it dismisses on outside clicks.
	 *
	 * Dismissing writes `visible = false` and `elapsed = 0` on the
	 * component and calls its dismiss handler; widget systems such as
	 * systems::Snackbar rely on that handler to learn about a timeout.
	 * An overlay hidden by its owner (its component written invisible) is
	 * only removed from the stack: the handler is not called, because the
	 * owner already knows.
	 * While at least one overlay is open, the engine is told not to
	 * capture viewport input; capture is given back when the stack empties.
	 *
	 * The stack holds entity identifiers of one scene. When the component
	 * registry bound to the system changes (the active scene changed), the
	 * stack is emptied without calling any handler; the visible overlays of
	 * the new scene are pushed again as they are visited.
	 *
	 * @note A mouse button press is tested once, against the overlay that
	 * is on top when prepare() runs. systems::MouseButton runs earlier in
	 * the phase and an overlay is pushed after prepare(), so the press that
	 * opens an overlay is never tested against that overlay.
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
		public event::EventManager<utility::event::KeyboardEvent>,
		public event::EventManager<utility::event::MouseButtonEvent>
	{
		private:
		utility::Engine &_engine;	 ///< Engine told whether the viewport
									 ///< may capture input.
		std::vector<ecs::Entity::Identifier>
			_stack;	   ///< Open overlays, oldest first; the top is the back.
		std::chrono::steady_clock::time_point
			_lastTime;	  ///< When prepare() last ran, to measure the frame
						  ///< time for auto-dismiss.
		std::shared_ptr<utility::event::MouseButtonEvent>
			_lastClick;	   ///< Mouse button event already tested for an
						   ///< outside click, so it is applied once.
		const ecs::ComponentRegistry *_lastRegistry {
			nullptr
		};	  ///< Registry the stack belongs to; nullptr before the first
			  ///< prepare().

		/**
		 * @brief Take an overlay out of the stack without reporting it.
		 *
		 * Resets its elapsed time and gives viewport input capture back
		 * when the stack becomes empty. The visibility is not written and
		 * the dismiss handler is not called.
		 * @param entityIdentifier The overlay entity to remove.
		 * @return True when the overlay was in the stack.
		 */
		bool remove(const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Empty the stack when the bound component registry is not
		 * the one of the previous frame, that is, when the scene changed.
		 */
		void resetOnRegistryChange(void);

		/**
		 * @brief Take the next pending mouse button event and, when it is a
		 * new button press, test it against the top overlay.
		 */
		void handleMouseButton(void);

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
		 * @brief Once per frame: forget the stack if the scene changed, take
		 * the next pending keyboard and mouse button events, count down the
		 * overlays, and handle Escape and outside clicks.
		 *
		 * The time passed to advance() is measured on a steady clock since
		 * the previous call. Escape is recognized by its scan code, on a
		 * key-down event. A mouse button press is turned into a ray with
		 * the engine view and passed to handleOutsideClick().
		 *
		 * A component registry must be bound (the frame loop binds it).
		 */
		void prepare(void) override;

		/**
		 * @brief Make the stack agree with the visibility of one overlay.
		 *
		 * A visible overlay that is not open is pushed on top and viewport
		 * input capture is turned off; a hidden overlay that is open is
		 * removed from the stack and its elapsed time reset. Its dismiss
		 * handler is not called: whoever hid it reports the dismissal
		 * itself when there is one to report.
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
		 * @brief The overlay opened last, the one Escape and outside clicks
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
		 * The orientation and depth of the overlay are ignored; prepare()
		 * uses the ray overload instead.
		 * @param point The click position, in the coordinate space of the
		 * overlay's Transform position.
		 */
		void handleOutsideClick(const utility::math::Vector2F &point);

		/**
		 * @brief Close the top overlay if a pointer ray misses it and its
		 * component says it dismisses on outside clicks.
		 *
		 * The ray is tested against the rectangle of the overlay with
		 * systems::hitTest, like the input systems do. Called by prepare()
		 * on a mouse button press.
		 * @param ray The pointer ray, in world space.
		 */
		void handleOutsideClick(const utility::graphic::RayF &ray);

		/**
		 * @brief Count down the open overlays and close those whose time is
		 * up.
		 *
		 * Adds @p deltaTime to the elapsed time of every open overlay that
		 * has an auto-dismiss delay, wherever it is in the stack, and
		 * dismisses each one whose elapsed time reaches its delay. Called
		 * by prepare() each frame.
		 * @param deltaTime The time to add, in seconds.
		 */
		void advance(float deltaTime);
	};

}	 // namespace guillaume::systems
