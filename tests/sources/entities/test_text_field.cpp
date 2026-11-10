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

#include "entities/test_text_field.hpp"

#include <guillaume/entities/style_helpers.hpp>

#include <guillaume/components/text.hpp>

#include <guillaume/components/transform.hpp>

#include <guillaume/components/layout.hpp>

#include <guillaume/components/focus.hpp>

#include <algorithm>

#include <guillaume/components/style_state.hpp>

#include <guillaume/components/mouse_hover_interaction.hpp>

#include <guillaume/components/mouse_button_interaction.hpp>

#include <guillaume/components/accessibility.hpp>

#include <memory>
#include <string>

#include <guillaume/components/borders.hpp>
#include <guillaume/theme.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/line.hpp>
#include <guillaume/components/text_field.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<TextField> makeTextField(
			ecs::ComponentRegistry &registry,
			TextField::Variant variant = TextField::Variant::Filled)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			TextField::Builder builder(registry, entityRegistry);
			TextField::Director director;

			return director.makeTextField(builder, nullptr, variant, "Name",
										  "person", "clear", "Support", false,
										  "value");
		}
	}	 // namespace

	TEST_F(TestTextField, DefaultGeometry)
	{
		ecs::ComponentRegistry registry;
		auto field = makeTextField(registry);

		field->initialize();
		field->update();

		const auto &bound =
			registry.getComponent<components::Bound>(field->getIdentifier());
		EXPECT_GE(bound.getWidth(), TextField::MinWidth);
		EXPECT_FLOAT_EQ(bound.getHeight(), 56.0f);
	}

	TEST_F(TestTextField, FilledUsesBottomLine)
	{
		ecs::ComponentRegistry registry;
		auto field = makeTextField(registry, TextField::Variant::Filled);

		field->initialize();
		field->update();

		const auto &line =
			registry.getComponent<components::Line>(field->getIdentifier());
		EXPECT_FLOAT_EQ(line.getThickness(), 1.0f);
		EXPECT_EQ(line.getPlacement(), components::Line::Placement::Bottom);
		ASSERT_TRUE(line.hasColor());
		EXPECT_EQ(line.getColor(),
				  getActiveScheme()
					  .getColor(SchemeColorRole::OnSurfaceVariant)
					  .getColor());

		const auto &color =
			registry.getComponent<components::Color>(field->getIdentifier());
		EXPECT_GT(color.getColor().getAlpha(), 0);
	}

	TEST_F(TestTextField, OutlinedUsesFullBorder)
	{
		ecs::ComponentRegistry registry;
		auto field = makeTextField(registry, TextField::Variant::Outlined);

		field->initialize();
		field->update();

		const auto &line =
			registry.getComponent<components::Line>(field->getIdentifier());
		EXPECT_FLOAT_EQ(line.getThickness(), 0.0f);

		const auto &borders =
			registry.getComponent<components::Borders>(field->getIdentifier());
		EXPECT_TRUE(borders.hasStroke());
		EXPECT_FLOAT_EQ(borders.getWidth(), 1.0f);
		EXPECT_EQ(
			borders.getColor(),
			getActiveScheme().getColor(SchemeColorRole::Outline).getColor());
	}

	TEST_F(TestTextField, TextFieldComponentCarriesState)
	{
		ecs::ComponentRegistry registry;
		auto field = makeTextField(registry);

		field->initialize();
		field->setSupportingText("Helper");
		field->setError(true);

		const auto &textField = registry.getComponent<components::TextField>(
			field->getIdentifier());
		EXPECT_EQ(textField.getSupportingText(), "Helper");
		EXPECT_TRUE(textField.isError());
	}

	TEST_F(TestTextField, TextRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto field = makeTextField(registry);

		field->initialize();
		field->setText("hello");

		EXPECT_EQ(field->getText(), "hello");
	}

	TEST_F(TestTextField, ErrorRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto field = makeTextField(registry);

		field->setError(true);
		EXPECT_TRUE(field->isError());
		field->setError(false);
		EXPECT_FALSE(field->isError());
	}

	TEST_F(TestTextField, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto field = makeTextField(registry, TextField::Variant::Filled);

		field->setVariant(TextField::Variant::Outlined);

		EXPECT_EQ(field->getVariant(), TextField::Variant::Outlined);
	}

	TEST_F(TestTextField, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		TextField::Builder builder(registry, entityRegistry);
		TextField::Director director;

		auto field = director.makeTextField(
			builder, parent, TextField::Variant::Outlined, "Name", "person",
			"clear", "Support", true, "value");

		ASSERT_NE(field, nullptr);
		EXPECT_EQ(field->getParent(), parent);
		EXPECT_EQ(field->getVariant(), TextField::Variant::Outlined);
		EXPECT_TRUE(field->isError());
	}

	namespace
	{
		utility::graphic::Color32Bit role(SchemeColorRole value)
		{
			return getActiveScheme().getColor(value).getColor();
		}

		void focusField(ecs::ComponentRegistry &registry,
						const std::shared_ptr<TextField> &field, bool focused)
		{
			registry.getComponent<components::Focus>(field->getIdentifier())
				.setFocused(focused);
			field->update();
		}
	}	 // namespace

	TEST_F(TestTextField, LabelRestsThenFloatsOnFocus)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer &entityRegistry = scratchEntityRegistry();
		TextField::Builder builder(registry, entityRegistry);
		auto field = builder.withPlaceholder("Name").registerEntity(nullptr);
		field->initialize();
		field->update();

		const auto &label =
			registry.getComponent<components::Text>(field->getLabelIdentifier());
		EXPECT_FALSE(field->isLabelFloating());
		EXPECT_FLOAT_EQ(label.getFontSize(),
						getTypeToken(theme::TypeRole::BodyLarge).fontSize);

		focusField(registry, field, true);
		EXPECT_TRUE(field->isFocused());
		EXPECT_TRUE(field->isLabelFloating());
		EXPECT_FLOAT_EQ(label.getFontSize(),
						getTypeToken(theme::TypeRole::BodySmall).fontSize);
		EXPECT_EQ(
			registry.getComponent<components::Color>(field->getLabelIdentifier())
				.getColor(),
			role(SchemeColorRole::Primary));

		const auto &line =
			registry.getComponent<components::Line>(field->getIdentifier());
		EXPECT_FLOAT_EQ(line.getThickness(), 2.0f);
		EXPECT_EQ(line.getColor(), role(SchemeColorRole::Primary));
		EXPECT_TRUE(registry.getComponent<components::TextField>(
							   field->getIdentifier())
						.isLabelFloating());
	}

	TEST_F(TestTextField, OutlinedFocusUsesPrimaryTwoDpOutline)
	{
		ecs::ComponentRegistry registry;
		auto field = makeTextField(registry, TextField::Variant::Outlined);
		field->initialize();
		focusField(registry, field, true);

		const auto &borders =
			registry.getComponent<components::Borders>(field->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getWidth(), 2.0f);
		EXPECT_EQ(borders.getColor(), role(SchemeColorRole::Primary));
	}

	TEST_F(TestTextField, ErrorShowsErrorTextAndIcon)
	{
		ecs::ComponentRegistry registry;
		auto field = makeTextField(registry);
		field->initialize();
		field->setErrorText("Required");
		field->setError(true);
		field->update();

		EXPECT_EQ(registry
					  .getComponent<components::Text>(
						  field->getSupportingIdentifier())
					  .getContent(),
				  "Required");
		EXPECT_EQ(registry
					  .getComponent<components::Color>(
						  field->getSupportingIdentifier())
					  .getColor(),
				  role(SchemeColorRole::Error));
		EXPECT_EQ(registry
					  .getComponent<components::Color>(
						  field->getErrorIconIdentifier())
					  .getColor(),
				  role(SchemeColorRole::Error));
		EXPECT_EQ(
			registry.getComponent<components::Line>(field->getIdentifier())
				.getColor(),
			role(SchemeColorRole::Error));
	}

	TEST_F(TestTextField, SupportingTextSitsBelowTheField)
	{
		ecs::ComponentRegistry registry;
		auto field = makeTextField(registry);
		field->initialize();
		field->update();

		const auto fieldPose =
			registry.getComponent<components::Transform>(field->getIdentifier())
				.getPose();
		const auto supportPose =
			registry
				.getComponent<components::Transform>(
					field->getSupportingIdentifier())
				.getPose();
		EXPECT_GE(supportPose.getPosition().getY()
					  - fieldPose.getPosition().getY(),
				  56.0f);
	}

	TEST_F(TestTextField, InputIsEditableOnlyWhileFocusedAndEnabled)
	{
		ecs::ComponentRegistry registry;
		auto field = makeTextField(registry);
		field->initialize();
		field->update();
		const auto input = field->getInputIdentifier();

		EXPECT_FALSE(registry.hasComponent<components::Focus>(input));
		focusField(registry, field, true);
		EXPECT_TRUE(registry.hasComponent<components::Focus>(input));

		field->setEditable(false);
		EXPECT_FALSE(registry.hasComponent<components::Focus>(input));
		field->setEditable(true);
		EXPECT_TRUE(registry.hasComponent<components::Focus>(input));

		field->setDisabled(true);
		field->update();
		EXPECT_TRUE(field->isDisabled());
		EXPECT_FALSE(registry.hasComponent<components::Focus>(input));
		EXPECT_EQ(registry.getComponent<components::Color>(input).getColor(),
				  disabledContentColor());
	}

	TEST_F(TestTextField, AffixesShowWhileTheLabelFloats)
	{
		ecs::ComponentRegistry registry;
		auto field = makeTextField(registry);
		field->setPrefix("$");
		field->setSuffix("kg");
		field->initialize();
		field->update();

		ASSERT_NE(field->getPrefixIdentifier(), ecs::Entity::InvalidIdentifier);
		ASSERT_NE(field->getSuffixIdentifier(), ecs::Entity::InvalidIdentifier);
		const auto &children =
			registry.getComponent<components::Layout>(field->getIdentifier())
				.getChildren();
		EXPECT_NE(std::find(children.begin(), children.end(),
							field->getPrefixIdentifier()),
				  children.end());

		field->setText("");
		field->update();
		const auto &resting =
			registry.getComponent<components::Layout>(field->getIdentifier())
				.getChildren();
		EXPECT_EQ(std::find(resting.begin(), resting.end(),
							field->getSuffixIdentifier()),
				  resting.end());
	}

	TEST_F(TestTextField, OnChangeReportsEdits)
	{
		ecs::ComponentRegistry registry;
		auto field = makeTextField(registry);
		field->initialize();
		field->update();

		std::string reported;
		field->setOnChange([&reported](const std::string &text) {
			reported = text;
		});
		registry.getComponent<components::Text>(field->getInputIdentifier())
			.setContent("edited");
		field->update();

		EXPECT_EQ(reported, "edited");
		EXPECT_EQ(field->getText(), "edited");
	}

	TEST_F(TestTextField, DenseMultilineAndWidth)
	{
		ecs::ComponentRegistry registry;
		auto field = makeTextField(registry);
		field->initialize();
		field->setDense(true);
		field->update();
		EXPECT_FLOAT_EQ(
			registry.getComponent<components::Bound>(field->getIdentifier())
				.getHeight(),
			52.0f);

		field->setWidth(40.0f);
		field->update();
		EXPECT_FLOAT_EQ(
			registry.getComponent<components::Bound>(field->getIdentifier())
				.getWidth(),
			TextField::MinWidth);

		field->setMultiline(true);
		EXPECT_TRUE(field->isMultiline());
		EXPECT_FALSE(
			registry.getComponent<components::Layout>(field->getIdentifier())
				.hasFixedHeight());
	}

	TEST_F(TestTextField, ExposesTextFieldSemantics)
	{
		ecs::ComponentRegistry registry;
		auto field = makeTextField(registry);
		field->initialize();

		const auto &accessibility =
			registry.getComponent<components::Accessibility>(
				field->getIdentifier());
		EXPECT_EQ(accessibility.getRole(),
				  components::AccessibilityRole::TextField);
		EXPECT_EQ(accessibility.getLabel(), "Name");
		EXPECT_EQ(accessibility.getHint(), "Support");
	}

}	 // namespace guillaume::entities::tests
