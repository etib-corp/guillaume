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

#include <string>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/chip.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Styles a chip and arranges its children.
	 *
	 * Reads components::Chip, components::Selectable, the interaction state
	 * and the active theme, and writes the chip's Color, Borders, Elevation,
	 * Layout, Bound and Accessibility, plus the content and color of its
	 * icon and label children. Runs in the Layout phase, after the children
	 * have been measured.
	 */
	class Chip: public ecs::SystemFiller<components::Chip>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		Chip(void);

		/**
		 * @brief Default destructor.
		 */
		~Chip(void) override = default;

		/**
		 * @brief Make an entity behave as a chip.
		 *
		 * Adds the interaction components and wires press and selection
		 * changes to this system. Call once, after the entity has its
		 * components::Chip, Selectable, Elevation, Borders, Color, Layout,
		 * Transform and Bound.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Make the trailing icon of a chip a close button.
		 *
		 * Adds the interaction components to the trailing icon recorded in
		 * components::Chip and wires its press to close(). Does nothing
		 * without a trailing icon.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 */
		static void
			installCloseIcon(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one chip and arrange its children.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Activate as the user would: ignored when disabled, a Filter
		 * chip toggles its selection, and the press callback is called.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 */
		static void press(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Activate the close icon as the user would: ignored when
		 * disabled, otherwise the close callback is called.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 */
		static void close(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Set the selected state and restyle.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 * @param selected The new selected state.
		 */
		static void setSelected(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								bool selected);

		/**
		 * @brief Whether the chip is selected.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 * @return True when selected.
		 */
		static bool isSelected(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Enable or disable the chip and restyle.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 * @param disabled True to disable.
		 */
		static void setDisabled(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								bool disabled);

		/**
		 * @brief Whether the chip ignores input.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 * @return True for a disabled entity.
		 */
		static bool isDisabled(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the glyph of the leading icon the chip should have.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 * @return The configured glyph, the default one for an Assist chip
		 * without a glyph, or an empty string for no leading icon.
		 */
		static std::string
			leadingGlyph(const ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the chip should have a selected check child.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 * @return True for a Filter chip.
		 */
		static bool
			needsCheckIcon(const ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the chip should have a trailing close icon.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 * @return True for an Input chip or when a close callback is set.
		 */
		static bool
			needsCloseIcon(const ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style the chip with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
