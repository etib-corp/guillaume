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

#include <optional>
#include <string>

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief Configuration of a badge.
	 *
	 * This component is the whole state of a badge, including the transient
	 * state of its visibility fade. systems::Badge turns it into the badge's
	 * Layout, Borders and Color, and into the font size, content and color
	 * of its label.
	 */
	class Badge: public ecs::Component
	{
		public:
		/**
		 * @brief Badge variants.
		 */
		enum class Variant {
			Dot,	  ///< 6 dp dot without label.
			Small,	  ///< 16 dp label badge.
			Large	  ///< 20 dp label badge (Paper default).
		};

		/// Height of the Dot variant, in dp.
		static constexpr float DotSize = 6.0f;

		/// Height of the Small variant, in dp.
		static constexpr float SmallSize = 16.0f;

		/// Height of the Large variant, in dp.
		static constexpr float LargeSize = 20.0f;

		/// Space on each side of the label, in dp (Paper Badge padding).
		static constexpr float HorizontalPadding = 3.0f;

		/// Label font size as a fraction of the badge height (Paper).
		static constexpr float LabelScale = 0.5f;

		private:
		Variant _variant { Variant::Large };	///< Badge variant
		std::string _label {};					///< Label content
		ecs::Entity::Identifier _labelEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Label child

		std::optional<float> _size {};	  ///< Size override
		std::optional<utility::graphic::Color32Bit>
			_containerColor {};	   ///< Container color override
		std::optional<utility::graphic::Color32Bit>
			_contentColor {};		 ///< Label color override
		bool _visible { true };		 ///< Target visibility
		float _opacity { 1.0f };	 ///< Current fade opacity
		float _fadeFrom { 1.0f };	 ///< Opacity at the start of the fade

		public:
		/**
		 * @brief Default constructor.
		 */
		Badge(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Badge(void) = default;

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const noexcept;

		/** @brief Set the variant. @param variant It. @return It. */
		Badge &setVariant(Variant variant);

		/** @brief Get the label content. @return The label. */
		const std::string &getLabel(void) const noexcept;

		/** @brief Set the label content. @param label It. @return It. */
		Badge &setLabel(const std::string &label);

		/** @brief Get the label child. @return Its identifier. */
		ecs::Entity::Identifier getLabelEntity(void) const noexcept;

		/** @brief Set the label child. @param identifier It. @return It. */
		Badge &setLabelEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the size override. @return The size in dp, if any. */
		const std::optional<float> &getSize(void) const noexcept;

		/** @brief Set the size override. @param size It. @return It. */
		Badge &setSize(std::optional<float> size);

		/** @brief Get the container color override. @return It, if any. */
		const std::optional<utility::graphic::Color32Bit> &
			getContainerColor(void) const noexcept;

		/** @brief Set the container color override. @param color It. @return
		 * It. */
		Badge &setContainerColor(
			std::optional<utility::graphic::Color32Bit> color);

		/** @brief Get the label color override. @return It, if any. */
		const std::optional<utility::graphic::Color32Bit> &
			getContentColor(void) const noexcept;

		/** @brief Set the label color override. @param color It. @return It.
		 */
		Badge &
			setContentColor(std::optional<utility::graphic::Color32Bit> color);

		/** @brief Get the target visibility. @return True when visible. */
		bool isVisible(void) const noexcept;

		/** @brief Set the target visibility. @param visible It. @return It. */
		Badge &setVisible(bool visible);

		/** @brief Get the current fade opacity. @return It, in [0, 1]. */
		float getOpacity(void) const noexcept;

		/** @brief Set the current fade opacity. @param opacity It. @return It.
		 */
		Badge &setOpacity(float opacity);

		/** @brief Get the opacity the running fade started from. @return It. */
		float getFadeFrom(void) const noexcept;

		/** @brief Set the opacity the fade starts from. @param opacity It.
		 * @return It. */
		Badge &setFadeFrom(float opacity);
	};

}	 // namespace guillaume::components
