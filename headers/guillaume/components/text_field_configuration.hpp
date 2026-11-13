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
	 * @brief Configuration of a text field widget.
	 *
	 * Together with components::TextField (placeholder, supporting text,
	 * affixes and error state), this component is the whole state of a text
	 * field: its variant, flags, callbacks and the identifiers of its child
	 * entities. systems::TextField turns both into the field's Layout,
	 * Color, Borders and Line, and into the content, color and placement of
	 * the children.
	 *
	 * @see components::TextField
	 * @see systems::TextField
	 */
	class TextFieldConfiguration: public ecs::Component
	{
		public:
		/**
		 * @brief Text field variant.
		 */
		enum class Variant { Filled, Outlined };

		/// Called with the new text when the input changes.
		using ChangeHandler = std::function<void(const std::string &)>;

		/// Called when the field gains or loses the focus.
		using Handler = std::function<void(void)>;

		/// Smallest intrinsic width.
		static constexpr float MinWidth = 100.0f;

		private:
		Variant _variant { Variant::Filled };	 ///< Field variant
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
		 * @brief Default constructor.
		 */
		TextFieldConfiguration(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~TextFieldConfiguration(void) = default;

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const noexcept;

		/** @brief Set the variant. @param variant It. @return It. */
		TextFieldConfiguration &setVariant(Variant variant);

		/** @brief Get the leading icon glyph. @return The glyph. */
		const std::string &getLeadingIcon(void) const noexcept;

		/** @brief Set the leading icon glyph. @param glyph It. @return It. */
		TextFieldConfiguration &setLeadingIcon(const std::string &glyph);

		/** @brief Get the trailing icon glyph. @return The glyph. */
		const std::string &getTrailingIcon(void) const noexcept;

		/** @brief Set the trailing icon glyph. @param glyph It. @return It. */
		TextFieldConfiguration &setTrailingIcon(const std::string &glyph);

		/**
		 * @brief Get the last known input text.
		 *
		 * The live text is the content of the input child; this copy is what
		 * systems::TextField compares it with to report edits, and the text
		 * of a field whose input child does not exist yet.
		 * @return The text.
		 */
		const std::string &getText(void) const noexcept;

		/** @brief Set the last known input text. @param text It. @return It. */
		TextFieldConfiguration &setText(const std::string &text);

		/** @brief Check the editable flag. @return True when editable. */
		bool isEditable(void) const noexcept;

		/** @brief Set the editable flag. @param editable It. @return It. */
		TextFieldConfiguration &setEditable(bool editable);

		/** @brief Check the dense flag. @return True when dense. */
		bool isDense(void) const noexcept;

		/** @brief Set the dense flag. @param dense It. @return It. */
		TextFieldConfiguration &setDense(bool dense);

		/** @brief Check the multiline flag. @return True when multiline. */
		bool isMultiline(void) const noexcept;

		/** @brief Set the multiline flag. @param multiline It. @return It. */
		TextFieldConfiguration &setMultiline(bool multiline);

		/** @brief Get the forced width. @return The width, if forced. */
		const std::optional<float> &getWidth(void) const noexcept;

		/**
		 * @brief Force the width, or follow the content again.
		 * @param width The width (clamped to MinWidth), or nothing.
		 * @return Reference to this component for chaining.
		 */
		TextFieldConfiguration &setWidth(std::optional<float> width);

		/** @brief Get the text change callback. @return The callback. */
		const ChangeHandler &getOnChange(void) const noexcept;

		/** @brief Set the text change callback. @param handler It. @return It.
		 */
		TextFieldConfiguration &setOnChange(ChangeHandler handler);

		/** @brief Get the focus callback. @return The callback. */
		const Handler &getOnFocus(void) const noexcept;

		/** @brief Set the focus callback. @param handler It. @return It. */
		TextFieldConfiguration &setOnFocus(Handler handler);

		/** @brief Get the blur callback. @return The callback. */
		const Handler &getOnBlur(void) const noexcept;

		/** @brief Set the blur callback. @param handler It. @return It. */
		TextFieldConfiguration &setOnBlur(Handler handler);

		/** @brief Get the leading icon child. @return Its identifier. */
		ecs::Entity::Identifier getLeadingIconEntity(void) const noexcept;

		/** @brief Set the leading icon child. @param identifier It. @return It.
		 */
		TextFieldConfiguration &
			setLeadingIconEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the trailing icon child. @return Its identifier. */
		ecs::Entity::Identifier getTrailingIconEntity(void) const noexcept;

		/** @brief Set the trailing icon child. @param identifier It. @return
		 * It. */
		TextFieldConfiguration &
			setTrailingIconEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the error icon child. @return Its identifier. */
		ecs::Entity::Identifier getErrorIconEntity(void) const noexcept;

		/** @brief Set the error icon child. @param identifier It. @return It.
		 */
		TextFieldConfiguration &
			setErrorIconEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the column holding the label and the input. @return Its
		 * identifier. */
		ecs::Entity::Identifier getInputColumn(void) const noexcept;

		/** @brief Set the column holding the label and the input. @param
		 * identifier It. @return It. */
		TextFieldConfiguration &
			setInputColumn(ecs::Entity::Identifier identifier);

		/** @brief Get the floating label child. @return Its identifier. */
		ecs::Entity::Identifier getLabelEntity(void) const noexcept;

		/** @brief Set the floating label child. @param identifier It. @return
		 * It. */
		TextFieldConfiguration &
			setLabelEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the input text child. @return Its identifier. */
		ecs::Entity::Identifier getInputEntity(void) const noexcept;

		/** @brief Set the input text child. @param identifier It. @return It.
		 */
		TextFieldConfiguration &
			setInputEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the prefix child. @return Its identifier. */
		ecs::Entity::Identifier getPrefixEntity(void) const noexcept;

		/** @brief Set the prefix child. @param identifier It. @return It. */
		TextFieldConfiguration &
			setPrefixEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the suffix child. @return Its identifier. */
		ecs::Entity::Identifier getSuffixEntity(void) const noexcept;

		/** @brief Set the suffix child. @param identifier It. @return It. */
		TextFieldConfiguration &
			setSuffixEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the supporting text child. @return Its identifier. */
		ecs::Entity::Identifier getSupportingEntity(void) const noexcept;

		/** @brief Set the supporting text child. @param identifier It. @return
		 * It. */
		TextFieldConfiguration &
			setSupportingEntity(ecs::Entity::Identifier identifier);
	};

}	 // namespace guillaume::components
