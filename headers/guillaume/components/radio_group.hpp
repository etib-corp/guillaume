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
	 * @brief Configuration of a radio group.
	 *
	 * This component is the whole state of a radio group: its option labels,
	 * the selected option and the radio button child of each option.
	 * systems::RadioGroup turns it into the selection of those children.
	 */
	class RadioGroup: public ecs::Component
	{
		public:
		/// Called with the option index when the user selects another option.
		using ValueChangeHandler = std::function<void(std::size_t)>;

		/// Vertical space between 20 dp rings so their 40 dp targets do not
		/// overlap, in dp.
		static constexpr float RowSpacing = 20.0f;

		private:
		std::vector<std::string> _options {};	 ///< Option labels
		std::optional<std::size_t> _value {};	 ///< Selected option
		std::vector<ecs::Entity::Identifier>
			_buttons {};	///< Radio button child of each option
		ValueChangeHandler _onValueChange {};	 ///< User selection callback

		public:
		/**
		 * @brief Default constructor.
		 */
		RadioGroup(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~RadioGroup(void) = default;

		/** @brief Get the option labels. @return The labels. */
		const std::vector<std::string> &getOptions(void) const noexcept;

		/** @brief Set the option labels. @param options They. @return It. */
		RadioGroup &setOptions(const std::vector<std::string> &options);

		/** @brief Get the selected option. @return Its index, or none. */
		std::optional<std::size_t> getValue(void) const noexcept;

		/** @brief Set the selected option. @param value Its index, or none.
		 * @return It. */
		RadioGroup &setValue(std::optional<std::size_t> value);

		/** @brief Get the radio button children. @return Their identifiers,
		 * in option order. */
		const std::vector<ecs::Entity::Identifier> &
			getButtons(void) const noexcept;

		/** @brief Set the radio button children. @param buttons Their
		 * identifiers, in option order. @return It. */
		RadioGroup &
			setButtons(const std::vector<ecs::Entity::Identifier> &buttons);

		/** @brief Get the selection callback. @return The callback. */
		const ValueChangeHandler &getOnValueChange(void) const noexcept;

		/** @brief Set the selection callback. @param handler It. @return It.
		 */
		RadioGroup &setOnValueChange(ValueChangeHandler handler);
	};

}	 // namespace guillaume::components
