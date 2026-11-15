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

#include "guillaume/components/radio_button.hpp"

namespace guillaume::systems
{
	/**
	 * @brief The rules of a radio button: selecting it, coloring its ring
	 * and placing its dot and label.
	 *
	 * The radio button entity is the 20 dp ring itself. The dot shown when
	 * it is selected and its optional label are child entities, recorded by
	 * identifier in components::RadioButton. Whether it is selected is not
	 * in that component: it is the flag of components::Selectable.
	 *
	 * - **Reads**: components::RadioButton (variant, label, child
	 *   identifiers), components::Selectable (selected), the enabled
	 *   state, the components::Bound of the ring and of its children, the
	 *   active theme.
	 * - **Writes**: the ring's components::Ring (2 dp thickness),
	 *   components::Color, components::Accessibility (label, selected and
	 *   checked state) and enabled flag; the components::Color of the dot
	 *   and of the label and their components::Transform. The actions also
	 *   write the selected flag of components::Selectable.
	 * - **Phase**: Layout, registered after systems::Layout, so the ring has
	 *   its position and the label its measured size.
	 *
	 * A radio button alone can be selected but never deselects itself or
	 * its neighbours on a press. Exclusive choice comes from the parent:
	 * systems::RadioGroup, or a components::SelectionGroup handled by
	 * systems::Selection.
	 *
	 * The Disabled variant wins over
	 * `systems::interaction::setEnabled`: `apply` rewrites the enabled flag
	 * from the variant each frame.
	 *
	 * @see guillaume::components::RadioButton, guillaume::systems::RadioGroup,
	 * guillaume::components::Selectable
	 */
	class RadioButton: public ecs::SystemFiller<components::RadioButton>
	{
		public:
		/**
		 * @brief Create the system for the Layout phase, matching entities
		 * that have a components::RadioButton.
		 */
		RadioButton(void);

		/**
		 * @brief Default destructor.
		 */
		~RadioButton(void) override = default;

		/**
		 * @brief Make an entity behave as a radio button.
		 *
		 * Does three things:
		 * - calls systems::interaction::makeInteractive with the Radio
		 *   accessibility role, so a click or a hand press calls press and
		 *   any hover or press change calls `apply`;
		 * - sets the accessibility margin of the hover components so the
		 *   ring can be hit on a square of
		 *   components::RadioButton::TouchTarget (40 dp) without being
		 *   drawn larger;
		 * - sets the selection handler of components::Selectable so a
		 *   selection written from outside (by a group) is drawn at once.
		 *
		 * Call it once when the entity is built; entities::RadioButton does
		 * it for you. It does not style anything: call `apply` after it.
		 * @param componentRegistry The registry holding the components. The
		 * handlers keep its address, so it must outlive the entity.
		 * @param entityIdentifier The radio button entity. It must already
		 * have components::RadioButton, components::Selectable and a
		 * components::Layout whose fixed width is the width of the ring.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Draw one radio button from its components: colors,
		 * accessibility, dot and label position.
		 *
		 * The ring is Primary when selected and OnSurfaceVariant otherwise;
		 * Error replaces both in the Error variant, and the disabled
		 * content color when disabled. The dot takes the same color when
		 * selected, is centered in the ring, and is transparent otherwise.
		 * The label is placed to the right of the touch target, after
		 * components::RadioButton::LabelGap, vertically centered on the
		 * ring.
		 *
		 * The frame calls it through `update`. Call it yourself after
		 * writing components::RadioButton when the result must be visible
		 * before the next Layout phase; the setters of
		 * entities::RadioButton do.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The radio button entity. It must have
		 * components::RadioButton, components::Selectable,
		 * components::Ring, components::Color and
		 * components::Accessibility. The dot and the label are optional.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the radio button ignores input and is drawn
		 * disabled.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The radio button entity.
		 * @return True when the variant of components::RadioButton is
		 * Disabled or when the components::StyleState of the entity is not
		 * enabled.
		 */
		static bool isDisabled(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Select or deselect a radio button from code and show it
		 * at once.
		 *
		 * Writes the selected flag of components::Selectable, then calls
		 * `apply`. The press callback is not called, the call works on a
		 * disabled radio button, and the other radio buttons of a group
		 * are not touched. systems::RadioGroup uses it to apply its value.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The radio button entity.
		 * @param selected True to select it, false to deselect it.
		 */
		static void select(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   bool selected);

		/**
		 * @brief Select the radio button as a click does.
		 *
		 * Selects it (it stays selected when it already was), draws it,
		 * then calls the press callback of the component, on every press,
		 * whether or not the selection changed. Nothing happens, and
		 * nothing is called, when the radio button is disabled.
		 *
		 * The handlers set by install call it; call it directly to simulate
		 * a click, for example in a test on a bare identifier.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The radio button entity.
		 */
		static void press(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Frame entry point: call `apply` on one radio button.
		 * @param entityIdentifier The radio button entity. An entity
		 * without a components::RadioButton is logged and skipped.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
