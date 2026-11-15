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
	 * @brief The rules of a chip: pressing it, selecting it, closing it,
	 * coloring it and arranging its icons and label.
	 *
	 * The chip entity is the rounded container, a horizontal layout. Its
	 * leading icon, selected check, label and trailing close icon are
	 * optional child entities, recorded by identifier in components::Chip.
	 * Whether it is selected is the flag of components::Selectable; whether
	 * it is disabled is the enabled flag of components::StyleState.
	 *
	 * - **Reads**: components::Chip (variant, mode, label, options, child
	 *   identifiers), components::Selectable, the hover, press and enabled
	 *   state, the active theme.
	 * - **Writes**: the chip's components::Color, components::Borders,
	 *   components::Elevation, components::Accessibility (label and
	 *   selected state), the padding and the children of its
	 *   components::Layout, which it then arranges through
	 *   systems::Layout::apply; the components::Color of each child and
	 *   the text of the label. The actions also write
	 *   components::Selectable and the enabled flag.
	 * - **Phase**: Layout, registered after systems::Layout. The children
	 *   were measured in the Measure phase of the same frame.
	 *
	 * @code
	 * // Select a filter chip from code; the press callback is not called.
	 * systems::Chip::setSelected(registry, identifier, true);
	 * @endcode
	 *
	 * This system never creates a child: leadingGlyph, needsCheckIcon and
	 * needsCloseIcon tell the widget class (entities::Chip) which children
	 * to build. While the selected check is shown, it takes the place of
	 * the leading icon in the layout.
	 *
	 * @see guillaume::components::Chip, guillaume::entities::Chip,
	 * guillaume::systems::interaction
	 */
	class Chip: public ecs::SystemFiller<components::Chip>
	{
		public:
		/**
		 * @brief Create the system for the Layout phase, matching entities
		 * that have a components::Chip.
		 */
		Chip(void);

		/**
		 * @brief Default destructor.
		 */
		~Chip(void) override = default;

		/**
		 * @brief Make an entity behave as a chip.
		 *
		 * Calls systems::interaction::makeInteractive with the Button
		 * accessibility role, so a click or a hand press calls press and
		 * any hover or press change calls `apply`, and sets the selection
		 * handler of components::Selectable so a selection written from
		 * outside is drawn at once.
		 *
		 * Call it once when the entity is built; entities::Chip does it for
		 * you. It does not style anything: call `apply` after it.
		 * @param componentRegistry The registry holding the components. The
		 * handlers keep its address, so it must outlive the entity.
		 * @param entityIdentifier The chip entity. It must already have a
		 * components::Selectable.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Make the trailing icon of a chip a close button.
		 *
		 * Makes the trailing icon recorded in components::Chip interactive
		 * on its own (Button role, accessibility label "Close") so that
		 * pressing it calls close on the chip instead of pressing the chip.
		 * Call it after the trailing icon child has been created and
		 * recorded. Does nothing when the chip has no trailing icon.
		 * @param componentRegistry The registry holding the components. The
		 * handler keeps its address, so it must outlive the entity.
		 * @param entityIdentifier The chip entity, not the icon.
		 */
		static void
			installCloseIcon(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Draw one chip from its components and arrange its
		 * children.
		 *
		 * - Container: SecondaryContainer for a Flat chip and for any
		 *   selected chip; transparent with a 1 dp Outline border for an
		 *   unselected Outlined chip. A selected chip gets an extra tint
		 *   when the component asks for the selected overlay, and the
		 *   hover or press tint comes on top.
		 * - Content: the label and trailing icon use the content color of
		 *   that container; the leading icon and the check are Primary, or
		 *   the content color when selected.
		 * - Disabled: disabled container and content colors, no tint.
		 * - Elevation: level 1 for an elevated, enabled chip, else level 0.
		 * - Layout: padding of components::Chip::Padding, or
		 *   CompactPadding for a compact chip; children in the order check
		 *   or leading icon, label, trailing icon.
		 *
		 * A chip whose component records no child at all keeps the layout
		 * children it was given.
		 *
		 * The frame calls it through `update`. Call it yourself after
		 * writing components::Chip when the result must be visible before
		 * the next Layout phase; the setters of entities::Chip do.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity. It must have
		 * components::Chip, components::Selectable, components::Color,
		 * components::Borders, components::Elevation, components::Layout
		 * and components::Accessibility.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Press the chip as a click does.
		 *
		 * A Filter chip flips its selection and is redrawn; the other
		 * variants keep theirs. Then the press callback of the component is
		 * called. Nothing happens, and nothing is called, when the chip is
		 * disabled.
		 *
		 * The handlers set by install call it; call it directly to simulate
		 * a click, for example in a test on a bare identifier.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 */
		static void press(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Press the close icon of the chip as a click does.
		 *
		 * Calls the close callback of the component, and nothing else: the
		 * chip is not removed or hidden, the callback decides what closing
		 * means. Nothing is called when the chip is disabled or has no
		 * close callback.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 */
		static void close(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Select or deselect the chip from code and show it at
		 * once.
		 *
		 * Writes the selected flag of components::Selectable, then calls
		 * `apply`. The press callback is not called, and the call works on
		 * any variant and on a disabled chip.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 * @param selected True to select the chip, false to deselect it.
		 */
		static void setSelected(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								bool selected);

		/**
		 * @brief Whether the chip is selected.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity. It must have a
		 * components::Selectable.
		 * @return The selected flag of its components::Selectable.
		 */
		static bool isSelected(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Enable or disable the chip and show it at once.
		 *
		 * Writes the enabled flag of components::StyleState (through
		 * systems::interaction::setEnabled), then calls `apply`. A disabled
		 * chip ignores press and close and is drawn with the disabled
		 * colors.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 * @param disabled True to disable the chip, false to enable it.
		 */
		static void setDisabled(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								bool disabled);

		/**
		 * @brief Whether the chip ignores input and is drawn disabled.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 * @return True when the entity has a components::StyleState that is
		 * not enabled.
		 */
		static bool isDisabled(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the name of the glyph the leading icon of the chip
		 * should show.
		 *
		 * The widget class calls it to decide whether to build a leading
		 * icon child and with which glyph.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 * @return The glyph configured in components::Chip;
		 * components::Chip::DefaultAssistGlyph for an Assist chip without
		 * one; an empty string when the chip should have no leading icon.
		 */
		static std::string
			leadingGlyph(const ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the widget class should build a selected check
		 * child for this chip.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 * @return True for the Filter variant, false for the others.
		 */
		static bool
			needsCheckIcon(const ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the widget class should build a trailing close
		 * icon child for this chip.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The chip entity.
		 * @return True for the Input variant, or for any variant with a
		 * close callback set.
		 */
		static bool
			needsCloseIcon(const ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Frame entry point: call `apply` on one chip.
		 * @param entityIdentifier The chip entity. An entity without a
		 * components::Chip is logged and skipped.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
