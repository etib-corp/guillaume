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

#include "guillaume/components/checkbox.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Styles a checkbox and places its label.
	 *
	 * Reads components::Checkbox, the interaction state and the active theme,
	 * and writes the box's Color, Borders, Line and Accessibility, plus the
	 * color and position of the check glyph and label children. Runs in the
	 * Layout phase, after the children have been measured.
	 */
	class Checkbox: public ecs::SystemFiller<components::Checkbox>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		Checkbox(void);

		/**
		 * @brief Default destructor.
		 */
		~Checkbox(void) override = default;

		/**
		 * @brief Make an entity behave as a checkbox.
		 *
		 * Adds the interaction components, grows the touch target and wires
		 * press and selection changes to this system. Call once, after the
		 * entity has its components::Checkbox, Selectable, Line, Borders,
		 * Color, Layout, Transform and Bound.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The checkbox entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one checkbox and place its label.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The checkbox entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Set the checked state and restyle.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The checkbox entity.
		 * @param state The new state.
		 */
		static void setState(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier,
							 components::Checkbox::State state);

		/**
		 * @brief Toggle as the user would: ignored when disabled, and the
		 * value-change callback is called.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The checkbox entity.
		 */
		static void toggle(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the checkbox ignores input.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The checkbox entity.
		 * @return True for the Disabled variant or a disabled entity.
		 */
		static bool isDisabled(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style the checkbox with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
