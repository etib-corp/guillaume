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

#include "guillaume/systems/fab_style.hpp"

#include <array>
#include <optional>
#include <stdexcept>

#include "guillaume/theme.hpp"

namespace guillaume::systems::fab_style::detail
{
	namespace
	{
		using Role = SchemeColorRole;

		/// Metrics of one FAB size.
		struct SizeMetrics {
			float containerSize;	   ///< Container extent.
			float iconFontSize;		   ///< Icon font size of a regular FAB.
			float labelFontSize;	   ///< Font size of an extended FAB.
			theme::ShapeRole shape;	   ///< Corner shape.
		};

		/// Indexed by Size (Small, Medium, Large).
		constexpr std::array<SizeMetrics, 3> sizeTable { {
			{ 40.0f, 24.0f, 18.0f, theme::ShapeRole::Medium },
			{ 56.0f, 24.0f, 24.0f, theme::ShapeRole::Large },
			{ 96.0f, 36.0f, 36.0f, theme::ShapeRole::ExtraLarge },
		} };

		/// Scheme roles of one variant; no container role means the tonal
		/// elevated surface color.
		struct Palette {
			std::optional<Role> container;	  ///< Container role.
			Role content;					  ///< Icon and label role.
		};

		/// Indexed by Variant (Surface, Primary, Secondary, Tertiary).
		constexpr std::array<Palette, 4> paletteTable { {
			{ std::nullopt, Role::Primary },
			{ Role::PrimaryContainer, Role::OnPrimaryContainer },
			{ Role::SecondaryContainer, Role::OnSecondaryContainer },
			{ Role::TertiaryContainer, Role::OnTertiaryContainer },
		} };

		const SizeMetrics &metrics(std::size_t size)
		{
			if (size >= sizeTable.size()) {
				throw std::runtime_error("Invalid FAB size");
			}
			return sizeTable[size];
		}

		const Palette &palette(std::size_t variant)
		{
			if (variant >= paletteTable.size()) {
				throw std::runtime_error("Invalid FAB variant");
			}
			return paletteTable[variant];
		}
	}	 // namespace

	float containerSize(std::size_t size)
	{
		return metrics(size).containerSize;
	}

	float iconFontSize(std::size_t size)
	{
		return metrics(size).iconFontSize;
	}

	float labelFontSize(std::size_t size)
	{
		return metrics(size).labelFontSize;
	}

	float borderRadius(std::size_t size)
	{
		return getShapeRadius(metrics(size).shape);
	}

	utility::graphic::Color32Bit containerColor(std::size_t variant,
												bool lowered)
	{
		const auto role = palette(variant).container;
		if (role) {
			return getActiveColor(*role);
		}
		// Paper's surface FAB uses the tonal elevation.level3 color (level1
		// when lowered).
		return getElevatedSurfaceColor(lowered ? theme::ElevationLevel::Level1
											   : theme::ElevationLevel::Level3);
	}

	utility::graphic::Color32Bit contentColor(std::size_t variant)
	{
		return getActiveColor(palette(variant).content);
	}
}	 // namespace guillaume::systems::fab_style::detail
