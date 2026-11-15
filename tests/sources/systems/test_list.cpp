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

#include "systems/test_list.hpp"

#include <guillaume/components/mouse_hover_interaction.hpp>
#include <guillaume/components/style_state.hpp>
#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Variant = components::List::Variant;
		using Ids	  = std::vector<ecs::Entity::Identifier>;
	}	 // namespace

	TEST_F(TestListSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestListSystem, ListIsASurfaceColumnOfItsTopLevelRows)
	{
		_system.update(_entity);

		const auto &layout =
			_registry.getComponent<components::Layout>(_entity);
		EXPECT_EQ(layout.getAxis(), components::Layout::Axis::Vertical);
		EXPECT_FLOAT_EQ(layout.getFixedWidth(), components::List::DefaultWidth);
		EXPECT_EQ(colorOf(_entity), getActiveColor(SchemeColorRole::Surface));
		EXPECT_EQ(laidOut(_entity), (Ids { _inbox, _folders, _more }));
	}

	TEST_F(TestListSystem, ForcedWidthIsKeptAndGivenToTheRows)
	{
		_registry.getComponent<components::Layout>(_entity).setFixedWidth(
			240.0f);
		_system.update(_entity);

		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getWidth(),
			240.0f);
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_inbox).getWidth(),
			240.0f);
	}

	TEST_F(TestListSystem, RowHeightFollowsItsContent)
	{
		_system.update(_entity);

		const auto &row = _registry.getComponent<components::Bound>(_inbox);
		const auto &content =
			_registry.getComponent<components::Bound>(_inboxContent);

		// One 24 dp line plus the 16 dp M3 padding on both edges.
		EXPECT_FLOAT_EQ(content.getHeight(), 24.0f);
		EXPECT_FLOAT_EQ(row.getHeight(), 56.0f);
		EXPECT_FLOAT_EQ(row.getWidth(), components::List::DefaultWidth);
		EXPECT_EQ(laidOut(_inbox), (Ids { _inboxLead, _inboxAction }));
		EXPECT_EQ(laidOut(_inboxLead), (Ids { _inboxIcon, _inboxContent }));

		float rows = 0.0f;
		for (const auto child: { _inbox, _folders, _more }) {
			rows +=
				_registry.getComponent<components::Bound>(child).getHeight();
		}
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getHeight(),
			rows);
	}

	TEST_F(TestListSystem, VariantControlsTheDescription)
	{
		_system.update(_entity);
		EXPECT_EQ(laidOut(_inboxContent), (Ids { _inboxTitle }));
		EXPECT_EQ(colorOf(_inboxDetails).getAlpha(), 0);

		list().setVariant(Variant::TwoLine);
		_system.update(_entity);

		EXPECT_EQ(laidOut(_inboxContent), (Ids { _inboxTitle, _inboxDetails }));
		EXPECT_EQ(colorOf(_inboxDetails),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_inbox).getHeight(),
			24.0f + 20.0f + 2.0f * theme::SpacingScale::get(4));
	}

	TEST_F(TestListSystem, TrailingIconSpreadsTheRow)
	{
		_system.update(_entity);

		EXPECT_EQ(_registry.getComponent<components::Layout>(_inbox)
					  .getMainAxisAlignment(),
				  components::Layout::MainAxisAlignment::SpaceBetween);

		guillaume::systems::List::setExpanded(_registry, _entity, 1, true);
		EXPECT_EQ(_registry.getComponent<components::Layout>(_work)
					  .getMainAxisAlignment(),
				  components::Layout::MainAxisAlignment::Start);
	}

	TEST_F(TestListSystem, RowsExposeTheirEntryAndReportPresses)
	{
		_system.update(_entity);

		const auto &accessibility =
			_registry.getComponent<components::Accessibility>(_inbox);
		EXPECT_EQ(accessibility.getRole(),
				  components::AccessibilityRole::ListItem);
		EXPECT_EQ(accessibility.getLabel(), "Inbox");
		EXPECT_EQ(accessibility.getHint(), "3 new");
		EXPECT_EQ(colorOf(_inboxTitle),
				  getActiveColor(SchemeColorRole::OnSurface));
		EXPECT_EQ(colorOf(_inboxIcon),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));

		click(_inbox);
		EXPECT_EQ(_inboxPresses, 1);
		EXPECT_FALSE(_registry.hasComponent<components::Accessibility>(_more));
	}

	TEST_F(TestListSystem, DisabledRowIsInertAndDimmed)
	{
		guillaume::systems::List::setItemDisabled(_registry, _entity, 0, true);

		click(_inbox);
		guillaume::systems::List::pressItem(_registry, _entity, { 0 });

		EXPECT_EQ(_inboxPresses, 0);
		EXPECT_FALSE(
			_registry.getComponent<components::StyleState>(_inbox).isEnabled());
		EXPECT_EQ(colorOf(_inboxTitle), entities::disabledContentColor());
		EXPECT_EQ(colorOf(_inboxIcon), entities::disabledContentColor());

		guillaume::systems::List::setItemDisabled(_registry, _entity, 0, false);
		click(_inbox);
		EXPECT_EQ(_inboxPresses, 1);
	}

	TEST_F(TestListSystem, HoverPaintsTheStateLayer)
	{
		_system.update(_entity);
		EXPECT_EQ(colorOf(_inbox).getAlpha(), 0);

		auto &hover =
			_registry.getComponent<components::MouseHoverInteraction>(_inbox);
		hover.setHovered(true);
		hover.getOnHoverHandler()();

		EXPECT_EQ(colorOf(_inbox).getAlpha(),
				  getStateLayerAlpha(theme::StateLayer::Hovered));
	}

	TEST_F(TestListSystem, CollapsedAccordionHidesItsChildren)
	{
		_system.update(_entity);

		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_folders)
					  .getExpanded(),
				  false);
		EXPECT_EQ(_registry.getComponent<components::Glyph>(_chevron).getName(),
				  components::List::CollapsedGlyph);
		EXPECT_EQ(colorOf(_workTitle).getAlpha(), 0);
		EXPECT_FALSE(
			_registry.getComponent<components::StyleState>(_work).isEnabled());

		click(_work);
		guillaume::systems::List::pressItem(_registry, _entity, { 1, 0 });
		EXPECT_EQ(_workPresses, 0);
	}

	TEST_F(TestListSystem, PressTogglesAnAccordionAndReportsIt)
	{
		_system.update(_entity);

		click(_folders);

		EXPECT_EQ(_foldersPresses, 1);
		EXPECT_TRUE(
			guillaume::systems::List::isExpanded(_registry, _entity, 1));
		EXPECT_EQ(laidOut(_entity),
				  (Ids { _inbox, _folders, _work, _home, _more }));
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_folders)
					  .getExpanded(),
				  true);
		EXPECT_EQ(_registry.getComponent<components::Glyph>(_chevron).getName(),
				  components::List::ExpandedGlyph);
		EXPECT_EQ(colorOf(_workTitle),
				  getActiveColor(SchemeColorRole::OnSurface));
		EXPECT_TRUE(
			_registry.getComponent<components::StyleState>(_work).isEnabled());
		// Children are inset to line up with the parent title.
		EXPECT_EQ(laidOut(_workLead), (Ids { _workSpacer, _workContent }));
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_workSpacer).getWidth(),
			40.0f);

		click(_work);
		EXPECT_EQ(_workPresses, 1);

		click(_folders);
		EXPECT_FALSE(
			guillaume::systems::List::isExpanded(_registry, _entity, 1));
		EXPECT_EQ(laidOut(_entity), (Ids { _inbox, _folders, _more }));
		EXPECT_FALSE(
			_registry.getComponent<components::StyleState>(_work).isEnabled());
	}

	TEST_F(TestListSystem, DisabledChildStaysDisabledWhenExpanded)
	{
		guillaume::systems::List::setExpanded(_registry, _entity, 1, true);

		EXPECT_FALSE(
			_registry.getComponent<components::StyleState>(_home).isEnabled());
		EXPECT_EQ(colorOf(_homeTitle), entities::disabledContentColor());

		click(_home);
		EXPECT_EQ(_homePresses, 0);
	}

	TEST_F(TestListSystem, ExpandingIgnoresEntriesWithoutChildren)
	{
		guillaume::systems::List::setExpanded(_registry, _entity, 0, true);
		guillaume::systems::List::setExpanded(_registry, _entity, 9, true);

		EXPECT_FALSE(
			guillaume::systems::List::isExpanded(_registry, _entity, 0));
		EXPECT_FALSE(
			guillaume::systems::List::isExpanded(_registry, _entity, 9));
		EXPECT_EQ(laidOut(_entity), (Ids { _inbox, _folders, _more }));
	}

	TEST_F(TestListSystem, StaleRowsAreTolerated)
	{
		list().setItems({});

		EXPECT_NO_THROW(_system.update(_entity));
		EXPECT_NO_THROW(
			guillaume::systems::List::pressItem(_registry, _entity, { 0 }));
		EXPECT_EQ(_inboxPresses, 0);
	}

	TEST_F(TestListSystem, AnUnchangedListIsNotMarkedChanged)
	{
		guillaume::systems::List::setExpanded(_registry, _entity, 1, true);
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);

		for (const auto identifier:
			 { _entity,		 _inbox,		_inboxLead,		 _inboxContent,
			   _inboxTitle,	 _inboxDetails, _inboxIcon,		 _inboxAction,
			   _folders,	 _foldersLead,	_foldersContent, _foldersTitle,
			   _chevron,	 _work,			_workLead,		 _workSpacer,
			   _workContent, _workTitle,	_home,			 _homeLead,
			   _homeSpacer,	 _homeContent,	_homeTitle,		 _more }) {
			EXPECT_FALSE(_registry.hasChanged(identifier)) << identifier;
		}
	}

}	 // namespace guillaume::systems::tests
