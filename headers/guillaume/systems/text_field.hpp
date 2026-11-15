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

#include "guillaume/components/text.hpp"
#include "guillaume/components/text_field.hpp"
#include "guillaume/components/text_field_configuration.hpp"
#include "guillaume/theme/tokens.hpp"

namespace guillaume::systems
{

	/**
	 * @brief Styles a text field, arranges its label, input, affixes, icons
	 * and supporting text, and reports what the user typed.
	 *
	 * Runs in the Layout phase on every entity that has a
	 * components::TextField. It is registered after systems::Layout and
	 * after systems::Search, so the children are already measured and a
	 * search field has already written its components::TextField.
	 *
	 * What it does depends on the other components of the entity:
	 *
	 * **With a components::TextFieldConfiguration** (the text field
	 * widget), it runs apply():
	 * - **Reads**: components::TextFieldConfiguration (variant, dense,
	 *   multiline and editable flags, forced width, callbacks, child
	 *   identifiers), components::TextField (placeholder, prefix, suffix,
	 *   supporting and error texts, error flag), the focus, hover, press
	 *   and enabled state of the field, the Text of the input child and the
	 *   active theme.
	 * - **Writes** on the field: components::Layout, components::Color,
	 *   components::Borders, components::Line, the label and hint of
	 *   components::Accessibility, the label and floating state of
	 *   components::TextField, and the last known text of the
	 *   configuration.
	 * - **Writes** on the children: their Text, Typography, Color,
	 *   Transform and Bound, and adds or removes the components::Focus of
	 *   the input child.
	 *
	 * **With only its own components::Text**, it keeps the label-floating
	 * flag of components::TextField in step with the content, through
	 * reconcile(). An entity with neither is left alone.
	 *
	 * The label "floats" (moves up and shrinks) while the field is focused
	 * or holds text; it rests in place of the input otherwise. The prefix
	 * and suffix show only while it floats. In the error state the error
	 * icon replaces the trailing icon and the error text replaces the
	 * supporting text.
	 *
	 * The text typed by the user lives in the components::Text of the input
	 * child. The change callback is called from apply() when that text
	 * differs from the last one recorded, so an edit is reported on the
	 * frame after it was typed.
	 *
	 * @see components::TextField, components::TextFieldConfiguration,
	 * entities::TextField, systems::TextInput
	 */
	class TextField: public ecs::SystemFiller<components::TextField>
	{
		public:
		/// Height of a single-line field that is not dense, in dp.
		static constexpr float Height = 56.0f;

		/// Height of a dense single-line field that has a label, in dp.
		static constexpr float DenseLabelHeight = 52.0f;

		/// Height of a dense single-line field without a label, in dp.
		static constexpr float DenseHeight = 40.0f;

		/// Space between the left and right edges of the field and its
		/// content: 16 dp.
		static constexpr float Padding = theme::SpacingScale::get(4);

		/// Gap between the icons, the affixes and the input: 12 dp.
		static constexpr float Spacing = theme::SpacingScale::get(3);

		/// Gap between the bottom of the field and the supporting text:
		/// 4 dp.
		static constexpr float SupportingGap = theme::SpacingScale::get(1);

		/// Width assumed for the leading icon when the outlined floating
		/// label is placed after it, in dp.
		static constexpr float IconSize = 24.0f;

		/**
		 * @brief Create the system in the Layout phase.
		 */
		TextField(void);

		/**
		 * @brief Default destructor.
		 */
		~TextField(void) override = default;

		/**
		 * @brief Make an entity focusable and hoverable as a text field.
		 *
		 * Adds the interaction components (hover and press state, focus,
		 * style state, the TextField accessibility role), restyles the
		 * container with applyContainer() on a hover or press change, and
		 * makes gaining and losing the focus call the focus and blur
		 * callbacks of components::TextFieldConfiguration. Call it once,
		 * after the entity has its components::TextField,
		 * TextFieldConfiguration, Line, Layout, Transform, Bound, Color and
		 * Borders.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the handlers and must outlive the entity.
		 * @param entityIdentifier The text field entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Report an edit, then style one text field and arrange its
		 * children.
		 *
		 * This is what update() runs each frame on a text field widget.
		 * Call it directly after changing components::TextField or
		 * components::TextFieldConfiguration to see the result before the
		 * next frame, which is what the setters of entities::TextField do.
		 *
		 * In order: calls the change callback when the input text differs
		 * from the last one recorded; styles the container
		 * (applyContainer()); writes the content, type role and color of
		 * the children; records and places the layout children; places the
		 * supporting text under the field and, for the outlined variant,
		 * the floating label on the outline; updates the editing focus
		 * (applyEditing()).
		 *
		 * The field is as wide as its content plus padding, or as the width
		 * forced in the configuration, and never narrower than
		 * components::TextFieldConfiguration::MinWidth.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::TextFieldConfiguration, TextField, Layout, Color,
		 * Borders, Line or Bound.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style the container only, leaving the children alone.
		 *
		 * Writes the row configuration and height of components::Layout and
		 * the look of the variant: an outlined field has a transparent fill
		 * and a stroke around it; a filled field has a tinted fill and an
		 * active indicator line at the bottom. The stroke or indicator is
		 * 2 dp thick while the field is focused and enabled, 1 dp otherwise,
		 * and takes the error, primary or disabled color of its state. A
		 * multiline field has no fixed height. This is the cheap restyle run
		 * on a hover or press change.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 */
		static void
			applyContainer(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Let the input child receive typed text only while the
		 * field is being edited.
		 *
		 * Adds a components::Focus to the input child while the field is
		 * editable, enabled and focused, and removes it otherwise. The text
		 * input system only writes to entities that have that component.
		 * Does nothing when no input child is recorded.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 */
		static void
			applyEditing(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the field has the input focus.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 * @return True when its components::Focus or its
		 * components::StyleState reports the focused state; false when it
		 * has neither component.
		 */
		static bool isFocused(const ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the field ignores input.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 * @return True when the entity has been disabled, see setDisabled().
		 */
		static bool isDisabled(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the label is in its floating position.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 * @return True while the field is focused or its text is not empty.
		 */
		static bool
			isLabelFloating(const ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the text the field holds.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 * @return The content of the input child when it has a
		 * components::Text, otherwise the last text recorded in
		 * components::TextFieldConfiguration. Empty when nothing is typed.
		 */
		static std::string
			text(const ecs::ComponentRegistry &componentRegistry,
				 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Replace the text from code and restyle the field.
		 *
		 * The change callback is not called: it is reserved for edits made
		 * by the user.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 * @param text The new text; an empty string lets the label rest
		 * again when the field is not focused.
		 */
		static void setText(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							const std::string &text);

		/**
		 * @brief Enable or disable the field and restyle it.
		 *
		 * A disabled field is drawn dimmed and its input child cannot
		 * receive typed text.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 * @param disabled True to disable the field, false to enable it.
		 */
		static void setDisabled(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								bool disabled);

		/**
		 * @brief Get the text shown below the field.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 * @return The error text of components::TextField when the field is
		 * in the error state and has one, its supporting text otherwise.
		 * Empty when there is nothing to show.
		 */
		static std::string
			supportingText(const ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Update one entity that has a components::TextField; called
		 * by the frame loop.
		 *
		 * Runs apply() when the entity also has a
		 * components::TextFieldConfiguration. Otherwise, when it has its own
		 * components::Text, calls reconcile() with whether that text is
		 * empty. Logs a warning and does nothing when the entity has no
		 * components::TextField.
		 * @param entityIdentifier The entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		/**
		 * @brief Derive the label-floating flag of a field that has no
		 * configuration, and store it.
		 *
		 * The label floats when the field has content or is in the error
		 * state. Focus is not taken into account here, unlike
		 * isLabelFloating(). systems::Search uses it for the field it
		 * drives.
		 * @param textField The component to update.
		 * @param hasContent True when the field currently holds text.
		 * @return The floating state that was stored.
		 */
		static bool reconcile(components::TextField &textField,
							  bool hasContent);
	};

}	 // namespace guillaume::systems
