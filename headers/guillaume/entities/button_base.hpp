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
	 * @brief What Button, IconButton, FloatingActionButton and
	 * ExtendedFloatingActionButton have in common: an icon, a label, a
	 * click, a toggle state.
	 *
	 * This class is not created directly; use one of the four widgets above.
	 * It holds no field. The shared state lives in components::Button (glyph
	 * names, label, toggle, selected, loading, visible, callbacks, color
	 * overrides), the enabled flag in components::StyleState and the label
	 * read by assistive tools in components::Accessibility. The rules are in
	 * two systems:
	 * - systems::Button (Event phase) turns hover, press and release into
	 *   state, calls the click or long-press handler and keeps the icon
	 *   child's glyph up to date.
	 * - The system of the concrete widget (systems::CommonButton,
	 *   systems::IconButton, ...) computes colors, size and the position of
	 *   the icon and label. It is reached through the restyle function
	 *   stored in components::Button.
	 *
	 * The setters below write the components and, when the look depends on
	 * the value, restyle at once.
	 *
	 * @code
	 * Button::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto save = builder.makeButton(nullptr, "Save", [] {},
	 * 	Button::Color::Filled, Button::Shape::Round, Button::Size::Medium,
	 * 	false);
	 * save->setIconGlyphName("save").setDisabled(true);
	 * save->setOnClick([] {
	 * 	// called when the button is released
	 * });
	 * @endcode
	 *
	 * Watch out for:
	 * - The icon and the label are child entities (Icon and Text) owned by
	 *   the button and created once, when the widget is built. Their colors,
	 *   sizes and positions are overwritten by the concrete widget's system.
	 * - The handlers are called on release, not on press. A hidden or
	 *   disabled button calls none.
	 *
	 * @see components::Button, systems::Button, Button, IconButton
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
		 * @brief Change the icon.
		 *
		 * The icon child shows it at once, unless the button is loading or
		 * is selected with a selected glyph set.
		 * @param iconGlyphName The name of the glyph, such as "add".
		 * @return This button, for chaining.
		 */
		ButtonBase &setIconGlyphName(const std::string &iconGlyphName);

		/**
		 * @brief Change the style the icon glyph is drawn in.
		 * @param iconStyle The glyph style (outlined, filled, ...), applied
		 * to the icon child at once.
		 * @return This button, for chaining.
		 */
		ButtonBase &setIconStyle(const components::Glyph::Style &iconStyle);

		/**
		 * @brief Change the text of the label.
		 *
		 * Writes the content of the label child. It has no visible effect on
		 * a widget without label child (see usesLabel()).
		 * @param labelContent The new text.
		 * @return This button, for chaining.
		 */
		ButtonBase &setLabelContent(const std::string &labelContent);

		/**
		 * @brief Make the button a toggle, or a plain push button.
		 *
		 * A toggle flips its selected state each time the user releases it.
		 * @param isToggle True for a toggle. False also clears the selected
		 * state.
		 * @return This button, for chaining.
		 */
		ButtonBase &setIsToggle(const bool &isToggle);

		/**
		 * @brief Tell whether a toggle button is on.
		 * @return True when the button is a toggle and is selected. Always
		 * false for a push button.
		 */
		bool isSelected(void) const;

		/**
		 * @brief Turn a toggle button on or off from code.
		 *
		 * Updates the icon and restyles at once. The click handler is not
		 * called. Ignored when the button is not a toggle.
		 * @param selected True to select the button.
		 * @return This button, for chaining.
		 */
		ButtonBase &setSelected(const bool &selected);

		/**
		 * @brief Set the icon shown instead of the regular one while the
		 * button is selected.
		 * @param selectedIconGlyphName The glyph name. An empty name keeps
		 * the regular icon when selected.
		 * @return This button, for chaining.
		 */
		ButtonBase &
			setSelectedIconGlyphName(const std::string &selectedIconGlyphName);

		/**
		 * @brief Choose whether the button changes shape when selected, and
		 * restyle at once.
		 * @param isMorph True to let the concrete widget's system use a
		 * different shape for the selected state. Only systems::CommonButton
		 * (Button) reads this flag.
		 * @return This button, for chaining.
		 */
		ButtonBase &setMorph(const bool &isMorph);

		/**
		 * @brief Put the icon before or after the label, and restyle at
		 * once.
		 * @param isTrailingIcon True to place the icon after the label.
		 * @return This button, for chaining.
		 */
		ButtonBase &setTrailingIcon(const bool &isTrailingIcon);

		/**
		 * @brief Disable or enable the button, and restyle at once.
		 *
		 * A disabled button ignores hover, press and release and calls no
		 * handler. The flag is stored in components::StyleState.
		 * @param isDisabled True to disable the button.
		 * @return This button, for chaining.
		 */
		ButtonBase &setDisabled(const bool &isDisabled);

		/**
		 * @brief Tell whether the button is disabled.
		 * @return True when its components::StyleState is not enabled.
		 */
		bool isDisabled(void) const;

		/**
		 * @brief Set the name assistive tools announce for the button.
		 * @param accessibilityLabel The name. At construction it defaults to
		 * the label text.
		 * @return This button, for chaining.
		 */
		ButtonBase &
			setAccessibilityLabel(const std::string &accessibilityLabel);

		/**
		 * @brief Get the name assistive tools announce for the button.
		 * @return The label stored in components::Accessibility.
		 */
		const std::string &getAccessibilityLabel(void) const;

		/**
		 * @brief Set the sentence that tells assistive tools what activating
		 * the button does.
		 * @param accessibilityHint The hint, such as "Opens the settings".
		 * @return This button, for chaining.
		 */
		ButtonBase &setAccessibilityHint(const std::string &accessibilityHint);

		/**
		 * @brief Change the role announced by assistive tools.
		 * @param role The role. It is components::AccessibilityRole::Button
		 * until this is called.
		 * @return This button, for chaining.
		 */
		ButtonBase &setAccessibilityRole(components::AccessibilityRole role);

		/**
		 * @brief Set the function called when the user holds the button.
		 *
		 * When the press lasted at least LongPressDuration, the release
		 * calls this handler instead of the click handler. A toggle button
		 * still flips its selected state.
		 *
		 * @param onLongPress The handler. An empty function turns long press
		 * off: every release is then a click.
		 * @return This button, for chaining.
		 */
		ButtonBase &setOnLongPress(std::function<void(void)> onLongPress);

		/**
		 * @brief Show or hide the loading state.
		 *
		 * While loading, the icon child shows LoadingGlyph instead of the
		 * icon. The button stays clickable.
		 *
		 * @param loading True to show the loading glyph.
		 * @return This button, for chaining.
		 */
		ButtonBase &setLoading(bool loading);

		/**
		 * @brief Show or hide the button, and restyle at once.
		 *
		 * A hidden button ignores press and release and is skipped by
		 * keyboard traversal (its Accessibility is not focusable). Its
		 * enabled state is kept. Painting nothing is up to the concrete
		 * widget's system: systems::FloatingActionButton and
		 * systems::ExtendedFloatingActionButton do it; systems::CommonButton
		 * (Button) and systems::IconButton do not read this flag, so a
		 * hidden Button or IconButton is still drawn.
		 *
		 * @param visible False to hide the button.
		 * @return This button, for chaining.
		 */
		ButtonBase &setVisible(bool visible);

		/**
		 * @brief Tell whether the button is shown.
		 * @return False after setVisible(false). True by default.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Tell whether the button is in the loading state.
		 * @return True between setLoading(true) and setLoading(false).
		 */
		bool isLoading(void) const;

		/**
		 * @brief Replace the theme's background color with a custom one,
		 * and restyle at once.
		 * @param color The custom color, or `std::nullopt` to go back to the
		 * color of the theme.
		 * @return This button, for chaining.
		 */
		ButtonBase &setContainerColorOverride(
			std::optional<utility::graphic::Color32Bit> color);

		/**
		 * @brief Replace the theme's icon and label color with a custom
		 * one, and restyle at once.
		 * @param color The custom color, or `std::nullopt` to go back to the
		 * color of the theme.
		 * @return This button, for chaining.
		 */
		ButtonBase &setContentColorOverride(
			std::optional<utility::graphic::Color32Bit> color);

		/**
		 * @brief Name of the glyph the icon child shows while loading.
		 */
		static constexpr const char *LoadingGlyph =
			systems::Button::LoadingGlyph;

		/**
		 * @brief How long a press must last to count as a long press
		 * (500 ms).
		 */
		static constexpr std::chrono::milliseconds LongPressDuration {
			systems::Button::LongPressDuration
		};

		/**
		 * @brief Get the sentence that tells assistive tools what activating
		 * the button does.
		 * @return The hint stored in components::Accessibility, empty when
		 * none was set.
		 */
		const std::string &getAccessibilityHint(void) const;

		/**
		 * @brief Set the function called when the user clicks the button.
		 *
		 * It is called when the left mouse button or the hand button A is
		 * released over the button, unless the release counts as a long
		 * press. For a toggle, isSelected() already returns the new state.
		 * @param onClick The handler. An empty function means no reaction.
		 * @return This button, for chaining.
		 */
		ButtonBase &setOnClick(std::function<void(void)> onClick);

		/**
		 * @brief Get the icon child, to read or adjust its components.
		 * @return The identifier of the Icon child, or
		 * `ecs::Entity::InvalidIdentifier` when the widget has none.
		 */
		ecs::Entity::Identifier getIconIdentifier(void) const;

		/**
		 * @brief Get the label child, to read or adjust its components.
		 * @return The identifier of the Text child, or
		 * `ecs::Entity::InvalidIdentifier` when the widget has none.
		 */
		ecs::Entity::Identifier getLabelIdentifier(void) const;

		protected:
		/**
		 * @brief Create the icon and label children, then style the button.
		 *
		 * Called once by the `create()` of the concrete widget, after
		 * construction: a child needs its parent to be owned by a
		 * `std::shared_ptr`. usesIcon() and usesLabel() decide which
		 * children exist; their identifiers are stored in
		 * components::Button. The button itself is the entity registry that
		 * owns them.
		 */
		void build(void);

		/**
		 * @brief Create the entity with its components and store the
		 * initial state. The children are created later, by build().
		 * @param registry The registry that stores the components.
		 * @param iconGlyphName The name of the icon glyph, empty for none.
		 * @param iconStyle The style the glyph is drawn in.
		 * @param labelContent The text of the label, empty for none.
		 * @param isToggle True when a release flips the selected state.
		 * @param isMorph True when the shape changes while selected.
		 * @param isDisabled True to start disabled.
		 * @param isTrailingIcon True to place the icon after the label.
		 * @param accessibilityLabel The name announced by assistive tools.
		 * When empty, @p labelContent is used.
		 * @param onClick The function called on click (may be empty).
		 */
		ButtonBase(ecs::ComponentRegistry &registry,
				   const std::string &iconGlyphName,
				   const components::Glyph::Style &iconStyle,
				   const std::string &labelContent, bool isToggle, bool isMorph,
				   bool isDisabled, bool isTrailingIcon,
				   const std::string &accessibilityLabel,
				   std::function<void(void)> onClick);

		/**
		 * @brief Access the shared button state, for writing.
		 * @return The components::Button of this entity.
		 */
		components::Button &state(void);

		/**
		 * @brief Access the shared button state, for reading.
		 * @return The components::Button of this entity.
		 */
		const components::Button &state(void) const;

		/**
		 * @brief Recompute colors and geometry now.
		 *
		 * Runs the restyle function the concrete widget's system stored in
		 * components::Button, if any. That system does the same every frame;
		 * calling it from a setter makes the change visible immediately.
		 */
		void restyle(void);

		/**
		 * @brief Size the icon child is created with.
		 * @return The glyph size, in dp.
		 */
		virtual float getIconFontSize(void) const = 0;

		/**
		 * @brief Tell build() whether to create a label child.
		 * @return True for the widgets that show text. False by default.
		 */
		virtual bool usesLabel(void) const
		{
			return false;
		}

		/**
		 * @brief Tell build() whether to create an icon child.
		 * @return True by default.
		 */
		virtual bool usesIcon(void) const
		{
			return true;
		}

		/**
		 * @brief Color the icon and label children are created with.
		 * @return The content color for the initial state of the widget.
		 * The first restyle replaces it.
		 */
		virtual utility::graphic::Color32Bit
			getInitialContentColor(void) const = 0;

		/**
		 * @brief Tell whether a pointer is over the button.
		 * @return True when the mouse or a hand hovers it.
		 */
		bool isHovered(void);

		/**
		 * @brief Tell whether the button is held down.
		 * @return True while the left mouse button or the hand button A is
		 * pressed on it.
		 */
		bool isPressed(void);

		/**
		 * @brief Get the glyph the icon child should show right now.
		 * @return LoadingGlyph while loading; otherwise the selected glyph
		 * when the button is selected and one is set; otherwise the regular
		 * glyph.
		 */
		const std::string &getActiveIconGlyphName(void) const;

		public:
		/**
		 * @brief Compute the pose of content drawn in front of a button.
		 *
		 * Forwards to systems::Layout::contentLayerPose: the position is
		 * moved toward the viewer, along the forward axis of the
		 * orientation, by one depth step per layer plus one, so the icon
		 * and the label do not overlap the container surface in depth.
		 * @param position The position of the content on the button plane.
		 * @param orientation The orientation of the button.
		 * @param layer The layer of the button in the entity tree.
		 * @return The pose to give to the content.
		 */
		static const utility::graphic::PoseF applyLayerToPosition(
			const utility::graphic::PositionF &position,
			const utility::graphic::OrientationF &orientation,
			const std::uint32_t &layer);

		/**
		 * @brief Replace the alpha of a color and keep its red, green and
		 * blue.
		 * @param color The base color.
		 * @param alpha The new alpha, from 0 (transparent) to 255 (opaque).
		 * @return The color with the new alpha.
		 */
		static utility::graphic::Color32Bit
			applyStateAlpha(const utility::graphic::Color32Bit &color,
							std::uint8_t alpha);

		/**
		 * @brief Blend a translucent state layer (hover, press) over a
		 * container color.
		 * @param base The container color.
		 * @param overlay The color of the state layer, usually the content
		 * color.
		 * @param alpha The opacity of the layer, from 0 to 255.
		 * @return The container color with the layer blended on top.
		 */
		static utility::graphic::Color32Bit
			applyStateLayer(const utility::graphic::Color32Bit &base,
							const utility::graphic::Color32Bit &overlay,
							std::uint8_t alpha);

		/**
		 * @brief Get the background color of a disabled button.
		 * @return The OnSurface color of the active scheme at the
		 * disabled-container opacity of the theme.
		 */
		static utility::graphic::Color32Bit getDisabledContainerColor(void);

		/**
		 * @brief Get the icon and label color of a disabled button.
		 * @return The OnSurface color of the active scheme at the
		 * disabled-content opacity of the theme.
		 */
		static utility::graphic::Color32Bit getDisabledContentColor(void);

		/**
		 * @brief Get the outline color of a disabled button.
		 * @return The OnSurface color of the active scheme at the
		 * disabled-container opacity of the theme (the same value as
		 * getDisabledContainerColor()).
		 */
		static utility::graphic::Color32Bit getDisabledBorderColor(void);

		/**
		 * @brief Tell the button a pointer started hovering it.
		 *
		 * Restyles unless the button is disabled. The framework does not
		 * call this: systems::Button wires the hover components itself.
		 * Useful to drive a button from code or from a test.
		 */
		void hoverHandler(void);

		/**
		 * @brief Tell the button a pointer stopped hovering it.
		 *
		 * Same effect as hoverHandler(): restyles unless disabled.
		 */
		void unHoverHandler(void);

		/**
		 * @brief Press the button from code, as the user would.
		 *
		 * Starts timing the press and restyles. Ignored when the button is
		 * disabled or hidden.
		 */
		void buttonPressHandler(void);

		/**
		 * @brief Release the button from code, as the user would.
		 *
		 * Flips the selected state of a toggle, restyles, then calls the
		 * long-press or the click handler. Ignored when the button is
		 * disabled or hidden.
		 */
		void buttonReleaseHandler(void);

		protected:
		/**
		 * @brief Make a small button easier to hit.
		 *
		 * Sets the accessibility margin of the mouse and hand hover
		 * components so the area that reacts to pointers is at least
		 * `theme::SpacingScale::MinTouchTarget` (48 dp) on both axes. The
		 * drawn size does not change.
		 *
		 * @param width The drawn width of the button, in dp.
		 * @param height The drawn height of the button, in dp.
		 */
		void applyTouchTarget(float width, float height);

		/**
		 * @brief Copy the current state to the icon child and to
		 * Accessibility.
		 *
		 * Forwards to systems::Button::syncContent: the icon child gets the
		 * active glyph and style, and components::Accessibility is marked
		 * selected when the button is a selected toggle.
		 */
		void syncAccessibilityState(void);

		/**
		 * @brief Pick the background color to paint.
		 * @param color The color computed from the theme.
		 * @return The color given to setContainerColorOverride() when one
		 * is set, otherwise @p color.
		 */
		utility::graphic::Color32Bit resolveContainerColor(
			const utility::graphic::Color32Bit &color) const;

		/**
		 * @brief Pick the icon and label color to paint.
		 * @param color The color computed from the theme.
		 * @return The color given to setContentColorOverride() when one is
		 * set, otherwise @p color.
		 */
		utility::graphic::Color32Bit resolveContentColor(
			const utility::graphic::Color32Bit &color) const;
		static constexpr float _layerDepthStep {
			1.0f
		};	  ///< Depth between two layers, toward the viewer.
	};

}	 // namespace guillaume::entities
