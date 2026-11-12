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
	 * @brief Configuration of a switch.
	 *
	 * This component is the whole state of a switch besides its on/off value
	 * (components::Value). systems::Switch turns it into the track's Color,
	 * Borders and Accessibility, and into the color and position of its thumb,
	 * thumb icon and label.
	 */
	class Switch: public ecs::Component
	{
		public:
		/**
		 * @brief Visual variant.
		 */
		enum class Variant { Enabled, Disabled, WithIcon };

		/// Called with the new value when the user toggles the switch.
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
		std::string _label {};					  ///< Label content
		ecs::Entity::Identifier _thumb {
			ecs::Entity::InvalidIdentifier
		};	  ///< Thumb child

		ecs::Entity::Identifier _icon {
			ecs::Entity::InvalidIdentifier
		};	  ///< Thumb icon child

		ecs::Entity::Identifier _labelEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Label child

		ValueChangeHandler _onValueChange {};	 ///< User toggle callback

		public:
		/**
		 * @brief Default constructor.
		 */
		Switch(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Switch(void) = default;

		/** @brief Get: visual variant. @return The value. */
		Variant getVariant(void) const noexcept;

		/** @brief Set: visual variant. @param variant The value. @return It. */
		Switch &setVariant(Variant variant);

		/** @brief Get: thumb icon glyph name. @return The value. */
		const std::string &getIconGlyph(void) const noexcept;

		/** @brief Set: thumb icon glyph name. @param glyph The value. @return
		 * It. */
		Switch &setIconGlyph(const std::string &glyph);

		/** @brief Get: label content. @return The value. */
		const std::string &getLabel(void) const noexcept;

		/** @brief Set: label content. @param label The value. @return It. */
		Switch &setLabel(const std::string &label);

		/** @brief Get: thumb child. @return The value. */
		ecs::Entity::Identifier getThumb(void) const noexcept;

		/** @brief Set: thumb child. @param identifier The value. @return It. */
		Switch &setThumb(ecs::Entity::Identifier identifier);

		/** @brief Get: thumb icon child. @return The value. */
		ecs::Entity::Identifier getIcon(void) const noexcept;

		/** @brief Set: thumb icon child. @param identifier The value. @return
		 * It. */
		Switch &setIcon(ecs::Entity::Identifier identifier);

		/** @brief Get: label child. @return The value. */
		ecs::Entity::Identifier getLabelEntity(void) const noexcept;

		/** @brief Set: label child. @param identifier The value. @return It. */
		Switch &setLabelEntity(ecs::Entity::Identifier identifier);

		/** @brief Get: user toggle callback. @return The value. */
		const ValueChangeHandler &getOnValueChange(void) const noexcept;

		/** @brief Set: user toggle callback. @param handler The value. @return
		 * It. */
		Switch &setOnValueChange(ValueChangeHandler handler);
	};

}	 // namespace guillaume::components
