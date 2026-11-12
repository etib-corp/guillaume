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

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/button.hpp"
#include "guillaume/components/extended_floating_action_button.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Styles and lays out a Material 3 extended floating action button.
	 *
	 * Reads components::ExtendedFloatingActionButton, components::Button, the
	 * interaction state and the active theme, and writes the button's Color,
	 * Borders, Bound and Elevation plus the color and position of its icon and
	 * label. Runs in the Layout phase, after the children have been measured.
	 */
	class ExtendedFloatingActionButton:
		public ecs::SystemFiller<components::ExtendedFloatingActionButton,
								 components::Button>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		ExtendedFloatingActionButton(void);

		/**
		 * @brief Default destructor.
		 */
		~ExtendedFloatingActionButton(void) override = default;

		/**
		 * @brief Make an entity behave as this button variant.
		 *
		 * Adds the Elevation component, wires input through
		 * systems::Button::install and registers apply() as the button's
		 * restyle function. Call once, after the entity has its
		 * components::ExtendedFloatingActionButton, components::Button and
		 * interaction components.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void install(ecs::ComponentRegistry &registry,
							const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Get the icon font size of the configured size.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 * @return The font size.
		 */
		static float iconFontSize(const ecs::ComponentRegistry &registry,
								  const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Get the resting content color (icon and label).
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 * @return The color.
		 */
		static utility::graphic::Color32Bit
			contentColor(const ecs::ComponentRegistry &registry,
						 const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Resolve the shadow level from the interaction state.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void elevation(ecs::ComponentRegistry &registry,
							  const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Resolve colors from the configuration and interaction state.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void styleState(ecs::ComponentRegistry &registry,
							   const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Resolve the size and place the icon and label.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void geometry(ecs::ComponentRegistry &registry,
							 const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Style and lay out one button.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void apply(ecs::ComponentRegistry &registry,
						  const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Style and lay out the button with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
