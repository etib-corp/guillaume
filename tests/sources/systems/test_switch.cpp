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
#include "systems/test_switch.hpp"

#include <guillaume/components/accessibility.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Variant = components::Switch::Variant;
	}	 // namespace

	TEST_F(TestSwitchSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestSwitchSystem, ToggleFlipsTheValueAndReportsIt)
	{
		bool reported = false;
		_registry.getComponent<components::Switch>(_entity).setOnValueChange(
			[&reported](bool checked) {
				reported = checked;
			});

		guillaume::systems::Switch::toggle(_registry, _entity);

		EXPECT_TRUE(guillaume::systems::Switch::isChecked(_registry, _entity));
		EXPECT_TRUE(reported);
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_entity)
					  .getChecked(),
				  components::CheckedState::Checked);
	}

	TEST_F(TestSwitchSystem, DisabledVariantIgnoresToggle)
	{
		_registry.getComponent<components::Switch>(_entity).setVariant(
			Variant::Disabled);
		_system.update(_entity);

		guillaume::systems::Switch::toggle(_registry, _entity);

		EXPECT_FALSE(guillaume::systems::Switch::isChecked(_registry, _entity));
	}

	TEST_F(TestSwitchSystem, ThumbSlidesAcrossTheTrack)
	{
		_system.update(_entity);
		EXPECT_FLOAT_EQ(offsetX(_mark), components::Switch::ThumbInset);

		guillaume::systems::Switch::setChecked(_registry, _entity, true);

		EXPECT_FLOAT_EQ(offsetX(_mark),
						52.0f - 10.0f - components::Switch::ThumbInset);
	}

	TEST_F(TestSwitchSystem, TrackAndThumbFollowTheValue)
	{
		guillaume::systems::Switch::setChecked(_registry, _entity, true);

		EXPECT_EQ(_registry.getComponent<components::Color>(_entity).getColor(),
				  getActiveColor(SchemeColorRole::Primary));
		EXPECT_EQ(_registry.getComponent<components::Color>(_mark).getColor(),
				  getActiveColor(SchemeColorRole::OnPrimary));
	}

	TEST_F(TestSwitchSystem, LabelIsPlacedBesideTheTrack)
	{
		_system.update(_entity);

		EXPECT_FLOAT_EQ(offsetX(_label), 52.0f + components::Switch::LabelGap);
	}

	TEST_F(TestSwitchSystem, AnUnchangedSwitchIsNotMarkedChanged)
	{
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
		EXPECT_FALSE(_registry.hasChanged(_mark));
	}

}	 // namespace guillaume::systems::tests
