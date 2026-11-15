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

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief The texts around the input of a text field, and its error
	 * state: placeholder, label, supporting text, prefix, suffix and error
	 * text.
	 *
	 * It is shared by the two widgets that contain an input:
	 * entities::TextField (together with
	 * components::TextFieldConfiguration) and entities::Search. The widget
	 * writes the texts and the error flag. systems::TextField reads them in
	 * the Layout phase to fill and color the label, affix and supporting
	 * text children.
	 *
	 * Two members are derived, not chosen: systems::TextField and
	 * systems::Search overwrite the label with the placeholder, and the
	 * label-floating flag with the state of the field, every frame. The
	 * text the user types is not stored here; it is the components::Text of
	 * the input.
	 *
	 * @see systems::TextField, components::TextFieldConfiguration
	 */
	class TextField: public ecs::Component
	{
		private:
		std::string _placeholder {};	   ///< Placeholder text.
		std::string _label {};			   ///< Floating label.
		std::string _supportingText {};	   ///< Supporting text.
		std::string _prefix {};			   ///< Leading affix text.
		std::string _suffix {};			   ///< Trailing affix text.
		std::string _errorText {};		   ///< Text shown when in error.
		bool _error { false };			   ///< Error state.
		bool _labelFloating {
			false
		};	  ///< Whether the label is floated above the input.

		public:
		/**
		 * @brief Create a field with no text and no error.
		 */
		TextField(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~TextField(void) = default;

		/**
		 * @brief Get the placeholder: the text that names the field.
		 * @return The placeholder; it is also the field's accessible name.
		 */
		const std::string &getPlaceholder(void) const;

		/**
		 * @brief Set the placeholder: the text that names the field, such
		 * as "Email".
		 *
		 * It is drawn by the label child, inside the field while it is
		 * empty and above the input once it floats. Marks the component
		 * changed only when the text differs.
		 * @param placeholder The new placeholder.
		 * @return This component, for chaining.
		 */
		TextField &setPlaceholder(const std::string &placeholder);

		/**
		 * @brief Get the label.
		 * @return The label, which the systems keep equal to the
		 * placeholder.
		 */
		const std::string &getLabel(void) const;

		/**
		 * @brief Store the label.
		 *
		 * systems::TextField and systems::Search overwrite it with the
		 * placeholder every frame, so set the placeholder instead. Marks
		 * the component changed only when the text differs.
		 * @param label The new label.
		 * @return This component, for chaining.
		 */
		TextField &setLabel(const std::string &label);

		/**
		 * @brief Get the helper text shown under the field.
		 * @return The supporting text; empty for none.
		 */
		const std::string &getSupportingText(void) const;

		/**
		 * @brief Set the helper text shown under the field, such as "At
		 * least 8 characters".
		 *
		 * While the field is in error and has an error text, the error
		 * text is shown in its place. Marks the component changed only
		 * when the text differs.
		 * @param supportingText The new supporting text.
		 * @return This component, for chaining.
		 */
		TextField &setSupportingText(const std::string &supportingText);

		/**
		 * @brief Get the fixed text shown before the input.
		 * @return The prefix; empty for none.
		 */
		const std::string &getPrefix(void) const;

		/**
		 * @brief Set the fixed text shown before the input, such as "$".
		 *
		 * Marks the component changed only when the text differs.
		 * @param prefix The new prefix.
		 * @return This component, for chaining.
		 */
		TextField &setPrefix(const std::string &prefix);

		/**
		 * @brief Get the fixed text shown after the input.
		 * @return The suffix; empty for none.
		 */
		const std::string &getSuffix(void) const;

		/**
		 * @brief Set the fixed text shown after the input, such as "kg".
		 *
		 * Marks the component changed only when the text differs.
		 * @param suffix The new suffix.
		 * @return This component, for chaining.
		 */
		TextField &setSuffix(const std::string &suffix);

		/**
		 * @brief Get the message shown under the field while it is in
		 * error.
		 * @return The error text; empty for none.
		 */
		const std::string &getErrorText(void) const;

		/**
		 * @brief Set the message shown under the field while it is in
		 * error.
		 *
		 * It replaces the supporting text only while isError() is true.
		 * Marks the component changed only when the text differs.
		 * @param errorText The new error text.
		 * @return This component, for chaining.
		 */
		TextField &setErrorText(const std::string &errorText);

		/**
		 * @brief Tell whether the field is in error.
		 * @return True when in error.
		 */
		bool isError(void) const;

		/**
		 * @brief Put the field in error or clear the error.
		 *
		 * A field in error is drawn with the error colors, shows its error
		 * icon in place of the trailing icon and its error text in place
		 * of the supporting text. Marks the component changed only when
		 * the flag differs.
		 * @param error True when the content is invalid.
		 * @return This component, for chaining.
		 */
		TextField &setError(bool error);

		/**
		 * @brief Tell whether the label is drawn small, above the input.
		 * @return True when the label floats; false when it rests in the
		 * place of the input.
		 */
		bool isLabelFloating(void) const;

		/**
		 * @brief Store whether the label floats above the input.
		 *
		 * The systems overwrite it every frame: the label of a text field
		 * widget floats while the field is focused or has content; the
		 * label of any other entity with this component floats while it
		 * has content or is in error. Marks the component changed only
		 * when the flag differs.
		 * @param floating True when the label floats.
		 * @return This component, for chaining.
		 */
		TextField &setLabelFloating(bool floating);
	};

}	 // namespace guillaume::components
