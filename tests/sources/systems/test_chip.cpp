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

#include "systems/test_chip.hpp"

#include <guillaume/components/accessibility.hpp>
#include <guillaume/components/mouse_hover_interaction.hpp>
#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Mode	  = components::Chip::Mode;
		using Variant = components::Chip::Variant;
		using Ids	  = std::vector<ecs::Entity::Identifier>;
	}	 // namespace

	TEST_F(TestChipSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestChipSystem, FlatChipSitsOnSecondaryContainerWithoutStroke)
	{
		_system.update(_entity);

		EXPECT_EQ(colorOf(_entity),
				  getActiveColor(SchemeColorRole::SecondaryContainer));
		EXPECT_EQ(colorOf(_label),
				  getActiveColor(SchemeColorRole::OnSecondaryContainer));
		EXPECT_EQ(colorOf(_leading), getActiveColor(SchemeColorRole::Primary));
		EXPECT_FALSE(
			_registry.getComponent<components::Borders>(_entity).hasStroke());
	}

	TEST_F(TestChipSystem, OutlinedChipIsTransparentUntilSelected)
	{
		chip().setMode(Mode::Outlined);
		_system.update(_entity);

		const auto &borders =
			_registry.getComponent<components::Borders>(_entity);
		EXPECT_EQ(colorOf(_entity).getAlpha(), 0);
		EXPECT_EQ(colorOf(_label),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
		EXPECT_EQ(borders.getColor(), getActiveColor(SchemeColorRole::Outline));
		EXPECT_FLOAT_EQ(borders.getWidth(), 1.0f);

		guillaume::systems::Chip::setSelected(_registry, _entity, true);
		EXPECT_EQ(colorOf(_entity),
				  getActiveColor(SchemeColorRole::SecondaryContainer));
		EXPECT_FALSE(borders.hasStroke());
	}

	TEST_F(TestChipSystem, PressTogglesAFilterChipAndReportsIt)
	{
		int presses = 0;
		chip().setOnPress([&presses]() {
			++presses;
		});

		guillaume::systems::Chip::press(_registry, _entity);
		EXPECT_TRUE(guillaume::systems::Chip::isSelected(_registry, _entity));
		EXPECT_EQ(presses, 1);

		guillaume::systems::Chip::press(_registry, _entity);
		EXPECT_FALSE(guillaume::systems::Chip::isSelected(_registry, _entity));
		EXPECT_EQ(presses, 2);
	}

	TEST_F(TestChipSystem, PressDoesNotSelectOtherVariants)
	{
		int presses = 0;
		chip().setVariant(Variant::Assist).setOnPress([&presses]() {
			++presses;
		});

		guillaume::systems::Chip::press(_registry, _entity);

		EXPECT_FALSE(guillaume::systems::Chip::isSelected(_registry, _entity));
		EXPECT_EQ(presses, 1);
	}

	TEST_F(TestChipSystem, InputReachesPressAndClose)
	{
		int presses = 0;
		int closes	= 0;
		chip()
			.setOnPress([&presses]() {
				++presses;
			})
			.setOnClose([&closes]() {
				++closes;
			});

		click(_entity);
		EXPECT_EQ(presses, 1);
		EXPECT_EQ(closes, 0);
		EXPECT_TRUE(guillaume::systems::Chip::isSelected(_registry, _entity));
		EXPECT_EQ(laidOut(), (Ids { _check, _label, _trailing }));

		click(_trailing);
		EXPECT_EQ(presses, 1);
		EXPECT_EQ(closes, 1);
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_trailing)
					  .getLabel(),
				  "Close");
	}

	TEST_F(TestChipSystem, DisabledChipIgnoresPressAndClose)
	{
		int calls = 0;
		chip()
			.setOnPress([&calls]() {
				++calls;
			})
			.setOnClose([&calls]() {
				++calls;
			});

		guillaume::systems::Chip::setDisabled(_registry, _entity, true);
		guillaume::systems::Chip::press(_registry, _entity);
		guillaume::systems::Chip::close(_registry, _entity);
		click(_entity);
		click(_trailing);

		EXPECT_TRUE(guillaume::systems::Chip::isDisabled(_registry, _entity));
		EXPECT_FALSE(guillaume::systems::Chip::isSelected(_registry, _entity));
		EXPECT_EQ(calls, 0);
		EXPECT_EQ(colorOf(_entity), entities::disabledContainerColor());
		EXPECT_EQ(colorOf(_label), entities::disabledContentColor());

		guillaume::systems::Chip::setDisabled(_registry, _entity, false);
		guillaume::systems::Chip::close(_registry, _entity);
		EXPECT_EQ(calls, 1);
	}

	TEST_F(TestChipSystem, CheckReplacesTheLeadingIconWhileSelected)
	{
		_system.update(_entity);
		EXPECT_EQ(laidOut(), (Ids { _leading, _label, _trailing }));
		EXPECT_EQ(colorOf(_check), entities::transparentColor());

		guillaume::systems::Chip::setSelected(_registry, _entity, true);
		EXPECT_EQ(laidOut(), (Ids { _check, _label, _trailing }));
		EXPECT_EQ(colorOf(_check),
				  getActiveColor(SchemeColorRole::OnSecondaryContainer));

		chip().setShowSelectedCheck(false);
		_system.update(_entity);
		EXPECT_EQ(laidOut(), (Ids { _leading, _label, _trailing }));
		EXPECT_EQ(colorOf(_check), entities::transparentColor());
	}

	TEST_F(TestChipSystem, LaidOutChildrenResolveTheChipWidth)
	{
		_system.update(_entity);

		// Three 18 dp children, two 8 dp gaps and 12 dp padding on each side.
		const auto &bound = _registry.getComponent<components::Bound>(_entity);
		EXPECT_FLOAT_EQ(bound.getWidth(), 3.0f * 18.0f + 2.0f * 8.0f + 24.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 32.0f);

		chip().setCompact(true);
		_system.update(_entity);
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Layout>(_entity).getPadding(),
			components::Chip::CompactPadding);
		EXPECT_FLOAT_EQ(bound.getWidth(), 3.0f * 18.0f + 2.0f * 8.0f + 16.0f);
	}

	TEST_F(TestChipSystem, AccessibilityFollowsTheLabelAndSelection)
	{
		_system.update(_entity);
		const auto &accessibility =
			_registry.getComponent<components::Accessibility>(_entity);
		EXPECT_EQ(accessibility.getRole(),
				  components::AccessibilityRole::Button);
		EXPECT_EQ(accessibility.getLabel(), "Tag");
		EXPECT_FALSE(accessibility.isSelected());

		chip().setLabel("Other");
		guillaume::systems::Chip::setSelected(_registry, _entity, true);
		EXPECT_EQ(accessibility.getLabel(), "Other");
		EXPECT_TRUE(accessibility.isSelected());
	}

	TEST_F(TestChipSystem, ElevatedCastsALevelOneShadowUnlessDisabled)
	{
		chip().setElevated(true);
		_system.update(_entity);
		const auto &elevation =
			_registry.getComponent<components::Elevation>(_entity);
		EXPECT_EQ(elevation.getElevationLevel(), theme::ElevationLevel::Level1);

		guillaume::systems::Chip::setDisabled(_registry, _entity, true);
		EXPECT_EQ(elevation.getElevationLevel(), theme::ElevationLevel::Level0);
	}

	TEST_F(TestChipSystem, HoverAndSelectedOverlayAddAStateLayer)
	{
		_system.update(_entity);
		const auto rest = colorOf(_entity);

		_registry.getComponent<components::MouseHoverInteraction>(_entity)
			.setHovered(true);
		_system.update(_entity);
		EXPECT_NE(colorOf(_entity), rest);

		_registry.getComponent<components::MouseHoverInteraction>(_entity)
			.setHovered(false);
		chip().setShowSelectedOverlay(true);
		guillaume::systems::Chip::setSelected(_registry, _entity, true);
		EXPECT_NE(colorOf(_entity), rest);
	}

	TEST_F(TestChipSystem, VariantDecidesWhichChildrenAChipNeeds)
	{
		EXPECT_TRUE(
			guillaume::systems::Chip::needsCheckIcon(_registry, _entity));
		EXPECT_FALSE(
			guillaume::systems::Chip::needsCloseIcon(_registry, _entity));
		EXPECT_EQ(guillaume::systems::Chip::leadingGlyph(_registry, _entity),
				  "");

		chip().setOnClose([]() {
		});
		EXPECT_TRUE(
			guillaume::systems::Chip::needsCloseIcon(_registry, _entity));

		chip().setVariant(Variant::Input).setOnClose({});
		EXPECT_FALSE(
			guillaume::systems::Chip::needsCheckIcon(_registry, _entity));
		EXPECT_TRUE(
			guillaume::systems::Chip::needsCloseIcon(_registry, _entity));

		chip().setVariant(Variant::Assist);
		EXPECT_EQ(guillaume::systems::Chip::leadingGlyph(_registry, _entity),
				  components::Chip::DefaultAssistGlyph);
		chip().setIconGlyph("star");
		EXPECT_EQ(guillaume::systems::Chip::leadingGlyph(_registry, _entity),
				  "star");
	}

	TEST_F(TestChipSystem, AnUnchangedChipIsNotMarkedChanged)
	{
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
		for (const auto child: { _leading, _check, _label, _trailing }) {
			EXPECT_FALSE(_registry.hasChanged(child));
		}
	}

}	 // namespace guillaume::systems::tests
