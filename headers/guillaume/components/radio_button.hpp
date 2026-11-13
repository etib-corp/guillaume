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
#include <string>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief Configuration of a radio button: its variant, its label and
	 * the identifiers of its dot and label children.
	 *
	 * Whether the radio is selected is not stored here but in the
	 * components::Selectable of the same entity.
	 *
	 * Role in the frame:
	 * - entities::RadioButton writes the configuration, creates the dot and
	 *   label children and records their identifiers here.
	 * - systems::RadioButton (Layout phase) reads it with Selectable and
	 *   writes the ring's Color, Ring and Accessibility, shows or hides the
	 *   dot and places the label to the right of the ring.
	 *
	 * To select a radio from code call systems::RadioButton::select(); a
	 * user press goes through systems::RadioButton::press(), which also runs
	 * the press callback.
	 *
	 * @see systems::RadioButton, components::Selectable,
	 * components::RadioGroup
	 */
	class RadioButton: public ecs::Component
	{
		public:
		/**
		 * @brief Visual variant.
		 *
		 * Enabled is the regular look. Disabled greys the radio and makes it
		 * ignore presses. Error draws the ring, the dot and the label in the
		 * error color.
		 */
		enum class Variant { Enabled, Disabled, Error };

		/// Called when the user selects the radio.
		using PressHandler = std::function<void(void)>;

		/// Diameter of the selected dot, in dp.
		static constexpr float DotSize = 10.0f;

		/// Edge of the square touch target centered on the ring, in dp.
		static constexpr float TouchTarget = 40.0f;

		/// Gap between the touch target and the label, in dp.
		static constexpr float LabelGap = 4.0f;

		private:
		Variant _variant { Variant::Enabled };	  ///< Visual variant
		std::string _label {};					  ///< Label text
		ecs::Entity::Identifier _dot {
			ecs::Entity::InvalidIdentifier
		};	  ///< Inner dot child, or InvalidIdentifier

		ecs::Entity::Identifier _labelEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Label child, or InvalidIdentifier

		PressHandler _onPress {};	 ///< User press callback

		public:
		/**
		 * @brief Create the configuration of an enabled radio with no label.
		 */
		RadioButton(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~RadioButton(void) = default;

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
		RadioButton &setVariant(Variant variant);

		/**
		 * @brief Get the label text.
		 * @return The text, empty when the radio has no label.
		 */
		const std::string &getLabel(void) const noexcept;

		/**
		 * @brief Set the label text.
		 *
		 * Marks the component changed when the text differs. The system
		 * copies it to the radio's Accessibility label; the text drawn on
		 * screen is the one of the label child.
		 * @param label The label text.
		 * @return This component, for chaining.
		 */
		RadioButton &setLabel(const std::string &label);

		/**
		 * @brief Get the child entity drawn as the inner dot.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * radio has no dot child.
		 */
		ecs::Entity::Identifier getDot(void) const noexcept;

		/**
		 * @brief Record which child entity is the inner dot.
		 *
		 * Marks the component changed when the identifier differs. The
		 * system centers the dot in the ring and makes it transparent while
		 * the radio is not selected.
		 * @param identifier The dot child.
		 * @return This component, for chaining.
		 */
		RadioButton &setDot(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child entity that draws the label.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * radio has no label child.
		 */
		ecs::Entity::Identifier getLabelEntity(void) const noexcept;

		/**
		 * @brief Record which child entity draws the label.
		 *
		 * Marks the component changed when the identifier differs.
		 * @param identifier The label child.
		 * @return This component, for chaining.
		 */
		RadioButton &setLabelEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the callback run when the user selects the radio.
		 * @return The callback, empty when none is set.
		 */
		const PressHandler &getOnPress(void) const noexcept;

		/**
		 * @brief Set the callback run when the user selects the radio.
		 *
		 * It runs on every press of an enabled radio, after the selection
		 * was set, and not for a selection made from code. A radio inside a
		 * radio group has this callback replaced by the group's. Does not
		 * mark the component changed.
		 * @param handler The callback, or an empty one to remove it.
		 * @return This component, for chaining.
		 */
		RadioButton &setOnPress(PressHandler handler);
	};

}	 // namespace guillaume::components
