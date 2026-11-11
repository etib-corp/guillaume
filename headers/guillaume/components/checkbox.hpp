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
	 * @brief State of a checkbox: checked state, variant, label, color
	 * overrides and the identifiers of its check glyph and label children.
	 *
	 * Role in the frame:
	 * - entities::Checkbox writes the configuration, creates the check
	 *   glyph and label children and records their identifiers here.
	 * - systems::Checkbox (Layout phase) reads it and writes the box's
	 *   Color, Borders, Line (the indeterminate dash) and Accessibility,
	 *   shows or hides the check glyph and places the label to the right of
	 *   the box.
	 *
	 * The entity also has a components::Selectable that mirrors the state
	 * (selected unless Unchecked). setState() here does not update it: to
	 * change the state from code call systems::Checkbox::setState(), which
	 * writes both and restyles at once. A user press goes through
	 * systems::Checkbox::toggle(), which also runs the value-change
	 * callback.
	 *
	 * @code
	 * systems::Checkbox::setState(registry, identifier,
	 * 	components::Checkbox::State::Indeterminate);
	 * @endcode
	 *
	 * @see systems::Checkbox, components::Selectable
	 */
	class Checkbox: public ecs::Component
	{
		public:
		/**
		 * @brief Checked state.
		 *
		 * Indeterminate draws a dash in a filled box. A user press on an
		 * Indeterminate or Unchecked box checks it.
		 */
		enum class State { Unchecked, Checked, Indeterminate };

		/**
		 * @brief Visual variant.
		 *
		 * Enabled is the regular look. Disabled greys the checkbox and makes
		 * it ignore presses. Error draws the box and the label in the error
		 * color and ignores the color overrides.
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
		std::string _label {};					  ///< Label text
		ecs::Entity::Identifier _checkIcon {
			ecs::Entity::InvalidIdentifier
		};	  ///< Check glyph child, or InvalidIdentifier

		ecs::Entity::Identifier _labelEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Label child, or InvalidIdentifier

		std::optional<utility::graphic::Color32Bit>
			_checkedColor {};	 ///< Fill override when checked
		std::optional<utility::graphic::Color32Bit>
			_uncheckedColor {};	   ///< Outline override when unchecked
		ValueChangeHandler _onValueChange {};	 ///< User toggle callback

		public:
		/**
		 * @brief Create the state of an enabled, unchecked checkbox with no
		 * label.
		 */
		Checkbox(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Checkbox(void) = default;

		/**
		 * @brief Get the checked state.
		 * @return Unchecked (the default), Checked or Indeterminate.
		 */
		State getState(void) const noexcept;

		/**
		 * @brief Store the checked state.
		 *
		 * Marks the component changed when the state differs. It does not
		 * update the entity's Selectable nor run the callback; prefer
		 * systems::Checkbox::setState().
		 * @param state The new state.
		 * @return This component, for chaining.
		 */
		Checkbox &setState(State state);

		/**
		 * @brief Get the visual variant.
		 * @return The variant (Enabled by default).
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Choose the visual variant.
		 *
		 * Marks the component changed when the variant differs. The system
		 * enables or disables the entity's interaction to match.
		 * @param variant Enabled, Disabled or Error.
		 * @return This component, for chaining.
		 */
		Checkbox &setVariant(Variant variant);

		/**
		 * @brief Get the label text.
		 * @return The text, empty when the checkbox has no label.
		 */
		const std::string &getLabel(void) const noexcept;

		/**
		 * @brief Set the label text.
		 *
		 * Marks the component changed when the text differs. The system
		 * copies it to the checkbox's Accessibility label; the text drawn
		 * on screen is the one of the label child.
		 * @param label The label text.
		 * @return This component, for chaining.
		 */
		Checkbox &setLabel(const std::string &label);

		/**
		 * @brief Get the child entity that draws the check glyph.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * checkbox has no check glyph child.
		 */
		ecs::Entity::Identifier getCheckIcon(void) const noexcept;

		/**
		 * @brief Record which child entity draws the check glyph.
		 *
		 * Marks the component changed when the identifier differs. The
		 * system makes the glyph transparent unless the state is Checked.
		 * @param identifier The check glyph child.
		 * @return This component, for chaining.
		 */
		Checkbox &setCheckIcon(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child entity that draws the label.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * checkbox has no label child.
		 */
		ecs::Entity::Identifier getLabelEntity(void) const noexcept;

		/**
		 * @brief Record which child entity draws the label.
		 *
		 * Marks the component changed when the identifier differs.
		 * @param identifier The label child.
		 * @return This component, for chaining.
		 */
		Checkbox &setLabelEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the color that replaces the themed fill of a checked
		 * box.
		 * @return The color, or std::nullopt when the theme decides.
		 */
		const std::optional<utility::graphic::Color32Bit> &
			getCheckedColor(void) const noexcept;

		/**
		 * @brief Replace the themed fill of a checked or indeterminate box.
		 *
		 * Marks the component changed when the color differs. Ignored by
		 * the Disabled and Error variants.
		 * @param color The fill color, or std::nullopt to follow the theme
		 * (the primary color).
		 * @return This component, for chaining.
		 */
		Checkbox &
			setCheckedColor(std::optional<utility::graphic::Color32Bit> color);

		/**
		 * @brief Get the color that replaces the themed outline of an
		 * unchecked box.
		 * @return The color, or std::nullopt when the theme decides.
		 */
		const std::optional<utility::graphic::Color32Bit> &
			getUncheckedColor(void) const noexcept;

		/**
		 * @brief Replace the themed outline of an unchecked box.
		 *
		 * Marks the component changed when the color differs. Ignored by
		 * the Disabled and Error variants.
		 * @param color The outline color, or std::nullopt to follow the
		 * theme.
		 * @return This component, for chaining.
		 */
		Checkbox &setUncheckedColor(
			std::optional<utility::graphic::Color32Bit> color);

		/**
		 * @brief Get the callback run when the user toggles the box.
		 * @return The callback, empty when none is set.
		 */
		const ValueChangeHandler &getOnValueChange(void) const noexcept;

		/**
		 * @brief Set the callback run when the user toggles the box.
		 *
		 * It is not run when the state is set from code. Does not mark the
		 * component changed.
		 * @param handler The callback; it receives the new state.
		 * @return This component, for chaining.
		 */
		Checkbox &setOnValueChange(ValueChangeHandler handler);
	};

}	 // namespace guillaume::components
