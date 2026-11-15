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

#include <functional>
#include <optional>
#include <string>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief State of a text field widget that is not text around the
	 * input: its variant, icons, flags, width, callbacks and the
	 * identifiers of its children.
	 *
	 * A text field has two components. components::TextField holds the
	 * placeholder, supporting text, affixes and error state. This one holds
	 * the rest. The entities::TextField widget writes both.
	 * systems::TextField reads both every frame in the Layout phase and
	 * turns them into the field's Layout, Color, Borders and Line, and into
	 * the content, color and placement of the children.
	 *
	 * The text the user types is the components::Text of the input child.
	 * getText() is only the copy systems::TextField compares it with to
	 * detect an edit. To change the text from code, call
	 * systems::TextField::setText.
	 *
	 * @see components::TextField, systems::TextField, entities::TextField
	 */
	class TextFieldConfiguration: public ecs::Component
	{
		public:
		/**
		 * @brief Look of the field: Filled has a tinted background and a
		 * line under it; Outlined has a border the label sits on.
		 */
		enum class Variant { Filled, Outlined };

		/// Callback that receives the new text after the user edits the
		/// input.
		using ChangeHandler = std::function<void(const std::string &)>;

		/// Callback that takes no argument, used when the field gains or
		/// loses the focus.
		using Handler = std::function<void(void)>;

		/// Smallest width of a text field, in dp.
		static constexpr float MinWidth = 100.0f;

		private:
		Variant _variant { Variant::Filled };	 ///< Look of the field
		std::string _leadingIcon {};			 ///< Leading icon glyph
		std::string _trailingIcon {};			 ///< Trailing icon glyph
		std::string _text {};					 ///< Last known input text
		bool _editable { true };				 ///< Editable flag
		bool _dense { false };					 ///< Dense flag
		bool _multiline { false };				 ///< Multiline flag
		std::optional<float> _width {};			 ///< Forced width
		ChangeHandler _onChange {};				 ///< Text change callback
		Handler _onFocus {};					 ///< Focus callback
		Handler _onBlur {};						 ///< Blur callback
		ecs::Entity::Identifier _leadingIconEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Leading icon child

		ecs::Entity::Identifier _trailingIconEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Trailing icon child

		ecs::Entity::Identifier _errorIconEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Error icon child

		ecs::Entity::Identifier _inputColumn {
			ecs::Entity::InvalidIdentifier
		};	  ///< Column holding the label and the input

		ecs::Entity::Identifier _labelEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Floating label child

		ecs::Entity::Identifier _inputEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Input text child

		ecs::Entity::Identifier _prefixEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Prefix child

		ecs::Entity::Identifier _suffixEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Suffix child

		ecs::Entity::Identifier _supportingEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Supporting text child

		public:
		/**
		 * @brief Create the configuration of an editable, single-line
		 * filled field with no icon and no forced width.
		 */
		TextFieldConfiguration(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~TextFieldConfiguration(void) = default;

		/**
		 * @brief Get the look of the field.
		 * @return The variant, Variant::Filled by default.
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Choose between the filled and the outlined look.
		 *
		 * Marks the component changed only when the variant differs.
		 * @param variant The new variant.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &setVariant(Variant variant);

		/**
		 * @brief Get the icon shown before the input.
		 * @return The glyph name; empty for none.
		 */
		const std::string &getLeadingIcon(void) const noexcept;

		/**
		 * @brief Set the icon shown before the input.
		 *
		 * The widget uses it when it builds the leading icon child. Marks
		 * the component changed only when the name differs.
		 * @param glyph The glyph name; empty for none.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &setLeadingIcon(const std::string &glyph);

		/**
		 * @brief Get the icon shown after the input.
		 * @return The glyph name; empty for none.
		 */
		const std::string &getTrailingIcon(void) const noexcept;

		/**
		 * @brief Set the icon shown after the input.
		 *
		 * The widget uses it when it builds the trailing icon child. While
		 * the field is in error, the error icon is shown in its place.
		 * Marks the component changed only when the name differs.
		 * @param glyph The glyph name; empty for none.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &setTrailingIcon(const std::string &glyph);

		/**
		 * @brief Get the last known input text.
		 *
		 * The live text is the content of the input child; this copy is
		 * what systems::TextField compares it with to report edits, and the
		 * text of a field whose input child does not exist yet.
		 * @return The text as of the last pass of systems::TextField.
		 */
		const std::string &getText(void) const noexcept;

		/**
		 * @brief Store the last known input text.
		 *
		 * This does not change what the field shows; use
		 * systems::TextField::setText for that. Marks the component changed
		 * only when the text differs.
		 * @param text The text of the input child.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &setText(const std::string &text);

		/**
		 * @brief Tell whether the user can edit the text.
		 * @return True when editable, which is the default.
		 */
		bool isEditable(void) const noexcept;

		/**
		 * @brief Allow or forbid editing by the user.
		 *
		 * The input child receives the keyboard only while the field is
		 * editable, enabled and focused. A read-only field can still be
		 * focused. Marks the component changed only when the flag differs.
		 * @param editable True to let the user edit the text.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &setEditable(bool editable);

		/**
		 * @brief Tell whether the field uses the compact height.
		 * @return True when dense.
		 */
		bool isDense(void) const noexcept;

		/**
		 * @brief Choose between the regular and the compact height.
		 *
		 * A regular field is 56 dp high. A dense one is 52 dp high with a
		 * placeholder and 40 dp without. Ignored by a multiline field.
		 * Marks the component changed only when the flag differs.
		 * @param dense True for the compact height.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &setDense(bool dense);

		/**
		 * @brief Tell whether the field grows with its content.
		 * @return True when multiline.
		 */
		bool isMultiline(void) const noexcept;

		/**
		 * @brief Choose between a fixed height and a height that follows
		 * the content.
		 *
		 * Marks the component changed only when the flag differs.
		 * @param multiline True to let the height follow the content.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &setMultiline(bool multiline);

		/**
		 * @brief Get the forced width of the field.
		 * @return The width in dp, or std::nullopt when the width follows
		 * the content.
		 */
		const std::optional<float> &getWidth(void) const noexcept;

		/**
		 * @brief Force the width of the field, or let it follow its
		 * content.
		 *
		 * Marks the component changed only when the stored value differs.
		 * @param width The width in dp, raised to MinWidth when smaller;
		 * std::nullopt to follow the content (never below MinWidth).
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &setWidth(std::optional<float> width);

		/**
		 * @brief Get the callback run when the user edits the text.
		 * @return The callback; empty when none is set.
		 */
		const ChangeHandler &getOnChange(void) const noexcept;

		/**
		 * @brief Set the callback run when the user edits the text.
		 *
		 * systems::TextField calls it with the new text, on the frame it
		 * sees the input differ from the last known text. It is not called
		 * for a text set with systems::TextField::setText. Does not mark
		 * the component changed.
		 * @param handler The callback; an empty one removes it.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &setOnChange(ChangeHandler handler);

		/**
		 * @brief Get the callback run when the field gains the focus.
		 * @return The callback; empty when none is set.
		 */
		const Handler &getOnFocus(void) const noexcept;

		/**
		 * @brief Set the callback run when the field gains the focus.
		 *
		 * Does not mark the component changed.
		 * @param handler The callback; an empty one removes it.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &setOnFocus(Handler handler);

		/**
		 * @brief Get the callback run when the field loses the focus.
		 * @return The callback; empty when none is set.
		 */
		const Handler &getOnBlur(void) const noexcept;

		/**
		 * @brief Set the callback run when the field loses the focus.
		 *
		 * Does not mark the component changed.
		 * @param handler The callback; an empty one removes it.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &setOnBlur(Handler handler);

		/**
		 * @brief Get the child that draws the leading icon.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * field has none.
		 */
		ecs::Entity::Identifier getLeadingIconEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the leading icon.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &
			setLeadingIconEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the trailing icon.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * field has none.
		 */
		ecs::Entity::Identifier getTrailingIconEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the trailing icon.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &
			setTrailingIconEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the icon shown while the field is
		 * in error.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getErrorIconEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the icon shown while the field
		 * is in error.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &
			setErrorIconEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the column that stacks the label above the input.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getInputColumn(void) const noexcept;

		/**
		 * @brief Record the column that stacks the label above the input.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The column's identifier.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &
			setInputColumn(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the label (the placeholder
		 * text).
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getLabelEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the label.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &
			setLabelEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child the user types in.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getInputEntity(void) const noexcept;

		/**
		 * @brief Record the child the user types in.
		 *
		 * From then on its components::Text is the text of the field.
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &
			setInputEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the prefix.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getPrefixEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the prefix.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &
			setPrefixEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the suffix.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getSuffixEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the suffix.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &
			setSuffixEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the supporting or error text
		 * under the field.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getSupportingEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the supporting or error text
		 * under the field.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		TextFieldConfiguration &
			setSupportingEntity(ecs::Entity::Identifier identifier);
	};

}	 // namespace guillaume::components
