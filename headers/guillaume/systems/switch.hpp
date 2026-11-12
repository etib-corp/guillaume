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
	 * @brief Styles a switch and places its thumb, icon and label.
	 *
	 * Reads components::Switch, the interaction state and the active theme, and
	 * writes the entity's own render components plus the color and position
	 * of its children. Runs in the Layout phase, after the children have
	 * been measured.
	 */
	class Switch: public ecs::SystemFiller<components::Switch>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		Switch(void);

		/**
		 * @brief Default destructor.
		 */
		~Switch(void) override = default;

		/**
		 * @brief Make an entity behave as this widget.
		 *
		 * Adds the interaction components and wires input and value changes
		 * to this system. Call once, after the entity has its
		 * components::Switch and the components of a surface.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one widget and place its children.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the widget ignores input.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The widget entity.
		 * @return True for the Disabled variant or a disabled entity.
		 */
		static bool isDisabled(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the switch is on.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The switch entity.
		 * @return True when its Value is at least 0.5.
		 */
		static bool isChecked(const ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Turn the switch on or off.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The switch entity.
		 * @param checked The new value.
		 */
		static void setChecked(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   bool checked);

		/**
		 * @brief Toggle as the user would: ignored when disabled, and the
		 * value-change callback is called.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The switch entity.
		 */
		static void toggle(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style the widget with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
