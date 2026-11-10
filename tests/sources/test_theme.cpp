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

#include "test_theme.hpp"

#include <cstddef>

namespace guillaume::tests
{
	namespace
	{
		constexpr std::size_t roleCount =
			static_cast<std::size_t>(SchemeColorRole::Count);

		bool isUnset(const ExtendedColor &color)
		{
			return color.getName().empty();
		}

		bool sameColor(const utility::graphic::Color32Bit &lhs,
					   const utility::graphic::Color32Bit &rhs)
		{
			return lhs.getRed() == rhs.getRed()
				&& lhs.getGreen() == rhs.getGreen()
				&& lhs.getBlue() == rhs.getBlue()
				&& lhs.getAlpha() == rhs.getAlpha();
		}
	}	 // namespace

	TEST_F(TestTheme, EverySchemePopulatesEveryRole)
	{
		for (std::size_t scheme = 0; scheme < ThemeSchemeRole::ThemeSchemeCount;
			 ++scheme) {
			const Scheme &current =
				defaultTheme.getScheme(static_cast<ThemeSchemeRole>(scheme));
			for (std::size_t role = 0; role < roleCount; ++role) {
				EXPECT_FALSE(isUnset(
					current.getColor(static_cast<SchemeColorRole>(role))))
					<< "scheme " << scheme << " role " << role;
			}
		}
	}

	TEST_F(TestTheme, SurfaceRolesAreNotOpaqueBlack)
	{
		const SchemeColorRole surfaces[] = {
			SchemeColorRole::OutlineVariant,
			SchemeColorRole::SurfaceDim,
			SchemeColorRole::SurfaceBright,
			SchemeColorRole::SurfaceContainerLowest,
			SchemeColorRole::SurfaceContainerLow,
			SchemeColorRole::SurfaceContainer,
			SchemeColorRole::SurfaceContainerHigh,
			SchemeColorRole::SurfaceContainerHighest,
		};
		const utility::graphic::Color32Bit black(0, 0, 0, 255);
		for (ThemeSchemeRole scheme :
			 {ThemeSchemeRole::Light, ThemeSchemeRole::Dark}) {
			for (SchemeColorRole role : surfaces) {
				EXPECT_FALSE(sameColor(
					defaultTheme.getScheme(scheme).getColor(role).getColor(),
					black))
					<< "scheme " << scheme << " role "
					<< static_cast<std::size_t>(role);
			}
		}
	}

	TEST_F(TestTheme, LightMatchesMaterialBaseline)
	{
		const Scheme &light = defaultTheme.getScheme(ThemeSchemeRole::Light);
		EXPECT_TRUE(
			sameColor(light.getColor(SchemeColorRole::Primary).getColor(),
					  utility::graphic::Color32Bit(103, 80, 164, 255)));
		EXPECT_TRUE(sameColor(
			light.getColor(SchemeColorRole::OutlineVariant).getColor(),
			utility::graphic::Color32Bit(202, 196, 208, 255)));
		EXPECT_TRUE(sameColor(
			light.getColor(SchemeColorRole::SurfaceContainer).getColor(),
			utility::graphic::Color32Bit(243, 237, 247, 255)));
	}

	TEST_F(TestTheme, LightAndDarkDifferOnSurfaces)
	{
		const Scheme &light = defaultTheme.getScheme(ThemeSchemeRole::Light);
		const Scheme &dark	= defaultTheme.getScheme(ThemeSchemeRole::Dark);
		for (SchemeColorRole role :
			 {SchemeColorRole::Surface, SchemeColorRole::OnSurface,
			  SchemeColorRole::SurfaceContainerLow,
			  SchemeColorRole::SurfaceContainerHighest,
			  SchemeColorRole::OutlineVariant}) {
			EXPECT_FALSE(sameColor(light.getColor(role).getColor(),
								   dark.getColor(role).getColor()))
				<< static_cast<std::size_t>(role);
		}
	}

	TEST_F(TestTheme, FixedRolesAreSchemeIndependent)
	{
		const Scheme &light = defaultTheme.getScheme(ThemeSchemeRole::Light);
		const Scheme &dark	= defaultTheme.getScheme(ThemeSchemeRole::Dark);
		EXPECT_TRUE(
			sameColor(light.getColor(SchemeColorRole::PrimaryFixed).getColor(),
					  dark.getColor(SchemeColorRole::PrimaryFixed).getColor()));
	}

	TEST_F(TestTheme, DisabledRolesDeriveFromOnSurface)
	{
		for (std::size_t scheme = 0; scheme < ThemeSchemeRole::ThemeSchemeCount;
			 ++scheme) {
			const Scheme &current =
				defaultTheme.getScheme(static_cast<ThemeSchemeRole>(scheme));
			const auto &onSurface =
				current.getColor(SchemeColorRole::OnSurface).getColor();
			const auto &container =
				current.getColor(SchemeColorRole::SurfaceDisabled).getColor();
			const auto &content =
				current.getColor(SchemeColorRole::OnSurfaceDisabled)
					.getColor();
			EXPECT_EQ(container.getRed(), onSurface.getRed());
			EXPECT_EQ(container.getAlpha(), 31);
			EXPECT_EQ(content.getBlue(), onSurface.getBlue());
			EXPECT_EQ(content.getAlpha(), 97);
			EXPECT_EQ(current.getColor(SchemeColorRole::Backdrop)
						  .getColor()
						  .getAlpha(),
					  102);
		}
	}

	TEST_F(TestTheme, SwitchingSchemeChangesResolvedColors)
	{
		setActiveScheme(ThemeSchemeRole::Light);
		const auto light =
			getActiveScheme().getColor(SchemeColorRole::Surface).getColor();
		setActiveScheme(ThemeSchemeRole::Dark);
		const auto dark =
			getActiveScheme().getColor(SchemeColorRole::Surface).getColor();
		EXPECT_FALSE(sameColor(light, dark));
	}

}	 // namespace guillaume::tests
