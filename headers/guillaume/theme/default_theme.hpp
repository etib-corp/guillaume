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

#include "guillaume/theme.hpp"

namespace guillaume
{
	/**
	 * @brief Build the default Material Design 3 theme.
	 *
	 * Fills the six color schemes (light and dark, each with medium and high
	 * contrast variants) and the five tonal palettes with the Material 3
	 * baseline colors, and derives the `SurfaceDisabled`,
	 * `OnSurfaceDisabled` and `Backdrop` roles of each scheme. The type
	 * scale, shapes, elevations, state layers and motion keep the baseline
	 * values a Theme is constructed with.
	 *
	 * Application code does not need to call it: `defaultTheme` and the
	 * initial active theme are both built with it. Call it to get a fresh,
	 * modifiable copy of the default theme.
	 * @return A new theme named "Material Design 3 Theme".
	 * @see getActiveTheme
	 */
	Theme initializeDefaultTheme(void);

}	 // namespace guillaume
