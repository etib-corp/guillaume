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
 * @brief Style resolution shared by entities::FloatingActionButton and
 * entities::ExtendedFloatingActionButton.
 *
 * Both entities declare the same `Size` (Small, Medium, Large) and `Variant`
 * (Surface, Primary, Secondary, Tertiary) enumerations, so the functions are
 * templated on the enumeration type and resolve through one shared table.
 */
namespace guillaume::systems::fab_style
{
	namespace detail
	{
		/** @brief Container extent. @param size Size index. @return dp. */
		float containerSize(std::size_t size);
		/** @brief Icon font size. @param size Size index. @return dp. */
		float iconFontSize(std::size_t size);
		/** @brief Label font size. @param size Size index. @return dp. */
		float labelFontSize(std::size_t size);
		/** @brief Corner radius. @param size Size index. @return dp. */
		float borderRadius(std::size_t size);
		/**
		 * @brief Container color.
		 * @param variant Variant index.
		 * @param lowered True for the lowered elevation.
		 * @return The color.
		 */
		utility::graphic::Color32Bit containerColor(std::size_t variant,
													bool lowered);
		/** @brief Content color. @param variant Variant index. @return It. */
		utility::graphic::Color32Bit contentColor(std::size_t variant);
	}	 // namespace detail

	/**
	 * @brief Container extent of a FAB size.
	 * @param size The FAB size.
	 * @return The extent in dp.
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	template<typename SizeType> float getContainerSize(SizeType size)
	{
		return detail::containerSize(static_cast<std::size_t>(size));
	}

	/**
	 * @brief Icon font size of a regular FAB.
	 * @param size The FAB size.
	 * @return The font size in dp.
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	template<typename SizeType> float getIconFontSize(SizeType size)
	{
		return detail::iconFontSize(static_cast<std::size_t>(size));
	}

	/**
	 * @brief Label and icon font size of an extended FAB.
	 * @param size The FAB size.
	 * @return The font size in dp.
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	template<typename SizeType> float getLabelFontSize(SizeType size)
	{
		return detail::labelFontSize(static_cast<std::size_t>(size));
	}

	/**
	 * @brief Corner radius of a FAB size.
	 * @param size The FAB size.
	 * @return The radius in dp.
	 * @throws std::runtime_error If the size is not a valid enumerator.
	 */
	template<typename SizeType> float getBorderRadius(SizeType size)
	{
		return detail::borderRadius(static_cast<std::size_t>(size));
	}

	/**
	 * @brief Container color of a FAB variant.
	 * @param variant The FAB variant.
	 * @param lowered True for the lowered elevation.
	 * @return The container color.
	 * @throws std::runtime_error If the variant is not a valid enumerator.
	 */
	template<typename VariantType> utility::graphic::Color32Bit
		getContainerColor(VariantType variant, bool lowered)
	{
		return detail::containerColor(static_cast<std::size_t>(variant),
									  lowered);
	}

	/**
	 * @brief Icon and label color of a FAB variant.
	 * @param variant The FAB variant.
	 * @return The content color.
	 * @throws std::runtime_error If the variant is not a valid enumerator.
	 */
	template<typename VariantType>
	utility::graphic::Color32Bit getContentColor(VariantType variant)
	{
		return detail::contentColor(static_cast<std::size_t>(variant));
	}

}	 // namespace guillaume::systems::fab_style
