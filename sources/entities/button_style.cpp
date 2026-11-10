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

#include "guillaume/entities/button_style.hpp"

#include <array>
#include <cstddef>
#include <optional>
#include <stdexcept>

#include "guillaume/theme.hpp"

namespace guillaume::entities::button_style
{
	namespace
	{
		using Role = SchemeColorRole;

		/// Metrics of one button size.
		struct SizeMetrics {
			float heightPadding;			  ///< Vertical padding.
			float widthPadding;				  ///< Horizontal padding.
			float iconFontSize;				  ///< Icon font size.
			float iconLabelGap;				  ///< Icon to label gap.
			theme::ShapeRole pressedShape;	  ///< Corner shape when pressed.
			theme::ShapeRole squareShape;	  ///< Corner shape when square.
		};

		/// Indexed by Button::Size.
		constexpr std::array<SizeMetrics, 5> sizeTable { {
			// ExtraSmall
			{ 6.0f, 12.0f, 15.0f, 4.0f, theme::ShapeRole::Small,
			  theme::ShapeRole::Medium },
			// Small
			{ 8.0f, 16.0f, 18.0f, 8.0f, theme::ShapeRole::Small,
			  theme::ShapeRole::Medium },
			// Medium
			{ 12.0f, 24.0f, 18.0f, 8.0f, theme::ShapeRole::Medium,
			  theme::ShapeRole::Large },
			// Large
			{ 24.0f, 48.0f, 24.0f, 12.0f, theme::ShapeRole::Large,
			  theme::ShapeRole::ExtraLarge },
			// ExtraLarge
			{ 32.0f, 64.0f, 30.0f, 16.0f, theme::ShapeRole::Large,
			  theme::ShapeRole::ExtraLarge },
		} };

		/// Scheme roles of one mode; no container role means transparent.
		struct ModePalette {
			std::optional<Role> container;	  ///< Container role.
			Role content;					  ///< Icon and label role.
		};

		/// Palette of one color style.
		struct Palette {
			ModePalette plain;		   ///< Non-toggle button.
			ModePalette unselected;	   ///< Unselected toggle.
			ModePalette selected;	   ///< Selected toggle.
		};

		/// Indexed by Button::Color.
		constexpr std::array<Palette, 6> paletteTable { {
			// Elevated
			{ { Role::SurfaceContainerLow, Role::Primary },
			  { Role::SurfaceContainerLow, Role::Primary },
			  { Role::Primary, Role::OnPrimary } },
			// Filled
			{ { Role::Primary, Role::OnPrimary },
			  { Role::SurfaceContainer, Role::OnSurfaceVariant },
			  { Role::Primary, Role::OnPrimary } },
			// Tonal
			{ { Role::SecondaryContainer, Role::OnSecondaryContainer },
			  { Role::SecondaryContainer, Role::OnSecondaryContainer },
			  { Role::Secondary, Role::OnSecondary } },
			// Outlined
			{ { std::nullopt, Role::Primary },
			  { std::nullopt, Role::Primary },
			  { Role::SecondaryContainer, Role::OnSecondaryContainer } },
			// Text
			{ { std::nullopt, Role::Primary },
			  { std::nullopt, Role::Primary },
			  { std::nullopt, Role::Primary } },
			// Segmented
			{ { std::nullopt, Role::OnSurfaceVariant },
			  { std::nullopt, Role::OnSurfaceVariant },
			  { Role::SecondaryContainer, Role::OnSecondaryContainer } },
		} };

		const SizeMetrics &metrics(Button::Size size)
		{
			const auto index = static_cast<std::size_t>(size);
			if (index >= sizeTable.size()) {
				throw std::runtime_error("Invalid button size");
			}
			return sizeTable[index];
		}

		const ModePalette &palette(Button::Color style, bool isToggle,
								   bool isSelected)
		{
			const auto index = static_cast<std::size_t>(style);
			if (index >= paletteTable.size()) {
				throw std::runtime_error("Invalid button color style");
			}
			const Palette &entry = paletteTable[index];
			if (!isToggle) {
				return entry.plain;
			}
			return isSelected ? entry.selected : entry.unselected;
		}

		utility::graphic::Color32Bit transparent(void)
		{
			return utility::graphic::Color32Bit(0, 0, 0, 0);
		}

		utility::graphic::Color32Bit getBaseContainerColor(Button::Color style,
														   bool isToggle,
														   bool isSelected)
		{
			const auto role = palette(style, isToggle, isSelected).container;
			return role ? getActiveColor(*role) : transparent();
		}
	}	 // namespace

	float getHeightPadding(Button::Size size)
	{
		return metrics(size).heightPadding;
	}

	float getWidthPadding(Button::Size size, bool compact)
	{
		// Paper compact buttons halve the horizontal label margin.
		const float padding = metrics(size).widthPadding;
		return compact ? padding / 2.0f : padding;
	}

	float getIconFontSize(Button::Size size)
	{
		return metrics(size).iconFontSize;
	}

	float getSpaceBetweenIconAndLabel(Button::Size size)
	{
		return metrics(size).iconLabelGap;
	}

	float getBorderRadius(Button::Size size, Button::Shape shape,
						  bool isButtonPressed)
	{
		const SizeMetrics &entry = metrics(size);

		if (isButtonPressed) {
			return getShapeRadius(entry.pressedShape);
		}
		if (shape == Button::Shape::Round) {
			return getShapeRadius(theme::ShapeRole::Full);
		}
		return getShapeRadius(entry.squareShape);
	}

	Button::Shape getRestingShape(Button::Shape baseShape, bool isToggle,
								  bool isSelected, bool isMorph)
	{
		if (!isMorph || !isToggle || !isSelected) {
			return baseShape;
		}

		return (baseShape == Button::Shape::Round) ? Button::Shape::Square
												   : Button::Shape::Round;
	}

	utility::graphic::Color32Bit
		getContainerColor(Button::Color style, bool isToggle, bool isSelected,
						  bool isHovered, bool isButtonPressed)
	{
		const auto base = getBaseContainerColor(style, isToggle, isSelected);

		if (!isButtonPressed && !isHovered) {
			return base;
		}
		return ButtonBase::applyStateLayer(
			base, getContentColor(style, isToggle, isSelected),
			getStateLayerAlpha(isButtonPressed ? theme::StateLayer::Pressed
											   : theme::StateLayer::Hovered));
	}

	utility::graphic::Color32Bit getContentColor(Button::Color style,
												 bool isToggle, bool isSelected)
	{
		return getActiveColor(palette(style, isToggle, isSelected).content);
	}

	utility::graphic::Color32Bit getBorderColor(Button::Color style,
												bool isToggle, bool isSelected)
	{
		// Segmented buttons keep their outline when selected (Paper
		// SegmentedButtons); outlined toggles drop it.
		const bool outlined = style == Button::Color::Segmented
			|| (style == Button::Color::Outlined && !(isToggle && isSelected));

		return outlined ? getActiveColor(Role::Outline) : transparent();
	}
}	 // namespace guillaume::entities::button_style
