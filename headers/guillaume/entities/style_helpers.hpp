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

#include <cstdint>

#include <utility/graphic/color.hpp>

#include "guillaume/theme.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Replace the alpha channel of a color.
	 * @param color The base color.
	 * @param alpha The new alpha value.
	 * @return The recolored value.
	 */
	inline utility::graphic::Color32Bit
		withAlpha(const utility::graphic::Color32Bit &color, std::uint8_t alpha)
	{
		return utility::graphic::Color32Bit(color.getRed(), color.getGreen(),
											color.getBlue(), alpha);
	}

	/**
	 * @brief Composite a translucent state layer over a base color.
	 * @param base The base color.
	 * @param overlay The state layer color.
	 * @param alpha The state layer opacity.
	 * @return The composited color.
	 */
	inline utility::graphic::Color32Bit
		stateLayer(const utility::graphic::Color32Bit &base,
				   const utility::graphic::Color32Bit &overlay,
				   std::uint8_t alpha)
	{
		return overlay.withAlpha(alpha).blendOver(base);
	}

	/**
	 * @brief Get a color role from the active scheme.
	 * @param role The scheme color role.
	 * @return The role color.
	 */
	inline utility::graphic::Color32Bit schemeColor(SchemeColorRole role)
	{
		return guillaume::getActiveScheme().getColor(role).getColor();
	}

	/**
	 * @brief Transparent color constant.
	 * @return A fully transparent color.
	 */
	inline utility::graphic::Color32Bit transparentColor(void)
	{
		return utility::graphic::Color32Bit(0, 0, 0, 0);
	}

}	 // namespace guillaume::entities
