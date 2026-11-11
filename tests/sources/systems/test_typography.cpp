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

#include "systems/test_typography.hpp"

#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	TEST_F(TestTypographySystem, RunsInTheMeasurePhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Measure);
	}

	TEST_F(TestTypographySystem, ResolvesTheRoleIntoTextMetrics)
	{
		_registry.getComponent<components::Typography>(_entity).setRole(
			theme::TypeRole::LabelLarge);

		_system.update(_entity);

		const auto &token = getTypeToken(theme::TypeRole::LabelLarge);
		const auto &text  = _registry.getComponent<components::Text>(_entity);
		EXPECT_FLOAT_EQ(text.getFontSize(), token.fontSize);
		EXPECT_FLOAT_EQ(text.getLineHeight(), token.lineHeight);
		EXPECT_FLOAT_EQ(text.getLetterSpacing(), token.letterSpacing);
		EXPECT_EQ(text.getFontWeight(), token.fontWeight);
	}

	TEST_F(TestTypographySystem, ExplicitSizeOverridesOnlyTheRoleSize)
	{
		_registry.getComponent<components::Typography>(_entity)
			.setRole(theme::TypeRole::TitleMedium)
			.setFontSize(30.0f);

		_system.update(_entity);

		const auto &token = getTypeToken(theme::TypeRole::TitleMedium);
		const auto &text  = _registry.getComponent<components::Text>(_entity);
		EXPECT_FLOAT_EQ(text.getFontSize(), 30.0f);
		EXPECT_FLOAT_EQ(text.getLineHeight(), token.lineHeight);
	}

	TEST_F(TestTypographySystem, ClearingTheExplicitSizeRestoresTheRoleSize)
	{
		auto &typography =
			_registry.getComponent<components::Typography>(_entity);
		typography.setRole(theme::TypeRole::TitleMedium).setFontSize(30.0f);
		_system.update(_entity);

		typography.setFontSize(std::nullopt);
		_system.update(_entity);

		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Text>(_entity).getFontSize(),
			getTypeToken(theme::TypeRole::TitleMedium).fontSize);
	}

	TEST_F(TestTypographySystem, FollowsAThemeChange)
	{
		_registry.getComponent<components::Typography>(_entity).setRole(
			theme::TypeRole::BodyLarge);
		_system.update(_entity);
		const float before =
			_registry.getComponent<components::Text>(_entity).getFontSize();

		Theme custom   = defaultTheme;
		auto token	   = custom.getTypescale().get(theme::TypeRole::BodyLarge);
		token.fontSize = before + 6.0f;
		custom.getTypescale().set(theme::TypeRole::BodyLarge, token);
		setActiveTheme(custom);

		_system.update(_entity);
		const float after =
			_registry.getComponent<components::Text>(_entity).getFontSize();

		setActiveTheme(defaultTheme);

		EXPECT_FLOAT_EQ(after, before + 6.0f);
	}

	TEST_F(TestTypographySystem, SteadyStateDoesNotMarkTheTextChanged)
	{
		_registry.getComponent<components::Typography>(_entity)
			.setRole(theme::TypeRole::TitleMedium)
			.setFontSize(30.0f);
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
	}
}	 // namespace guillaume::systems::tests
