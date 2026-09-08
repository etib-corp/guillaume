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

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/focus.hpp"
#include "guillaume/components/hand_button_interaction.hpp"
#include "guillaume/components/hand_hover_interaction.hpp"
#include "guillaume/components/mouse_hover_interaction.hpp"
#include "guillaume/components/mouse_button_interaction.hpp"

#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Base class for all interactive button entities.
	 *
	 * `ButtonBase` owns the shared component signature, the optional icon and
	 * label child entities, the toggle/selected/morph/disabled states, the
	 * accessibility label and the click handler. Concrete button classes
	 * (Button, IconButton, FloatingActionButton,
	 * ExtendedFloatingActionButton) derive from it and only implement the
	 * visual contract through protected virtual hooks.
	 *
	 * Toggle behavior is a parameter (`isToggle`), available on every leaf
	 * type.
	 */
	class ButtonBase:
		public std::enable_shared_from_this<ButtonBase>,
		public ecs::ParentEntityFiller<
			components::Transform, components::Bound, components::Color,
			components::Borders, components::Focus,
			components::HandButtonInteraction, components::HandHoverInteraction,
			components::MouseHoverInteraction,
			components::MouseButtonInteraction>
	{
		public:
		/**
		 * @brief Default destructor.
		 */
		~ButtonBase(void) override = default;

		/**
		 * @brief Set the icon glyph name.
		 * @param iconGlyphName The glyph name of the icon to attach.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &setIconGlyphName(const std::string &iconGlyphName);

		/**
		 * @brief Set the style of the icon.
		 * @param iconStyle The style of the icon to attach.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &setIconStyle(const components::Glyph::Style &iconStyle);

		/**
		 * @brief Set the label content.
		 * @param labelContent The label content to attach.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &setLabelContent(const std::string &labelContent);

		/**
		 * @brief Set if the button is a toggle button.
		 * @param isToggle Whether the button should be a toggle button.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &setIsToggle(const bool &isToggle);

		/**
		 * @brief Get whether the button is currently selected (for toggle
		 * buttons).
		 * @return True when the button is a toggle button and is selected,
		 * false otherwise.
		 */
		bool isSelected(void) const;

		/**
		 * @brief Set the selected state of the button (for toggle buttons).
		 * @param selected The new selected state to set.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &setSelected(const bool &selected);

		/**
		 * @brief Set the icon glyph name shown when the button is selected.
		 * @param selectedIconGlyphName The glyph shown when selected.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &
			setSelectedIconGlyphName(const std::string &selectedIconGlyphName);

		/**
		 * @brief Set if the button morphs its shape when selected.
		 * @param isMorph Whether the button morphs when selected.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &setMorph(const bool &isMorph);

		/**
		 * @brief Set if the icon is placed after the label.
		 * @param isTrailingIcon Whether the icon trails the label.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &setTrailingIcon(const bool &isTrailingIcon);

		/**
		 * @brief Set if the button is disabled.
		 * @param isDisabled Whether the button should be disabled.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &setDisabled(const bool &isDisabled);

		/**
		 * @brief Get whether the button is disabled.
		 * @return True when the button is disabled, false otherwise.
		 */
		bool isDisabled(void) const;

		/**
		 * @brief Set the accessibility label.
		 * @param accessibilityLabel The accessibility label to set.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &
			setAccessibilityLabel(const std::string &accessibilityLabel);

		/**
		 * @brief Get the accessibility label.
		 * @return The accessibility label.
		 */
		const std::string &getAccessibilityLabel(void) const;

		/**
		 * @brief Set the click event handler.
		 * @param onClick The new click event handler to set.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &setOnClick(std::function<void(void)> onClick);

		/**
		 * @brief Get the identifier of the internal icon child entity.
		 * @return The icon entity identifier, or InvalidIdentifier when no icon
		 * is attached.
		 */
		ecs::Entity::Identifier getIconIdentifier(void) const;

		/**
		 * @brief Get the identifier of the internal label child entity.
		 * @return The label entity identifier, or InvalidIdentifier when no
		 * label is attached.
		 */
		ecs::Entity::Identifier getLabelIdentifier(void) const;

		/**
		 * @brief Initialize the button entity's derived state.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the button entity's derived state.
		 */
		void update(void) override;

		protected:
		/**
		 * @brief Construct a ButtonBase entity.
		 * @param registry Reference to the component registry.
		 * @param iconGlyphName Icon glyph name to attach.
		 * @param iconStyle Style of the icon to attach.
		 * @param labelContent Label content to attach.
		 * @param isToggle Whether the button is a toggle button.
		 * @param isMorph Initial morph state.
		 * @param isDisabled Initial disabled state.
		 * @param isTrailingIcon Initial trailing icon state.
		 * @param accessibilityLabel Initial accessibility label.
		 * @param onClick Click event handler.
		 */
		ButtonBase(ecs::ComponentRegistry &registry,
				   const std::string &iconGlyphName,
				   const components::Glyph::Style &iconStyle,
				   const std::string &labelContent, bool isToggle, bool isMorph,
				   bool isDisabled, bool isTrailingIcon,
				   const std::string &accessibilityLabel,
				   std::function<void(void)> onClick);

		/**
		 * @brief Re-apply the visual state (colors, borders, radius) from the
		 * current interaction/selected state.
		 */
		virtual void applyStyleState(void) = 0;

		/**
		 * @brief Re-measure and lay out the button and its children.
		 */
		virtual void applyGeometry(void) = 0;

		/**
		 * @brief Font size used for the icon child.
		 * @return The icon font size.
		 */
		virtual float getIconFontSize(void) const = 0;

		/**
		 * @brief Whether this button type displays a text label.
		 * @return True when a label child is used.
		 */
		virtual bool usesLabel(void) const
		{
			return false;
		}

		/**
		 * @brief Whether this button type displays an icon child.
		 * @return True when an icon child is used.
		 */
		virtual bool usesIcon(void) const
		{
			return true;
		}

		/**
		 * @brief Get the initial content color used when creating children.
		 * @return The initial content color.
		 */
		virtual utility::graphic::Color32Bit
			getInitialContentColor(void) const = 0;

		/**
		 * @brief Check whether the button is currently hovered by any pointer.
		 * @return True when hovered.
		 */
		bool isHovered(void);

		/**
		 * @brief Check whether the button is currently pressed by any pointer.
		 * @return True when pressed.
		 */
		bool isPressed(void);

		/**
		 * @brief Get the icon glyph name to display for the current state.
		 * @return The selected glyph when selected (and provided), otherwise
		 * the base glyph.
		 */
		const std::string &getActiveIconGlyphName(void) const;

		public:
		/**
		 * @brief Apply the current layer depth offset to a position.
		 * @param position The base position.
		 * @param orientation The orientation.
		 * @param layer The button layer.
		 * @return The offset pose.
		 */
		static const utility::graphic::PoseF applyLayerToPosition(
			const utility::graphic::PositionF &position,
			const utility::graphic::OrientationF &orientation,
			const std::uint32_t &layer);

		/**
		 * @brief Replace the alpha of a color while keeping its RGB channels.
		 * @param color The base color.
		 * @param alpha The new alpha channel.
		 * @return The color with the updated alpha.
		 */
		static utility::graphic::Color32Bit
			applyStateAlpha(const utility::graphic::Color32Bit &color,
							std::uint8_t alpha);

		/**
		 * @brief Apply a Material Design state layer over a base color.
		 * @param base The base container color.
		 * @param overlay The state layer overlay color.
		 * @param alpha The overlay opacity (0-255).
		 * @return The base color with the overlay blended on top.
		 */
		static utility::graphic::Color32Bit
			applyStateLayer(const utility::graphic::Color32Bit &base,
							const utility::graphic::Color32Bit &overlay,
							std::uint8_t alpha);

		/**
		 * @brief Disabled container color used by every button type.
		 * @return The disabled container color.
		 */
		static utility::graphic::Color32Bit getDisabledContainerColor(void);

		/**
		 * @brief Disabled content color used by every button type.
		 * @return The disabled content color.
		 */
		static utility::graphic::Color32Bit getDisabledContentColor(void);

		/**
		 * @brief Disabled border color used by every button type.
		 * @return The disabled border color.
		 */
		static utility::graphic::Color32Bit getDisabledBorderColor(void);

		/**
		 * @brief Hover event handler for the button.
		 */
		void hoverHandler(void);

		/**
		 * @brief Unhover event handler for the button.
		 */
		void unHoverHandler(void);

		/**
		 * @brief Press event handler for the button.
		 */
		void buttonPressHandler(void);

		/**
		 * @brief Release event handler for the button.
		 */
		void buttonReleaseHandler(void);

		protected:
		std::string _iconGlyphName {};	  ///< Icon glyph name to attach.
		components::Glyph::Style _iconStyle {
			components::Glyph::Style::Outlined
		};	  ///< Style of the icon to attach.
		std::string
			_selectedIconGlyphName {};	  ///< Icon glyph name when selected.
		std::string _labelContent {};	  ///< Label content to attach.
		std::shared_ptr<Icon> _icon;	  ///< Internal child icon entity
		std::shared_ptr<Text> _label;	  ///< Internal child label entity
		bool _isToggle { false };	 ///< Whether the button is a toggle button
		bool _isSelected { false };	   ///< Whether the button is selected
		bool _isMorph { false };	///< Whether the button morphs when selected
		bool _isDisabled { false };	   ///< Whether the button is disabled
		bool _isTrailingIcon {
			false
		};	  ///< Whether the icon trails the label
		std::string _accessibilityLabel {};		  ///< Accessibility label
		std::function<void(void)> _onClick {};	  ///< Click event handler
		static constexpr float _layerDepthStep {
			1.0f
		};	  ///< Distance pushed toward the camera per layer.
	};

}	 // namespace guillaume::entities
