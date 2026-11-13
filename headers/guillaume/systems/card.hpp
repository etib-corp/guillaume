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
	 * @brief Styles a card, animates its elevation while pressed and
	 * arranges its cover, title, content and actions sections.
	 *
	 * Runs in the Layout phase on every entity that has a
	 * components::Card. It is registered after systems::Layout, so the
	 * children are already measured when it arranges them.
	 * - **Reads**: components::Card (variant, texts, cover path, press
	 *   state, child identifiers), the hover and press state of the card
	 *   and the active theme.
	 * - **Writes** on the card: components::Color, components::Borders,
	 *   components::Elevation and the child list of components::Layout,
	 *   then the Transform and Bound of the sections through
	 *   systems::Layout::apply.
	 * - **Writes** on the children: the texture path of the cover Image,
	 *   the Text of the title, subtitle and content, the Accessibility
	 *   label of the title section, and the Layout of the title and
	 *   actions sections.
	 *
	 * A card is static until setOnPress() is called, which makes it
	 * focusable and pressable. An elevated card then rises from elevation
	 * level 1 to level 2 while it is held down; the motion is driven by the
	 * card's components::Animation, wired by install().
	 *
	 * A card whose components::Card records no section keeps the child list
	 * its components::Layout already has.
	 *
	 * @see components::Card, entities::Card, systems::Layout
	 */
	class Card: public ecs::SystemFiller<components::Card>
	{
		public:
		/**
		 * @brief Create the system in the Layout phase.
		 */
		Card(void);

		/**
		 * @brief Default destructor.
		 */
		~Card(void) override = default;

		/**
		 * @brief Prepare the press-elevation motion of a card.
		 *
		 * Sets the duration and easing of the entity's
		 * components::Animation from the theme and makes each of its
		 * samples move the press progress of components::Card towards its
		 * target (1 while pressed, 0 at rest) and call apply(). Call it
		 * once, after the entity has its components::Card, Animation,
		 * Elevation, Borders, Color, Layout, Transform and Bound.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the sample handler and must outlive the entity.
		 * @param entityIdentifier The card entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity has
		 * no components::Animation.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one card and arrange its sections.
		 *
		 * This is what update() runs each frame. Call it directly after
		 * changing components::Card to see the result before the next
		 * frame, which is what the setters of entities::Card do.
		 *
		 * The container color and stroke depend on the variant: elevated
		 * (tinted surface with a shadow), filled (surfaceVariant) or
		 * outlined (surface with a 1 dp outline). An interactive card also
		 * blends its hover or press state layer into the container color.
		 * The sections are laid out in the order cover, title, content,
		 * actions, skipping the ones that do not exist.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The card entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::Card, Layout, Color, Borders or Elevation.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Set the press handler and make the card pressable.
		 *
		 * The first call adds the interaction components (hover and press
		 * state, focus, the Button accessibility role) and wires an
		 * activation to press() and a hover or press change to
		 * interactionChanged(). Later calls only replace the handler. The
		 * card stays interactive even when the handler is empty. The card is
		 * restyled before returning.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the interaction handlers and must outlive the
		 * entity.
		 * @param entityIdentifier The card entity.
		 * @param handler Called each time the card is activated; may be
		 * empty.
		 */
		static void setOnPress(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   components::Card::Handler handler);

		/**
		 * @brief Activate the card as a click or a key press would.
		 *
		 * Calls the press handler of components::Card when there is one.
		 * Does nothing when the card is disabled. Use it to trigger the
		 * card from code or from a test.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The card entity.
		 */
		static void press(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief React to a change of the hover or press state of the card.
		 *
		 * Called by the interaction components once setOnPress() has made
		 * the card interactive. When the pressed state of an elevated card
		 * flips, it records the new state and plays the elevation motion
		 * from the current press progress. The card is then restyled, so
		 * the state layer follows the pointer. Filled and outlined cards
		 * have no elevation motion.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The card entity.
		 */
		static void
			interactionChanged(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Enable or disable the card and restyle it.
		 *
		 * A disabled card ignores press(). Disabling a card that is held
		 * down drops it back to its resting elevation at once, without
		 * motion.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The card entity.
		 * @param disabled True to disable, false to enable.
		 */
		static void setDisabled(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								bool disabled);

		/**
		 * @brief Whether the card ignores input.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The card entity.
		 * @return True when the entity has been disabled, see setDisabled().
		 */
		static bool isDisabled(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the width a section of the card can use, in dp.
		 *
		 * entities::Card uses it to size the sections it creates.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The card entity.
		 * @return The fixed width of the card's components::Layout minus its
		 * padding on both sides, never negative; 0 when the card has no
		 * fixed width.
		 */
		static float
			contentWidth(const ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Run apply() on one card; called by the frame loop.
		 *
		 * Logs a warning and does nothing when the entity has no
		 * components::Card.
		 * @param entityIdentifier The card entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
