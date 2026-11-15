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
	 * @brief Configuration of a switch: its variant, its thumb icon, its
	 * label and the identifiers of its thumb, icon and label children.
	 *
	 * Whether the switch is on is not stored here but in the
	 * components::Value of the same entity: 1 is on, 0 is off.
	 *
	 * Role in the frame:
	 * - entities::Switch writes the configuration, creates the thumb, icon
	 *   and label children and records their identifiers here.
	 * - systems::Switch (Layout phase) reads it with Value and writes the
	 *   track's Color, Borders and Accessibility, slides the thumb to the
	 *   matching end of the track, and colors and places the icon and the
	 *   label.
	 *
	 * To switch from code call systems::Switch::setChecked(); a user press
	 * goes through systems::Switch::toggle(), which also runs the
	 * value-change callback.
	 *
	 * @see systems::Switch, components::Value
	 */
	class Switch: public ecs::Component
	{
		public:
		/**
		 * @brief Visual variant.
		 *
		 * Enabled is the regular look. Disabled greys the switch and makes
		 * it ignore presses. WithIcon is the variant of a switch whose thumb
		 * carries an icon; it is styled like Enabled.
		 */
		enum class Variant { Enabled, Disabled, WithIcon };

		/// Called with the new value (true when on) when the user toggles
		/// the switch.
		using ValueChangeHandler = std::function<void(bool)>;

		/// Diameter of the thumb, in dp.
		static constexpr float ThumbSize = 24.0f;

		/// Inset of the thumb from the track ends, in dp.
		static constexpr float ThumbInset = 4.0f;

		/// Gap between the track and the label, in dp.
		static constexpr float LabelGap = 16.0f;

		private:
		Variant _variant { Variant::Enabled };	  ///< Visual variant
		std::string _iconGlyph {};				  ///< Thumb icon glyph name
		std::string _label {};					  ///< Label text
		ecs::Entity::Identifier _thumb {
			ecs::Entity::InvalidIdentifier
		};	  ///< Thumb child, or InvalidIdentifier

		ecs::Entity::Identifier _icon {
			ecs::Entity::InvalidIdentifier
		};	  ///< Thumb icon child, or InvalidIdentifier

		ecs::Entity::Identifier _labelEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Label child, or InvalidIdentifier

		ValueChangeHandler _onValueChange {};	 ///< User toggle callback

		public:
		/**
		 * @brief Create the configuration of an enabled switch with no icon
		 * and no label.
		 */
		Switch(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Switch(void) = default;

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
		 * @param variant Enabled, Disabled or WithIcon.
		 * @return This component, for chaining.
		 */
		Switch &setVariant(Variant variant);

		/**
		 * @brief Get the name of the glyph drawn on the thumb.
		 * @return The glyph name, empty when the thumb has no icon.
		 */
		const std::string &getIconGlyph(void) const noexcept;

		/**
		 * @brief Set the name of the glyph drawn on the thumb.
		 *
		 * Marks the component changed when the name differs. The widget
		 * class reads it when it creates the icon child; an empty name
		 * creates none.
		 * @param glyph The glyph name in the icon font.
		 * @return This component, for chaining.
		 */
		Switch &setIconGlyph(const std::string &glyph);

		/**
		 * @brief Get the label text.
		 * @return The text, empty when the switch has no label.
		 */
		const std::string &getLabel(void) const noexcept;

		/**
		 * @brief Set the label text.
		 *
		 * Marks the component changed when the text differs. The system
		 * copies it to the switch's Accessibility label; the text drawn on
		 * screen is the one of the label child.
		 * @param label The label text.
		 * @return This component, for chaining.
		 */
		Switch &setLabel(const std::string &label);

		/**
		 * @brief Get the child entity drawn as the thumb.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * switch has no thumb child.
		 */
		ecs::Entity::Identifier getThumb(void) const noexcept;

		/**
		 * @brief Record which child entity is the thumb.
		 *
		 * Marks the component changed when the identifier differs.
		 * @param identifier The thumb child.
		 * @return This component, for chaining.
		 */
		Switch &setThumb(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child entity that draws the thumb icon.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * thumb has no icon.
		 */
		ecs::Entity::Identifier getIcon(void) const noexcept;

		/**
		 * @brief Record which child entity draws the thumb icon.
		 *
		 * Marks the component changed when the identifier differs. The
		 * system keeps the icon centered on the thumb.
		 * @param identifier The icon child.
		 * @return This component, for chaining.
		 */
		Switch &setIcon(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child entity that draws the label.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * switch has no label child.
		 */
		ecs::Entity::Identifier getLabelEntity(void) const noexcept;

		/**
		 * @brief Record which child entity draws the label.
		 *
		 * Marks the component changed when the identifier differs.
		 * @param identifier The label child.
		 * @return This component, for chaining.
		 */
		Switch &setLabelEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the callback run when the user toggles the switch.
		 * @return The callback, empty when none is set.
		 */
		const ValueChangeHandler &getOnValueChange(void) const noexcept;

		/**
		 * @brief Set the callback run when the user toggles the switch.
		 *
		 * It is not run when the value is set from code. Does not mark the
		 * component changed.
		 * @param handler The callback; it receives true when the switch was
		 * just turned on.
		 * @return This component, for chaining.
		 */
		Switch &setOnValueChange(ValueChangeHandler handler);
	};

}	 // namespace guillaume::components
