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

#include <optional>

#include <utility/engine.hpp>

#include "guillaume/ecs/system_filler.hpp"
#include "guillaume/event/event_bus.hpp"
#include "guillaume/event/event_manager.hpp"

#include <utility/event/keyboard_event.hpp>

#include <vector>

#include "guillaume/components/focus.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/mouse_button_interaction.hpp"
#include "guillaume/components/hand_button_interaction.hpp"
#include "guillaume/components/hand_pinch_interaction.hpp"
#include "guillaume/components/hand_poke_interaction.hpp"

namespace guillaume::systems
{

	/**
	 * @brief Decides which single entity has the keyboard focus.
	 *
	 * Runs in the Event phase on entities that have a components::Focus,
	 * Transform, Bound, MouseButtonInteraction, HandButtonInteraction,
	 * HandPinchInteraction and HandPokeInteraction (all seven are required;
	 * interaction::makeInteractive alone does not add the pinch and poke
	 * ones).
	 *
	 * Focus moves in two ways:
	 * - **Press**: an entity takes the focus while its left mouse button,
	 *   one of its hand buttons A, B, X or Y, a pinch or a poke is active on
	 *   it. This reads the interaction components written earlier in the
	 *   phase by systems::MouseButton and the hand systems.
	 * - **Tab / Shift+Tab**: after all entities have been visited, a Tab
	 *   key-down event moves the focus to the next (or previous) focusable
	 *   entity. See traverse() for the order.
	 *
	 * It writes the focused flag of components::Focus and of
	 * components::StyleState (when present), calls the focus-gained and
	 * focus-lost handlers of components::Focus, and tells the engine
	 * whether the viewport may capture input: not while a press gives the
	 * focus to an entity, again once no entity is focused. A disabled
	 * entity cannot take the focus and loses it when visited.
	 *
	 * @note The system remembers one focused entity across frames and
	 * scenes; it is not reset when the active scene changes.
	 * @note setFocus(), clearFocus() and traverse() read components through
	 * the registry bound to the system. Outside the frame loop, call
	 * ecs::System::bindComponentRegistry first; without it the flags and
	 * handlers are skipped and only the remembered identifier changes.
	 *
	 * @see components::Focus
	 * @see components::Accessibility
	 * @see systems::FocusRingRender
	 */
	class Focus:
		public ecs::SystemFiller<
			components::Focus, components::Transform, components::Bound,
			components::MouseButtonInteraction,
			components::HandButtonInteraction, components::HandPinchInteraction,
			components::HandPokeInteraction>,
		public event::EventManager<utility::event::KeyboardEvent>
	{
		private:
		utility::Engine &_engine;	 ///< Engine told whether the viewport
									 ///< may capture input
		std::optional<ecs::Entity::Identifier>
			_focusedEntity;	   ///< Entity holding the focus, nullopt if none
		std::optional<ecs::Entity::Identifier>
			_lastFocusedEntity;	   ///< Entity that held the focus before the
								   ///< current one, nullopt if none yet
		std::vector<std::pair<ecs::Entity::Identifier, int>>
			_focusables;	///< Focusable entities met this frame, in visit
							///< order, each with its accessibility focus
							///< order (0 when it has none)

		/**
		 * @brief Whether Tab traversal may stop on an entity.
		 * @param entityIdentifier The entity to test.
		 * @return True when the entity has a components::Focus, is enabled,
		 * and its components::Accessibility (if any) does not mark it as
		 * not focusable.
		 */
		bool isFocusable(const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Write the focused flag of an entity in its
		 * components::Focus and components::StyleState, whichever it has.
		 * @param entityIdentifier The entity gaining or losing the focus.
		 * @param focused True when it gains the focus.
		 */
		void markFocused(const ecs::Entity::Identifier &entityIdentifier,
						 bool focused);

		public:
		/**
		 * @brief Create the system, in the Event phase, and subscribe it to
		 * keyboard events (for Tab traversal).
		 * @param eventBus The event bus delivering the keyboard events.
		 * @param engine The engine told whether the viewport may capture
		 * input. It must outlive the system.
		 */
		Focus(event::EventBus &eventBus, utility::Engine &engine);

		/**
		 * @brief Give the focus to an entity.
		 *
		 * When another entity held the focus, it becomes the last focused
		 * entity, its focus-lost handler is called and its focused flags are
		 * cleared. The focused flags of the new entity are then set and its
		 * focus-gained handler is called; that handler is called on every
		 * call, also when the entity already held the focus.
		 * @param entityIdentifier The entity to focus. It is not checked
		 * for being focusable or enabled.
		 */
		void setFocus(const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Take the focus away from the focused entity.
		 *
		 * The entity becomes the last focused entity, its focus-lost handler
		 * is called and its focused flags are cleared. Does nothing when no
		 * entity is focused.
		 */
		void clearFocus(void);

		/**
		 * @brief The entity holding the focus.
		 * @return Its identifier, or `std::nullopt` when no entity is
		 * focused.
		 */
		std::optional<ecs::Entity::Identifier> getFocusedEntity(void) const;

		/**
		 * @brief The entity that held the focus before the current one, or
		 * before the focus was cleared.
		 * @return Its identifier, or `std::nullopt` when the focus has never
		 * left an entity.
		 */
		std::optional<ecs::Entity::Identifier> getLastFocusedEntity(void) const;

		/**
		 * @brief Move the focus to the next or previous candidate.
		 *
		 * Candidates with a positive focus order come first, in ascending
		 * order; the others follow in the order they are given. From the
		 * focused entity the focus moves one step and wraps at the ends.
		 * When no entity is focused, or the focused one is not a candidate,
		 * the first candidate is focused (the last one when going backward).
		 * Does nothing when @p candidates is empty.
		 *
		 * @param candidates The focusable entities, each paired with the
		 * focus order of its components::Accessibility (0 or less: no
		 * explicit order).
		 * @param backward True to go to the previous one (Shift+Tab).
		 */
		void traverse(const std::vector<std::pair<ecs::Entity::Identifier, int>>
						  &candidates,
					  bool backward);

		/**
		 * @brief Empty the list of focusable entities and take the next
		 * pending keyboard event, once per frame.
		 */
		void prepare(void) override;

		/**
		 * @brief Run Tab traversal once every entity has been visited.
		 *
		 * When the current keyboard event is a Tab key-down, calls
		 * traverse() on the focusable entities collected by update(),
		 * backward when a Shift modifier is set.
		 */
		void cleanup(void) override;

		/**
		 * @brief Focus one entity if it is being pressed, and record it as
		 * a Tab candidate if it is focusable.
		 *
		 * A disabled entity is skipped, after losing the focus if it held
		 * it. A pressed entity gets the focus and viewport input capture is
		 * turned off; otherwise capture is turned back on when no entity is
		 * focused.
		 * @param entityIdentifier The entity to examine.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
