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

#include <chrono>
#include <optional>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/button.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/focus.hpp"
#include "guillaume/components/hand_button_interaction.hpp"
#include "guillaume/components/hand_hover_interaction.hpp"
#include "guillaume/components/mouse_hover_interaction.hpp"
#include "guillaume/components/mouse_button_interaction.hpp"
#include "guillaume/components/style_state.hpp"

#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/systems/button.hpp"

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
			components::MouseButtonInteraction, components::StyleState,
			components::Accessibility, components::Button>
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
		 * @brief Set the accessibility hint (result of activation).
		 * @param accessibilityHint New hint.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &setAccessibilityHint(const std::string &accessibilityHint);

		/**
		 * @brief Override the semantic role (default Button).
		 * @param role New role.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &setAccessibilityRole(components::AccessibilityRole role);

		/**
		 * @brief Set the long-press handler (Paper `onLongPress`).
		 *
		 * A press held for at least `LongPressDuration` calls this handler
		 * instead of the click handler.
		 *
		 * @param onLongPress Handler (empty to disable long press).
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &setOnLongPress(std::function<void(void)> onLongPress);

		/**
		 * @brief Show or hide the loading state (Paper `loading`).
		 *
		 * While loading the icon shows the progress glyph.
		 *
		 * @param loading New loading state.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &setLoading(bool loading);

		/**
		 * @brief Show or hide the button (Paper `visible`).
		 *
		 * A hidden button paints nothing, ignores activation and is skipped by
		 * keyboard traversal; its enabled state is preserved.
		 *
		 * @param visible New visibility.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &setVisible(bool visible);

		/**
		 * @brief Check the visibility.
		 * @return True when shown.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Check the loading state.
		 * @return True while loading.
		 */
		bool isLoading(void) const;

		/**
		 * @brief Override the theme container color (Paper `buttonColor` /
		 * `containerColor`).
		 * @param color Custom color, or none to restore the theme color.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &setContainerColorOverride(
			std::optional<utility::graphic::Color32Bit> color);

		/**
		 * @brief Override the theme content color (Paper `textColor` /
		 * `iconColor`).
		 * @param color Custom color, or none to restore the theme color.
		 * @return Reference to this button for chaining.
		 */
		ButtonBase &setContentColorOverride(
			std::optional<utility::graphic::Color32Bit> color);

		/**
		 * @brief Glyph shown by the icon while loading.
		 */
		static constexpr const char *LoadingGlyph =
			systems::Button::LoadingGlyph;

		/**
		 * @brief Minimum press duration that counts as a long press.
		 */
		static constexpr std::chrono::milliseconds LongPressDuration {
			systems::Button::LongPressDuration
		};

		/**
		 * @brief Get the accessibility hint.
		 * @return The hint.
		 */
		const std::string &getAccessibilityHint(void) const;

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

		protected:
		/**
		 * @brief Create the icon and label children.
		 *
		 * Called once by the subclass's create(), when the button is owned
		 * and can be given as parent to its children.
		 */
		void build(void);

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
		 * @brief Access the button state component.
		 * @return The components::Button of this entity.
		 */
		components::Button &state(void);

		/**
		 * @brief Access the button state component.
		 * @return The components::Button of this entity.
		 */
		const components::Button &state(void) const;

		/**
		 * @brief Restyle now through the variant system.
		 *
		 * The variant system does the same every frame; this makes the result
		 * of a setter visible immediately.
		 */
		void restyle(void);

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
		/**
		 * @brief Grow the hit area of a small control to the M3 touch target.
		 *
		 * Sets the hover accessibility margins (consumed by the hit test of
		 * every pointer and hand system) so the interactive area is at least
		 * `theme::SpacingScale::MinTouchTarget` on both axes.
		 *
		 * @param width Visual width of the control.
		 * @param height Visual height of the control.
		 */
		void applyTouchTarget(float width, float height);

		/**
		 * @brief Mirror the toggle selection on the Accessibility component.
		 */
		void syncAccessibilityState(void);

		/**
		 * @brief Resolve the container color, honouring the override.
		 * @param color Theme-derived container color.
		 * @return The override when set, otherwise @p color.
		 */
		utility::graphic::Color32Bit resolveContainerColor(
			const utility::graphic::Color32Bit &color) const;

		/**
		 * @brief Resolve the content color, honouring the override.
		 * @param color Theme-derived content color.
		 * @return The override when set, otherwise @p color.
		 */
		utility::graphic::Color32Bit resolveContentColor(
			const utility::graphic::Color32Bit &color) const;
		static constexpr float _layerDepthStep {
			1.0f
		};	  ///< Distance pushed toward the camera per layer.
	};

}	 // namespace guillaume::entities
