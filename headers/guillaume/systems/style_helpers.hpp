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

/**
 * @file
 * @brief Color helpers shared by the widget systems: scheme lookup, state
 * layers and the disabled colors.
 *
 * Everything here is a small inline function of guillaume::systems::style
 * that returns a color; nothing reads or writes a component. The widget
 * systems (Checkbox, Switch, Chip, CommonButton, Menu, TextField, ...) call
 * them from their `apply` function to resolve the colors they write into
 * components::Color and components::Borders, and
 * `guillaume/systems/interaction_helpers.hpp` builds its hover and press
 * state layers on them. `guillaume/entities/style_helpers.hpp` re-exports
 * the same names in guillaume::entities for the widget classes.
 *
 * Colors come from the active theme at the time of the call, so a result
 * must not be cached across a theme change.
 */

/**
 * @namespace guillaume::systems::style
 * @brief Color helpers shared by the widget systems.
 * @see guillaume::systems::interaction, guillaume::systems::placement
 */
namespace guillaume::systems::style
{

	/**
	 * @brief Copy a color with another alpha channel.
	 * @param color The color whose red, green and blue channels are kept.
	 * @param alpha The alpha of the result, 0 (transparent) to 255 (opaque).
	 * It replaces the alpha of `color`; the two are not multiplied.
	 * @return The same color with the given alpha.
	 */
	inline utility::graphic::Color32Bit
		withAlpha(const utility::graphic::Color32Bit &color, std::uint8_t alpha)
	{
		return utility::graphic::Color32Bit(color.getRed(), color.getGreen(),
											color.getBlue(), alpha);
	}

	/**
	 * @brief Blend a translucent tint over a base color.
	 *
	 * This is how a hover, focus or press is shown: the content color is
	 * laid over the container color at a low opacity.
	 * @param base The color underneath, usually the container color.
	 * @param overlay The tint, usually the content color. Its own alpha is
	 * ignored and replaced by `alpha`.
	 * @param alpha The opacity of the tint, 0 (no tint) to 255 (opaque).
	 * @return The blended color.
	 */
	inline utility::graphic::Color32Bit
		stateLayer(const utility::graphic::Color32Bit &base,
				   const utility::graphic::Color32Bit &overlay,
				   std::uint8_t alpha)
	{
		return overlay.withAlpha(alpha).blendOver(base);
	}

	/**
	 * @brief Look up a color role in the color scheme of the active theme.
	 * @param role The role to resolve, for example SchemeColorRole::Primary.
	 * @return The color the active theme assigns to that role.
	 * @see guillaume::getActiveColor
	 */
	inline utility::graphic::Color32Bit schemeColor(SchemeColorRole role)
	{
		return guillaume::getActiveColor(role);
	}

	/**
	 * @brief Blend a tint over a base color at the opacity the active theme
	 * gives to an interaction state.
	 *
	 * @code
	 * // Container color of a hovered filled button.
	 * const auto color = style::stateLayer(
	 * 	style::schemeColor(SchemeColorRole::Primary),
	 * 	style::schemeColor(SchemeColorRole::OnPrimary),
	 * 	theme::StateLayer::Hovered);
	 * @endcode
	 * @param base The color underneath, usually the container color.
	 * @param overlay The tint, usually the content color.
	 * @param layer The interaction state (Hovered, Focused, Pressed, ...)
	 * whose opacity is read from the active theme.
	 * @return The blended color.
	 */
	inline utility::graphic::Color32Bit
		stateLayer(const utility::graphic::Color32Bit &base,
				   const utility::graphic::Color32Bit &overlay,
				   theme::StateLayer layer)
	{
		return stateLayer(base, overlay, getStateLayerAlpha(layer));
	}

	/**
	 * @brief Color of the icon, label or outline of a disabled widget.
	 * @return The OnSurface color of the active scheme at the
	 * theme::StateLayer::DisabledContent opacity (38% in Material 3).
	 */
	inline utility::graphic::Color32Bit disabledContentColor(void)
	{
		return withAlpha(
			schemeColor(SchemeColorRole::OnSurface),
			getStateLayerAlpha(theme::StateLayer::DisabledContent));
	}

	/**
	 * @brief Fill color of the container of a disabled widget.
	 * @return The OnSurface color of the active scheme at the
	 * theme::StateLayer::DisabledContainer opacity (12% in Material 3).
	 */
	inline utility::graphic::Color32Bit disabledContainerColor(void)
	{
		return withAlpha(
			schemeColor(SchemeColorRole::OnSurface),
			getStateLayerAlpha(theme::StateLayer::DisabledContainer));
	}

	/**
	 * @brief The color written to hide a fill or an outline without removing
	 * its component.
	 * @return Black with an alpha of 0.
	 */
	inline utility::graphic::Color32Bit transparentColor(void)
	{
		return utility::graphic::Color32Bit(0, 0, 0, 0);
	}

}	 // namespace guillaume::systems::style
