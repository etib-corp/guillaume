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

#include "guillaume/components/switch.hpp"

namespace guillaume::systems
{
	/**
	 * @brief The rules of a switch: turning it on and off, coloring its
	 * track and placing its thumb, thumb icon and label.
	 *
	 * The switch entity is the track itself. The thumb that slides in it,
	 * the optional icon on the thumb and the optional label are child
	 * entities, recorded by identifier in components::Switch. Whether the
	 * switch is on is not in that component: it is the components::Value of
	 * the entity, 0 for off and 1 for on.
	 *
	 * - **Reads**: components::Switch (variant, label, child identifiers),
	 *   components::Value (on or off), the enabled state, the
	 *   components::Bound of the track and of its children, the active
	 *   theme.
	 * - **Writes**: the track's components::Color, the outline color of its
	 *   components::Borders, components::Accessibility (label and checked
	 *   state) and enabled flag; the components::Color and
	 *   components::Transform of the thumb, of its icon and of the label.
	 *   The actions also write components::Value.
	 * - **Phase**: Layout, registered after systems::Layout, so the track
	 *   has its position and the children their size.
	 *
	 * @code
	 * // Turn a switch on from code; the value-change callback is not
	 * // called.
	 * systems::Switch::setChecked(registry, identifier, true);
	 * @endcode
	 *
	 * The Disabled variant wins over
	 * `systems::interaction::setEnabled`: `apply` rewrites the enabled flag
	 * from the variant each frame.
	 *
	 * @see guillaume::components::Switch, guillaume::components::Value,
	 * guillaume::entities::Switch
	 */
	class Switch: public ecs::SystemFiller<components::Switch>
	{
		public:
		/**
		 * @brief Create the system for the Layout phase, matching entities
		 * that have a components::Switch.
		 */
		Switch(void);

		/**
		 * @brief Default destructor.
		 */
		~Switch(void) override = default;

		/**
		 * @brief Make an entity behave as a switch.
		 *
		 * Does two things:
		 * - calls systems::interaction::makeInteractive with the Switch
		 *   accessibility role, so a click or a hand press calls toggle and
		 *   any hover or press change calls `apply`;
		 * - restricts the components::Value of the entity to 0 and 1
		 *   (minimum 0, maximum 1, step 1) and sets its change handler to
		 *   call `apply`, so any write of the value is drawn at once.
		 *
		 * Call it once when the entity is built; entities::Switch does it
		 * for you. It does not style anything: call `apply` after it.
		 * @param componentRegistry The registry holding the components. The
		 * handlers keep its address, so it must outlive the entity.
		 * @param entityIdentifier The switch entity. It must already have
		 * components::Switch and components::Value.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Draw one switch from its components: thumb position,
		 * colors and accessibility.
		 *
		 * - Thumb: against the right end of the track when on, the left end
		 *   when off, components::Switch::ThumbInset from the edge;
		 *   OnPrimary when on, Outline when off. Its icon stays centered
		 *   on it.
		 * - Track: Primary and no outline when on, SurfaceVariant with an
		 *   Outline border when off.
		 * - Label: to the right of the track, after
		 *   components::Switch::LabelGap, vertically centered on it.
		 * - Disabled variant: the track takes the disabled container color
		 *   and the thumb, icon and label the disabled content color.
		 *
		 * The frame calls it through `update`. Call it yourself after
		 * writing components::Switch when the result must be visible before
		 * the next Layout phase; the setters of entities::Switch do.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The switch entity. It must have
		 * components::Switch, components::Value, components::Color,
		 * components::Borders and components::Accessibility. The thumb,
		 * its icon and the label are optional.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the switch ignores input and is drawn disabled.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The switch entity.
		 * @return True when the variant of components::Switch is Disabled
		 * or when the components::StyleState of the entity is not enabled.
		 */
		static bool isDisabled(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the switch is on.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The switch entity. It must have a
		 * components::Value.
		 * @return True when the value of its components::Value is at least
		 * 0.5.
		 */
		static bool isChecked(const ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Turn the switch on or off from code.
		 *
		 * Writes 1 or 0 into components::Value. The value-change callback
		 * of components::Switch is not called, and the call works on a
		 * disabled switch. The switch is redrawn at once by the change
		 * handler install set on the value; on an entity that did not go
		 * through install, it is redrawn at the next Layout phase.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The switch entity.
		 * @param checked True to turn the switch on, false to turn it off.
		 */
		static void setChecked(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   bool checked);

		/**
		 * @brief Flip the switch as a click does.
		 *
		 * Turns it on when off and off when on (through setChecked), then
		 * calls the value-change callback of the component with the new
		 * state. Nothing happens, and nothing is called, when the switch is
		 * disabled.
		 *
		 * The handlers set by install call it; call it directly to simulate
		 * a click, for example in a test on a bare identifier.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The switch entity.
		 */
		static void toggle(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Frame entry point: call `apply` on one switch.
		 * @param entityIdentifier The switch entity. An entity without a
		 * components::Switch is logged and skipped.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
