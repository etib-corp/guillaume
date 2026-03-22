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

#include "guillaume/theme.hpp"

#include <cstdint>
#include <utility/graphics/color.hpp>

namespace guillaume {

namespace {
/**
 * @brief Initialize light color scheme.
 */
void initializeLightScheme(Scheme &scheme) {
    scheme.setColor(SchemeColorRole::Primary,
                    utility::graphics::Color<std::uint8_t>(143, 76, 56, 255),
                    "primary");
    scheme.setColor(SchemeColorRole::SurfaceTint,
                    utility::graphics::Color<std::uint8_t>(143, 76, 56, 255),
                    "surfaceTint");
    scheme.setColor(SchemeColorRole::OnPrimary,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onPrimary");
    scheme.setColor(SchemeColorRole::PrimaryContainer,
                    utility::graphics::Color<std::uint8_t>(255, 219, 209, 255),
                    "primaryContainer");
    scheme.setColor(SchemeColorRole::OnPrimaryContainer,
                    utility::graphics::Color<std::uint8_t>(114, 53, 35, 255),
                    "onPrimaryContainer");
    scheme.setColor(SchemeColorRole::Secondary,
                    utility::graphics::Color<std::uint8_t>(119, 87, 78, 255),
                    "secondary");
    scheme.setColor(SchemeColorRole::OnSecondary,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onSecondary");
    scheme.setColor(SchemeColorRole::SecondaryContainer,
                    utility::graphics::Color<std::uint8_t>(255, 219, 209, 255),
                    "secondaryContainer");
    scheme.setColor(SchemeColorRole::OnSecondaryContainer,
                    utility::graphics::Color<std::uint8_t>(93, 64, 55, 255),
                    "onSecondaryContainer");
    scheme.setColor(SchemeColorRole::Tertiary,
                    utility::graphics::Color<std::uint8_t>(108, 93, 47, 255),
                    "tertiary");
    scheme.setColor(SchemeColorRole::OnTertiary,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onTertiary");
    scheme.setColor(SchemeColorRole::TertiaryContainer,
                    utility::graphics::Color<std::uint8_t>(245, 225, 167, 255),
                    "tertiaryContainer");
    scheme.setColor(SchemeColorRole::OnTertiaryContainer,
                    utility::graphics::Color<std::uint8_t>(83, 70, 25, 255),
                    "onTertiaryContainer");
    scheme.setColor(SchemeColorRole::Error,
                    utility::graphics::Color<std::uint8_t>(186, 26, 26, 255),
                    "error");
    scheme.setColor(SchemeColorRole::OnError,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onError");
    scheme.setColor(SchemeColorRole::ErrorContainer,
                    utility::graphics::Color<std::uint8_t>(255, 218, 214, 255),
                    "errorContainer");
    scheme.setColor(SchemeColorRole::OnErrorContainer,
                    utility::graphics::Color<std::uint8_t>(147, 0, 10, 255),
                    "onErrorContainer");
    scheme.setColor(SchemeColorRole::Background,
                    utility::graphics::Color<std::uint8_t>(255, 248, 246, 255),
                    "background");
    scheme.setColor(SchemeColorRole::OnBackground,
                    utility::graphics::Color<std::uint8_t>(35, 25, 23, 255),
                    "onBackground");
    scheme.setColor(SchemeColorRole::Surface,
                    utility::graphics::Color<std::uint8_t>(255, 248, 246, 255),
                    "surface");
    scheme.setColor(SchemeColorRole::OnSurface,
                    utility::graphics::Color<std::uint8_t>(35, 25, 23, 255),
                    "onSurface");
    scheme.setColor(SchemeColorRole::SurfaceVariant,
                    utility::graphics::Color<std::uint8_t>(245, 222, 216, 255),
                    "surfaceVariant");
    scheme.setColor(SchemeColorRole::OnSurfaceVariant,
                    utility::graphics::Color<std::uint8_t>(83, 67, 63, 255),
                    "onSurfaceVariant");
    scheme.setColor(SchemeColorRole::Outline,
                    utility::graphics::Color<std::uint8_t>(133, 115, 110, 255),
                    "outline");
    scheme.setColor(SchemeColorRole::OutlineVariant,
                    utility::graphics::Color<std::uint8_t>(216, 194, 188, 255),
                    "outlineVariant");
    scheme.setColor(SchemeColorRole::Shadow,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "shadow");
    scheme.setColor(SchemeColorRole::Scrim,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "scrim");
    scheme.setColor(SchemeColorRole::InverseSurface,
                    utility::graphics::Color<std::uint8_t>(57, 46, 43, 255),
                    "inverseSurface");
    scheme.setColor(SchemeColorRole::InverseOnSurface,
                    utility::graphics::Color<std::uint8_t>(255, 237, 232, 255),
                    "inverseOnSurface");
    scheme.setColor(SchemeColorRole::InversePrimary,
                    utility::graphics::Color<std::uint8_t>(255, 181, 160, 255),
                    "inversePrimary");
    scheme.setColor(SchemeColorRole::PrimaryFixed,
                    utility::graphics::Color<std::uint8_t>(255, 219, 209, 255),
                    "primaryFixed");
    scheme.setColor(SchemeColorRole::OnPrimaryFixed,
                    utility::graphics::Color<std::uint8_t>(58, 11, 1, 255),
                    "onPrimaryFixed");
    scheme.setColor(SchemeColorRole::PrimaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(255, 181, 160, 255),
                    "primaryFixedDim");
    scheme.setColor(SchemeColorRole::OnPrimaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(114, 53, 35, 255),
                    "onPrimaryFixedVariant");
    scheme.setColor(SchemeColorRole::SecondaryFixed,
                    utility::graphics::Color<std::uint8_t>(255, 219, 209, 255),
                    "secondaryFixed");
    scheme.setColor(SchemeColorRole::OnSecondaryFixed,
                    utility::graphics::Color<std::uint8_t>(44, 21, 15, 255),
                    "onSecondaryFixed");
    scheme.setColor(SchemeColorRole::SecondaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(231, 189, 178, 255),
                    "secondaryFixedDim");
    scheme.setColor(SchemeColorRole::OnSecondaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(93, 64, 55, 255),
                    "onSecondaryFixedVariant");
    scheme.setColor(SchemeColorRole::TertiaryFixed,
                    utility::graphics::Color<std::uint8_t>(245, 225, 167, 255),
                    "tertiaryFixed");
    scheme.setColor(SchemeColorRole::OnTertiaryFixed,
                    utility::graphics::Color<std::uint8_t>(35, 27, 0, 255),
                    "onTertiaryFixed");
    scheme.setColor(SchemeColorRole::TertiaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(216, 197, 141, 255),
                    "tertiaryFixedDim");
    scheme.setColor(SchemeColorRole::OnTertiaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(83, 70, 25, 255),
                    "onTertiaryFixedVariant");
    scheme.setColor(SchemeColorRole::SurfaceDim,
                    utility::graphics::Color<std::uint8_t>(232, 214, 210, 255),
                    "surfaceDim");
    scheme.setColor(SchemeColorRole::SurfaceBright,
                    utility::graphics::Color<std::uint8_t>(255, 248, 246, 255),
                    "surfaceBright");
    scheme.setColor(SchemeColorRole::SurfaceContainerLowest,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "surfaceContainerLowest");
    scheme.setColor(SchemeColorRole::SurfaceContainerLow,
                    utility::graphics::Color<std::uint8_t>(255, 241, 237, 255),
                    "surfaceContainerLow");
    scheme.setColor(SchemeColorRole::SurfaceContainer,
                    utility::graphics::Color<std::uint8_t>(252, 234, 229, 255),
                    "surfaceContainer");
    scheme.setColor(SchemeColorRole::SurfaceContainerHigh,
                    utility::graphics::Color<std::uint8_t>(247, 228, 224, 255),
                    "surfaceContainerHigh");
    scheme.setColor(SchemeColorRole::SurfaceContainerHighest,
                    utility::graphics::Color<std::uint8_t>(241, 223, 218, 255),
                    "surfaceContainerHighest");
}

/**
 * @brief Initialize light medium contrast scheme.
 */
void initializeLightMediumContrastScheme(Scheme &scheme) {
    scheme.setColor(SchemeColorRole::Primary,
                    utility::graphics::Color<std::uint8_t>(93, 37, 20, 255),
                    "primary");
    scheme.setColor(SchemeColorRole::SurfaceTint,
                    utility::graphics::Color<std::uint8_t>(143, 76, 56, 255),
                    "surfaceTint");
    scheme.setColor(SchemeColorRole::OnPrimary,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onPrimary");
    scheme.setColor(SchemeColorRole::PrimaryContainer,
                    utility::graphics::Color<std::uint8_t>(161, 90, 69, 255),
                    "primaryContainer");
    scheme.setColor(SchemeColorRole::OnPrimaryContainer,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onPrimaryContainer");
    scheme.setColor(SchemeColorRole::Secondary,
                    utility::graphics::Color<std::uint8_t>(75, 47, 40, 255),
                    "secondary");
    scheme.setColor(SchemeColorRole::OnSecondary,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onSecondary");
    scheme.setColor(SchemeColorRole::SecondaryContainer,
                    utility::graphics::Color<std::uint8_t>(135, 101, 92, 255),
                    "secondaryContainer");
    scheme.setColor(SchemeColorRole::OnSecondaryContainer,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onSecondaryContainer");
    scheme.setColor(SchemeColorRole::Tertiary,
                    utility::graphics::Color<std::uint8_t>(65, 53, 10, 255),
                    "tertiary");
    scheme.setColor(SchemeColorRole::OnTertiary,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onTertiary");
    scheme.setColor(SchemeColorRole::TertiaryContainer,
                    utility::graphics::Color<std::uint8_t>(123, 108, 60, 255),
                    "tertiaryContainer");
    scheme.setColor(SchemeColorRole::OnTertiaryContainer,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onTertiaryContainer");
    scheme.setColor(SchemeColorRole::Error,
                    utility::graphics::Color<std::uint8_t>(116, 0, 6, 255),
                    "error");
    scheme.setColor(SchemeColorRole::OnError,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onError");
    scheme.setColor(SchemeColorRole::ErrorContainer,
                    utility::graphics::Color<std::uint8_t>(207, 44, 39, 255),
                    "errorContainer");
    scheme.setColor(SchemeColorRole::OnErrorContainer,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onErrorContainer");
    scheme.setColor(SchemeColorRole::Background,
                    utility::graphics::Color<std::uint8_t>(255, 248, 246, 255),
                    "background");
    scheme.setColor(SchemeColorRole::OnBackground,
                    utility::graphics::Color<std::uint8_t>(35, 25, 23, 255),
                    "onBackground");
    scheme.setColor(SchemeColorRole::Surface,
                    utility::graphics::Color<std::uint8_t>(255, 248, 246, 255),
                    "surface");
    scheme.setColor(SchemeColorRole::OnSurface,
                    utility::graphics::Color<std::uint8_t>(24, 15, 13, 255),
                    "onSurface");
    scheme.setColor(SchemeColorRole::SurfaceVariant,
                    utility::graphics::Color<std::uint8_t>(245, 222, 216, 255),
                    "surfaceVariant");
    scheme.setColor(SchemeColorRole::OnSurfaceVariant,
                    utility::graphics::Color<std::uint8_t>(65, 51, 47, 255),
                    "onSurfaceVariant");
    scheme.setColor(SchemeColorRole::Outline,
                    utility::graphics::Color<std::uint8_t>(95, 79, 74, 255),
                    "outline");
    scheme.setColor(SchemeColorRole::OutlineVariant,
                    utility::graphics::Color<std::uint8_t>(123, 105, 100, 255),
                    "outlineVariant");
    scheme.setColor(SchemeColorRole::Shadow,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "shadow");
    scheme.setColor(SchemeColorRole::Scrim,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "scrim");
    scheme.setColor(SchemeColorRole::InverseSurface,
                    utility::graphics::Color<std::uint8_t>(57, 46, 43, 255),
                    "inverseSurface");
    scheme.setColor(SchemeColorRole::InverseOnSurface,
                    utility::graphics::Color<std::uint8_t>(255, 237, 232, 255),
                    "inverseOnSurface");
    scheme.setColor(SchemeColorRole::InversePrimary,
                    utility::graphics::Color<std::uint8_t>(255, 181, 160, 255),
                    "inversePrimary");
    scheme.setColor(SchemeColorRole::PrimaryFixed,
                    utility::graphics::Color<std::uint8_t>(161, 90, 69, 255),
                    "primaryFixed");
    scheme.setColor(SchemeColorRole::OnPrimaryFixed,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onPrimaryFixed");
    scheme.setColor(SchemeColorRole::PrimaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(132, 66, 47, 255),
                    "primaryFixedDim");
    scheme.setColor(SchemeColorRole::OnPrimaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onPrimaryFixedVariant");
    scheme.setColor(SchemeColorRole::SecondaryFixed,
                    utility::graphics::Color<std::uint8_t>(135, 101, 92, 255),
                    "secondaryFixed");
    scheme.setColor(SchemeColorRole::OnSecondaryFixed,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onSecondaryFixed");
    scheme.setColor(SchemeColorRole::SecondaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(109, 77, 69, 255),
                    "secondaryFixedDim");
    scheme.setColor(SchemeColorRole::OnSecondaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onSecondaryFixedVariant");
    scheme.setColor(SchemeColorRole::TertiaryFixed,
                    utility::graphics::Color<std::uint8_t>(123, 108, 60, 255),
                    "tertiaryFixed");
    scheme.setColor(SchemeColorRole::OnTertiaryFixed,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onTertiaryFixed");
    scheme.setColor(SchemeColorRole::TertiaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(97, 84, 38, 255),
                    "tertiaryFixedDim");
    scheme.setColor(SchemeColorRole::OnTertiaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onTertiaryFixedVariant");
    scheme.setColor(SchemeColorRole::SurfaceDim,
                    utility::graphics::Color<std::uint8_t>(212, 195, 190, 255),
                    "surfaceDim");
    scheme.setColor(SchemeColorRole::SurfaceBright,
                    utility::graphics::Color<std::uint8_t>(255, 248, 246, 255),
                    "surfaceBright");
    scheme.setColor(SchemeColorRole::SurfaceContainerLowest,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "surfaceContainerLowest");
    scheme.setColor(SchemeColorRole::SurfaceContainerLow,
                    utility::graphics::Color<std::uint8_t>(255, 241, 237, 255),
                    "surfaceContainerLow");
    scheme.setColor(SchemeColorRole::SurfaceContainer,
                    utility::graphics::Color<std::uint8_t>(247, 228, 224, 255),
                    "surfaceContainer");
    scheme.setColor(SchemeColorRole::SurfaceContainerHigh,
                    utility::graphics::Color<std::uint8_t>(235, 217, 212, 255),
                    "surfaceContainerHigh");
    scheme.setColor(SchemeColorRole::SurfaceContainerHighest,
                    utility::graphics::Color<std::uint8_t>(223, 206, 201, 255),
                    "surfaceContainerHighest");
}

/**
 * @brief Initialize light high contrast scheme.
 */
void initializeLightHighContrastScheme(Scheme &scheme) {
    scheme.setColor(SchemeColorRole::Primary,
                    utility::graphics::Color<std::uint8_t>(80, 27, 11, 255),
                    "primary");
    scheme.setColor(SchemeColorRole::SurfaceTint,
                    utility::graphics::Color<std::uint8_t>(143, 76, 56, 255),
                    "surfaceTint");
    scheme.setColor(SchemeColorRole::OnPrimary,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onPrimary");
    scheme.setColor(SchemeColorRole::PrimaryContainer,
                    utility::graphics::Color<std::uint8_t>(117, 55, 37, 255),
                    "primaryContainer");
    scheme.setColor(SchemeColorRole::OnPrimaryContainer,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onPrimaryContainer");
    scheme.setColor(SchemeColorRole::Secondary,
                    utility::graphics::Color<std::uint8_t>(63, 38, 30, 255),
                    "secondary");
    scheme.setColor(SchemeColorRole::OnSecondary,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onSecondary");
    scheme.setColor(SchemeColorRole::SecondaryContainer,
                    utility::graphics::Color<std::uint8_t>(96, 66, 58, 255),
                    "secondaryContainer");
    scheme.setColor(SchemeColorRole::OnSecondaryContainer,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onSecondaryContainer");
    scheme.setColor(SchemeColorRole::Tertiary,
                    utility::graphics::Color<std::uint8_t>(54, 43, 2, 255),
                    "tertiary");
    scheme.setColor(SchemeColorRole::OnTertiary,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onTertiary");
    scheme.setColor(SchemeColorRole::TertiaryContainer,
                    utility::graphics::Color<std::uint8_t>(85, 72, 28, 255),
                    "tertiaryContainer");
    scheme.setColor(SchemeColorRole::OnTertiaryContainer,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onTertiaryContainer");
    scheme.setColor(SchemeColorRole::Error,
                    utility::graphics::Color<std::uint8_t>(96, 0, 4, 255),
                    "error");
    scheme.setColor(SchemeColorRole::OnError,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onError");
    scheme.setColor(SchemeColorRole::ErrorContainer,
                    utility::graphics::Color<std::uint8_t>(152, 0, 10, 255),
                    "errorContainer");
    scheme.setColor(SchemeColorRole::OnErrorContainer,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onErrorContainer");
    scheme.setColor(SchemeColorRole::Background,
                    utility::graphics::Color<std::uint8_t>(255, 248, 246, 255),
                    "background");
    scheme.setColor(SchemeColorRole::OnBackground,
                    utility::graphics::Color<std::uint8_t>(35, 25, 23, 255),
                    "onBackground");
    scheme.setColor(SchemeColorRole::Surface,
                    utility::graphics::Color<std::uint8_t>(255, 248, 246, 255),
                    "surface");
    scheme.setColor(SchemeColorRole::OnSurface,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "onSurface");
    scheme.setColor(SchemeColorRole::SurfaceVariant,
                    utility::graphics::Color<std::uint8_t>(245, 222, 216, 255),
                    "surfaceVariant");
    scheme.setColor(SchemeColorRole::OnSurfaceVariant,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "onSurfaceVariant");
    scheme.setColor(SchemeColorRole::Outline,
                    utility::graphics::Color<std::uint8_t>(55, 41, 37, 255),
                    "outline");
    scheme.setColor(SchemeColorRole::OutlineVariant,
                    utility::graphics::Color<std::uint8_t>(85, 70, 65, 255),
                    "outlineVariant");
    scheme.setColor(SchemeColorRole::Shadow,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "shadow");
    scheme.setColor(SchemeColorRole::Scrim,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "scrim");
    scheme.setColor(SchemeColorRole::InverseSurface,
                    utility::graphics::Color<std::uint8_t>(57, 46, 43, 255),
                    "inverseSurface");
    scheme.setColor(SchemeColorRole::InverseOnSurface,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "inverseOnSurface");
    scheme.setColor(SchemeColorRole::InversePrimary,
                    utility::graphics::Color<std::uint8_t>(255, 181, 160, 255),
                    "inversePrimary");
    scheme.setColor(SchemeColorRole::PrimaryFixed,
                    utility::graphics::Color<std::uint8_t>(117, 55, 37, 255),
                    "primaryFixed");
    scheme.setColor(SchemeColorRole::OnPrimaryFixed,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onPrimaryFixed");
    scheme.setColor(SchemeColorRole::PrimaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(89, 33, 17, 255),
                    "primaryFixedDim");
    scheme.setColor(SchemeColorRole::OnPrimaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onPrimaryFixedVariant");
    scheme.setColor(SchemeColorRole::SecondaryFixed,
                    utility::graphics::Color<std::uint8_t>(96, 66, 58, 255),
                    "secondaryFixed");
    scheme.setColor(SchemeColorRole::OnSecondaryFixed,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onSecondaryFixed");
    scheme.setColor(SchemeColorRole::SecondaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(71, 44, 36, 255),
                    "secondaryFixedDim");
    scheme.setColor(SchemeColorRole::OnSecondaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onSecondaryFixedVariant");
    scheme.setColor(SchemeColorRole::TertiaryFixed,
                    utility::graphics::Color<std::uint8_t>(85, 72, 28, 255),
                    "tertiaryFixed");
    scheme.setColor(SchemeColorRole::OnTertiaryFixed,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onTertiaryFixed");
    scheme.setColor(SchemeColorRole::TertiaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(61, 50, 6, 255),
                    "tertiaryFixedDim");
    scheme.setColor(SchemeColorRole::OnTertiaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onTertiaryFixedVariant");
    scheme.setColor(SchemeColorRole::SurfaceDim,
                    utility::graphics::Color<std::uint8_t>(198, 181, 177, 255),
                    "surfaceDim");
    scheme.setColor(SchemeColorRole::SurfaceBright,
                    utility::graphics::Color<std::uint8_t>(255, 248, 246, 255),
                    "surfaceBright");
    scheme.setColor(SchemeColorRole::SurfaceContainerLowest,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "surfaceContainerLowest");
    scheme.setColor(SchemeColorRole::SurfaceContainerLow,
                    utility::graphics::Color<std::uint8_t>(255, 237, 232, 255),
                    "surfaceContainerLow");
    scheme.setColor(SchemeColorRole::SurfaceContainer,
                    utility::graphics::Color<std::uint8_t>(241, 223, 218, 255),
                    "surfaceContainer");
    scheme.setColor(SchemeColorRole::SurfaceContainerHigh,
                    utility::graphics::Color<std::uint8_t>(226, 209, 204, 255),
                    "surfaceContainerHigh");
    scheme.setColor(SchemeColorRole::SurfaceContainerHighest,
                    utility::graphics::Color<std::uint8_t>(212, 195, 190, 255),
                    "surfaceContainerHighest");
}

/**
 * @brief Initialize dark color scheme.
 */
void initializeDarkScheme(Scheme &scheme) {
    scheme.setColor(SchemeColorRole::Primary,
                    utility::graphics::Color<std::uint8_t>(255, 181, 160, 255),
                    "primary");
    scheme.setColor(SchemeColorRole::SurfaceTint,
                    utility::graphics::Color<std::uint8_t>(255, 181, 160, 255),
                    "surfaceTint");
    scheme.setColor(SchemeColorRole::OnPrimary,
                    utility::graphics::Color<std::uint8_t>(86, 31, 15, 255),
                    "onPrimary");
    scheme.setColor(SchemeColorRole::PrimaryContainer,
                    utility::graphics::Color<std::uint8_t>(114, 53, 35, 255),
                    "primaryContainer");
    scheme.setColor(SchemeColorRole::OnPrimaryContainer,
                    utility::graphics::Color<std::uint8_t>(255, 219, 209, 255),
                    "onPrimaryContainer");
    scheme.setColor(SchemeColorRole::Secondary,
                    utility::graphics::Color<std::uint8_t>(231, 189, 178, 255),
                    "secondary");
    scheme.setColor(SchemeColorRole::OnSecondary,
                    utility::graphics::Color<std::uint8_t>(68, 42, 34, 255),
                    "onSecondary");
    scheme.setColor(SchemeColorRole::SecondaryContainer,
                    utility::graphics::Color<std::uint8_t>(93, 64, 55, 255),
                    "secondaryContainer");
    scheme.setColor(SchemeColorRole::OnSecondaryContainer,
                    utility::graphics::Color<std::uint8_t>(255, 219, 209, 255),
                    "onSecondaryContainer");
    scheme.setColor(SchemeColorRole::Tertiary,
                    utility::graphics::Color<std::uint8_t>(216, 197, 141, 255),
                    "tertiary");
    scheme.setColor(SchemeColorRole::OnTertiary,
                    utility::graphics::Color<std::uint8_t>(59, 47, 5, 255),
                    "onTertiary");
    scheme.setColor(SchemeColorRole::TertiaryContainer,
                    utility::graphics::Color<std::uint8_t>(83, 70, 25, 255),
                    "tertiaryContainer");
    scheme.setColor(SchemeColorRole::OnTertiaryContainer,
                    utility::graphics::Color<std::uint8_t>(245, 225, 167, 255),
                    "onTertiaryContainer");
    scheme.setColor(SchemeColorRole::Error,
                    utility::graphics::Color<std::uint8_t>(255, 180, 171, 255),
                    "error");
    scheme.setColor(SchemeColorRole::OnError,
                    utility::graphics::Color<std::uint8_t>(105, 0, 5, 255),
                    "onError");
    scheme.setColor(SchemeColorRole::ErrorContainer,
                    utility::graphics::Color<std::uint8_t>(147, 0, 10, 255),
                    "errorContainer");
    scheme.setColor(SchemeColorRole::OnErrorContainer,
                    utility::graphics::Color<std::uint8_t>(255, 218, 214, 255),
                    "onErrorContainer");
    scheme.setColor(SchemeColorRole::Background,
                    utility::graphics::Color<std::uint8_t>(26, 17, 15, 255),
                    "background");
    scheme.setColor(SchemeColorRole::OnBackground,
                    utility::graphics::Color<std::uint8_t>(241, 223, 218, 255),
                    "onBackground");
    scheme.setColor(SchemeColorRole::Surface,
                    utility::graphics::Color<std::uint8_t>(26, 17, 15, 255),
                    "surface");
    scheme.setColor(SchemeColorRole::OnSurface,
                    utility::graphics::Color<std::uint8_t>(241, 223, 218, 255),
                    "onSurface");
    scheme.setColor(SchemeColorRole::SurfaceVariant,
                    utility::graphics::Color<std::uint8_t>(83, 67, 63, 255),
                    "surfaceVariant");
    scheme.setColor(SchemeColorRole::OnSurfaceVariant,
                    utility::graphics::Color<std::uint8_t>(216, 194, 188, 255),
                    "onSurfaceVariant");
    scheme.setColor(SchemeColorRole::Outline,
                    utility::graphics::Color<std::uint8_t>(160, 140, 135, 255),
                    "outline");
    scheme.setColor(SchemeColorRole::OutlineVariant,
                    utility::graphics::Color<std::uint8_t>(83, 67, 63, 255),
                    "outlineVariant");
    scheme.setColor(SchemeColorRole::Shadow,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "shadow");
    scheme.setColor(SchemeColorRole::Scrim,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "scrim");
    scheme.setColor(SchemeColorRole::InverseSurface,
                    utility::graphics::Color<std::uint8_t>(241, 223, 218, 255),
                    "inverseSurface");
    scheme.setColor(SchemeColorRole::InverseOnSurface,
                    utility::graphics::Color<std::uint8_t>(57, 46, 43, 255),
                    "inverseOnSurface");
    scheme.setColor(SchemeColorRole::InversePrimary,
                    utility::graphics::Color<std::uint8_t>(143, 76, 56, 255),
                    "inversePrimary");
    scheme.setColor(SchemeColorRole::PrimaryFixed,
                    utility::graphics::Color<std::uint8_t>(255, 219, 209, 255),
                    "primaryFixed");
    scheme.setColor(SchemeColorRole::OnPrimaryFixed,
                    utility::graphics::Color<std::uint8_t>(58, 11, 1, 255),
                    "onPrimaryFixed");
    scheme.setColor(SchemeColorRole::PrimaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(255, 181, 160, 255),
                    "primaryFixedDim");
    scheme.setColor(SchemeColorRole::OnPrimaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(114, 53, 35, 255),
                    "onPrimaryFixedVariant");
    scheme.setColor(SchemeColorRole::SecondaryFixed,
                    utility::graphics::Color<std::uint8_t>(255, 219, 209, 255),
                    "secondaryFixed");
    scheme.setColor(SchemeColorRole::OnSecondaryFixed,
                    utility::graphics::Color<std::uint8_t>(44, 21, 15, 255),
                    "onSecondaryFixed");
    scheme.setColor(SchemeColorRole::SecondaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(231, 189, 178, 255),
                    "secondaryFixedDim");
    scheme.setColor(SchemeColorRole::OnSecondaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(93, 64, 55, 255),
                    "onSecondaryFixedVariant");
    scheme.setColor(SchemeColorRole::TertiaryFixed,
                    utility::graphics::Color<std::uint8_t>(245, 225, 167, 255),
                    "tertiaryFixed");
    scheme.setColor(SchemeColorRole::OnTertiaryFixed,
                    utility::graphics::Color<std::uint8_t>(35, 27, 0, 255),
                    "onTertiaryFixed");
    scheme.setColor(SchemeColorRole::TertiaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(216, 197, 141, 255),
                    "tertiaryFixedDim");
    scheme.setColor(SchemeColorRole::OnTertiaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(83, 70, 25, 255),
                    "onTertiaryFixedVariant");
    scheme.setColor(SchemeColorRole::SurfaceDim,
                    utility::graphics::Color<std::uint8_t>(26, 17, 15, 255),
                    "surfaceDim");
    scheme.setColor(SchemeColorRole::SurfaceBright,
                    utility::graphics::Color<std::uint8_t>(66, 55, 52, 255),
                    "surfaceBright");
    scheme.setColor(SchemeColorRole::SurfaceContainerLowest,
                    utility::graphics::Color<std::uint8_t>(20, 12, 10, 255),
                    "surfaceContainerLowest");
    scheme.setColor(SchemeColorRole::SurfaceContainerLow,
                    utility::graphics::Color<std::uint8_t>(35, 25, 23, 255),
                    "surfaceContainerLow");
    scheme.setColor(SchemeColorRole::SurfaceContainer,
                    utility::graphics::Color<std::uint8_t>(39, 29, 27, 255),
                    "surfaceContainer");
    scheme.setColor(SchemeColorRole::SurfaceContainerHigh,
                    utility::graphics::Color<std::uint8_t>(50, 40, 37, 255),
                    "surfaceContainerHigh");
    scheme.setColor(SchemeColorRole::SurfaceContainerHighest,
                    utility::graphics::Color<std::uint8_t>(61, 50, 47, 255),
                    "surfaceContainerHighest");
}

/**
 * @brief Initialize dark medium contrast scheme.
 */
void initializeDarkMediumContrastScheme(Scheme &scheme) {
    scheme.setColor(SchemeColorRole::Primary,
                    utility::graphics::Color<std::uint8_t>(255, 210, 198, 255),
                    "primary");
    scheme.setColor(SchemeColorRole::SurfaceTint,
                    utility::graphics::Color<std::uint8_t>(255, 181, 160, 255),
                    "surfaceTint");
    scheme.setColor(SchemeColorRole::OnPrimary,
                    utility::graphics::Color<std::uint8_t>(72, 21, 6, 255),
                    "onPrimary");
    scheme.setColor(SchemeColorRole::PrimaryContainer,
                    utility::graphics::Color<std::uint8_t>(203, 124, 101, 255),
                    "primaryContainer");
    scheme.setColor(SchemeColorRole::OnPrimaryContainer,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "onPrimaryContainer");
    scheme.setColor(SchemeColorRole::Secondary,
                    utility::graphics::Color<std::uint8_t>(253, 211, 199, 255),
                    "secondary");
    scheme.setColor(SchemeColorRole::OnSecondary,
                    utility::graphics::Color<std::uint8_t>(56, 31, 24, 255),
                    "onSecondary");
    scheme.setColor(SchemeColorRole::SecondaryContainer,
                    utility::graphics::Color<std::uint8_t>(174, 136, 126, 255),
                    "secondaryContainer");
    scheme.setColor(SchemeColorRole::OnSecondaryContainer,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "onSecondaryContainer");
    scheme.setColor(SchemeColorRole::Tertiary,
                    utility::graphics::Color<std::uint8_t>(239, 219, 161, 255),
                    "tertiary");
    scheme.setColor(SchemeColorRole::OnTertiary,
                    utility::graphics::Color<std::uint8_t>(47, 37, 0, 255),
                    "onTertiary");
    scheme.setColor(SchemeColorRole::TertiaryContainer,
                    utility::graphics::Color<std::uint8_t>(160, 144, 92, 255),
                    "tertiaryContainer");
    scheme.setColor(SchemeColorRole::OnTertiaryContainer,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "onTertiaryContainer");
    scheme.setColor(SchemeColorRole::Error,
                    utility::graphics::Color<std::uint8_t>(255, 210, 204, 255),
                    "error");
    scheme.setColor(SchemeColorRole::OnError,
                    utility::graphics::Color<std::uint8_t>(84, 0, 3, 255),
                    "onError");
    scheme.setColor(SchemeColorRole::ErrorContainer,
                    utility::graphics::Color<std::uint8_t>(255, 84, 73, 255),
                    "errorContainer");
    scheme.setColor(SchemeColorRole::OnErrorContainer,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "onErrorContainer");
    scheme.setColor(SchemeColorRole::Background,
                    utility::graphics::Color<std::uint8_t>(26, 17, 15, 255),
                    "background");
    scheme.setColor(SchemeColorRole::OnBackground,
                    utility::graphics::Color<std::uint8_t>(241, 223, 218, 255),
                    "onBackground");
    scheme.setColor(SchemeColorRole::Surface,
                    utility::graphics::Color<std::uint8_t>(26, 17, 15, 255),
                    "surface");
    scheme.setColor(SchemeColorRole::OnSurface,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onSurface");
    scheme.setColor(SchemeColorRole::SurfaceVariant,
                    utility::graphics::Color<std::uint8_t>(83, 67, 63, 255),
                    "surfaceVariant");
    scheme.setColor(SchemeColorRole::OnSurfaceVariant,
                    utility::graphics::Color<std::uint8_t>(238, 215, 209, 255),
                    "onSurfaceVariant");
    scheme.setColor(SchemeColorRole::Outline,
                    utility::graphics::Color<std::uint8_t>(194, 157, 152, 255),
                    "outline");
    scheme.setColor(SchemeColorRole::OutlineVariant,
                    utility::graphics::Color<std::uint8_t>(160, 140, 135, 255),
                    "outlineVariant");
    scheme.setColor(SchemeColorRole::Shadow,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "shadow");
    scheme.setColor(SchemeColorRole::Scrim,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "scrim");
    scheme.setColor(SchemeColorRole::InverseSurface,
                    utility::graphics::Color<std::uint8_t>(241, 223, 218, 255),
                    "inverseSurface");
    scheme.setColor(SchemeColorRole::InverseOnSurface,
                    utility::graphics::Color<std::uint8_t>(50, 40, 37, 255),
                    "inverseOnSurface");
    scheme.setColor(SchemeColorRole::InversePrimary,
                    utility::graphics::Color<std::uint8_t>(116, 54, 36, 255),
                    "inversePrimary");
    scheme.setColor(SchemeColorRole::PrimaryFixed,
                    utility::graphics::Color<std::uint8_t>(255, 219, 209, 255),
                    "primaryFixed");
    scheme.setColor(SchemeColorRole::OnPrimaryFixed,
                    utility::graphics::Color<std::uint8_t>(40, 5, 0, 255),
                    "onPrimaryFixed");
    scheme.setColor(SchemeColorRole::PrimaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(255, 181, 160, 255),
                    "primaryFixedDim");
    scheme.setColor(SchemeColorRole::OnPrimaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(93, 37, 20, 255),
                    "onPrimaryFixedVariant");
    scheme.setColor(SchemeColorRole::SecondaryFixed,
                    utility::graphics::Color<std::uint8_t>(255, 219, 209, 255),
                    "secondaryFixed");
    scheme.setColor(SchemeColorRole::OnSecondaryFixed,
                    utility::graphics::Color<std::uint8_t>(32, 11, 6, 255),
                    "onSecondaryFixed");
    scheme.setColor(SchemeColorRole::SecondaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(231, 189, 178, 255),
                    "secondaryFixedDim");
    scheme.setColor(SchemeColorRole::OnSecondaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(75, 47, 40, 255),
                    "onSecondaryFixedVariant");
    scheme.setColor(SchemeColorRole::TertiaryFixed,
                    utility::graphics::Color<std::uint8_t>(245, 225, 167, 255),
                    "tertiaryFixed");
    scheme.setColor(SchemeColorRole::OnTertiaryFixed,
                    utility::graphics::Color<std::uint8_t>(23, 17, 0, 255),
                    "onTertiaryFixed");
    scheme.setColor(SchemeColorRole::TertiaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(216, 197, 141, 255),
                    "tertiaryFixedDim");
    scheme.setColor(SchemeColorRole::OnTertiaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(65, 53, 10, 255),
                    "onTertiaryFixedVariant");
    scheme.setColor(SchemeColorRole::SurfaceDim,
                    utility::graphics::Color<std::uint8_t>(26, 17, 15, 255),
                    "surfaceDim");
    scheme.setColor(SchemeColorRole::SurfaceBright,
                    utility::graphics::Color<std::uint8_t>(78, 67, 63, 255),
                    "surfaceBright");
    scheme.setColor(SchemeColorRole::SurfaceContainerLowest,
                    utility::graphics::Color<std::uint8_t>(13, 6, 4, 255),
                    "surfaceContainerLowest");
    scheme.setColor(SchemeColorRole::SurfaceContainerLow,
                    utility::graphics::Color<std::uint8_t>(37, 27, 25, 255),
                    "surfaceContainerLow");
    scheme.setColor(SchemeColorRole::SurfaceContainer,
                    utility::graphics::Color<std::uint8_t>(48, 38, 35, 255),
                    "surfaceContainer");
    scheme.setColor(SchemeColorRole::SurfaceContainerHigh,
                    utility::graphics::Color<std::uint8_t>(59, 48, 45, 255),
                    "surfaceContainerHigh");
    scheme.setColor(SchemeColorRole::SurfaceContainerHighest,
                    utility::graphics::Color<std::uint8_t>(70, 59, 56, 255),
                    "surfaceContainerHighest");
}

/**
 * @brief Initialize dark high contrast scheme.
 */
void initializeDarkHighContrastScheme(Scheme &scheme) {
    scheme.setColor(SchemeColorRole::Primary,
                    utility::graphics::Color<std::uint8_t>(255, 236, 231, 255),
                    "primary");
    scheme.setColor(SchemeColorRole::SurfaceTint,
                    utility::graphics::Color<std::uint8_t>(255, 181, 160, 255),
                    "surfaceTint");
    scheme.setColor(SchemeColorRole::OnPrimary,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "onPrimary");
    scheme.setColor(SchemeColorRole::PrimaryContainer,
                    utility::graphics::Color<std::uint8_t>(255, 175, 152, 255),
                    "primaryContainer");
    scheme.setColor(SchemeColorRole::OnPrimaryContainer,
                    utility::graphics::Color<std::uint8_t>(30, 3, 0, 255),
                    "onPrimaryContainer");
    scheme.setColor(SchemeColorRole::Secondary,
                    utility::graphics::Color<std::uint8_t>(255, 236, 231, 255),
                    "secondary");
    scheme.setColor(SchemeColorRole::OnSecondary,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "onSecondary");
    scheme.setColor(SchemeColorRole::SecondaryContainer,
                    utility::graphics::Color<std::uint8_t>(227, 185, 174, 255),
                    "secondaryContainer");
    scheme.setColor(SchemeColorRole::OnSecondaryContainer,
                    utility::graphics::Color<std::uint8_t>(25, 6, 3, 255),
                    "onSecondaryContainer");
    scheme.setColor(SchemeColorRole::Tertiary,
                    utility::graphics::Color<std::uint8_t>(255, 239, 196, 255),
                    "tertiary");
    scheme.setColor(SchemeColorRole::OnTertiary,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "onTertiary");
    scheme.setColor(SchemeColorRole::TertiaryContainer,
                    utility::graphics::Color<std::uint8_t>(212, 194, 137, 255),
                    "tertiaryContainer");
    scheme.setColor(SchemeColorRole::OnTertiaryContainer,
                    utility::graphics::Color<std::uint8_t>(16, 11, 0, 255),
                    "onTertiaryContainer");
    scheme.setColor(SchemeColorRole::Error,
                    utility::graphics::Color<std::uint8_t>(255, 236, 233, 255),
                    "error");
    scheme.setColor(SchemeColorRole::OnError,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "onError");
    scheme.setColor(SchemeColorRole::ErrorContainer,
                    utility::graphics::Color<std::uint8_t>(255, 170, 164, 255),
                    "errorContainer");
    scheme.setColor(SchemeColorRole::OnErrorContainer,
                    utility::graphics::Color<std::uint8_t>(34, 0, 1, 255),
                    "onErrorContainer");
    scheme.setColor(SchemeColorRole::Background,
                    utility::graphics::Color<std::uint8_t>(26, 17, 15, 255),
                    "background");
    scheme.setColor(SchemeColorRole::OnBackground,
                    utility::graphics::Color<std::uint8_t>(241, 223, 218, 255),
                    "onBackground");
    scheme.setColor(SchemeColorRole::Surface,
                    utility::graphics::Color<std::uint8_t>(26, 17, 15, 255),
                    "surface");
    scheme.setColor(SchemeColorRole::OnSurface,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onSurface");
    scheme.setColor(SchemeColorRole::SurfaceVariant,
                    utility::graphics::Color<std::uint8_t>(83, 67, 63, 255),
                    "surfaceVariant");
    scheme.setColor(SchemeColorRole::OnSurfaceVariant,
                    utility::graphics::Color<std::uint8_t>(255, 255, 255, 255),
                    "onSurfaceVariant");
    scheme.setColor(SchemeColorRole::Outline,
                    utility::graphics::Color<std::uint8_t>(255, 236, 231, 255),
                    "outline");
    scheme.setColor(SchemeColorRole::OutlineVariant,
                    utility::graphics::Color<std::uint8_t>(212, 190, 184, 255),
                    "outlineVariant");
    scheme.setColor(SchemeColorRole::Shadow,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "shadow");
    scheme.setColor(SchemeColorRole::Scrim,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "scrim");
    scheme.setColor(SchemeColorRole::InverseSurface,
                    utility::graphics::Color<std::uint8_t>(241, 223, 218, 255),
                    "inverseSurface");
    scheme.setColor(SchemeColorRole::InverseOnSurface,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "inverseOnSurface");
    scheme.setColor(SchemeColorRole::InversePrimary,
                    utility::graphics::Color<std::uint8_t>(116, 54, 36, 255),
                    "inversePrimary");
    scheme.setColor(SchemeColorRole::PrimaryFixed,
                    utility::graphics::Color<std::uint8_t>(255, 219, 209, 255),
                    "primaryFixed");
    scheme.setColor(SchemeColorRole::OnPrimaryFixed,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "onPrimaryFixed");
    scheme.setColor(SchemeColorRole::PrimaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(255, 181, 160, 255),
                    "primaryFixedDim");
    scheme.setColor(SchemeColorRole::OnPrimaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(40, 5, 0, 255),
                    "onPrimaryFixedVariant");
    scheme.setColor(SchemeColorRole::SecondaryFixed,
                    utility::graphics::Color<std::uint8_t>(255, 219, 209, 255),
                    "secondaryFixed");
    scheme.setColor(SchemeColorRole::OnSecondaryFixed,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "onSecondaryFixed");
    scheme.setColor(SchemeColorRole::SecondaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(231, 189, 178, 255),
                    "secondaryFixedDim");
    scheme.setColor(SchemeColorRole::OnSecondaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(32, 11, 6, 255),
                    "onSecondaryFixedVariant");
    scheme.setColor(SchemeColorRole::TertiaryFixed,
                    utility::graphics::Color<std::uint8_t>(245, 225, 167, 255),
                    "tertiaryFixed");
    scheme.setColor(SchemeColorRole::OnTertiaryFixed,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "onTertiaryFixed");
    scheme.setColor(SchemeColorRole::TertiaryFixedDim,
                    utility::graphics::Color<std::uint8_t>(216, 197, 141, 255),
                    "tertiaryFixedDim");
    scheme.setColor(SchemeColorRole::OnTertiaryFixedVariant,
                    utility::graphics::Color<std::uint8_t>(23, 17, 0, 255),
                    "onTertiaryFixedVariant");
    scheme.setColor(SchemeColorRole::SurfaceDim,
                    utility::graphics::Color<std::uint8_t>(26, 17, 15, 255),
                    "surfaceDim");
    scheme.setColor(SchemeColorRole::SurfaceBright,
                    utility::graphics::Color<std::uint8_t>(90, 77, 74, 255),
                    "surfaceBright");
    scheme.setColor(SchemeColorRole::SurfaceContainerLowest,
                    utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                    "surfaceContainerLowest");
    scheme.setColor(SchemeColorRole::SurfaceContainerLow,
                    utility::graphics::Color<std::uint8_t>(39, 29, 27, 255),
                    "surfaceContainerLow");
    scheme.setColor(SchemeColorRole::SurfaceContainer,
                    utility::graphics::Color<std::uint8_t>(57, 46, 43, 255),
                    "surfaceContainer");
    scheme.setColor(SchemeColorRole::SurfaceContainerHigh,
                    utility::graphics::Color<std::uint8_t>(68, 57, 54, 255),
                    "surfaceContainerHigh");
    scheme.setColor(SchemeColorRole::SurfaceContainerHighest,
                    utility::graphics::Color<std::uint8_t>(80, 68, 65, 255),
                    "surfaceContainerHighest");
}

/**
 * @brief Initialize all palettes with their tones.
 */
void initializePalettes(Theme &theme) {
    // Primary palette
    Palette primaryPalette("Primary", "Primary color palette");
    primaryPalette.setTone(Palette::Tone::T0,
                           utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                           "0");
    primaryPalette.setTone(
        Palette::Tone::T10,
        utility::graphics::Color<std::uint8_t>(58, 11, 1, 255), "10");
    primaryPalette.setTone(
        Palette::Tone::T20,
        utility::graphics::Color<std::uint8_t>(62, 21, 9, 255), "20");
    primaryPalette.setTone(
        Palette::Tone::T30,
        utility::graphics::Color<std::uint8_t>(134, 34, 0, 255), "30");
    primaryPalette.setTone(
        Palette::Tone::T40,
        utility::graphics::Color<std::uint8_t>(171, 53, 15, 255), "40");
    primaryPalette.setTone(
        Palette::Tone::T50,
        utility::graphics::Color<std::uint8_t>(205, 77, 38, 255), "50");
    primaryPalette.setTone(
        Palette::Tone::T60,
        utility::graphics::Color<std::uint8_t>(239, 102, 61, 255), "60");
    primaryPalette.setTone(
        Palette::Tone::T70,
        utility::graphics::Color<std::uint8_t>(255, 139, 105, 255), "70");
    primaryPalette.setTone(
        Palette::Tone::T80,
        utility::graphics::Color<std::uint8_t>(255, 181, 160, 255), "80");
    primaryPalette.setTone(
        Palette::Tone::T90,
        utility::graphics::Color<std::uint8_t>(255, 219, 209, 255), "90");
    primaryPalette.setTone(
        Palette::Tone::T100,
        utility::graphics::Color<std::uint8_t>(255, 255, 255, 255), "100");
    theme.setPrimaryPalette(primaryPalette);

    // Secondary palette
    Palette secondaryPalette("Secondary", "Secondary color palette");
    secondaryPalette.setTone(
        Palette::Tone::T0, utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
        "0");
    secondaryPalette.setTone(
        Palette::Tone::T10,
        utility::graphics::Color<std::uint8_t>(36, 8, 3, 255), "10");
    secondaryPalette.setTone(
        Palette::Tone::T20,
        utility::graphics::Color<std::uint8_t>(49, 19, 9, 255), "20");
    secondaryPalette.setTone(
        Palette::Tone::T30,
        utility::graphics::Color<std::uint8_t>(62, 28, 13, 255), "30");
    secondaryPalette.setTone(
        Palette::Tone::T40,
        utility::graphics::Color<std::uint8_t>(127, 83, 71, 255), "40");
    secondaryPalette.setTone(
        Palette::Tone::T50,
        utility::graphics::Color<std::uint8_t>(155, 108, 98, 255), "50");
    secondaryPalette.setTone(
        Palette::Tone::T60,
        utility::graphics::Color<std::uint8_t>(184, 133, 124, 255), "60");
    secondaryPalette.setTone(
        Palette::Tone::T70,
        utility::graphics::Color<std::uint8_t>(213, 159, 143, 255), "70");
    secondaryPalette.setTone(
        Palette::Tone::T80,
        utility::graphics::Color<std::uint8_t>(242, 185, 169, 255), "80");
    secondaryPalette.setTone(
        Palette::Tone::T90,
        utility::graphics::Color<std::uint8_t>(255, 219, 209, 255), "90");
    secondaryPalette.setTone(
        Palette::Tone::T100,
        utility::graphics::Color<std::uint8_t>(255, 255, 255, 255), "100");
    theme.setSecondaryPalette(secondaryPalette);

    // Tertiary palette
    Palette tertiaryPalette("Tertiary", "Tertiary color palette");
    tertiaryPalette.setTone(
        Palette::Tone::T0, utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
        "0");
    tertiaryPalette.setTone(
        Palette::Tone::T10,
        utility::graphics::Color<std::uint8_t>(35, 16, 0, 255), "10");
    tertiaryPalette.setTone(
        Palette::Tone::T20,
        utility::graphics::Color<std::uint8_t>(47, 27, 0, 255), "20");
    tertiaryPalette.setTone(
        Palette::Tone::T30,
        utility::graphics::Color<std::uint8_t>(59, 39, 0, 255), "30");
    tertiaryPalette.setTone(
        Palette::Tone::T40,
        utility::graphics::Color<std::uint8_t>(72, 51, 16, 255), "40");
    tertiaryPalette.setTone(
        Palette::Tone::T50,
        utility::graphics::Color<std::uint8_t>(138, 117, 45, 255), "50");
    tertiaryPalette.setTone(
        Palette::Tone::T60,
        utility::graphics::Color<std::uint8_t>(165, 143, 68, 255), "60");
    tertiaryPalette.setTone(
        Palette::Tone::T70,
        utility::graphics::Color<std::uint8_t>(193, 170, 92, 255), "70");
    tertiaryPalette.setTone(
        Palette::Tone::T80,
        utility::graphics::Color<std::uint8_t>(222, 197, 116, 255), "80");
    tertiaryPalette.setTone(
        Palette::Tone::T90,
        utility::graphics::Color<std::uint8_t>(252, 225, 141, 255), "90");
    tertiaryPalette.setTone(
        Palette::Tone::T100,
        utility::graphics::Color<std::uint8_t>(255, 255, 255, 255), "100");
    theme.setTertiaryPalette(tertiaryPalette);

    // Neutral palette
    Palette neutralPalette("Neutral", "Neutral color palette");
    neutralPalette.setTone(Palette::Tone::T0,
                           utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
                           "0");
    neutralPalette.setTone(
        Palette::Tone::T10,
        utility::graphics::Color<std::uint8_t>(21, 16, 14, 255), "10");
    neutralPalette.setTone(
        Palette::Tone::T20,
        utility::graphics::Color<std::uint8_t>(32, 26, 24, 255), "20");
    neutralPalette.setTone(
        Palette::Tone::T30,
        utility::graphics::Color<std::uint8_t>(45, 37, 35, 255), "30");
    neutralPalette.setTone(
        Palette::Tone::T40,
        utility::graphics::Color<std::uint8_t>(54, 46, 42, 255), "40");
    neutralPalette.setTone(
        Palette::Tone::T50,
        utility::graphics::Color<std::uint8_t>(127, 115, 114, 255), "50");
    neutralPalette.setTone(
        Palette::Tone::T60,
        utility::graphics::Color<std::uint8_t>(152, 140, 140, 255), "60");
    neutralPalette.setTone(
        Palette::Tone::T70,
        utility::graphics::Color<std::uint8_t>(180, 169, 166, 255), "70");
    neutralPalette.setTone(
        Palette::Tone::T80,
        utility::graphics::Color<std::uint8_t>(208, 196, 193, 255), "80");
    neutralPalette.setTone(
        Palette::Tone::T90,
        utility::graphics::Color<std::uint8_t>(237, 224, 220, 255), "90");
    neutralPalette.setTone(
        Palette::Tone::T100,
        utility::graphics::Color<std::uint8_t>(255, 255, 255, 255), "100");
    theme.setNeutralPalette(neutralPalette);

    // Neutral Variant palette
    Palette neutralVariantPalette("Neutral Variant",
                                  "Neutral variant color palette");
    neutralVariantPalette.setTone(
        Palette::Tone::T0, utility::graphics::Color<std::uint8_t>(0, 0, 0, 255),
        "0");
    neutralVariantPalette.setTone(
        Palette::Tone::T10,
        utility::graphics::Color<std::uint8_t>(25, 14, 11, 255), "10");
    neutralVariantPalette.setTone(
        Palette::Tone::T20,
        utility::graphics::Color<std::uint8_t>(37, 25, 21, 255), "20");
    neutralVariantPalette.setTone(
        Palette::Tone::T30,
        utility::graphics::Color<std::uint8_t>(48, 35, 31, 255), "30");
    neutralVariantPalette.setTone(
        Palette::Tone::T40,
        utility::graphics::Color<std::uint8_t>(108, 91, 86, 255), "40");
    neutralVariantPalette.setTone(
        Palette::Tone::T50,
        utility::graphics::Color<std::uint8_t>(133, 115, 110, 255), "50");
    neutralVariantPalette.setTone(
        Palette::Tone::T60,
        utility::graphics::Color<std::uint8_t>(160, 140, 135, 255), "60");
    neutralVariantPalette.setTone(
        Palette::Tone::T70,
        utility::graphics::Color<std::uint8_t>(188, 167, 161, 255), "70");
    neutralVariantPalette.setTone(
        Palette::Tone::T80,
        utility::graphics::Color<std::uint8_t>(216, 194, 188, 255), "80");
    neutralVariantPalette.setTone(
        Palette::Tone::T90,
        utility::graphics::Color<std::uint8_t>(245, 222, 216, 255), "90");
    neutralVariantPalette.setTone(
        Palette::Tone::T100,
        utility::graphics::Color<std::uint8_t>(255, 255, 255, 255), "100");
    theme.setNeutralVariantPalette(neutralVariantPalette);
}
} // namespace

/**
 * @brief Initialize the defaultTheme with Material Design 3 colors and schemes
 * from material-theme.json.
 */
Theme initializeDefaultTheme() {
    Theme theme("Material Design 3 Theme",
                "A theme with Material Design 3 colors and multiple contrast "
                "levels");

    // Initialize all color schemes
    initializeLightScheme(theme.getLightScheme());
    initializeLightMediumContrastScheme(theme.getLightMediumContrastScheme());
    initializeLightHighContrastScheme(theme.getLightHighContrastScheme());
    initializeDarkScheme(theme.getDarkScheme());
    initializeDarkMediumContrastScheme(theme.getDarkMediumContrastScheme());
    initializeDarkHighContrastScheme(theme.getDarkHighContrastScheme());

    // Initialize all palettes
    initializePalettes(theme);

    return theme;
}

const Theme defaultTheme = initializeDefaultTheme();

} // namespace guillaume
