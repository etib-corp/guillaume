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
	 * @brief State shared by every pressable button.
	 *
	 * Holds what the button shows (icon glyph, label, the identifiers of the
	 * icon and label children), how it reacts (toggle, callbacks, long press)
	 * and its transient state (selected, loading, visible). systems::Button
	 * turns input into this state; a variant system (systems::CommonButton,
	 * systems::IconButton, ...) turns it into colors and geometry.
	 */
	class Button: public ecs::Component
	{
		public:
		/// Called when the button is clicked or long-pressed.
		using Handler = std::function<void(void)>;

		/// Clock used to time a press.
		using TimePoint = std::chrono::steady_clock::time_point;

		/// An optional color override.
		using OptionalColor = std::optional<utility::graphic::Color32Bit>;

		/// Function of the variant system that restyles the button now.
		using RestyleFunction = void (*)(ecs::ComponentRegistry &,
										 const ecs::Entity::Identifier &);

		private:
		std::string _iconGlyphName {};	  ///< Icon glyph name
		Glyph::Style _iconStyle { Glyph::Style::Outlined };	   ///< Icon style
		std::string
			_selectedIconGlyphName {};	  ///< Icon glyph name when selected
		std::string _labelContent {};	  ///< Label content
		ecs::Entity::Identifier _icon {
			ecs::Entity::InvalidIdentifier
		};	  ///< Icon child

		ecs::Entity::Identifier _label {
			ecs::Entity::InvalidIdentifier
		};	  ///< Label child

		bool _toggle { false };		 ///< Whether a click toggles the selection
		bool _selected { false };	 ///< Whether the button is selected
		bool _morph { false };		 ///< Whether the shape morphs when selected
		bool _trailingIcon { false };	 ///< Whether the icon trails the label
		bool _loading {
			false
		};	  ///< Whether a progress glyph replaces the icon

		bool _visible { true };	   ///< Whether the button is shown
		Handler _onClick {};	   ///< Click callback
		Handler
			_onLongPress {};	///< Long-press callback (replaces the click)
		TimePoint _pressStart {};	 ///< When the current press started
		OptionalColor _containerColorOverride {};	 ///< Custom container color
		OptionalColor _contentColorOverride {};		 ///< Custom content color
		RestyleFunction _restyle {
			nullptr
		};	  ///< Restyle function of the variant system

		public:
		/**
		 * @brief Default constructor.
		 */
		Button(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Button(void) = default;

		/** @brief Get: icon glyph name. @return The value. */
		const std::string &getIconGlyphName(void) const noexcept;

		/** @brief Set: icon glyph name. @param name The value. @return It. */
		Button &setIconGlyphName(const std::string &name);

		/** @brief Get: icon style. @return The value. */
		Glyph::Style getIconStyle(void) const noexcept;

		/** @brief Set: icon style. @param style The value. @return It. */
		Button &setIconStyle(Glyph::Style style);

		/** @brief Get: icon glyph name when selected. @return The value. */
		const std::string &getSelectedIconGlyphName(void) const noexcept;

		/** @brief Set: icon glyph name when selected. @param name The value.
		 * @return It. */
		Button &setSelectedIconGlyphName(const std::string &name);

		/** @brief Get: label content. @return The value. */
		const std::string &getLabelContent(void) const noexcept;

		/** @brief Set: label content. @param content The value. @return It. */
		Button &setLabelContent(const std::string &content);

		/** @brief Get: icon child. @return The value. */
		ecs::Entity::Identifier getIcon(void) const noexcept;

		/** @brief Set: icon child. @param identifier The value. @return It. */
		Button &setIcon(ecs::Entity::Identifier identifier);

		/** @brief Get: label child. @return The value. */
		ecs::Entity::Identifier getLabel(void) const noexcept;

		/** @brief Set: label child. @param identifier The value. @return It. */
		Button &setLabel(ecs::Entity::Identifier identifier);

		/** @brief Get: whether a click toggles the selection. @return The
		 * value. */
		bool isToggle(void) const noexcept;

		/** @brief Set: whether a click toggles the selection. @param toggle The
		 * value. @return It. */
		Button &setToggle(bool toggle);

		/** @brief Get: whether the button is selected. @return The value. */
		bool isSelected(void) const noexcept;

		/** @brief Set: whether the button is selected. @param selected The
		 * value. @return It. */
		Button &setSelected(bool selected);

		/** @brief Get: whether the shape morphs when selected. @return The
		 * value. */
		bool isMorph(void) const noexcept;

		/** @brief Set: whether the shape morphs when selected. @param morph The
		 * value. @return It. */
		Button &setMorph(bool morph);

		/** @brief Get: whether the icon trails the label. @return The value. */
		bool isTrailingIcon(void) const noexcept;

		/** @brief Set: whether the icon trails the label. @param trailing The
		 * value. @return It. */
		Button &setTrailingIcon(bool trailing);

		/** @brief Get: whether a progress glyph replaces the icon. @return The
		 * value. */
		bool isLoading(void) const noexcept;

		/** @brief Set: whether a progress glyph replaces the icon. @param
		 * loading The value. @return It. */
		Button &setLoading(bool loading);

		/** @brief Get: whether the button is shown. @return The value. */
		bool isVisible(void) const noexcept;

		/** @brief Set: whether the button is shown. @param visible The value.
		 * @return It. */
		Button &setVisible(bool visible);

		/** @brief Get: click callback. @return The value. */
		const Handler &getOnClick(void) const noexcept;

		/** @brief Set: click callback. @param handler The value. @return It. */
		Button &setOnClick(Handler handler);

		/** @brief Get: long-press callback (replaces the click). @return The
		 * value. */
		const Handler &getOnLongPress(void) const noexcept;

		/** @brief Set: long-press callback (replaces the click). @param handler
		 * The value. @return It. */
		Button &setOnLongPress(Handler handler);

		/** @brief Get: when the current press started. @return The value. */
		TimePoint getPressStart(void) const noexcept;

		/** @brief Set: when the current press started. @param time The value.
		 * @return It. */
		Button &setPressStart(TimePoint time);

		/** @brief Get: custom container color. @return The value. */
		const OptionalColor &getContainerColorOverride(void) const noexcept;

		/** @brief Set: custom container color. @param color The value. @return
		 * It. */
		Button &setContainerColorOverride(const OptionalColor &color);

		/** @brief Get: custom content color. @return The value. */
		const OptionalColor &getContentColorOverride(void) const noexcept;

		/** @brief Set: custom content color. @param color The value. @return
		 * It. */
		Button &setContentColorOverride(const OptionalColor &color);

		/** @brief Get: restyle function of the variant system. @return The
		 * value. */
		RestyleFunction getRestyle(void) const noexcept;

		/** @brief Set: restyle function of the variant system. @param function
		 * The value. @return It. */
		Button &setRestyle(RestyleFunction function);
	};

}	 // namespace guillaume::components
