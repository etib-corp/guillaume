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
 * @brief Style resolution of entities::Button.
 *
 * Maps a button size, shape and color style to Material 3 metrics and scheme
 * colors. Every value comes from one lookup table per enumeration, so adding
 * a size or a color style is a single table row.
 */
namespace guillaume::systems::button_style
{
	/**
	 * @brief Vertical padding around the content.
	 * @param size The button size.
	 * @return The padding in dp.
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	float getHeightPadding(components::CommonButton::Size size);

	/**
	 * @brief Horizontal padding around the content.
	 * @param size The button size.
	 * @param compact True to halve the padding (Paper compact buttons).
	 * @return The padding in dp.
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	float getWidthPadding(components::CommonButton::Size size,
						  bool compact = false);

	/**
	 * @brief Icon font size.
	 * @param size The button size.
	 * @return The font size in dp.
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	float getIconFontSize(components::CommonButton::Size size);

	/**
	 * @brief Gap between the icon and the label.
	 * @param size The button size.
	 * @return The gap in dp.
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	float getSpaceBetweenIconAndLabel(components::CommonButton::Size size);

	/**
	 * @brief Corner radius of the container.
	 * @param size The button size.
	 * @param shape The resting shape.
	 * @param isButtonPressed True while the button is pressed.
	 * @return The radius in dp.
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	float getBorderRadius(components::CommonButton::Size size,
						  components::CommonButton::Shape shape,
						  bool isButtonPressed);

	/**
	 * @brief Shape shown at rest; a selected morphing toggle swaps it.
	 * @param baseShape The configured shape.
	 * @param isToggle True for a toggle button.
	 * @param isSelected True when the toggle is selected.
	 * @param isMorph True when the toggle morphs its shape.
	 * @return The shape to draw.
	 */
	components::CommonButton::Shape
		getRestingShape(components::CommonButton::Shape baseShape,
						bool isToggle, bool isSelected, bool isMorph);

	/**
	 * @brief Container color, including the hover and press state layers.
	 * @param style The color style.
	 * @param isToggle True for a toggle button.
	 * @param isSelected True when the toggle is selected.
	 * @param isHovered True while hovered.
	 * @param isButtonPressed True while pressed.
	 * @return The container color.
	 * @throws std::runtime_error If the style is not a valid enumerator.
	 */
	utility::graphic::Color32Bit
		getContainerColor(components::CommonButton::Color style, bool isToggle,
						  bool isSelected, bool isHovered,
						  bool isButtonPressed);

	/**
	 * @brief Icon and label color.
	 * @param style The color style.
	 * @param isToggle True for a toggle button.
	 * @param isSelected True when the toggle is selected.
	 * @return The content color.
	 * @throws std::runtime_error If the style is not a valid enumerator.
	 */
	utility::graphic::Color32Bit
		getContentColor(components::CommonButton::Color style, bool isToggle,
						bool isSelected);

	/**
	 * @brief Outline color; transparent for styles without an outline.
	 * @param style The color style.
	 * @param isToggle True for a toggle button.
	 * @param isSelected True when the toggle is selected.
	 * @return The border color.
	 */
	utility::graphic::Color32Bit
		getBorderColor(components::CommonButton::Color style, bool isToggle,
					   bool isSelected);

}	 // namespace guillaume::systems::button_style
