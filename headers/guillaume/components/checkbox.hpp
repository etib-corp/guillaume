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

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief Configuration of a checkbox.
	 *
	 * This component is the whole state of a checkbox. systems::Checkbox
	 * turns it into the box's Color, Borders, Line and Accessibility, and
	 * into the color and position of its check glyph and label.
	 */
	class Checkbox: public ecs::Component
	{
		public:
		/**
		 * @brief Checked state.
		 */
		enum class State { Unchecked, Checked, Indeterminate };

		/**
		 * @brief Visual variant.
		 */
		enum class Variant { Enabled, Disabled, Error };

		/// Called with the new state when the user toggles the box.
		using ValueChangeHandler = std::function<void(State)>;

		/// Edge of the square touch target centered on the box, in dp.
		static constexpr float TouchTarget = 40.0f;

		/// Gap between the touch target and the label, in dp.
		static constexpr float LabelGap = 4.0f;

		private:
		State _state { State::Unchecked };		  ///< Checked state
		Variant _variant { Variant::Enabled };	  ///< Visual variant
		std::string _label {};					  ///< Label content
		ecs::Entity::Identifier _checkIcon {
			ecs::Entity::InvalidIdentifier
		};	  ///< Check glyph child
		ecs::Entity::Identifier _labelEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Label child
		std::optional<utility::graphic::Color32Bit>
			_checkedColor {};	 ///< Fill override when checked
		std::optional<utility::graphic::Color32Bit>
			_uncheckedColor {};	   ///< Outline override when unchecked
		ValueChangeHandler _onValueChange {};	 ///< User toggle callback

		public:
		/**
		 * @brief Default constructor.
		 */
		Checkbox(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Checkbox(void) = default;

		/** @brief Get the checked state. @return The state. */
		State getState(void) const noexcept;

		/** @brief Set the checked state. @param state It. @return It. */
		Checkbox &setState(State state);

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const noexcept;

		/** @brief Set the variant. @param variant It. @return It. */
		Checkbox &setVariant(Variant variant);

		/** @brief Get the label content. @return The label. */
		const std::string &getLabel(void) const noexcept;

		/** @brief Set the label content. @param label It. @return It. */
		Checkbox &setLabel(const std::string &label);

		/** @brief Get the check glyph child. @return Its identifier. */
		ecs::Entity::Identifier getCheckIcon(void) const noexcept;

		/** @brief Set the check glyph child. @param identifier It. @return It.
		 */
		Checkbox &setCheckIcon(ecs::Entity::Identifier identifier);

		/** @brief Get the label child. @return Its identifier. */
		ecs::Entity::Identifier getLabelEntity(void) const noexcept;

		/** @brief Set the label child. @param identifier It. @return It. */
		Checkbox &setLabelEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the checked fill override. @return The color, if any. */
		const std::optional<utility::graphic::Color32Bit> &
			getCheckedColor(void) const noexcept;

		/** @brief Set the checked fill override. @param color It. @return It.
		 */
		Checkbox &
			setCheckedColor(std::optional<utility::graphic::Color32Bit> color);

		/** @brief Get the unchecked outline override. @return The color. */
		const std::optional<utility::graphic::Color32Bit> &
			getUncheckedColor(void) const noexcept;

		/** @brief Set the unchecked outline override. @param color It. @return
		 * It. */
		Checkbox &setUncheckedColor(
			std::optional<utility::graphic::Color32Bit> color);

		/** @brief Get the toggle callback. @return The callback. */
		const ValueChangeHandler &getOnValueChange(void) const noexcept;

		/** @brief Set the toggle callback. @param handler It. @return It. */
		Checkbox &setOnValueChange(ValueChangeHandler handler);
	};

}	 // namespace guillaume::components
