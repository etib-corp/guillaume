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
	 * @brief Configuration of a radio button.
	 *
	 * This component is the whole state of a radio button besides its
	 * selection (components::Selectable). systems::RadioButton turns it into
	 * the ring's Color, Ring and Accessibility, and into the color and position
	 * of its dot and label.
	 */
	class RadioButton: public ecs::Component
	{
		public:
		/**
		 * @brief Visual variant.
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
		std::string _label {};					  ///< Label content
		ecs::Entity::Identifier _dot {
			ecs::Entity::InvalidIdentifier
		};	  ///< Inner dot child
		ecs::Entity::Identifier _labelEntity {
			ecs::Entity::InvalidIdentifier
		};							 ///< Label child
		PressHandler _onPress {};	 ///< User press callback

		public:
		/**
		 * @brief Default constructor.
		 */
		RadioButton(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~RadioButton(void) = default;

		/** @brief Get: visual variant. @return The value. */
		Variant getVariant(void) const noexcept;

		/** @brief Set: visual variant. @param variant The value. @return It. */
		RadioButton &setVariant(Variant variant);

		/** @brief Get: label content. @return The value. */
		const std::string &getLabel(void) const noexcept;

		/** @brief Set: label content. @param label The value. @return It. */
		RadioButton &setLabel(const std::string &label);

		/** @brief Get: inner dot child. @return The value. */
		ecs::Entity::Identifier getDot(void) const noexcept;

		/** @brief Set: inner dot child. @param identifier The value. @return
		 * It. */
		RadioButton &setDot(ecs::Entity::Identifier identifier);

		/** @brief Get: label child. @return The value. */
		ecs::Entity::Identifier getLabelEntity(void) const noexcept;

		/** @brief Set: label child. @param identifier The value. @return It. */
		RadioButton &setLabelEntity(ecs::Entity::Identifier identifier);

		/** @brief Get: user press callback. @return The value. */
		const PressHandler &getOnPress(void) const noexcept;

		/** @brief Set: user press callback. @param handler The value. @return
		 * It. */
		RadioButton &setOnPress(PressHandler handler);
	};

}	 // namespace guillaume::components
