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

#include <cstddef>

#include <utility/graphic/color.hpp>

/**
 * @file
 * @brief Style tables of the floating action buttons (FAB): container
 * size, font sizes, corner radius and colors for each size and variant.
 *
 * The functions of guillaume::systems::fab_style are pure lookups: they
 * return a metric in dp or a color of the active theme and touch no
 * component. systems::FloatingActionButton and
 * systems::ExtendedFloatingActionButton call them from their `apply`
 * functions. `guillaume/entities/fab_style.hpp` re-exports the same names
 * for the widget classes.
 */

/**
 * @namespace guillaume::systems::fab_style
 * @brief Maps a FAB size and variant to Material 3 metrics and scheme
 * colors, for the regular and the extended FAB alike.
 *
 * components::FloatingActionButton and
 * components::ExtendedFloatingActionButton each declare their own `Size`
 * (Small, Medium, Large) and `Variant` (Surface, Primary, Secondary,
 * Tertiary) enumerations with the same enumerators in the same order. The
 * functions are therefore templated on the enumeration type and resolve
 * through one shared table, indexed by the integer value of the
 * enumerator.
 *
 * @code
 * using Size = components::FloatingActionButton::Size;
 * const float side = fab_style::getContainerSize(Size::Medium); // 56 dp
 * @endcode
 * @see guillaume::systems::FloatingActionButton,
 * guillaume::systems::ExtendedFloatingActionButton,
 * guillaume::systems::button_style
 */
namespace guillaume::systems::fab_style
{
	/**
	 * @brief The table lookups behind the templates, taking the enumerator
	 * as an index. Call the templated functions of fab_style instead.
	 */
	namespace detail
	{
		/**
		 * @brief Width and height of the container at its default size.
		 * @param size Index of the size: 0 Small, 1 Medium, 2 Large.
		 * @return The extent, in dp.
		 * @throws std::runtime_error If the index is out of range.
		 */
		float containerSize(std::size_t size);
		/**
		 * @brief Font size of the icon of a regular FAB.
		 * @param size Index of the size: 0 Small, 1 Medium, 2 Large.
		 * @return The font size, in dp.
		 * @throws std::runtime_error If the index is out of range.
		 */
		float iconFontSize(std::size_t size);
		/**
		 * @brief Font size of the label and icon of an extended FAB.
		 * @param size Index of the size: 0 Small, 1 Medium, 2 Large.
		 * @return The font size, in dp.
		 * @throws std::runtime_error If the index is out of range.
		 */
		float labelFontSize(std::size_t size);
		/**
		 * @brief Corner radius of the container.
		 * @param size Index of the size: 0 Small, 1 Medium, 2 Large.
		 * @return The radius, in dp, resolved against the active theme.
		 * @throws std::runtime_error If the index is out of range.
		 */
		float borderRadius(std::size_t size);
		/**
		 * @brief Fill color of the container.
		 * @param variant Index of the variant: 0 Surface, 1 Primary,
		 * 2 Secondary, 3 Tertiary.
		 * @param lowered True for a FAB drawn at the lowered elevation. Only
		 * the Surface variant changes color with it.
		 * @return The container color.
		 * @throws std::runtime_error If the index is out of range.
		 */
		utility::graphic::Color32Bit containerColor(std::size_t variant,
													bool lowered);
		/**
		 * @brief Color of the icon and of the label.
		 * @param variant Index of the variant: 0 Surface, 1 Primary,
		 * 2 Secondary, 3 Tertiary.
		 * @return The content color, from the active color scheme.
		 * @throws std::runtime_error If the index is out of range.
		 */
		utility::graphic::Color32Bit contentColor(std::size_t variant);
	}	 // namespace detail

	/**
	 * @brief Width and height of the container of a FAB at its default
	 * size: 40 dp for Small, 56 dp for Medium, 96 dp for Large.
	 *
	 * An extended FAB uses it as its height; its width follows the label.
	 * @tparam SizeType The `Size` enumeration of the FAB component.
	 * @param size The FAB size.
	 * @return The extent, in dp.
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	template<typename SizeType> float getContainerSize(SizeType size)
	{
		return detail::containerSize(static_cast<std::size_t>(size));
	}

	/**
	 * @brief Font size of the icon of a regular FAB: 24 dp for Small and
	 * Medium, 36 dp for Large.
	 * @tparam SizeType The `Size` enumeration of the FAB component.
	 * @param size The FAB size.
	 * @return The font size, in dp.
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	template<typename SizeType> float getIconFontSize(SizeType size)
	{
		return detail::iconFontSize(static_cast<std::size_t>(size));
	}

	/**
	 * @brief Font size shared by the label and the icon of an extended FAB:
	 * 18 dp for Small, 24 dp for Medium, 36 dp for Large.
	 * @tparam SizeType The `Size` enumeration of the FAB component.
	 * @param size The FAB size.
	 * @return The font size, in dp.
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	template<typename SizeType> float getLabelFontSize(SizeType size)
	{
		return detail::labelFontSize(static_cast<std::size_t>(size));
	}

	/**
	 * @brief Corner radius of the container: the Medium, Large and
	 * ExtraLarge shape of the active theme for a Small, Medium and Large
	 * FAB.
	 * @tparam SizeType The `Size` enumeration of the FAB component.
	 * @param size The FAB size.
	 * @return The radius, in dp.
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	template<typename SizeType> float getBorderRadius(SizeType size)
	{
		return detail::borderRadius(static_cast<std::size_t>(size));
	}

	/**
	 * @brief Fill color of the container of a FAB.
	 *
	 * Primary, Secondary and Tertiary use the matching container role of
	 * the active scheme. Surface has no role of its own: it uses the
	 * surface color tinted for elevation level 3, or level 1 when lowered.
	 * @tparam VariantType The `Variant` enumeration of the FAB component.
	 * @param variant The FAB variant.
	 * @param lowered True for a FAB drawn at the lowered elevation. Only the
	 * Surface variant changes color with it.
	 * @return The container color, without any hover or press tint.
	 * @throws std::runtime_error If the variant is not a valid enumerator.
	 */
	template<typename VariantType> utility::graphic::Color32Bit
		getContainerColor(VariantType variant, bool lowered)
	{
		return detail::containerColor(static_cast<std::size_t>(variant),
									  lowered);
	}

	/**
	 * @brief Color of the icon and of the label of a FAB: Primary for the
	 * Surface variant, the "on container" role of the others.
	 * @tparam VariantType The `Variant` enumeration of the FAB component.
	 * @param variant The FAB variant.
	 * @return The content color, from the active color scheme.
	 * @throws std::runtime_error If the variant is not a valid enumerator.
	 */
	template<typename VariantType>
	utility::graphic::Color32Bit getContentColor(VariantType variant)
	{
		return detail::contentColor(static_cast<std::size_t>(variant));
	}

}	 // namespace guillaume::systems::fab_style
