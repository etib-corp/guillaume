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
	 * @brief Component holding the input-container state of a text field.
	 *
	 * Carries the placeholder/label, supporting text, prefix/suffix and error
	 * state so `entities::TextField` and `entities::Search` can be thin
	 * composites over `components::Text`, `systems::TextInput` and
	 * `systems::KeyboardControl`.
	 *
	 * @see systems::TextField
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
		 * @brief Default constructor for the TextField component.
		 */
		TextField(void) = default;

		/**
		 * @brief Default destructor for the TextField component.
		 */
		~TextField(void) = default;

		/**
		 * @brief Get the placeholder text.
		 * @return The placeholder.
		 */
		const std::string &getPlaceholder(void) const;

		/**
		 * @brief Set the placeholder text.
		 * @param placeholder The new placeholder.
		 * @return Reference to this TextField for chaining.
		 */
		TextField &setPlaceholder(const std::string &placeholder);

		/**
		 * @brief Get the floating label.
		 * @return The label.
		 */
		const std::string &getLabel(void) const;

		/**
		 * @brief Set the floating label.
		 * @param label The new label.
		 * @return Reference to this TextField for chaining.
		 */
		TextField &setLabel(const std::string &label);

		/**
		 * @brief Get the supporting text.
		 * @return The supporting text.
		 */
		const std::string &getSupportingText(void) const;

		/**
		 * @brief Set the supporting text.
		 * @param supportingText The new supporting text.
		 * @return Reference to this TextField for chaining.
		 */
		TextField &setSupportingText(const std::string &supportingText);

		/**
		 * @brief Get the leading affix text.
		 * @return The prefix.
		 */
		const std::string &getPrefix(void) const;

		/**
		 * @brief Set the leading affix text.
		 * @param prefix The new prefix.
		 * @return Reference to this TextField for chaining.
		 */
		TextField &setPrefix(const std::string &prefix);

		/**
		 * @brief Get the trailing affix text.
		 * @return The suffix.
		 */
		const std::string &getSuffix(void) const;

		/**
		 * @brief Set the trailing affix text.
		 * @param suffix The new suffix.
		 * @return Reference to this TextField for chaining.
		 */
		TextField &setSuffix(const std::string &suffix);

		/**
		 * @brief Get the error text.
		 * @return The error text.
		 */
		const std::string &getErrorText(void) const;

		/**
		 * @brief Set the error text.
		 * @param errorText The new error text.
		 * @return Reference to this TextField for chaining.
		 */
		TextField &setErrorText(const std::string &errorText);

		/**
		 * @brief Check whether the field is in an error state.
		 * @return True when in error.
		 */
		bool isError(void) const;

		/**
		 * @brief Set the error state.
		 * @param error The new error state.
		 * @return Reference to this TextField for chaining.
		 */
		TextField &setError(bool error);

		/**
		 * @brief Check whether the label is floated.
		 * @return True when the label is floated.
		 */
		bool isLabelFloating(void) const;

		/**
		 * @brief Set whether the label is floated.
		 * @param floating Whether the label is floated.
		 * @return Reference to this TextField for chaining.
		 */
		TextField &setLabelFloating(bool floating);
	};

}	 // namespace guillaume::components
