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

#include "systems/test_search.hpp"

#include <guillaume/components/style_state.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Variant = components::Search::Variant;
		using Ids	  = std::vector<ecs::Entity::Identifier>;
	}	 // namespace

	TEST_F(TestSearchSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestSearchSystem, SearchBarIsAPill)
	{
		_system.update(_entity);

		const auto &bound = _registry.getComponent<components::Bound>(_entity);
		EXPECT_FLOAT_EQ(bound.getWidth(), components::Search::DefaultWidth);
		EXPECT_FLOAT_EQ(bound.getHeight(), components::Search::BarHeight);
		EXPECT_FLOAT_EQ(_registry.getComponent<components::Borders>(_entity)
							.getTopLeftRadius(),
						getShapeRadius(theme::ShapeRole::ExtraLarge));
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Line>(_entity).getThickness(),
			0.0f);
		EXPECT_EQ(colorOf(_entity),
				  getActiveColor(SchemeColorRole::SurfaceContainerHigh));
	}

	TEST_F(TestSearchSystem, FullScreenIsInlineWithADivider)
	{
		search().setVariant(Variant::FullScreen);
		_system.update(_entity);

		const auto &line = _registry.getComponent<components::Line>(_entity);
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getHeight(),
			components::Search::ViewHeight);
		EXPECT_FLOAT_EQ(_registry.getComponent<components::Borders>(_entity)
							.getTopLeftRadius(),
						getShapeRadius(theme::ShapeRole::None));
		EXPECT_FLOAT_EQ(line.getThickness(), 1.0f);
		EXPECT_EQ(line.getPlacement(), components::Line::Placement::Bottom);
		EXPECT_EQ(line.getColor(),
				  getActiveColor(SchemeColorRole::OutlineVariant));

		search().setVariant(Variant::SearchBar);
		_system.update(_entity);
		EXPECT_FLOAT_EQ(line.getThickness(), 0.0f);
		EXPECT_FALSE(line.hasColor());
	}

	TEST_F(TestSearchSystem, FieldIsAnInlineSearchLandmark)
	{
		_system.update(_entity);

		const auto &accessibility =
			_registry.getComponent<components::Accessibility>(_entity);
		const auto &overlay =
			_registry.getComponent<components::Overlay>(_entity);
		EXPECT_EQ(accessibility.getRole(),
				  components::AccessibilityRole::Search);
		EXPECT_EQ(accessibility.getLabel(), "Search");
		EXPECT_FALSE(overlay.isModal());
		EXPECT_FALSE(overlay.dismissesOnOutsideClick());
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_clear)
					  .getLabel(),
				  "clear search");
		EXPECT_EQ(
			_registry.getComponent<components::Accessibility>(_icon).getRole(),
			components::AccessibilityRole::Button);
	}

	TEST_F(TestSearchSystem, PlaceholderShowsWhileTheQueryIsEmpty)
	{
		_system.update(_entity);

		const auto &textField =
			_registry.getComponent<components::TextField>(_entity);
		EXPECT_EQ(laidOut(), (Ids { _icon, _placeholder, _clear }));
		EXPECT_EQ(
			_registry.getComponent<components::Text>(_placeholder).getContent(),
			"Search");
		EXPECT_EQ(colorOf(_placeholder),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
		EXPECT_EQ(colorOf(_clear).getAlpha(), 0);
		EXPECT_FALSE(
			_registry.getComponent<components::StyleState>(_clear).isEnabled());
		EXPECT_EQ(textField.getPlaceholder(), "Search");
		EXPECT_FALSE(textField.isLabelFloating());
	}

	TEST_F(TestSearchSystem, QueryReplacesThePlaceholderAndShowsClear)
	{
		guillaume::systems::Search::setText(_registry, _entity, "cats");

		EXPECT_EQ(guillaume::systems::Search::text(_registry, _entity), "cats");
		EXPECT_EQ(_registry.getComponent<components::Text>(_query).getContent(),
				  "cats");
		EXPECT_EQ(laidOut(), (Ids { _icon, _query, _clear }));
		EXPECT_EQ(colorOf(_placeholder).getAlpha(), 0);
		EXPECT_EQ(colorOf(_clear),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
		EXPECT_TRUE(
			_registry.getComponent<components::StyleState>(_clear).isEnabled());
		EXPECT_TRUE(_registry.getComponent<components::TextField>(_entity)
						.isLabelFloating());
	}

	TEST_F(TestSearchSystem, TypedQueryIsAdopted)
	{
		// The text-input systems edit the query child directly.
		_registry.getComponent<components::Text>(_query).setContent("dog");
		_system.update(_entity);

		EXPECT_EQ(search().getText(), "dog");
		EXPECT_EQ(laidOut(), (Ids { _icon, _query, _clear }));
	}

	TEST_F(TestSearchSystem, QueryLivesInTheComponentWithoutAQueryChild)
	{
		search().setTextEntity(ecs::Entity::InvalidIdentifier);

		guillaume::systems::Search::setText(_registry, _entity, "cats");

		EXPECT_EQ(guillaume::systems::Search::text(_registry, _entity), "cats");
		EXPECT_EQ(laidOut(), (Ids { _icon, _clear }));
	}

	TEST_F(TestSearchSystem, ClearButtonEmptiesTheQueryAndReportsIt)
	{
		guillaume::systems::Search::setText(_registry, _entity, "cats");

		click(_clear);

		EXPECT_EQ(_clearPresses, 1);
		EXPECT_EQ(guillaume::systems::Search::text(_registry, _entity), "");
		EXPECT_EQ(laidOut(), (Ids { _icon, _placeholder, _clear }));
		EXPECT_FALSE(
			_registry.getComponent<components::StyleState>(_clear).isEnabled());

		// The hidden button is inert.
		click(_clear);
		EXPECT_EQ(_clearPresses, 1);
	}

	TEST_F(TestSearchSystem, LoadingGlyphReplacesTheClearButton)
	{
		guillaume::systems::Search::setText(_registry, _entity, "cats");
		search().setLoading(true);
		_system.update(_entity);

		EXPECT_EQ(laidOut(), (Ids { _icon, _query, _loading }));
		EXPECT_EQ(colorOf(_loading), getActiveColor(SchemeColorRole::Primary));
		EXPECT_EQ(colorOf(_clear).getAlpha(), 0);
		EXPECT_FALSE(
			_registry.getComponent<components::StyleState>(_clear).isEnabled());

		search().setLoading(false);
		_system.update(_entity);
		EXPECT_EQ(colorOf(_loading).getAlpha(), 0);
		EXPECT_EQ(laidOut(), (Ids { _icon, _query, _clear }));
	}

	TEST_F(TestSearchSystem, LeadingIconFollowsItsGlyphAndReportsPresses)
	{
		search().setIconGlyph("arrow_back");
		_system.update(_entity);

		EXPECT_EQ(_registry.getComponent<components::Glyph>(_icon).getName(),
				  "arrow_back");

		click(_icon);
		EXPECT_EQ(_iconPresses, 1);
	}

	TEST_F(TestSearchSystem, TrailingIconShowsWhileItHasAGlyph)
	{
		_system.update(_entity);
		EXPECT_EQ(colorOf(_trailing).getAlpha(), 0);
		click(_trailing);
		EXPECT_EQ(_trailingPresses, 0);

		search().setTrailingGlyph("mic");
		_system.update(_entity);

		EXPECT_EQ(laidOut(), (Ids { _icon, _placeholder, _clear, _trailing }));
		EXPECT_EQ(
			_registry.getComponent<components::Glyph>(_trailing).getName(),
			"mic");
		EXPECT_EQ(colorOf(_trailing),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
		click(_trailing);
		EXPECT_EQ(_trailingPresses, 1);

		search().setTrailingGlyph("");
		_system.update(_entity);
		EXPECT_EQ(laidOut(), (Ids { _icon, _placeholder, _clear }));
	}

	TEST_F(TestSearchSystem, ElevationTintsTheSurface)
	{
		_registry.getComponent<components::Elevation>(_entity)
			.setElevationLevel(theme::ElevationLevel::Level3);
		_system.update(_entity);

		EXPECT_EQ(colorOf(_entity),
				  getElevatedSurfaceColor(theme::ElevationLevel::Level3));
	}

	TEST_F(TestSearchSystem, ShowAndHideToggleTheOverlay)
	{
		EXPECT_FALSE(guillaume::systems::Search::isVisible(_registry, _entity));

		guillaume::systems::Search::show(_registry, _entity);
		EXPECT_TRUE(guillaume::systems::Search::isVisible(_registry, _entity));

		guillaume::systems::Search::hide(_registry, _entity);
		EXPECT_FALSE(guillaume::systems::Search::isVisible(_registry, _entity));
	}

	TEST_F(TestSearchSystem, AnUnchangedSearchIsNotMarkedChanged)
	{
		guillaume::systems::Search::setText(_registry, _entity, "cats");
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);

		for (const auto identifier: { _entity, _icon, _query, _placeholder,
									  _clear, _loading, _trailing }) {
			EXPECT_FALSE(_registry.hasChanged(identifier)) << identifier;
		}
	}

}	 // namespace guillaume::systems::tests
