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
#include "systems/test_radio_button.hpp"

#include <guillaume/components/accessibility.hpp>
#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Variant = components::RadioButton::Variant;
	}	 // namespace

	TEST_F(TestRadioButtonSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestRadioButtonSystem, PressSelectsAndCallsTheHandler)
	{
		int presses = 0;
		_registry.getComponent<components::RadioButton>(_entity).setOnPress(
			[&presses]() {
				++presses;
			});

		guillaume::systems::RadioButton::press(_registry, _entity);

		EXPECT_TRUE(_registry.getComponent<components::Selectable>(_entity)
						.isSelected());
		EXPECT_EQ(presses, 1);
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_entity)
					  .getChecked(),
				  components::CheckedState::Checked);
	}

	TEST_F(TestRadioButtonSystem, DisabledVariantIgnoresPress)
	{
		_registry.getComponent<components::RadioButton>(_entity).setVariant(
			Variant::Disabled);
		_system.update(_entity);

		guillaume::systems::RadioButton::press(_registry, _entity);

		EXPECT_FALSE(_registry.getComponent<components::Selectable>(_entity)
						 .isSelected());
	}

	TEST_F(TestRadioButtonSystem, DotIsVisibleAndCenteredOnlyWhenSelected)
	{
		_system.update(_entity);
		EXPECT_EQ(_registry.getComponent<components::Color>(_mark).getColor(),
				  entities::transparentColor());

		guillaume::systems::RadioButton::select(_registry, _entity, true);

		EXPECT_EQ(_registry.getComponent<components::Color>(_mark).getColor(),
				  getActiveColor(SchemeColorRole::Primary));
		EXPECT_FLOAT_EQ(offsetX(_mark), (20.0f - 10.0f) / 2.0f);
	}

	TEST_F(TestRadioButtonSystem, SelectionDrivenThroughTheComponentRestyles)
	{
		_system.update(_entity);

		// A selection group writes Selectable directly; the wired handler
		// must restyle without anyone calling the system.
		_registry.getComponent<components::Selectable>(_entity).setSelected(
			true);

		EXPECT_EQ(_registry.getComponent<components::Color>(_entity).getColor(),
				  getActiveColor(SchemeColorRole::Primary));
	}

	TEST_F(TestRadioButtonSystem, LabelIsPlacedBesideTheTouchTarget)
	{
		_system.update(_entity);

		EXPECT_FLOAT_EQ(offsetX(_label),
						(20.0f + components::RadioButton::TouchTarget) / 2.0f
							+ components::RadioButton::LabelGap);
	}

	TEST_F(TestRadioButtonSystem, AnUnchangedRadioIsNotMarkedChanged)
	{
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
		EXPECT_FALSE(_registry.hasChanged(_mark));
	}

}	 // namespace guillaume::systems::tests
