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

#include "systems/test_text_field.hpp"

#include <guillaume/components/accessibility.hpp>
#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Configuration = components::TextFieldConfiguration;
		using Variant		= Configuration::Variant;
		using Ids			= std::vector<ecs::Entity::Identifier>;
		using Field			= guillaume::systems::TextField;
	}	 // namespace

	TEST_F(TestTextFieldSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestTextFieldSystem, FilledSitsOnAContainerWithABottomIndicator)
	{
		_system.update(_entity);

		const auto &line = _registry.getComponent<components::Line>(_entity);
		EXPECT_FLOAT_EQ(line.getThickness(), 1.0f);
		EXPECT_EQ(line.getPlacement(), components::Line::Placement::Bottom);
		ASSERT_TRUE(line.hasColor());
		EXPECT_EQ(line.getColor(),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
		EXPECT_EQ(colorOf(_entity),
				  getActiveColor(SchemeColorRole::SurfaceContainerHighest));
		EXPECT_FALSE(
			_registry.getComponent<components::Borders>(_entity).hasStroke());
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getHeight(),
			Field::Height);
	}

	TEST_F(TestTextFieldSystem, OutlinedIsTransparentWithAnOutline)
	{
		configuration().setVariant(Variant::Outlined);
		_system.update(_entity);

		const auto &borders =
			_registry.getComponent<components::Borders>(_entity);
		EXPECT_EQ(colorOf(_entity).getAlpha(), 0);
		EXPECT_FLOAT_EQ(borders.getWidth(), 1.0f);
		EXPECT_EQ(borders.getColor(), getActiveColor(SchemeColorRole::Outline));
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Line>(_entity).getThickness(),
			0.0f);
	}

	TEST_F(TestTextFieldSystem, RestingFieldShowsOnlyTheLabel)
	{
		_system.update(_entity);

		EXPECT_FALSE(Field::isLabelFloating(_registry, _entity));
		EXPECT_FALSE(field().isLabelFloating());
		EXPECT_EQ(laidOut(_entity), (Ids { _leading, _column, _trailing }));
		EXPECT_EQ(laidOut(_column), (Ids { _label }));
		EXPECT_EQ(contentOf(_label), "Name");
		EXPECT_EQ(colorOf(_label),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Text>(_label).getFontSize(),
			getTypeToken(theme::TypeRole::BodyLarge).fontSize);
		EXPECT_EQ(colorOf(_prefix).getAlpha(), 0);
		EXPECT_EQ(colorOf(_suffix).getAlpha(), 0);
		EXPECT_EQ(field().getLabel(), "Name");
	}

	TEST_F(TestTextFieldSystem, PopulatedFieldFloatsTheLabelAndShowsAffixes)
	{
		Field::setText(_registry, _entity, "value");

		EXPECT_TRUE(Field::isLabelFloating(_registry, _entity));
		EXPECT_TRUE(field().isLabelFloating());
		EXPECT_EQ(contentOf(_input), "value");
		EXPECT_EQ(laidOut(_entity),
				  (Ids { _leading, _prefix, _column, _suffix, _trailing }));
		EXPECT_EQ(laidOut(_column), (Ids { _label, _input }));
		EXPECT_EQ(contentOf(_prefix), "$");
		EXPECT_EQ(contentOf(_suffix), "kg");
		EXPECT_EQ(colorOf(_prefix),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Text>(_label).getFontSize(),
			getTypeToken(theme::TypeRole::BodySmall).fontSize);
		EXPECT_EQ(colorOf(_input), getActiveColor(SchemeColorRole::OnSurface));
	}

	TEST_F(TestTextFieldSystem, FocusFloatsTheLabelAndThickensTheIndicator)
	{
		focus(true);
		_system.update(_entity);

		const auto &line = _registry.getComponent<components::Line>(_entity);
		EXPECT_TRUE(Field::isFocused(_registry, _entity));
		EXPECT_TRUE(field().isLabelFloating());
		EXPECT_FLOAT_EQ(line.getThickness(), 2.0f);
		EXPECT_EQ(line.getColor(), getActiveColor(SchemeColorRole::Primary));
		EXPECT_EQ(colorOf(_label), getActiveColor(SchemeColorRole::Primary));

		configuration().setVariant(Variant::Outlined);
		_system.update(_entity);

		const auto &borders =
			_registry.getComponent<components::Borders>(_entity);
		EXPECT_FLOAT_EQ(borders.getWidth(), 2.0f);
		EXPECT_EQ(borders.getColor(), getActiveColor(SchemeColorRole::Primary));
	}

	TEST_F(TestTextFieldSystem, OutlinedFloatingLabelSitsOnTheOutline)
	{
		configuration().setVariant(Variant::Outlined);
		Field::setText(_registry, _entity, "value");

		// The label leaves the column and is placed over the top edge: it is
		// raised above it (y points up).
		EXPECT_EQ(laidOut(_column), (Ids { _input }));
		EXPECT_GT(yOf(_label), yOf(_entity));
	}

	TEST_F(TestTextFieldSystem, SupportingTextSitsBelowTheField)
	{
		_system.update(_entity);

		EXPECT_EQ(contentOf(_supporting), "Support");
		// Below means a smaller y (y points up).
		EXPECT_FLOAT_EQ(yOf(_entity) - yOf(_supporting),
						Field::Height + Field::SupportingGap);
		EXPECT_EQ(colorOf(_supporting),
				  getActiveColor(SchemeColorRole::OnSurfaceVariant));
	}

	TEST_F(TestTextFieldSystem, ErrorShowsTheErrorTextAndIcon)
	{
		field().setErrorText("Required").setError(true);
		_system.update(_entity);

		const auto error = getActiveColor(SchemeColorRole::Error);
		EXPECT_EQ(Field::supportingText(_registry, _entity), "Required");
		EXPECT_EQ(contentOf(_supporting), "Required");
		EXPECT_EQ(colorOf(_supporting), error);
		EXPECT_EQ(colorOf(_errorIcon), error);
		EXPECT_EQ(colorOf(_trailing).getAlpha(), 0);
		EXPECT_EQ(_registry.getComponent<components::Line>(_entity).getColor(),
				  error);
		// The error icon replaces the trailing icon in the row.
		EXPECT_EQ(laidOut(_entity), (Ids { _leading, _column, _errorIcon }));

		field().setError(false);
		_system.update(_entity);
		EXPECT_EQ(contentOf(_supporting), "Support");
		EXPECT_EQ(colorOf(_errorIcon).getAlpha(), 0);
		EXPECT_EQ(laidOut(_entity), (Ids { _leading, _column, _trailing }));
	}

	TEST_F(TestTextFieldSystem, ErrorWithoutErrorTextKeepsTheSupportingText)
	{
		field().setError(true);
		_system.update(_entity);

		EXPECT_EQ(contentOf(_supporting), "Support");
		EXPECT_EQ(colorOf(_supporting), getActiveColor(SchemeColorRole::Error));
	}

	TEST_F(TestTextFieldSystem, WidthFollowsTheContentUnlessForced)
	{
		Field::setText(_registry, _entity, "value");

		// Five row children: two icons, two affixes and the column.
		const float intrinsic = 2.0f * Field::Padding + 2.0f * 24.0f
			+ 3.0f * 40.0f + 4.0f * Field::Spacing;
		const auto &bound = _registry.getComponent<components::Bound>(_entity);
		EXPECT_FLOAT_EQ(bound.getWidth(), intrinsic);

		configuration().setWidth(400.0f);
		_system.update(_entity);
		EXPECT_FLOAT_EQ(bound.getWidth(), 400.0f);

		configuration().setWidth(40.0f);
		_system.update(_entity);
		EXPECT_FLOAT_EQ(bound.getWidth(), Configuration::MinWidth);
	}

	TEST_F(TestTextFieldSystem, DenseAndMultilineDriveTheHeight)
	{
		const auto &layout =
			_registry.getComponent<components::Layout>(_entity);

		configuration().setDense(true);
		_system.update(_entity);
		EXPECT_FLOAT_EQ(layout.getFixedHeight(), Field::DenseLabelHeight);

		field().setPlaceholder("");
		_system.update(_entity);
		EXPECT_FLOAT_EQ(layout.getFixedHeight(), Field::DenseHeight);

		configuration().setMultiline(true);
		_system.update(_entity);
		EXPECT_FALSE(layout.hasFixedHeight());
	}

	TEST_F(TestTextFieldSystem, InputIsEditableOnlyWhileFocusedAndEnabled)
	{
		_system.update(_entity);
		EXPECT_FALSE(_registry.hasComponent<components::Focus>(_input));

		focus(true);
		_system.update(_entity);
		EXPECT_TRUE(_registry.hasComponent<components::Focus>(_input));

		configuration().setEditable(false);
		Field::applyEditing(_registry, _entity);
		EXPECT_FALSE(_registry.hasComponent<components::Focus>(_input));

		configuration().setEditable(true);
		Field::applyEditing(_registry, _entity);
		EXPECT_TRUE(_registry.hasComponent<components::Focus>(_input));

		Field::setDisabled(_registry, _entity, true);
		EXPECT_TRUE(Field::isDisabled(_registry, _entity));
		EXPECT_FALSE(_registry.hasComponent<components::Focus>(_input));

		Field::setDisabled(_registry, _entity, false);
		focus(false);
		_system.update(_entity);
		EXPECT_FALSE(_registry.hasComponent<components::Focus>(_input));
	}

	TEST_F(TestTextFieldSystem, DisabledFieldUsesTheDisabledColors)
	{
		Field::setText(_registry, _entity, "value");
		Field::setDisabled(_registry, _entity, true);

		const auto disabled = entities::disabledContentColor();
		EXPECT_EQ(colorOf(_input), disabled);
		EXPECT_EQ(colorOf(_label), disabled);
		EXPECT_EQ(colorOf(_leading), disabled);
		EXPECT_EQ(_registry.getComponent<components::Line>(_entity).getColor(),
				  disabled);
		EXPECT_EQ(colorOf(_entity).getAlpha(), 10);

		// A disabled field shows no error color.
		field().setError(true);
		_system.update(_entity);
		EXPECT_EQ(colorOf(_errorIcon).getAlpha(), 0);
		EXPECT_EQ(colorOf(_supporting), disabled);
	}

	TEST_F(TestTextFieldSystem, EditsAreReportedOnceAndSetTextIsSilent)
	{
		int calls = 0;
		std::string reported;
		configuration().setOnChange(
			[&calls, &reported](const std::string &text) {
				++calls;
				reported = text;
			});

		Field::setText(_registry, _entity, "typed by code");
		EXPECT_EQ(calls, 0);
		EXPECT_EQ(Field::text(_registry, _entity), "typed by code");

		type("edited");
		_system.update(_entity);
		EXPECT_EQ(calls, 1);
		EXPECT_EQ(reported, "edited");
		EXPECT_EQ(configuration().getText(), "edited");

		_system.update(_entity);
		EXPECT_EQ(calls, 1);
	}

	TEST_F(TestTextFieldSystem, TextFallsBackToTheLastKnownOneWithoutInput)
	{
		configuration()
			.setInputEntity(ecs::Entity::InvalidIdentifier)
			.setText("pending");

		EXPECT_EQ(Field::text(_registry, _entity), "pending");
		EXPECT_TRUE(Field::isLabelFloating(_registry, _entity));
	}

	TEST_F(TestTextFieldSystem, FocusChangesReachTheCallbacks)
	{
		int gained = 0;
		int lost   = 0;
		configuration()
			.setOnFocus([&gained]() {
				++gained;
			})
			.setOnBlur([&lost]() {
				++lost;
			});

		const auto &focusComponent =
			_registry.getComponent<components::Focus>(_entity);
		focusComponent.getOnFocusGainedHandler()();
		EXPECT_EQ(gained, 1);
		EXPECT_EQ(lost, 0);

		focusComponent.getOnFocusLostHandler()();
		EXPECT_EQ(lost, 1);
	}

	TEST_F(TestTextFieldSystem, ExposesTextFieldSemantics)
	{
		_system.update(_entity);

		const auto &accessibility =
			_registry.getComponent<components::Accessibility>(_entity);
		EXPECT_EQ(accessibility.getRole(),
				  components::AccessibilityRole::TextField);
		EXPECT_EQ(accessibility.getLabel(), "Name");
		EXPECT_EQ(accessibility.getHint(), "Support");
	}

	TEST_F(TestTextFieldSystem, FieldWithoutChildrenIsStillStyled)
	{
		configuration()
			.setLeadingIconEntity(ecs::Entity::InvalidIdentifier)
			.setTrailingIconEntity(ecs::Entity::InvalidIdentifier)
			.setErrorIconEntity(ecs::Entity::InvalidIdentifier)
			.setInputColumn(ecs::Entity::InvalidIdentifier)
			.setLabelEntity(ecs::Entity::InvalidIdentifier)
			.setInputEntity(ecs::Entity::InvalidIdentifier)
			.setPrefixEntity(ecs::Entity::InvalidIdentifier)
			.setSuffixEntity(ecs::Entity::InvalidIdentifier)
			.setSupportingEntity(ecs::Entity::InvalidIdentifier);

		_system.update(_entity);

		EXPECT_TRUE(laidOut(_entity).empty());
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getWidth(),
			Configuration::MinWidth);
	}

	TEST_F(TestTextFieldSystem, AnUnchangedFieldIsNotMarkedChanged)
	{
		const Ids everyone { _entity, _leading,	  _trailing, _errorIcon,
							 _column, _label,	  _input,	 _prefix,
							 _suffix, _supporting };

		_system.update(_entity);
		_registry.resetChangedFlags();
		_system.update(_entity);
		for (const auto identifier: everyone) {
			EXPECT_FALSE(_registry.hasChanged(identifier)) << identifier;
		}

		// The same holds for a focused, populated, outlined field in error.
		configuration().setVariant(Variant::Outlined);
		field().setError(true);
		focus(true);
		Field::setText(_registry, _entity, "value");
		_registry.resetChangedFlags();
		_system.update(_entity);
		for (const auto identifier: everyone) {
			EXPECT_FALSE(_registry.hasChanged(identifier)) << identifier;
		}
	}

}	 // namespace guillaume::systems::tests
