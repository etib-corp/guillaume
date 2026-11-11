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

#include "systems/test_segmented_button.hpp"

namespace guillaume::systems::tests
{
	namespace
	{
		using Segmented		= guillaume::systems::SegmentedButton;
		using SharedButton	= guillaume::systems::Button;
		using SelectionMode = components::SegmentedButton::SelectionMode;
	}	 // namespace

	TEST_F(TestSegmentedButtonSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestSegmentedButtonSystem, InstallConfiguresTheRowAndItsSegments)
	{
		const auto &layout =
			_registry.getComponent<components::Layout>(_entity);

		EXPECT_EQ(layout.getAxis(), components::Layout::Axis::Horizontal);
		EXPECT_FLOAT_EQ(layout.getSpacing(), 0.0f);
		for (std::size_t index = 0; index < _buttons.size(); ++index) {
			EXPECT_TRUE(button(index).isToggle());
		}
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_buttons[1])
					  .getLabel(),
				  "Whole week");
		EXPECT_TRUE(_registry.getComponent<components::StyleState>(_buttons[0])
						.isEnabled());
		EXPECT_FALSE(_registry.getComponent<components::StyleState>(_buttons[2])
						 .isEnabled());
	}

	TEST_F(TestSegmentedButtonSystem, SegmentsShareTheWidestWidthInARow)
	{
		_registry.getComponent<components::CommonButton>(_buttons[1])
			.setMinWidth(120.0f);
		SharedButton::restyle(_registry, _buttons[1]);

		_system.update(_entity);

		for (const auto segment: _buttons) {
			EXPECT_FLOAT_EQ(
				_registry.getComponent<components::Bound>(segment).getWidth(),
				120.0f);
		}
		EXPECT_EQ(
			_registry.getComponent<components::Layout>(_entity).getChildren(),
			_buttons);
		EXPECT_FLOAT_EQ(positionX(1) - positionX(0), 120.0f);
		EXPECT_FLOAT_EQ(positionX(2) - positionX(1), 120.0f);
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getWidth(),
			360.0f);
	}

	TEST_F(TestSegmentedButtonSystem, SegmentsAreAtLeastTheMinimumWidth)
	{
		_system.update(_entity);

		for (const auto segment: _buttons) {
			EXPECT_FLOAT_EQ(
				_registry.getComponent<components::Bound>(segment).getWidth(),
				components::SegmentedButton::MinSegmentWidth);
		}
	}

	TEST_F(TestSegmentedButtonSystem, OnlyTheOuterCornersAreRounded)
	{
		_system.update(_entity);

		const float radius =
			_registry.getComponent<components::Bound>(_entity).getHeight()
			* 0.5f;
		const auto &first =
			_registry.getComponent<components::Borders>(_buttons[0]);
		const auto &middle =
			_registry.getComponent<components::Borders>(_buttons[1]);
		const auto &last =
			_registry.getComponent<components::Borders>(_buttons[2]);

		ASSERT_GT(radius, 0.0f);
		EXPECT_FLOAT_EQ(first.getTopLeftRadius(), radius);
		EXPECT_FLOAT_EQ(first.getBottomLeftRadius(), radius);
		EXPECT_FLOAT_EQ(first.getTopRightRadius(), 0.0f);
		EXPECT_FLOAT_EQ(first.getBottomRightRadius(), 0.0f);
		EXPECT_FLOAT_EQ(middle.getTopLeftRadius(), 0.0f);
		EXPECT_FLOAT_EQ(middle.getTopRightRadius(), 0.0f);
		EXPECT_FLOAT_EQ(middle.getBottomLeftRadius(), 0.0f);
		EXPECT_FLOAT_EQ(middle.getBottomRightRadius(), 0.0f);
		EXPECT_FLOAT_EQ(last.getTopLeftRadius(), 0.0f);
		EXPECT_FLOAT_EQ(last.getBottomLeftRadius(), 0.0f);
		EXPECT_FLOAT_EQ(last.getTopRightRadius(), radius);
		EXPECT_FLOAT_EQ(last.getBottomRightRadius(), radius);
	}

	TEST_F(TestSegmentedButtonSystem, SingleSelectionIsExclusive)
	{
		Segmented::select(_registry, _entity, 0);
		Segmented::select(_registry, _entity, 1);
		Segmented::select(_registry, _entity, 9);

		EXPECT_FALSE(Segmented::isSelected(_registry, _entity, 0));
		EXPECT_TRUE(Segmented::isSelected(_registry, _entity, 1));
		EXPECT_FALSE(Segmented::isSelected(_registry, _entity, 9));
		EXPECT_EQ(Segmented::selectedIndices(_registry, _entity),
				  std::vector<std::size_t> { 1 });
	}

	TEST_F(TestSegmentedButtonSystem, MultiSelectionKeepsSeveralSegments)
	{
		segmented().setSelectionMode(SelectionMode::Multi);

		Segmented::select(_registry, _entity, 0);
		Segmented::select(_registry, _entity, 2);
		EXPECT_EQ(Segmented::selectedIndices(_registry, _entity),
				  (std::vector<std::size_t> { 0, 2 }));

		Segmented::deselect(_registry, _entity, 0);
		EXPECT_EQ(Segmented::selectedIndices(_registry, _entity),
				  std::vector<std::size_t> { 2 });
	}

	TEST_F(TestSegmentedButtonSystem, ASelectedSegmentShowsTheCheckGlyph)
	{
		_system.update(_entity);
		EXPECT_EQ(glyph(1), "event");

		Segmented::select(_registry, _entity, 1);
		EXPECT_EQ(glyph(1), Segmented::SelectedGlyph);
		EXPECT_EQ(glyph(0), "");

		Segmented::select(_registry, _entity, 0);
		EXPECT_EQ(glyph(0), Segmented::SelectedGlyph);
		EXPECT_EQ(glyph(1), "event");
	}

	TEST_F(TestSegmentedButtonSystem, PressingASegmentCommitsItInSingleMode)
	{
		std::vector<std::size_t> reported;
		segmented().setOnChange([&reported](std::size_t index) {
			reported.push_back(index);
		});
		Segmented::select(_registry, _entity, 1);

		// The click handler wired by install() carries the segment index.
		SharedButton::release(_registry, _buttons[0]);
		EXPECT_EQ(Segmented::selectedIndices(_registry, _entity),
				  std::vector<std::size_t> { 0 });

		// Pressing the selected segment again keeps it selected.
		SharedButton::release(_registry, _buttons[0]);
		EXPECT_EQ(Segmented::selectedIndices(_registry, _entity),
				  std::vector<std::size_t> { 0 });
		EXPECT_EQ(reported, (std::vector<std::size_t> { 0, 0 }));
		EXPECT_EQ(glyph(0), Segmented::SelectedGlyph);
	}

	TEST_F(TestSegmentedButtonSystem, PressingASegmentTogglesItInMultiMode)
	{
		segmented().setSelectionMode(SelectionMode::Multi);
		Segmented::select(_registry, _entity, 1);

		SharedButton::release(_registry, _buttons[0]);
		EXPECT_EQ(Segmented::selectedIndices(_registry, _entity),
				  (std::vector<std::size_t> { 0, 1 }));

		SharedButton::release(_registry, _buttons[0]);
		EXPECT_EQ(Segmented::selectedIndices(_registry, _entity),
				  std::vector<std::size_t> { 1 });
	}

	TEST_F(TestSegmentedButtonSystem, ADisabledSegmentIgnoresThePress)
	{
		int changes = 0;
		segmented().setOnChange([&changes](std::size_t) {
			++changes;
		});

		SharedButton::release(_registry, _buttons[2]);
		EXPECT_FALSE(Segmented::isSelected(_registry, _entity, 2));
		EXPECT_EQ(changes, 0);

		Segmented::setSegmentDisabled(_registry, _entity, 2, false);
		EXPECT_FALSE(segmented().getSegments()[2].disabled);
		SharedButton::release(_registry, _buttons[2]);
		EXPECT_TRUE(Segmented::isSelected(_registry, _entity, 2));
		EXPECT_EQ(changes, 1);
	}

	TEST_F(TestSegmentedButtonSystem, DisablingASegmentKeepsTheCornersConnected)
	{
		_system.update(_entity);

		Segmented::setSegmentDisabled(_registry, _entity, 1, true);

		const auto &middle =
			_registry.getComponent<components::Borders>(_buttons[1]);
		EXPECT_FALSE(_registry.getComponent<components::StyleState>(_buttons[1])
						 .isEnabled());
		EXPECT_FLOAT_EQ(middle.getTopLeftRadius(), 0.0f);
		EXPECT_FLOAT_EQ(middle.getBottomRightRadius(), 0.0f);
	}

	TEST_F(TestSegmentedButtonSystem, AnUnchangedRowIsNotMarkedChanged)
	{
		Segmented::select(_registry, _entity, 1);
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
		for (std::size_t index = 0; index < _buttons.size(); ++index) {
			EXPECT_FALSE(_registry.hasChanged(_buttons[index]));
			EXPECT_FALSE(_registry.hasChanged(_icons[index]));
		}
	}

}	 // namespace guillaume::systems::tests
