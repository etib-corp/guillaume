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
	 * @brief Keeps at most one radio button of a radio group selected.
	 *
	 * Reads components::RadioGroup and writes the selection of its radio
	 * button children through systems::RadioButton, and the list of children
	 * the Layout system stacks. Runs in the Layout phase.
	 */
	class RadioGroup: public ecs::SystemFiller<components::RadioGroup>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		RadioGroup(void);

		/**
		 * @brief Default destructor.
		 */
		~RadioGroup(void) override = default;

		/**
		 * @brief Make an entity behave as a radio group.
		 *
		 * Publishes the `RadioGroup` accessibility role and wires the press of
		 * every radio button recorded in the component to this system. Call
		 * it again after the radio button children change.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The radio group entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Select the radio button of the group value, deselect the
		 * others and stack them.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The radio group entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Select an option programmatically (no callback).
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The radio group entity.
		 * @param index The option index.
		 * @throws std::out_of_range If @p index is not an option.
		 */
		static void select(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   std::size_t index);

		/**
		 * @brief Select an option as the user would: the value-change
		 * callback is called when the value changes.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The radio group entity.
		 * @param index The option index (ignored when out of range).
		 */
		static void press(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier,
						  std::size_t index);

		/**
		 * @brief Refresh the radio group with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
