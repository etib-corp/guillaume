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

#include <guillaume/motion/easing.hpp>

#include <cstddef>
#include <iterator>
#include <stdexcept>

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
		for (ThemeSchemeRole scheme:
			 { ThemeSchemeRole::Light, ThemeSchemeRole::Dark }) {
			for (SchemeColorRole role: surfaces) {
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
		for (SchemeColorRole role:
			 { SchemeColorRole::Surface, SchemeColorRole::OnSurface,
			   SchemeColorRole::SurfaceContainerLow,
			   SchemeColorRole::SurfaceContainerHighest,
			   SchemeColorRole::OutlineVariant }) {
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
				current.getColor(SchemeColorRole::OnSurfaceDisabled).getColor();
			EXPECT_EQ(container.getRed(), onSurface.getRed());
			EXPECT_EQ(container.getAlpha(),
					  getStateLayerAlpha(theme::StateLayer::DisabledContainer));
			EXPECT_EQ(content.getBlue(), onSurface.getBlue());
			EXPECT_EQ(content.getAlpha(),
					  getStateLayerAlpha(theme::StateLayer::DisabledContent));
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

	TEST_F(TestTheme, TypescaleMatchesMaterialBaseline)
	{
		struct Expected {
			theme::TypeRole role;
			float size;
			float lineHeight;
			float letterSpacing;
			theme::FontWeight weight;
		};
		constexpr auto R		  = theme::FontWeight::Regular;
		constexpr auto M		  = theme::FontWeight::Medium;
		const Expected expected[] = {
			{ theme::TypeRole::DisplayLarge, 57.0f, 64.0f, 0.0f, R },
			{ theme::TypeRole::DisplayMedium, 45.0f, 52.0f, 0.0f, R },
			{ theme::TypeRole::DisplaySmall, 36.0f, 44.0f, 0.0f, R },
			{ theme::TypeRole::HeadlineLarge, 32.0f, 40.0f, 0.0f, R },
			{ theme::TypeRole::HeadlineMedium, 28.0f, 36.0f, 0.0f, R },
			{ theme::TypeRole::HeadlineSmall, 24.0f, 32.0f, 0.0f, R },
			{ theme::TypeRole::TitleLarge, 22.0f, 28.0f, 0.0f, R },
			{ theme::TypeRole::TitleMedium, 16.0f, 24.0f, 0.15f, M },
			{ theme::TypeRole::TitleSmall, 14.0f, 20.0f, 0.1f, M },
			{ theme::TypeRole::LabelLarge, 14.0f, 20.0f, 0.1f, M },
			{ theme::TypeRole::LabelMedium, 12.0f, 16.0f, 0.5f, M },
			{ theme::TypeRole::LabelSmall, 11.0f, 16.0f, 0.5f, M },
			{ theme::TypeRole::BodyLarge, 16.0f, 24.0f, 0.15f, R },
			{ theme::TypeRole::BodyMedium, 14.0f, 20.0f, 0.25f, R },
			{ theme::TypeRole::BodySmall, 12.0f, 16.0f, 0.4f, R },
			{ theme::TypeRole::Default, 14.0f, 20.0f, 0.0f, R },
		};
		static_assert(std::size(expected)
						  == static_cast<std::size_t>(theme::TypeRole::Count),
					  "every typescale role must be checked");

		for (const auto &entry: expected) {
			const auto &token = getTypeToken(entry.role);
			const auto index  = static_cast<std::size_t>(entry.role);
			EXPECT_FLOAT_EQ(token.fontSize, entry.size) << index;
			EXPECT_FLOAT_EQ(token.lineHeight, entry.lineHeight) << index;
			EXPECT_FLOAT_EQ(token.letterSpacing, entry.letterSpacing) << index;
			EXPECT_EQ(token.fontWeight, entry.weight) << index;
			EXPECT_EQ(token.fontFamily, "Roboto") << index;
		}
	}

	TEST_F(TestTheme, TypescaleRejectsInvalidTokens)
	{
		theme::Typescale typescale;
		theme::TypeToken token;
		token.fontSize = 0.0f;
		EXPECT_THROW(typescale.set(theme::TypeRole::BodyLarge, token),
					 std::invalid_argument);
		EXPECT_THROW(typescale.get(theme::TypeRole::Count), std::out_of_range);
	}

	TEST_F(TestTheme, FontWeightSelectsBundledFont)
	{
		EXPECT_EQ(theme::fontPath(theme::FontWeight::Regular),
				  "fonts/Roboto-Regular.ttf");
		EXPECT_EQ(theme::fontPath(theme::FontWeight::Medium),
				  "fonts/Roboto-Medium.ttf");
	}

	TEST_F(TestTheme, ShapeScaleMatchesMaterialBaseline)
	{
		EXPECT_FLOAT_EQ(getShapeRadius(theme::ShapeRole::None), 0.0f);
		EXPECT_FLOAT_EQ(getShapeRadius(theme::ShapeRole::ExtraSmall), 4.0f);
		EXPECT_FLOAT_EQ(getShapeRadius(theme::ShapeRole::Small), 8.0f);
		EXPECT_FLOAT_EQ(getShapeRadius(theme::ShapeRole::Medium), 12.0f);
		EXPECT_FLOAT_EQ(getShapeRadius(theme::ShapeRole::Large), 16.0f);
		EXPECT_FLOAT_EQ(getShapeRadius(theme::ShapeRole::ExtraLarge), 28.0f);
		EXPECT_GE(getShapeRadius(theme::ShapeRole::Full), 1000.0f);
	}

	TEST_F(TestTheme, RoundnessScalesShapeRoles)
	{
		theme::ShapeScale shapes;
		shapes.setRoundness(8.0f);
		EXPECT_FLOAT_EQ(shapes.getRadius(theme::ShapeRole::Medium), 24.0f);
		EXPECT_FLOAT_EQ(shapes.getRadius(theme::ShapeRole::None), 0.0f);
		EXPECT_FLOAT_EQ(shapes.getRadius(theme::ShapeRole::Full),
						theme::ShapeScale::FullRadius);
		EXPECT_THROW(shapes.setRoundness(-1.0f), std::invalid_argument);
		EXPECT_THROW(shapes.getRadius(theme::ShapeRole::Count),
					 std::out_of_range);
	}

	TEST_F(TestTheme, StateLayersMatchPaperOpacities)
	{
		const auto &layers = defaultTheme.getStateLayers();
		EXPECT_FLOAT_EQ(layers.getOpacity(theme::StateLayer::Hovered), 0.08f);
		EXPECT_FLOAT_EQ(layers.getOpacity(theme::StateLayer::Focused), 0.12f);
		EXPECT_FLOAT_EQ(layers.getOpacity(theme::StateLayer::Pressed), 0.12f);
		EXPECT_FLOAT_EQ(layers.getOpacity(theme::StateLayer::Dragged), 0.16f);
		EXPECT_FLOAT_EQ(layers.getOpacity(theme::StateLayer::DisabledContent),
						0.38f);
		EXPECT_FLOAT_EQ(layers.getOpacity(theme::StateLayer::DisabledContainer),
						0.12f);
		EXPECT_EQ(getStateLayerAlpha(theme::StateLayer::Hovered), 20);
		EXPECT_EQ(getStateLayerAlpha(theme::StateLayer::Pressed), 31);
		EXPECT_EQ(getStateLayerAlpha(theme::StateLayer::DisabledContent), 97);

		theme::StateLayerScale custom;
		EXPECT_THROW(custom.setOpacity(theme::StateLayer::Hovered, 1.5f),
					 std::invalid_argument);
	}

	TEST_F(TestTheme, ElevationTintsSurfaceProgressively)
	{
		const float expected[] = { 0.0f, 0.05f, 0.08f, 0.11f, 0.12f, 0.14f };
		for (std::size_t level = 0; level < std::size(expected); ++level) {
			EXPECT_FLOAT_EQ(
				getElevationToken(static_cast<theme::ElevationLevel>(level))
					.tintOpacity,
				expected[level]);
		}

		const auto surface =
			getActiveScheme().getColor(SchemeColorRole::Surface).getColor();
		EXPECT_TRUE(sameColor(
			getElevatedSurfaceColor(theme::ElevationLevel::Level0), surface));
		EXPECT_FALSE(sameColor(
			getElevatedSurfaceColor(theme::ElevationLevel::Level3), surface));
		EXPECT_EQ(getElevationToken(theme::ElevationLevel::Level0).shadowAlpha,
				  0);
		EXPECT_GT(
			getElevationToken(theme::ElevationLevel::Level5).shadowSpread,
			getElevationToken(theme::ElevationLevel::Level1).shadowSpread);
	}

	TEST_F(TestTheme, MotionDurationsFollowMaterialTokens)
	{
		EXPECT_FLOAT_EQ(getMotionDuration(theme::MotionDuration::Short1),
						0.05f);
		EXPECT_FLOAT_EQ(getMotionDuration(theme::MotionDuration::Short4), 0.2f);
		EXPECT_FLOAT_EQ(getMotionDuration(theme::MotionDuration::Medium2),
						0.3f);
		EXPECT_FLOAT_EQ(getMotionDuration(theme::MotionDuration::Long2), 0.5f);
		EXPECT_FLOAT_EQ(getMotionDuration(theme::MotionDuration::ExtraLong4),
						1.0f);
		EXPECT_EQ(theme::MotionScale::enter(),
				  motion::Easing::EmphasizedDecelerate);
		EXPECT_EQ(theme::MotionScale::exit(),
				  motion::Easing::EmphasizedAccelerate);
	}

	TEST_F(TestTheme, EmphasizedEasingsHitEndpoints)
	{
		for (motion::Easing easing: { motion::Easing::EmphasizedDecelerate,
									  motion::Easing::EmphasizedAccelerate }) {
			EXPECT_NEAR(motion::applyEasing(easing, 0.0f), 0.0f, 1e-4f);
			EXPECT_NEAR(motion::applyEasing(easing, 1.0f), 1.0f, 1e-4f);
		}
		// Accelerate starts slow, decelerate starts fast.
		EXPECT_LT(
			motion::applyEasing(motion::Easing::EmphasizedAccelerate, 0.5f),
			motion::applyEasing(motion::Easing::EmphasizedDecelerate, 0.5f));
	}

	TEST_F(TestTheme, SpacingAndTouchTarget)
	{
		EXPECT_FLOAT_EQ(theme::SpacingScale::get(4), 16.0f);
		EXPECT_FLOAT_EQ(theme::SpacingScale::MinTouchTarget, 48.0f);
		EXPECT_FLOAT_EQ(theme::SpacingScale::touchMargin(40.0f), 4.0f);
		EXPECT_FLOAT_EQ(theme::SpacingScale::touchMargin(56.0f), 0.0f);
	}

	TEST_F(TestTheme, ActiveThemeCanBeReplaced)
	{
		const float defaultRadius = getShapeRadius(theme::ShapeRole::Medium);

		Theme custom = defaultTheme;
		custom.getShapes().setRoundness(custom.getShapes().getRoundness()
										* 2.0f);
		setActiveTheme(custom);

		EXPECT_FLOAT_EQ(getShapeRadius(theme::ShapeRole::Medium),
						defaultRadius * 2.0f);

		setActiveTheme(defaultTheme);

		EXPECT_FLOAT_EQ(getShapeRadius(theme::ShapeRole::Medium),
						defaultRadius);
	}

}	 // namespace guillaume::tests
