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

#include <utility/graphic/color.hpp>

#include "guillaume/components/common_button.hpp"

/**
 * @file
 * @brief Style tables of the common button: paddings, icon size, corner
 * radius and colors for each size, shape and color style.
 *
 * The functions of guillaume::systems::button_style are pure lookups: they
 * take values of the components::CommonButton enumerations and return a
 * metric in dp or a color of the active theme. They read and write no
 * component. systems::CommonButton is the caller: its `apply` function
 * turns the results into components::Color, components::Borders and
 * components::Layout values. `guillaume/entities/button_style.hpp`
 * re-exports the same names for the widget classes.
 */

/**
 * @namespace guillaume::systems::button_style
 * @brief Maps a common button size, shape and color style to Material 3
 * metrics and scheme colors.
 *
 * Every value comes from one lookup table per enumeration (in
 * `sources/systems/button_style.cpp`), so adding a size or a color style
 * is a single table row. Functions taking a size or a color style throw
 * std::runtime_error when the value is not an enumerator.
 * @see guillaume::systems::CommonButton, guillaume::components::CommonButton,
 * guillaume::systems::fab_style
 */
namespace guillaume::systems::button_style
{
	/**
	 * @brief Space between the content and the top edge of the container
	 * (and, equally, the bottom edge).
	 * @param size The button size.
	 * @return The padding of one side, in dp (6 for ExtraSmall up to 32 for
	 * ExtraLarge).
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	float getHeightPadding(components::CommonButton::Size size);

	/**
	 * @brief Space between the content and the left edge of the container
	 * (and, equally, the right edge).
	 * @param size The button size.
	 * @param compact True to halve the padding, as components::CommonButton
	 * asks for with `setCompact(true)`.
	 * @return The padding of one side, in dp (12 for ExtraSmall up to 64 for
	 * ExtraLarge, before halving).
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	float getWidthPadding(components::CommonButton::Size size,
						  bool compact = false);

	/**
	 * @brief Font size of the leading icon of a button.
	 * @param size The button size.
	 * @return The font size, in dp (15 for ExtraSmall up to 30 for
	 * ExtraLarge).
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	float getIconFontSize(components::CommonButton::Size size);

	/**
	 * @brief Horizontal gap between the icon and the label.
	 * @param size The button size.
	 * @return The gap, in dp (4 for ExtraSmall up to 16 for ExtraLarge).
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	float getSpaceBetweenIconAndLabel(components::CommonButton::Size size);

	/**
	 * @brief Corner radius of the container.
	 *
	 * A pressed button takes the pressed shape of its size whatever its
	 * shape. At rest, a Round button is fully rounded (theme::ShapeRole::Full)
	 * and a Square one takes the square shape of its size.
	 * @param size The button size.
	 * @param shape The shape at rest, usually the result of getRestingShape.
	 * @param isButtonPressed True while the button is pressed.
	 * @return The radius, in dp, resolved against the active theme.
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	float getBorderRadius(components::CommonButton::Size size,
						  components::CommonButton::Shape shape,
						  bool isButtonPressed);

	/**
	 * @brief Shape a button shows while it is not pressed.
	 *
	 * A toggle button that morphs shows the opposite shape while selected:
	 * Round becomes Square and Square becomes Round. Every other button
	 * shows its configured shape.
	 * @param baseShape The shape configured in components::CommonButton.
	 * @param isToggle True for a toggle button.
	 * @param isSelected True when the toggle is selected.
	 * @param isMorph True when the toggle changes shape on selection.
	 * @return The shape to draw at rest.
	 */
	components::CommonButton::Shape
		getRestingShape(components::CommonButton::Shape baseShape,
						bool isToggle, bool isSelected, bool isMorph);

	/**
	 * @brief Fill color of the container, hover or press tint included.
	 *
	 * The base color depends on the style and, for a toggle, on whether it
	 * is selected. While hovered or pressed, the content color is blended
	 * over it at the theme opacity of that state; a press wins over a hover.
	 * @param style The color style.
	 * @param isToggle True for a toggle button.
	 * @param isSelected True when the toggle is selected. Ignored when
	 * `isToggle` is false.
	 * @param isHovered True while a pointer or a hand hovers the button.
	 * @param isButtonPressed True while the button is pressed.
	 * @return The container color. Styles without a container (Outlined,
	 * Text, Segmented when not selected) give a transparent base, so only
	 * the tint shows.
	 * @throws std::runtime_error If the style is not a valid enumerator.
	 */
	utility::graphic::Color32Bit
		getContainerColor(components::CommonButton::Color style, bool isToggle,
						  bool isSelected, bool isHovered,
						  bool isButtonPressed);

	/**
	 * @brief Color of the icon and of the label of an enabled button.
	 * @param style The color style.
	 * @param isToggle True for a toggle button.
	 * @param isSelected True when the toggle is selected. Ignored when
	 * `isToggle` is false.
	 * @return The content color, from the active color scheme.
	 * @throws std::runtime_error If the style is not a valid enumerator.
	 */
	utility::graphic::Color32Bit
		getContentColor(components::CommonButton::Color style, bool isToggle,
						bool isSelected);

	/**
	 * @brief Color of the outline of an enabled button.
	 *
	 * Only two styles are outlined: Segmented, always, and Outlined, except
	 * for a selected toggle, which shows a filled container instead.
	 * @param style The color style.
	 * @param isToggle True for a toggle button.
	 * @param isSelected True when the toggle is selected.
	 * @return The Outline color of the active scheme, or a fully transparent
	 * color for a button without an outline. Never throws.
	 */
	utility::graphic::Color32Bit
		getBorderColor(components::CommonButton::Color style, bool isToggle,
					   bool isSelected);

}	 // namespace guillaume::systems::button_style
