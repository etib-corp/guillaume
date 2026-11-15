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
	 * @brief State of a badge: a small dot or pill, usually holding a
	 * count, drawn over another widget.
	 *
	 * It holds the variant, the label, the size and color overrides, the
	 * identifier of the label child, and the state of the fade that shows
	 * or hides the badge.
	 *
	 * Role in the frame:
	 * - entities::Badge writes the configuration, creates the label child
	 *   and records its identifier here.
	 * - systems::Badge (Layout phase) reads it and writes the badge's
	 *   Layout size, Borders and Color, and the content, font size and
	 *   color of the label. While a fade runs, it writes the opacity here
	 *   through the entity's components::Animation.
	 *
	 * Watch out:
	 * - To show or hide a badge with its fade call
	 *   systems::Badge::setVisible(). setVisible() here only stores the
	 *   target and starts no fade.
	 * - The opacity and fade-from fields belong to systems::Badge and are
	 *   overwritten while a fade runs.
	 *
	 * Every setter compares before writing and marks the component changed
	 * only when the value differs.
	 *
	 * @see systems::Badge, components::Animation
	 */
	class Badge: public ecs::Component
	{
		public:
		/**
		 * @brief Kind of badge; it sets the height and whether a label is
		 * shown.
		 */
		enum class Variant {
			Dot,	  ///< 6 dp dot without label.
			Small,	  ///< 16 dp high badge with a label.
			Large	  ///< 20 dp high badge with a label (the default).
		};

		/// Height of the Dot variant, in dp.
		static constexpr float DotSize = 6.0f;

		/// Height of the Small variant, in dp.
		static constexpr float SmallSize = 16.0f;

		/// Height of the Large variant, in dp.
		static constexpr float LargeSize = 20.0f;

		/// Space on each side of the label, in dp.
		static constexpr float HorizontalPadding = 3.0f;

		/// Label font size as a fraction of the badge height.
		static constexpr float LabelScale = 0.5f;

		private:
		Variant _variant { Variant::Large };	///< Badge variant
		std::string _label {};					///< Label text
		ecs::Entity::Identifier _labelEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Label child, or InvalidIdentifier

		std::optional<float> _size {};	  ///< Height override, in dp
		std::optional<utility::graphic::Color32Bit>
			_containerColor {};	   ///< Container color override
		std::optional<utility::graphic::Color32Bit>
			_contentColor {};		 ///< Label color override
		bool _visible { true };		 ///< Target visibility
		float _opacity { 1.0f };	 ///< Current fade opacity, 0 to 1
		float _fadeFrom { 1.0f };	 ///< Opacity at the start of the fade

		public:
		/**
		 * @brief Create the state of a visible Large badge with no label.
		 */
		Badge(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Badge(void) = default;

		/**
		 * @brief Get the kind of badge.
		 * @return The variant (Large by default).
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Choose the kind of badge.
		 * @param variant Dot, Small or Large.
		 * @return This component, for chaining.
		 */
		Badge &setVariant(Variant variant);

		/**
		 * @brief Get the label text.
		 * @return The text, typically a count such as `"3"` or `"99+"`;
		 * empty for none.
		 */
		const std::string &getLabel(void) const noexcept;

		/**
		 * @brief Set the label text.
		 *
		 * The system copies it to the label child. A label wider than the
		 * badge height stretches the badge into a pill. The Dot variant
		 * shows no label.
		 * @param label The label text.
		 * @return This component, for chaining.
		 */
		Badge &setLabel(const std::string &label);

		/**
		 * @brief Get the child entity that draws the label.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * badge has no label child.
		 */
		ecs::Entity::Identifier getLabelEntity(void) const noexcept;

		/**
		 * @brief Record which child entity draws the label.
		 * @param identifier The label child, or
		 * ecs::Entity::InvalidIdentifier for none.
		 * @return This component, for chaining.
		 */
		Badge &setLabelEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the height that replaces the one of the variant.
		 * @return The height, in dp, or std::nullopt when the variant
		 * decides. Use systems::Badge::sizeOf() for the height really used.
		 */
		const std::optional<float> &getSize(void) const noexcept;

		/**
		 * @brief Replace the height of the variant.
		 *
		 * The label font size follows: it is LabelScale times the height.
		 * @param size The height, in dp, or std::nullopt to follow the
		 * variant.
		 * @return This component, for chaining.
		 */
		Badge &setSize(std::optional<float> size);

		/**
		 * @brief Get the color that replaces the themed container color.
		 * @return The color, or std::nullopt when the theme decides.
		 */
		const std::optional<utility::graphic::Color32Bit> &
			getContainerColor(void) const noexcept;

		/**
		 * @brief Replace the themed container (background) color.
		 * @param color The container color, or std::nullopt to follow the
		 * theme (the error color).
		 * @return This component, for chaining.
		 */
		Badge &setContainerColor(
			std::optional<utility::graphic::Color32Bit> color);

		/**
		 * @brief Get the color that replaces the themed label color.
		 * @return The color, or std::nullopt when the theme decides.
		 */
		const std::optional<utility::graphic::Color32Bit> &
			getContentColor(void) const noexcept;

		/**
		 * @brief Replace the themed label color.
		 * @param color The label color, or std::nullopt to follow the theme
		 * (the on-error color).
		 * @return This component, for chaining.
		 */
		Badge &
			setContentColor(std::optional<utility::graphic::Color32Bit> color);

		/**
		 * @brief Check whether the badge is shown or fading in.
		 * @return True when the badge is, or is becoming, visible.
		 */
		bool isVisible(void) const noexcept;

		/**
		 * @brief Store the visibility the fade heads to.
		 *
		 * It starts no fade and leaves the opacity untouched; use
		 * systems::Badge::setVisible() to show or hide the badge.
		 * @param visible True when the badge should end up shown.
		 * @return This component, for chaining.
		 */
		Badge &setVisible(bool visible);

		/**
		 * @brief Get the opacity the badge is drawn with.
		 * @return The opacity, 0 (hidden) to 1 (fully shown).
		 */
		float getOpacity(void) const noexcept;

		/**
		 * @brief Set the opacity the badge is drawn with.
		 *
		 * Written by systems::Badge on each sample of the fade. The system
		 * multiplies the alpha of the container and label colors by it.
		 * The value is stored as given, without clamping.
		 * @param opacity The opacity, 0 (hidden) to 1 (fully shown).
		 * @return This component, for chaining.
		 */
		Badge &setOpacity(float opacity);

		/**
		 * @brief Get the opacity the running fade started from.
		 * @return The starting opacity, 0 to 1.
		 */
		float getFadeFrom(void) const noexcept;

		/**
		 * @brief Record the opacity a fade starts from.
		 *
		 * Written by systems::Badge::setVisible(), so a fade interrupted
		 * half-way reverses from where it was.
		 * @param opacity The starting opacity, 0 to 1.
		 * @return This component, for chaining.
		 */
		Badge &setFadeFrom(float opacity);
	};

}	 // namespace guillaume::components
