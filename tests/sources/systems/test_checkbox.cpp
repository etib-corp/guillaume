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
#include "systems/test_checkbox.hpp"

#include <guillaume/components/accessibility.hpp>
#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using State	  = components::Checkbox::State;
		using Variant = components::Checkbox::Variant;
	}	 // namespace

	TEST_F(TestCheckboxSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestCheckboxSystem, ToggleFlipsTheStateAndReportsIt)
	{
		State reported = State::Indeterminate;
		_registry.getComponent<components::Checkbox>(_entity).setOnValueChange(
			[&reported](State state) {
				reported = state;
			});

		guillaume::systems::Checkbox::toggle(_registry, _entity);

		EXPECT_EQ(
			_registry.getComponent<components::Checkbox>(_entity).getState(),
			State::Checked);
		EXPECT_TRUE(_registry.getComponent<components::Selectable>(_entity)
						.isSelected());
		EXPECT_EQ(reported, State::Checked);
	}

	TEST_F(TestCheckboxSystem, DisabledVariantIgnoresToggle)
	{
		_registry.getComponent<components::Checkbox>(_entity).setVariant(
			Variant::Disabled);
		_system.update(_entity);

		guillaume::systems::Checkbox::toggle(_registry, _entity);

		EXPECT_TRUE(
			guillaume::systems::Checkbox::isDisabled(_registry, _entity));
		EXPECT_EQ(
			_registry.getComponent<components::Checkbox>(_entity).getState(),
			State::Unchecked);
	}

	TEST_F(TestCheckboxSystem, CheckGlyphIsOnlyVisibleWhenChecked)
	{
		_system.update(_entity);
		EXPECT_EQ(_registry.getComponent<components::Color>(_mark).getColor(),
				  entities::transparentColor());

		guillaume::systems::Checkbox::setState(_registry, _entity,
											   State::Checked);
		EXPECT_EQ(_registry.getComponent<components::Color>(_mark).getColor(),
				  getActiveColor(SchemeColorRole::OnPrimary));
	}

	TEST_F(TestCheckboxSystem, IndeterminateDrawsTheDashAndIsMixed)
	{
		guillaume::systems::Checkbox::setState(_registry, _entity,
											   State::Indeterminate);

		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Line>(_entity).getThickness(),
			2.0f);
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_entity)
					  .getChecked(),
				  components::CheckedState::Mixed);
	}

	TEST_F(TestCheckboxSystem, LabelIsPlacedBesideTheTouchTarget)
	{
		_system.update(_entity);

		EXPECT_FLOAT_EQ(offsetX(_label),
						(20.0f + components::Checkbox::TouchTarget) / 2.0f
							+ components::Checkbox::LabelGap);
	}

	TEST_F(TestCheckboxSystem, ErrorVariantColorsTheLabel)
	{
		_registry.getComponent<components::Checkbox>(_entity).setVariant(
			Variant::Error);

		_system.update(_entity);

		EXPECT_EQ(_registry.getComponent<components::Color>(_label).getColor(),
				  getActiveColor(SchemeColorRole::Error));
	}

	TEST_F(TestCheckboxSystem, AnUnchangedCheckboxIsNotMarkedChanged)
	{
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
		EXPECT_FALSE(_registry.hasChanged(_label));
	}

}	 // namespace guillaume::systems::tests
