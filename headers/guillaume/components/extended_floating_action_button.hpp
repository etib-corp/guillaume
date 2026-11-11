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

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief Look of a Material 3 extended floating action button: a FAB
	 * that shows a label next to its icon and can collapse to the icon
	 * alone.
	 *
	 * It sits on the same entity as components::Button, which holds the
	 * glyph, the label text and the interaction state.
	 * entities::ExtendedFloatingActionButton writes it;
	 * systems::ExtendedFloatingActionButton reads both, each frame in the
	 * Layout phase and at once after every input change, and writes the
	 * button's Color, Elevation, Borders and Bound and places the icon and
	 * label children.
	 *
	 * Hiding the button is done on components::Button
	 * (`Button::setVisible(false)`): the system then draws it transparent
	 * and without shadow.
	 *
	 * Every setter compares before writing and marks the component changed
	 * only when the value differs.
	 *
	 * @see components::Button, systems::ExtendedFloatingActionButton,
	 * components::FloatingActionButton
	 */
	class ExtendedFloatingActionButton: public ecs::Component
	{
		public:
		/**
		 * @brief Which theme colors fill the container, the icon and the
		 * label.
		 *
		 * Primary, Secondary and Tertiary use the matching container role.
		 * Surface uses the tonal surface color of the button's elevation
		 * with primary content.
		 */
		enum class Variant { Surface, Primary, Secondary, Tertiary };

		/**
		 * @brief Whether the button casts a shadow (Elevated) or not (Flat).
		 */
		enum class Mode { Elevated, Flat };

		/**
		 * @brief Size step: a container 40, 56 or 96 dp high.
		 */
		enum class Size { Small, Medium, Large };

		private:
		Variant _variant { Variant::Primary };	  ///< Color variant
		Size _size { Size::Medium };			  ///< Size step
		Mode _mode { Mode::Elevated };			  ///< Shadow mode
		std::optional<float> _customSize {};	  ///< Container height, in dp
		bool _extended { true };	///< Whether the label is shown
		bool _lowered { false };	///< Whether the resting shadow is lower

		public:
		/**
		 * @brief Create the configuration of a medium, elevated, primary
		 * button with its label shown.
		 */
		ExtendedFloatingActionButton(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~ExtendedFloatingActionButton(void) = default;

		/**
		 * @brief Get the color variant.
		 * @return The variant (Primary by default).
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Choose which theme colors the button uses.
		 * @param variant The color variant.
		 * @return This component, for chaining.
		 */
		ExtendedFloatingActionButton &setVariant(Variant variant);

		/**
		 * @brief Get the size step.
		 * @return The size (Medium by default).
		 */
		Size getSize(void) const noexcept;

		/**
		 * @brief Choose the size step.
		 * @param size The size; it sets the container height, the corner
		 * radius and the icon font size (18, 24 or 36 dp).
		 * @return This component, for chaining.
		 */
		ExtendedFloatingActionButton &setSize(Size size);

		/**
		 * @brief Get the shadow mode.
		 * @return The mode (Elevated by default).
		 */
		Mode getMode(void) const noexcept;

		/**
		 * @brief Choose whether the button casts a shadow.
		 * @param mode Elevated for a shadow that rises one level on hover,
		 * Flat for no shadow at all.
		 * @return This component, for chaining.
		 */
		ExtendedFloatingActionButton &setMode(Mode mode);

		/**
		 * @brief Get the container height that replaces the one of the size
		 * step.
		 * @return The height, in dp, or std::nullopt when the size step
		 * decides.
		 */
		const std::optional<float> &getCustomSize(void) const noexcept;

		/**
		 * @brief Replace the container height of the size step.
		 *
		 * It is also the width of the collapsed button. The corner radius
		 * follows: it is the custom size divided by 3.5.
		 * @param size The height, in dp, or std::nullopt to follow the size
		 * step.
		 * @return This component, for chaining.
		 */
		ExtendedFloatingActionButton &
			setCustomSize(const std::optional<float> &size);

		/**
		 * @brief Check whether the label is shown.
		 * @return True when extended, false when collapsed to the icon.
		 */
		bool isExtended(void) const noexcept;

		/**
		 * @brief Show the label, or collapse the button to its icon.
		 *
		 * Collapsed, the button is a square with the icon centered; the
		 * label child stays in the tree but is drawn transparent. An
		 * extended button whose label measures nothing is laid out as
		 * collapsed.
		 * @param extended True to show the label.
		 * @return This component, for chaining.
		 */
		ExtendedFloatingActionButton &setExtended(bool extended);

		/**
		 * @brief Check whether the resting shadow is lowered.
		 * @return True when the button rests at elevation level 1.
		 */
		bool isLowered(void) const noexcept;

		/**
		 * @brief Lower the resting shadow, or restore it.
		 *
		 * An elevated button rests at elevation level 3, or level 1 when
		 * lowered. The Surface variant takes the surface tone of the same
		 * level.
		 * @param lowered True to rest at elevation level 1.
		 * @return This component, for chaining.
		 */
		ExtendedFloatingActionButton &setLowered(bool lowered);
	};

}	 // namespace guillaume::components
