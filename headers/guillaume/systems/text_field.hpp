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
	 * @brief Styles a text field, arranges its children and reports edits.
	 *
	 * For an entity carrying components::TextFieldConfiguration (the text
	 * field widget), reads it together with components::TextField and the
	 * active theme, and writes the field's Layout, Color, Borders, Line and
	 * Accessibility, plus the content, color and placement of the label,
	 * input, affix, icon and supporting children. It also gives the input
	 * child a components::Focus while the field may be edited, and calls the
	 * change callback when the input text differs from the last known one.
	 *
	 * For an entity that only carries components::TextField next to its own
	 * components::Text, it keeps the label floating state in step with the
	 * content (`reconcile`).
	 *
	 * Runs in the Layout phase, after the children have been measured.
	 *
	 * @see components::TextField
	 * @see components::TextFieldConfiguration
	 */
	class TextField: public ecs::SystemFiller<components::TextField>
	{
		public:
		/// M3 text field height, in dp.
		static constexpr float Height = 56.0f;

		/// Paper dense height with a label, in dp.
		static constexpr float DenseLabelHeight = 52.0f;

		/// Paper dense height without a label, in dp.
		static constexpr float DenseHeight = 40.0f;

		/// M3 horizontal padding, in dp.
		static constexpr float Padding = theme::SpacingScale::get(4);

		/// M3 gap between icons, affixes and the input, in dp.
		static constexpr float Spacing = theme::SpacingScale::get(3);

		/// M3 gap between the field and the supporting text, in dp.
		static constexpr float SupportingGap = theme::SpacingScale::get(1);

		/// M3 icon size, in dp.
		static constexpr float IconSize = 24.0f;

		/**
		 * @brief Construct a text field system in the Layout phase.
		 */
		TextField(void);

		/**
		 * @brief Default destructor.
		 */
		~TextField(void) override = default;

		/**
		 * @brief Make an entity behave as a text field.
		 *
		 * Adds the interaction, focus, style state and accessibility
		 * components, restyles the container on hover and press, and wires
		 * the focus handlers to the callbacks of
		 * components::TextFieldConfiguration. Call once, after the entity
		 * has its components::TextField, TextFieldConfiguration, Line,
		 * Layout, Transform, Bound, Color and Borders.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one text field and arrange its children.
		 *
		 * Reports an edit of the input child first, then styles the
		 * container, writes the content and color of the children, records
		 * the layout children, places the supporting text and the outlined
		 * floating label, and updates the editing focus.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style the container only: layout configuration, fill,
		 * outline and active indicator.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 */
		static void
			applyContainer(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Give the input child a components::Focus while the field is
		 * editable, enabled and focused, and remove it otherwise.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 */
		static void
			applyEditing(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Check whether the field owns the input focus.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 * @return True when its Focus or StyleState is focused.
		 */
		static bool isFocused(const ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Check whether the field is disabled.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 * @return True when disabled.
		 */
		static bool isDisabled(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Check whether the label floats.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 * @return True while the field is focused or populated.
		 */
		static bool
			isLabelFloating(const ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the input text.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 * @return The content of the input child, or the last known text
		 * while the input child does not exist.
		 */
		static std::string
			text(const ecs::ComponentRegistry &componentRegistry,
				 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Replace the input text without calling the change callback.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 * @param text The new text.
		 */
		static void setText(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							const std::string &text);

		/**
		 * @brief Enable or disable the field.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 * @param disabled Whether the field is disabled.
		 */
		static void setDisabled(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								bool disabled);

		/**
		 * @brief Text shown below the field: the error text in the error
		 * state when there is one, the supporting text otherwise.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text field entity.
		 * @return The text (may be empty).
		 */
		static std::string
			supportingText(const ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Update one entity carrying a components::TextField.
		 * @param entityIdentifier The target entity identifier.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		/**
		 * @brief Reconcile the label floating state with the field content.
		 * @param textField The text field component.
		 * @param hasContent Whether the field currently has content.
		 * @return The updated label floating state.
		 */
		static bool reconcile(components::TextField &textField,
							  bool hasContent);
	};

}	 // namespace guillaume::systems
