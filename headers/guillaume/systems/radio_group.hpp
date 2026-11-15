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

#include <cstddef>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/radio_group.hpp"

namespace guillaume::systems
{
	/**
	 * @brief The rules of a radio group: a list of radio buttons of which
	 * at most one is selected.
	 *
	 * The group entity is a layout container. Its options are radio button
	 * children built by entities::RadioGroup and recorded by identifier, in
	 * option order, in components::RadioGroup. The value of the group (the
	 * index of the selected option, or none) is the source of truth: each
	 * frame, the radio buttons are made to agree with it.
	 *
	 * - **Reads**: components::RadioGroup (value, option labels, button
	 *   identifiers) and the components::Selectable of each button.
	 * - **Writes**: the value of components::RadioGroup (in the actions);
	 *   the selection of each radio button, through
	 *   systems::RadioButton::select; the children of the group's
	 *   components::Layout, which it then arranges through
	 *   systems::Layout::apply.
	 * - **Phase**: Layout. It is registered after systems::Layout and after
	 *   systems::RadioButton.
	 *
	 * @code
	 * // Select the third option from code; no callback is called.
	 * systems::RadioGroup::select(registry, identifier, 2);
	 * @endcode
	 *
	 * Selecting a radio button of the group directly (through its own
	 * components::Selectable) does not change the value of the group: the
	 * next `apply` puts the button back. Go through select or press. The
	 * list of children in the group's components::Layout is overwritten
	 * each frame.
	 *
	 * @see guillaume::components::RadioGroup, guillaume::systems::RadioButton,
	 * guillaume::systems::Selection
	 */
	class RadioGroup: public ecs::SystemFiller<components::RadioGroup>
	{
		public:
		/**
		 * @brief Create the system for the Layout phase, matching entities
		 * that have a components::RadioGroup.
		 */
		RadioGroup(void);

		/**
		 * @brief Default destructor.
		 */
		~RadioGroup(void) override = default;

		/**
		 * @brief Make an entity behave as a radio group.
		 *
		 * Gives the entity a components::Accessibility (added when missing)
		 * with the RadioGroup role, not focusable itself, and replaces the
		 * press callback of every radio button recorded in the component
		 * with one that calls press with the index of that button.
		 *
		 * Call it once the radio buttons exist and are recorded in the
		 * component, and again whenever they change. A button identifier
		 * without a components::RadioButton is skipped. It does not select
		 * or place anything: `apply` does.
		 * @param componentRegistry The registry holding the components. The
		 * press callbacks keep its address, so it must outlive the group.
		 * @param entityIdentifier The radio group entity. It must have a
		 * components::RadioGroup.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Make the radio buttons agree with the value of the group,
		 * then stack them.
		 *
		 * The button at the index of the value is selected and every other
		 * one deselected; with no value, all are deselected. Only buttons
		 * whose selection differs are touched. When the group has a
		 * components::Layout, the buttons become its children and
		 * systems::Layout::apply places them at once.
		 *
		 * The frame calls it through `update`; select and press call it
		 * too. Call it yourself after writing
		 * `components::RadioGroup::setValue` directly.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The radio group entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Select an option from code and show it at once.
		 *
		 * Writes the value of the group and calls `apply`. The value-change
		 * callback is not called.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The radio group entity.
		 * @param index The zero-based index of the option to select.
		 * @throws std::out_of_range If `index` is not smaller than the
		 * number of option labels of the component. Nothing is changed in
		 * that case.
		 */
		static void select(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   std::size_t index);

		/**
		 * @brief Select an option as a click on its radio button does.
		 *
		 * Writes the value of the group, calls `apply`, then calls the
		 * value-change callback of the component with the index, but only
		 * when the value really changed: pressing the selected option again
		 * calls nothing.
		 *
		 * The press callbacks set by install call it; call it directly to
		 * simulate a click in a test.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The radio group entity.
		 * @param index The zero-based index of the pressed option. The call
		 * does nothing, and does not throw, when it is not smaller than
		 * the number of radio buttons of the component.
		 */
		static void press(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier,
						  std::size_t index);

		/**
		 * @brief Frame entry point: call `apply` on one radio group.
		 * @param entityIdentifier The radio group entity. An entity without
		 * a components::RadioGroup is logged and skipped.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
