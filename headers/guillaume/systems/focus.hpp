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

#include "guillaume/ecs/system_filler.hpp"
#include "guillaume/event/event_bus.hpp"

#include "guillaume/components/focus.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/mouse_button_interaction.hpp"
#include "guillaume/components/hand_button_interaction.hpp"
#include "guillaume/components/hand_pinch_interaction.hpp"
#include "guillaume/components/hand_poke_interaction.hpp"

#include "guillaume/engine.hpp"

namespace guillaume::systems
{

	/**
	 * @brief System managing focus state of UI entities.
	 *
	 * The Focus system tracks which entity currently has focus and manages
	 * focus transitions between entities. It triggers focus handlers when
	 * focus is gained or lost based on mouse or hand click interactions.
	 *
	 * @see components::Focus
	 */
	class Focus:
		public ecs::SystemFiller<
			components::Focus, components::Transform, components::Bound,
			components::MouseButtonInteraction,
			components::HandButtonInteraction, components::HandPinchInteraction,
			components::HandPokeInteraction>
	{
		private:
		std::unique_ptr<Engine> &_engine;	 ///< Engine instance
		std::optional<ecs::Entity::Identifier>
			_focusedEntity;	   ///< Currently focused entity identifier
		std::optional<ecs::Entity::Identifier>
			_lastFocusedEntity;	   ///< Last entity that had focus

		public:
		/**
		 * @brief Default constructor for the Focus system.
		 * @param eventBus The event bus for focus events.
		 * @param engine Pointer to the engine for viewport input control.
		 */
		Focus(event::EventBus &eventBus, std::unique_ptr<Engine> &engine);

		/**
		 * @brief Set focus to a specific entity.
		 * @param entityIdentifier The entity to focus.
		 */
		void setFocus(const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Clear the current focus.
		 */
		void clearFocus(void);

		/**
		 * @brief Get the currently focused entity.
		 * @return The focused entity identifier, or nullopt if no entity has
		 * focus.
		 */
		std::optional<ecs::Entity::Identifier> getFocusedEntity(void) const;

		/**
		 * @brief Get the last entity that had focus.
		 * @return The last focused entity identifier, or nullopt if no entity
		 * has had focus yet.
		 */
		std::optional<ecs::Entity::Identifier> getLastFocusedEntity(void) const;

		/**
		 * @brief Update the Focus system for the specified entity.
		 * @param entityIdentifier The identifier of the entity to update.
		 *
		 * Checks if the entity was clicked with mouse or hand and sets focus
		 * accordingly.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
