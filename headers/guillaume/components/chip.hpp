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
	 * @brief Configuration of a chip: a compact pill with a label and
	 * optional leading, check and close icons.
	 *
	 * It holds the variant, the mode, the label, the icon glyph, the look
	 * options, the callbacks and the identifiers of the chip's children.
	 * Two pieces of state live in other components of the same entity:
	 * whether the chip is selected (components::Selectable) and whether it
	 * is enabled (components::StyleState).
	 *
	 * Role in the frame:
	 * - entities::Chip writes the configuration, creates the children the
	 *   variant needs and records their identifiers here.
	 * - systems::Chip (Layout phase) reads it and writes the chip's Color,
	 *   Borders, Elevation, Layout padding and Accessibility, colors the
	 *   children, copies the label to the label child and hands the Layout
	 *   system the children to arrange in a row (the check takes the place
	 *   of the leading icon while it is shown).
	 *
	 * To select or disable a chip from code call
	 * systems::Chip::setSelected() or systems::Chip::setDisabled(). Every
	 * setter here except setOnPress() and setOnClose() compares before
	 * writing and marks the component changed only when the value differs.
	 *
	 * @see systems::Chip, components::Selectable, components::StyleState
	 */
	class Chip: public ecs::Component
	{
		public:
		/**
		 * @brief Kind of chip.
		 *
		 * Assist: an action; it shows DefaultAssistGlyph when it has no icon
		 * glyph. Filter: a press toggles its selection, and it has a check
		 * icon. Input: a piece of entered information; it has a close icon.
		 * Suggestion: a plain proposal.
		 */
		enum class Variant { Assist, Filter, Input, Suggestion };

		/**
		 * @brief Container style: Flat is filled with the secondary
		 * container color; Outlined is transparent with an outline until
		 * selected, where it is filled too.
		 */
		enum class Mode { Flat, Outlined };

		/// Callback run when the chip, or its close icon, is activated.
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
		Variant _variant { Variant::Assist };	 ///< Kind of chip
		Mode _mode { Mode::Flat };				 ///< Container style
		std::string _label {};					 ///< Label text
		std::string _iconGlyph {};				 ///< Leading icon glyph name
		ecs::Entity::Identifier _leadingIcon {
			ecs::Entity::InvalidIdentifier
		};	  ///< Leading icon child, or InvalidIdentifier

		ecs::Entity::Identifier _trailingIcon {
			ecs::Entity::InvalidIdentifier
		};	  ///< Trailing close icon child, or InvalidIdentifier

		ecs::Entity::Identifier _labelEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Label child, or InvalidIdentifier

		ecs::Entity::Identifier _checkIcon {
			ecs::Entity::InvalidIdentifier
		};	  ///< Selected check child, or InvalidIdentifier

		bool _compact { false };				///< Compact padding
		bool _elevated { false };				///< Level-1 shadow
		bool _showSelectedOverlay { false };	///< Selected state layer
		bool _showSelectedCheck { true };		///< Check when selected
		Handler _onPress {};					///< Activation callback
		Handler _onClose {};					///< Close-icon callback

		public:
		/**
		 * @brief Create the configuration of a flat Assist chip with no
		 * label.
		 */
		Chip(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Chip(void) = default;

		/**
		 * @brief Get the kind of chip.
		 * @return The variant (Assist by default).
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Choose the kind of chip.
		 * @param variant The variant; it decides whether a press toggles
		 * the selection and which icons the chip needs.
		 * @return This component, for chaining.
		 */
		Chip &setVariant(Variant variant);

		/**
		 * @brief Get the container style.
		 * @return Flat (the default) or Outlined.
		 */
		Mode getMode(void) const noexcept;

		/**
		 * @brief Choose the container style.
		 * @param mode Flat for a filled chip, Outlined for a transparent
		 * chip with an outline.
		 * @return This component, for chaining.
		 */
		Chip &setMode(Mode mode);

		/**
		 * @brief Get the label text.
		 * @return The text.
		 */
		const std::string &getLabel(void) const noexcept;

		/**
		 * @brief Set the label text.
		 *
		 * The system copies it to the label child's Text and to the chip's
		 * Accessibility label.
		 * @param label The label text.
		 * @return This component, for chaining.
		 */
		Chip &setLabel(const std::string &label);

		/**
		 * @brief Get the name of the leading icon glyph.
		 * @return The glyph name as set, empty when none was given. Use
		 * systems::Chip::leadingGlyph() for the glyph really shown.
		 */
		const std::string &getIconGlyph(void) const noexcept;

		/**
		 * @brief Set the name of the leading icon glyph.
		 * @param iconGlyph The glyph name in the icon font, or an empty
		 * string for none (an Assist chip then shows DefaultAssistGlyph).
		 * @return This component, for chaining.
		 */
		Chip &setIconGlyph(const std::string &iconGlyph);

		/**
		 * @brief Get the child entity that draws the leading icon.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * chip has none.
		 */
		ecs::Entity::Identifier getLeadingIcon(void) const noexcept;

		/**
		 * @brief Record which child entity draws the leading icon.
		 * @param identifier The leading icon child, or
		 * ecs::Entity::InvalidIdentifier for none.
		 * @return This component, for chaining.
		 */
		Chip &setLeadingIcon(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child entity that draws the trailing close icon.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * chip has none.
		 */
		ecs::Entity::Identifier getTrailingIcon(void) const noexcept;

		/**
		 * @brief Record which child entity draws the trailing close icon.
		 *
		 * systems::Chip::installCloseIcon() then makes that child run the
		 * close callback when pressed.
		 * @param identifier The close icon child, or
		 * ecs::Entity::InvalidIdentifier for none.
		 * @return This component, for chaining.
		 */
		Chip &setTrailingIcon(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child entity that draws the label.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * chip has none.
		 */
		ecs::Entity::Identifier getLabelEntity(void) const noexcept;

		/**
		 * @brief Record which child entity draws the label.
		 * @param identifier The label child, or
		 * ecs::Entity::InvalidIdentifier for none.
		 * @return This component, for chaining.
		 */
		Chip &setLabelEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child entity that draws the selected check.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * chip has none.
		 */
		ecs::Entity::Identifier getCheckIcon(void) const noexcept;

		/**
		 * @brief Record which child entity draws the selected check.
		 * @param identifier The check child, or
		 * ecs::Entity::InvalidIdentifier for none.
		 * @return This component, for chaining.
		 */
		Chip &setCheckIcon(ecs::Entity::Identifier identifier);

		/**
		 * @brief Check whether the chip uses the compact padding.
		 * @return True when the padding is CompactPadding instead of
		 * Padding.
		 */
		bool isCompact(void) const noexcept;

		/**
		 * @brief Choose between the regular and the compact padding.
		 * @param compact True for CompactPadding (8 dp), false for Padding
		 * (12 dp).
		 * @return This component, for chaining.
		 */
		Chip &setCompact(bool compact);

		/**
		 * @brief Check whether the chip casts a shadow.
		 * @return True when an enabled chip is drawn at elevation level 1.
		 */
		bool isElevated(void) const noexcept;

		/**
		 * @brief Choose whether the chip casts a shadow.
		 * @param elevated True for elevation level 1 while enabled, false
		 * for no shadow.
		 * @return This component, for chaining.
		 */
		Chip &setElevated(bool elevated);

		/**
		 * @brief Check whether a selected chip is darkened by a state layer.
		 * @return True when the layer is drawn while selected.
		 */
		bool isShowSelectedOverlay(void) const noexcept;

		/**
		 * @brief Choose whether a selected chip is darkened by a state
		 * layer.
		 * @param show True to blend the pressed state layer over the
		 * container while the chip is selected and enabled.
		 * @return This component, for chaining.
		 */
		Chip &setShowSelectedOverlay(bool show);

		/**
		 * @brief Check whether a selected chip shows its check.
		 * @return True when the check is shown while selected (the
		 * default).
		 */
		bool isShowSelectedCheck(void) const noexcept;

		/**
		 * @brief Choose whether a selected chip shows its check.
		 * @param show True so that, while selected, the check child takes
		 * the place of the leading icon; false to keep the leading icon.
		 * @return This component, for chaining.
		 */
		Chip &setShowSelectedCheck(bool show);

		/**
		 * @brief Get the callback run when the chip is pressed.
		 * @return The callback, empty when none is set.
		 */
		const Handler &getOnPress(void) const noexcept;

		/**
		 * @brief Set the callback run when the chip is pressed.
		 *
		 * It runs on every press of an enabled chip, after a Filter chip
		 * toggled its selection. Does not mark the component changed.
		 * @param handler The callback, or an empty one to remove it.
		 * @return This component, for chaining.
		 */
		Chip &setOnPress(Handler handler);

		/**
		 * @brief Get the callback run when the close icon is pressed.
		 * @return The callback, empty when none is set.
		 */
		const Handler &getOnClose(void) const noexcept;

		/**
		 * @brief Set the callback run when the close icon is pressed.
		 *
		 * A chip of any variant with a close callback needs a close icon
		 * (systems::Chip::needsCloseIcon()). The chip is not removed for
		 * you: do it in the callback. Does not mark the component changed.
		 * @param handler The callback, or an empty one to remove it.
		 * @return This component, for chaining.
		 */
		Chip &setOnClose(Handler handler);
	};

}	 // namespace guillaume::components
