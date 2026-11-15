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

#include <concepts>
#include <functional>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{
	/**
	 * @brief True when an event type declares a nested `Factory` type.
	 *
	 * Interaction uses it as the default value of its second template
	 * parameter, to pick the specialization that matches the event type.
	 *
	 * @tparam EventType The event type to inspect.
	 */
	template<typename EventType>
	concept HasFactoryType = requires { typename EventType::Factory; };

	/**
	 * @brief The part shared by every Interaction: one callback to run when
	 * the input the component stands for reaches the entity.
	 *
	 * User code (or a widget system) stores the handler; the input system of
	 * the matching event calls it during the Event phase.
	 *
	 * @tparam EventType The event the component reacts to. It only makes each
	 * interaction a distinct component type; the handler receives nothing.
	 * @see Interaction
	 */
	template<typename EventType> class InteractionBase: public ecs::Component
	{
		public:
		using EventHandler = std::function<void(
			void)>;	   ///< Callback run when the input reaches the entity.

		/**
		 * @brief Construct an interaction without handler.
		 */
		InteractionBase(void);

		/**
		 * @brief Store the callback to run when the input reaches the entity.
		 *
		 * Always marks the component changed.
		 *
		 * @param handler The callback, or `nullptr` to remove the current one.
		 */
		void setOnEventHandler(const EventHandler &handler);

		/**
		 * @brief Get the callback run when the input reaches the entity.
		 * @return A copy of the handler; empty (`nullptr`) when none was set.
		 */
		EventHandler getOnEventHandler(void) const;

		EventHandler _onEventHandler {
			nullptr
		};	  ///< Callback run when the input reaches the entity; empty by
			  ///< default.
	};

	/**
	 * @brief Component that makes an entity react to one kind of input event
	 * (a hand pinch, a hand poke, a trigger pull, ...).
	 *
	 * Each hand interaction component derives from it:
	 * HandPinchInteraction is `Interaction<utility::event::HandPinchEvent>`.
	 * The system of the same input (systems::HandPinch, ...) runs in the Event
	 * phase on the entities that have the component, a Transform and a Bound.
	 * systems::HandPinch and systems::HandPoke call the handler stored in
	 * InteractionBase; the other hand systems have no per-entity rule (see
	 * each derived component).
	 *
	 * @code
	 * auto &pinch =
	 * 	registry.addComponent<components::HandPinchInteraction>(identifier);
	 * pinch.setOnEventHandler([] { openMenu(); });
	 * @endcode
	 *
	 * The component also derives from `EventType`, so it has the fields of
	 * that event, but no system of the library writes them: read the state
	 * from the accessors of the derived component instead
	 * (HandPinchInteraction::isPinch, HandPokeInteraction::isPoke).
	 *
	 * @tparam EventType The event the component reacts to.
	 * @tparam HasFactory Whether `EventType` declares a nested `Factory`
	 * type. Deduced with HasFactoryType; leave it to its default.
	 * @see InteractionBase, HandPinchInteraction, HandPokeInteraction
	 */
	template<typename EventType, bool HasFactory = HasFactoryType<EventType>>
	class Interaction;

	/**
	 * @brief Interaction for an event type without nested `Factory` type.
	 *
	 * See the primary Interaction template for the role and the usage.
	 *
	 * @tparam EventType The event the component reacts to.
	 */
	template<typename EventType> class Interaction<EventType, false>:
		public EventType,
		public InteractionBase<EventType>
	{
		public:
		/**
		 * @brief Construct an interaction without handler, on a
		 * default-constructed event.
		 */
		Interaction(void);
	};

	/**
	 * @brief Interaction for an event type that declares a nested `Factory`
	 * type.
	 *
	 * It behaves like the other specialization; it only names the factory of
	 * the event in a private alias. See the primary Interaction template for
	 * the role and the usage.
	 *
	 * @tparam EventType The event the component reacts to.
	 */
	template<typename EventType> class Interaction<EventType, true>:
		public EventType,
		public InteractionBase<EventType>
	{
		public:
		/**
		 * @brief Construct an interaction without handler, on a
		 * default-constructed event.
		 */
		Interaction(void);

		private:
		using Factory =
			typename EventType::Factory;	///< Factory type declared by the
											///< event.
	};
}	 // namespace guillaume::components

#include "guillaume/components/interaction.tpp"
