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

#include "systems/test_radio_group.hpp"

#include <stdexcept>

#include <guillaume/components/accessibility.hpp>

namespace guillaume::systems::tests
{
	using GroupSystem = guillaume::systems::RadioGroup;

	TEST_F(TestRadioGroupSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestRadioGroupSystem, InstallPublishesTheRadioGroupRole)
	{
		const auto &accessibility =
			_registry.getComponent<components::Accessibility>(_entity);

		EXPECT_EQ(accessibility.getRole(),
				  components::AccessibilityRole::RadioGroup);
		EXPECT_FALSE(accessibility.isFocusable());
	}

	TEST_F(TestRadioGroupSystem, OnlyTheValueIsSelected)
	{
		group().setValue(1);
		// A stale selection written behind the group's back is cleared.
		_registry.getComponent<components::Selectable>(_radios[2])
			.setSelected(true);

		_system.update(_entity);

		EXPECT_FALSE(isSelected(0));
		EXPECT_TRUE(isSelected(1));
		EXPECT_FALSE(isSelected(2));
		EXPECT_TRUE(
			_registry.getComponent<components::Accessibility>(_radios[1])
				.isSelected());
	}

	TEST_F(TestRadioGroupSystem, NoValueSelectsNothing)
	{
		_system.update(_entity);

		EXPECT_FALSE(isSelected(0));
		EXPECT_FALSE(isSelected(1));
		EXPECT_FALSE(isSelected(2));
	}

	TEST_F(TestRadioGroupSystem, SelectSetsTheValueWithoutTheHandler)
	{
		int calls = 0;
		group().setOnValueChange([&calls](std::size_t) {
			++calls;
		});

		GroupSystem::select(_registry, _entity, 2);

		EXPECT_EQ(group().getValue(), 2u);
		EXPECT_TRUE(isSelected(2));
		EXPECT_EQ(calls, 0);
	}

	TEST_F(TestRadioGroupSystem, SelectRejectsAnUnknownOption)
	{
		EXPECT_THROW(GroupSystem::select(_registry, _entity, 3),
					 std::out_of_range);
		EXPECT_FALSE(group().getValue().has_value());
	}

	TEST_F(TestRadioGroupSystem, PressCallsTheHandlerOnlyOnAChange)
	{
		std::vector<std::size_t> reported;
		group().setOnValueChange([&reported](std::size_t index) {
			reported.push_back(index);
		});

		GroupSystem::press(_registry, _entity, 1);
		GroupSystem::press(_registry, _entity, 1);
		GroupSystem::press(_registry, _entity, 7);

		ASSERT_EQ(reported.size(), 1u);
		EXPECT_EQ(reported[0], 1u);
		EXPECT_EQ(group().getValue(), 1u);
	}

	TEST_F(TestRadioGroupSystem, PressingARadioButtonReachesTheGroup)
	{
		std::size_t reported = 99;
		group().setValue(0).setOnValueChange([&reported](std::size_t index) {
			reported = index;
		});
		_system.update(_entity);

		// The press handler wired by install() carries the option index.
		guillaume::systems::RadioButton::press(_registry, _radios[2]);

		EXPECT_EQ(reported, 2u);
		EXPECT_EQ(group().getValue(), 2u);
		EXPECT_FALSE(isSelected(0));
		EXPECT_TRUE(isSelected(2));
	}

	TEST_F(TestRadioGroupSystem, RadioButtonsAreStackedByTheLayout)
	{
		_system.update(_entity);

		EXPECT_EQ(
			_registry.getComponent<components::Layout>(_entity).getChildren(),
			_radios);
		EXPECT_FLOAT_EQ(positionY(1) - positionY(0),
						20.0f + components::RadioGroup::RowSpacing);
		EXPECT_FLOAT_EQ(positionY(2) - positionY(1),
						20.0f + components::RadioGroup::RowSpacing);
	}

	TEST_F(TestRadioGroupSystem, AnUnchangedGroupIsNotMarkedChanged)
	{
		group().setValue(1);
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
		for (const auto radio: _radios) {
			EXPECT_FALSE(_registry.hasChanged(radio));
		}
	}

}	 // namespace guillaume::systems::tests
