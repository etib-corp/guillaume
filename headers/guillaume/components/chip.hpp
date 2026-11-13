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
	 * @brief Configuration of a chip.
	 *
	 * This component is the whole state of a chip, apart from its selected
	 * state (components::Selectable) and enabled state
	 * (components::StyleState). systems::Chip turns it into the chip's
	 * Color, Borders, Elevation, Layout and Accessibility, and into the
	 * color and arrangement of its icon and label children.
	 */
	class Chip: public ecs::Component
	{
		public:
		/**
		 * @brief Chip variant.
		 */
		enum class Variant { Assist, Filter, Input, Suggestion };

		/**
		 * @brief Paper chip mode: tonal flat (default) or outlined.
		 */
		enum class Mode { Flat, Outlined };

		/// Called when the chip or its close icon is activated.
		using Handler = std::function<void(void)>;

		/// Glyph of the leading icon of an Assist chip without one.
		static constexpr const char *DefaultAssistGlyph = "event";

		/// Glyph of the selected check.
		static constexpr const char *CheckGlyph = "check";

		/// Glyph of the trailing close icon.
		static constexpr const char *CloseGlyph = "close";

		/// Font size of the icons, in dp.
		static constexpr float IconSize = 18.0f;

		/// Padding around the content, in dp.
		static constexpr float Padding = 12.0f;

		/// Padding around the content of a compact chip, in dp.
		static constexpr float CompactPadding = 8.0f;

		private:
		Variant _variant { Variant::Assist };	 ///< Chip variant
		Mode _mode { Mode::Flat };				 ///< Chip mode
		std::string _label {};					 ///< Label content
		std::string _iconGlyph {};				 ///< Leading icon glyph
		ecs::Entity::Identifier _leadingIcon {
			ecs::Entity::InvalidIdentifier
		};	  ///< Leading icon child

		ecs::Entity::Identifier _trailingIcon {
			ecs::Entity::InvalidIdentifier
		};	  ///< Trailing close icon child

		ecs::Entity::Identifier _labelEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Label child

		ecs::Entity::Identifier _checkIcon {
			ecs::Entity::InvalidIdentifier
		};	  ///< Selected check child

		bool _compact { false };				///< Compact padding
		bool _elevated { false };				///< Level-1 shadow
		bool _showSelectedOverlay { false };	///< Selected state layer
		bool _showSelectedCheck { true };		///< Check when selected
		Handler _onPress {};					///< Activation callback
		Handler _onClose {};					///< Close-icon callback

		public:
		/**
		 * @brief Default constructor.
		 */
		Chip(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Chip(void) = default;

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const noexcept;

		/** @brief Set the variant. @param variant It. @return It. */
		Chip &setVariant(Variant variant);

		/** @brief Get the mode. @return The mode. */
		Mode getMode(void) const noexcept;

		/** @brief Set the mode. @param mode It. @return It. */
		Chip &setMode(Mode mode);

		/** @brief Get the label content. @return The label. */
		const std::string &getLabel(void) const noexcept;

		/** @brief Set the label content. @param label It. @return It. */
		Chip &setLabel(const std::string &label);

		/** @brief Get the leading icon glyph. @return The glyph name. */
		const std::string &getIconGlyph(void) const noexcept;

		/** @brief Set the leading icon glyph. @param iconGlyph It. @return It.
		 */
		Chip &setIconGlyph(const std::string &iconGlyph);

		/** @brief Get the leading icon child. @return Its identifier. */
		ecs::Entity::Identifier getLeadingIcon(void) const noexcept;

		/** @brief Set the leading icon child. @param identifier It. @return
		 * It. */
		Chip &setLeadingIcon(ecs::Entity::Identifier identifier);

		/** @brief Get the trailing close icon child. @return Its identifier.
		 */
		ecs::Entity::Identifier getTrailingIcon(void) const noexcept;

		/** @brief Set the trailing close icon child. @param identifier It.
		 * @return It. */
		Chip &setTrailingIcon(ecs::Entity::Identifier identifier);

		/** @brief Get the label child. @return Its identifier. */
		ecs::Entity::Identifier getLabelEntity(void) const noexcept;

		/** @brief Set the label child. @param identifier It. @return It. */
		Chip &setLabelEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the selected check child. @return Its identifier. */
		ecs::Entity::Identifier getCheckIcon(void) const noexcept;

		/** @brief Set the selected check child. @param identifier It. @return
		 * It. */
		Chip &setCheckIcon(ecs::Entity::Identifier identifier);

		/** @brief Whether the padding is compact. @return True if so. */
		bool isCompact(void) const noexcept;

		/** @brief Set the compact padding. @param compact It. @return It. */
		Chip &setCompact(bool compact);

		/** @brief Whether a level-1 shadow is painted. @return True if so. */
		bool isElevated(void) const noexcept;

		/** @brief Set the level-1 shadow. @param elevated It. @return It. */
		Chip &setElevated(bool elevated);

		/** @brief Whether a state layer is overlaid while selected. @return
		 * True if so. */
		bool isShowSelectedOverlay(void) const noexcept;

		/** @brief Set the selected state layer. @param show It. @return It. */
		Chip &setShowSelectedOverlay(bool show);

		/** @brief Whether the check is shown while selected. @return True if
		 * so. */
		bool isShowSelectedCheck(void) const noexcept;

		/** @brief Set the selected check. @param show It. @return It. */
		Chip &setShowSelectedCheck(bool show);

		/** @brief Get the activation callback. @return The callback. */
		const Handler &getOnPress(void) const noexcept;

		/** @brief Set the activation callback. @param handler It. @return It.
		 */
		Chip &setOnPress(Handler handler);

		/** @brief Get the close-icon callback. @return The callback. */
		const Handler &getOnClose(void) const noexcept;

		/** @brief Set the close-icon callback. @param handler It. @return It.
		 */
		Chip &setOnClose(Handler handler);
	};

}	 // namespace guillaume::components
