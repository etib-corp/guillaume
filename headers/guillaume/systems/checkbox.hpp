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
	 * @brief The rules of a checkbox: toggling it, coloring its box and
	 * placing its label.
	 *
	 * The checkbox entity is the 18 dp box itself. Its check glyph and its
	 * optional label are child entities, recorded by identifier in
	 * components::Checkbox.
	 *
	 * - **Reads**: components::Checkbox (state, variant, label, custom
	 *   colors, child identifiers), the hover, press and enabled state,
	 *   the components::Bound of the box and of the label, the active
	 *   theme.
	 * - **Writes**: the box's components::Color (fill), components::Borders
	 *   (outline of the unchecked box), components::Line (the dash of the
	 *   indeterminate state), components::Accessibility (label and checked
	 *   state) and enabled flag; the components::Color of the check glyph
	 *   and of the label; the components::Transform of the label. The
	 *   actions also write the state of components::Checkbox and keep
	 *   components::Selectable in step with it.
	 * - **Phase**: Layout, registered after systems::Layout, so the box has
	 *   its position and the label its measured size.
	 *
	 * @code
	 * // Check a box from code; the value-change callback is not called.
	 * systems::Checkbox::setState(registry, identifier,
	 * 	components::Checkbox::State::Checked);
	 * @endcode
	 *
	 * The Disabled variant wins over
	 * `systems::interaction::setEnabled`: `apply` rewrites the enabled flag
	 * from the variant each frame.
	 *
	 * @see guillaume::components::Checkbox, guillaume::entities::Checkbox,
	 * guillaume::systems::interaction
	 */
	class Checkbox: public ecs::SystemFiller<components::Checkbox>
	{
		public:
		/**
		 * @brief Create the system for the Layout phase, matching entities
		 * that have a components::Checkbox.
		 */
		Checkbox(void);

		/**
		 * @brief Default destructor.
		 */
		~Checkbox(void) override = default;

		/**
		 * @brief Make an entity behave as a checkbox.
		 *
		 * Does three things:
		 * - calls systems::interaction::makeInteractive with the Checkbox
		 *   accessibility role, so a click or a hand press calls toggle and
		 *   any hover or press change calls `apply`;
		 * - sets the accessibility margin of the hover components so the
		 *   box can be hit on a square of
		 *   components::Checkbox::TouchTarget (40 dp) without being drawn
		 *   larger;
		 * - sets the selection handler of components::Selectable so a
		 *   selection written from outside (a selection group, for
		 *   example) becomes Checked or Unchecked and is drawn at once. An
		 *   Indeterminate box keeps its state.
		 *
		 * Call it once when the entity is built; entities::Checkbox does it
		 * for you. It does not style anything: call `apply` after it.
		 * @param componentRegistry The registry holding the components. The
		 * handlers keep its address, so it must outlive the entity.
		 * @param entityIdentifier The checkbox entity. It must already have
		 * components::Checkbox, components::Selectable and a
		 * components::Layout whose fixed width is the width of the box.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Draw one checkbox from its component: colors,
		 * accessibility and label position.
		 *
		 * - Checked or Indeterminate: the box is filled with the checked
		 *   color of the component, or Primary when none is set; no
		 *   outline. Checked shows the check glyph, Indeterminate a 2 dp
		 *   dash.
		 * - Unchecked: transparent fill and a 2 dp outline in the unchecked
		 *   color of the component, or OnSurfaceVariant when none is set.
		 * - Error variant: Error replaces those colors, custom ones
		 *   included, and tints the label.
		 * - Disabled: the disabled content color replaces the fill of a
		 *   checked box, the outline of an unchecked one and the label
		 *   color, and no hover or press tint is shown.
		 *
		 * The label is placed to the right of the touch target, after
		 * components::Checkbox::LabelGap, vertically centered on the box.
		 *
		 * The frame calls it through `update`. Call it yourself after
		 * writing components::Checkbox when the result must be visible
		 * before the next Layout phase; the setters of entities::Checkbox
		 * do.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The checkbox entity. It must have
		 * components::Checkbox, components::Color, components::Borders,
		 * components::Line and components::Accessibility. The check glyph
		 * and the label are optional.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Set the state of a checkbox from code and show it at once.
		 *
		 * Writes the state, marks components::Selectable selected for
		 * Checked and Indeterminate and unselected for Unchecked, then
		 * calls `apply`. The value-change callback is not called, and the
		 * call works on a disabled checkbox. It is also the only way to
		 * reach the Indeterminate state.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The checkbox entity. It must have a
		 * components::Selectable.
		 * @param state The state to show.
		 */
		static void setState(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier,
							 components::Checkbox::State state);

		/**
		 * @brief Toggle the checkbox as a click does.
		 *
		 * Checked becomes Unchecked; Unchecked and Indeterminate become
		 * Checked. The new state is drawn at once (through setState), then
		 * the value-change callback of the component is called with it.
		 * Nothing happens, and nothing is called, when the checkbox is
		 * disabled.
		 *
		 * The handlers set by install call it; call it directly to simulate
		 * a click, for example in a test on a bare identifier.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The checkbox entity.
		 */
		static void toggle(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the checkbox ignores input and is drawn disabled.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The checkbox entity.
		 * @return True when the variant of components::Checkbox is Disabled
		 * or when the components::StyleState of the entity is not enabled.
		 */
		static bool isDisabled(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Frame entry point: call `apply` on one checkbox.
		 * @param entityIdentifier The checkbox entity. An entity without a
		 * components::Checkbox is logged and skipped.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
