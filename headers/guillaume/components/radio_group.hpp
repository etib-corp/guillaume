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

#include <cstddef>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief State of a radio group: a column of radio buttons of which at
	 * most one is selected.
	 *
	 * It holds the label of each option, the index of the selected option,
	 * the selection callback and the identifier of the radio button child
	 * of each option.
	 *
	 * Role in the frame:
	 * - entities::RadioGroup writes the options, creates one radio button
	 *   child per option and records their identifiers here.
	 * - systems::RadioGroup (Layout phase) reads the value and selects the
	 *   matching child, deselecting the others. Its `press()` writes the
	 *   value when the user presses a radio.
	 *
	 * setValue() only stores the index. To select an option from code and
	 * see it at once, call systems::RadioGroup::select(), which throws
	 * std::out_of_range for an index past the options.
	 *
	 * @see systems::RadioGroup, components::RadioButton
	 */
	class RadioGroup: public ecs::Component
	{
		public:
		/// Called with the index of the newly selected option when the user
		/// selects another option than the current one.
		using ValueChangeHandler = std::function<void(std::size_t)>;

		/// Vertical space between 20 dp rings so their 40 dp targets do not
		/// overlap, in dp.
		static constexpr float RowSpacing = 20.0f;

		private:
		std::vector<std::string> _options {};	 ///< Option labels, in order
		std::optional<std::size_t> _value {};	 ///< Selected option index
		std::vector<ecs::Entity::Identifier>
			_buttons {};	///< Radio button child of each option
		ValueChangeHandler _onValueChange {};	 ///< User selection callback

		public:
		/**
		 * @brief Create the state of a group with no option and no
		 * selection.
		 */
		RadioGroup(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~RadioGroup(void) = default;

		/**
		 * @brief Get the label of each option.
		 * @return The labels, top to bottom.
		 */
		const std::vector<std::string> &getOptions(void) const noexcept;

		/**
		 * @brief Replace the label of each option.
		 *
		 * Marks the component changed when the list differs. The widget
		 * class creates one radio button child per label.
		 * @param options The labels, top to bottom.
		 * @return This component, for chaining.
		 */
		RadioGroup &setOptions(const std::vector<std::string> &options);

		/**
		 * @brief Get the selected option.
		 * @return Its index in the options, or std::nullopt when nothing is
		 * selected.
		 */
		std::optional<std::size_t> getValue(void) const noexcept;

		/**
		 * @brief Store the selected option.
		 *
		 * Marks the component changed when the value differs. The index is
		 * not checked here, and the selection callback is not run.
		 * @param value The index of the option, or std::nullopt to select
		 * nothing.
		 * @return This component, for chaining.
		 */
		RadioGroup &setValue(std::optional<std::size_t> value);

		/**
		 * @brief Get the radio button child of each option.
		 * @return Their identifiers, in option order; entry `i` is the radio
		 * of `getOptions()[i]`.
		 */
		const std::vector<ecs::Entity::Identifier> &
			getButtons(void) const noexcept;

		/**
		 * @brief Record the radio button child of each option.
		 *
		 * Marks the component changed when the list differs.
		 * @param buttons Their identifiers, in option order.
		 * @return This component, for chaining.
		 */
		RadioGroup &
			setButtons(const std::vector<ecs::Entity::Identifier> &buttons);

		/**
		 * @brief Get the callback run when the user changes the selection.
		 * @return The callback, empty when none is set.
		 */
		const ValueChangeHandler &getOnValueChange(void) const noexcept;

		/**
		 * @brief Set the callback run when the user changes the selection.
		 *
		 * It is not run when the user presses the option that is already
		 * selected, nor for a selection made from code. Does not mark the
		 * component changed.
		 * @param handler The callback; it receives the index of the newly
		 * selected option.
		 * @return This component, for chaining.
		 */
		RadioGroup &setOnValueChange(ValueChangeHandler handler);
	};

}	 // namespace guillaume::components
