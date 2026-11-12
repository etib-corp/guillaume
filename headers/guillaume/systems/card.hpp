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

#include "guillaume/components/card.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Styles a card, runs its press elevation and arranges its
	 * composed sections.
	 *
	 * Reads components::Card, the interaction state and the active theme,
	 * and writes the card's Color, Borders, Elevation, Layout and Bound, plus
	 * the content of its cover, title, subtitle and content children and the
	 * arrangement of its title and actions sections. Runs in the Layout
	 * phase, after the children have been measured.
	 */
	class Card: public ecs::SystemFiller<components::Card>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		Card(void);

		/**
		 * @brief Default destructor.
		 */
		~Card(void) override = default;

		/**
		 * @brief Make an entity behave as a card.
		 *
		 * Configures the press-elevation motion of its
		 * components::Animation and wires its samples to this system. Call
		 * once, after the entity has its components::Card, Animation,
		 * Elevation, Borders, Color, Layout, Transform and Bound.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The card entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one card and arrange its composed sections.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The card entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Set the press handler and make the card pressable.
		 *
		 * The first call adds the interaction components (hover and press
		 * state layers, focus, Button role) and wires them to press() and
		 * interactionChanged(); an empty handler keeps the card interactive.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The card entity.
		 * @param handler The press handler.
		 */
		static void setOnPress(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   components::Card::Handler handler);

		/**
		 * @brief Activate as the user would: ignored when disabled,
		 * otherwise the press handler is called.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The card entity.
		 */
		static void press(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief React to a hover or press change: an elevated card starts
		 * its elevation motion when its pressed state flips, and the card is
		 * restyled.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The card entity.
		 */
		static void
			interactionChanged(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Enable or disable the card and restyle; disabling a pressed
		 * card drops it back to rest.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The card entity.
		 * @param disabled True to disable.
		 */
		static void setDisabled(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								bool disabled);

		/**
		 * @brief Whether the card ignores input.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The card entity.
		 * @return True for a disabled entity.
		 */
		static bool isDisabled(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the width available to a composed section.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The card entity.
		 * @return The fixed width of the card minus its padding, or 0 when
		 * the card has no fixed width.
		 */
		static float
			contentWidth(const ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style the card with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
