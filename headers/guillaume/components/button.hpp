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
#include <functional>
#include <optional>
#include <string>

#include <utility/graphic/color.hpp>

#include "guillaume/components/glyph.hpp"
#include "guillaume/ecs/component_registry.hpp"

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief State shared by every pressable button, whatever its look.
	 *
	 * Holds what the button shows (icon glyph, label text, the identifiers of
	 * the icon and label child entities), how it reacts (toggle, click and
	 * long-press callbacks) and its transient state (selected, loading,
	 * visible, start of the current press).
	 *
	 * Role in the frame:
	 * - The widget classes (entities::Button, entities::IconButton, ...) write
	 *   the content and the options.
	 * - systems::Button (Event phase) turns hover, press and release into the
	 *   selection, the press timing and the callbacks, and copies the active
	 *   glyph to the icon child.
	 * - One variant system (systems::CommonButton, systems::IconButton,
	 *   systems::FloatingActionButton or
	 *   systems::ExtendedFloatingActionButton, Layout phase) reads it together
	 *   with its own configuration component to produce colors and geometry.
	 *   That system registers its static `apply` here with setRestyle() so
	 *   systems::Button can restyle the button the moment input changes it.
	 *
	 * @code
	 * auto &button = registry.getComponent<components::Button>(identifier);
	 * button.setToggle(true).setSelectedIconGlyphName("favorite");
	 * button.setOnClick([] { save(); });
	 * @endcode
	 *
	 * Every setter except setOnClick() and setOnLongPress() compares before
	 * writing and marks the component changed only when the value differs.
	 * A setter stores the value and nothing else: the screen follows on the
	 * next frame, or at once if the caller then runs
	 * systems::Button::restyle().
	 *
	 * @see systems::Button, components::CommonButton, components::IconButton
	 */
	class Button: public ecs::Component
	{
		public:
		/// Callback run when the button is clicked or long-pressed.
		using Handler = std::function<void(void)>;

		/// Point in time on the clock used to measure how long a press lasts.
		using TimePoint = std::chrono::steady_clock::time_point;

		/// A color that replaces a themed one, or nothing to keep the theme.
		using OptionalColor = std::optional<utility::graphic::Color32Bit>;

		/// Static function of a variant system that recomputes the colors and
		/// the geometry of one button at once, given the registry and the
		/// button's identifier.
		using RestyleFunction = void (*)(ecs::ComponentRegistry &,
										 const ecs::Entity::Identifier &);

		private:
		std::string _iconGlyphName {};	  ///< Icon glyph name; empty for none
		Glyph::Style _iconStyle { Glyph::Style::Outlined };	   ///< Icon style
		std::string
			_selectedIconGlyphName {};	  ///< Icon glyph name when selected
		std::string _labelContent {};	  ///< Label text
		ecs::Entity::Identifier _icon {
			ecs::Entity::InvalidIdentifier
		};	  ///< Icon child entity, or InvalidIdentifier

		ecs::Entity::Identifier _label {
			ecs::Entity::InvalidIdentifier
		};	  ///< Label child entity, or InvalidIdentifier

		bool _toggle { false };		 ///< Whether a click toggles the selection
		bool _selected { false };	 ///< Whether the button is selected
		bool _morph { false };		 ///< Whether the shape morphs when selected
		bool _trailingIcon { false };	 ///< Whether the icon trails the label
		bool _loading {
			false
		};	  ///< Whether a progress glyph replaces the icon

		bool _visible { true };	   ///< Whether the button is shown
		Handler _onClick {};	   ///< Click callback; empty for none
		Handler
			_onLongPress {};	///< Long-press callback (replaces the click)
		TimePoint _pressStart {};	 ///< When the current press started
		OptionalColor _containerColorOverride {};	 ///< Custom container color
		OptionalColor _contentColorOverride {};		 ///< Custom content color
		RestyleFunction _restyle {
			nullptr
		};	  ///< Restyle function of the variant system, or nullptr

		public:
		/**
		 * @brief Create the state of a plain button: no icon, no label, not
		 * a toggle, visible, no callback.
		 */
		Button(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Button(void) = default;

		/**
		 * @brief Get the name of the icon glyph shown at rest.
		 * @return The glyph name (for example `"add"`), empty when the button
		 * has no icon.
		 */
		const std::string &getIconGlyphName(void) const noexcept;

		/**
		 * @brief Set the name of the icon glyph shown at rest.
		 *
		 * systems::Button copies it to the icon child's Glyph, unless the
		 * button is loading or selected with a selected glyph.
		 * @param name The glyph name in the icon font, for example `"add"`.
		 * @return This component, for chaining.
		 */
		Button &setIconGlyphName(const std::string &name);

		/**
		 * @brief Get the style the icon glyph is drawn in.
		 * @return The glyph style (outlined by default).
		 */
		Glyph::Style getIconStyle(void) const noexcept;

		/**
		 * @brief Set the style the icon glyph is drawn in.
		 * @param style The glyph style, copied to the icon child's Glyph by
		 * systems::Button.
		 * @return This component, for chaining.
		 */
		Button &setIconStyle(Glyph::Style style);

		/**
		 * @brief Get the name of the icon glyph shown while selected.
		 * @return The glyph name, empty when the selected button keeps its
		 * regular glyph.
		 */
		const std::string &getSelectedIconGlyphName(void) const noexcept;

		/**
		 * @brief Set the name of the icon glyph shown while selected.
		 * @param name The glyph name, or an empty string to keep the regular
		 * glyph when selected.
		 * @return This component, for chaining.
		 */
		Button &setSelectedIconGlyphName(const std::string &name);

		/**
		 * @brief Get the text of the label.
		 * @return The text as the caller gave it, before any upper-casing
		 * done by the variant system.
		 */
		const std::string &getLabelContent(void) const noexcept;

		/**
		 * @brief Set the text of the label.
		 *
		 * The variant system writes it to the label child's Text.
		 * @param content The label text.
		 * @return This component, for chaining.
		 */
		Button &setLabelContent(const std::string &content);

		/**
		 * @brief Get the child entity that draws the icon.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * button has no icon child.
		 */
		ecs::Entity::Identifier getIcon(void) const noexcept;

		/**
		 * @brief Record which child entity draws the icon.
		 *
		 * The widget class calls it after creating the child; the component
		 * does not own the entity.
		 * @param identifier The icon child, or
		 * ecs::Entity::InvalidIdentifier for none.
		 * @return This component, for chaining.
		 */
		Button &setIcon(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child entity that draws the label.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * button has no label child.
		 */
		ecs::Entity::Identifier getLabel(void) const noexcept;

		/**
		 * @brief Record which child entity draws the label.
		 * @param identifier The label child, or
		 * ecs::Entity::InvalidIdentifier for none.
		 * @return This component, for chaining.
		 */
		Button &setLabel(ecs::Entity::Identifier identifier);

		/**
		 * @brief Check whether the button is a toggle.
		 * @return True when each release flips the selection.
		 */
		bool isToggle(void) const noexcept;

		/**
		 * @brief Make the button a toggle or a plain button.
		 * @param toggle True so that each release flips the selection; false
		 * so that a release only runs the callback.
		 * @return This component, for chaining.
		 */
		Button &setToggle(bool toggle);

		/**
		 * @brief Check whether the button is selected.
		 * @return True when selected. Only meaningful for a toggle.
		 */
		bool isSelected(void) const noexcept;

		/**
		 * @brief Store the selection.
		 *
		 * It only stores the flag, even when the button is not a toggle. Use
		 * systems::Button::setSelected() to also update the icon and
		 * restyle.
		 * @param selected True to select the button.
		 * @return This component, for chaining.
		 */
		Button &setSelected(bool selected);

		/**
		 * @brief Check whether the corner shape changes when selected.
		 * @return True when a selected toggle swaps round and square corners.
		 */
		bool isMorph(void) const noexcept;

		/**
		 * @brief Choose whether the corner shape changes when selected.
		 *
		 * Read by systems::CommonButton only: a selected toggle with morph
		 * draws the opposite of its configured shape (round becomes square
		 * and the reverse).
		 * @param morph True to swap the shape while selected.
		 * @return This component, for chaining.
		 */
		Button &setMorph(bool morph);

		/**
		 * @brief Check on which side of the label the icon sits.
		 * @return True when the icon comes after the label.
		 */
		bool isTrailingIcon(void) const noexcept;

		/**
		 * @brief Choose on which side of the label the icon sits.
		 *
		 * Read by systems::CommonButton only.
		 * @param trailing True to place the icon after the label, false to
		 * place it before.
		 * @return This component, for chaining.
		 */
		Button &setTrailingIcon(bool trailing);

		/**
		 * @brief Check whether the button shows a progress glyph.
		 * @return True while loading.
		 */
		bool isLoading(void) const noexcept;

		/**
		 * @brief Show a progress glyph in place of the icon, or the icon
		 * again.
		 *
		 * While loading, systems::Button gives the icon child the glyph
		 * systems::Button::LoadingGlyph. The button still reacts to input.
		 * @param loading True to show the progress glyph.
		 * @return This component, for chaining.
		 */
		Button &setLoading(bool loading);

		/**
		 * @brief Check whether the button is shown.
		 * @return True when shown (the default).
		 */
		bool isVisible(void) const noexcept;

		/**
		 * @brief Show or hide the button.
		 *
		 * A hidden button ignores press and release. Only the floating
		 * action button systems also draw it transparent and without
		 * shadow; the other variants keep drawing it.
		 * @param visible False to hide the button.
		 * @return This component, for chaining.
		 */
		Button &setVisible(bool visible);

		/**
		 * @brief Get the click callback.
		 * @return The callback, empty when none is set.
		 */
		const Handler &getOnClick(void) const noexcept;

		/**
		 * @brief Set the callback run when the button is released.
		 *
		 * It is not run when the release counts as a long press and a
		 * long-press callback is set. Does not mark the component changed.
		 * @param handler The callback, or an empty one to remove it.
		 * @return This component, for chaining.
		 */
		Button &setOnClick(Handler handler);

		/**
		 * @brief Get the long-press callback.
		 * @return The callback, empty when none is set.
		 */
		const Handler &getOnLongPress(void) const noexcept;

		/**
		 * @brief Set the callback run when the button is released after a
		 * long press.
		 *
		 * A press held for at least systems::Button::LongPressDuration runs
		 * it instead of the click callback. Does not mark the component
		 * changed.
		 * @param handler The callback, or an empty one so that every release
		 * is a click.
		 * @return This component, for chaining.
		 */
		Button &setOnLongPress(Handler handler);

		/**
		 * @brief Get when the current press started.
		 * @return The time of the press, or a default-constructed TimePoint
		 * when the button is not being pressed.
		 */
		TimePoint getPressStart(void) const noexcept;

		/**
		 * @brief Record when the current press started.
		 *
		 * Written by systems::Button: the time on press, a
		 * default-constructed TimePoint on release.
		 * @param time The start of the press, or `TimePoint {}` for none.
		 * @return This component, for chaining.
		 */
		Button &setPressStart(TimePoint time);

		/**
		 * @brief Get the color that replaces the themed container color.
		 * @return The color, or std::nullopt when the theme decides.
		 */
		const OptionalColor &getContainerColorOverride(void) const noexcept;

		/**
		 * @brief Replace the themed container (background) color.
		 *
		 * The variant system uses it as the resting color and still blends
		 * the hover and press layers over it. Disabled buttons keep the
		 * themed disabled color.
		 * @param color The container color, or std::nullopt to follow the
		 * theme.
		 * @return This component, for chaining.
		 */
		Button &setContainerColorOverride(const OptionalColor &color);

		/**
		 * @brief Get the color that replaces the themed content color.
		 * @return The color, or std::nullopt when the theme decides.
		 */
		const OptionalColor &getContentColorOverride(void) const noexcept;

		/**
		 * @brief Replace the themed content (icon and label) color.
		 * @param color The content color, or std::nullopt to follow the
		 * theme.
		 * @return This component, for chaining.
		 */
		Button &setContentColorOverride(const OptionalColor &color);

		/**
		 * @brief Get the function that restyles this button at once.
		 * @return The static `apply` of the variant system, or nullptr when
		 * no variant system was installed on the entity.
		 */
		RestyleFunction getRestyle(void) const noexcept;

		/**
		 * @brief Register the function that restyles this button at once.
		 *
		 * Each variant system's `install()` passes its static `apply`.
		 * systems::Button::restyle() calls it after every hover, press,
		 * release and selection change.
		 * @param function The restyle function, or nullptr for none.
		 * @return This component, for chaining.
		 */
		Button &setRestyle(RestyleFunction function);
	};

}	 // namespace guillaume::components
