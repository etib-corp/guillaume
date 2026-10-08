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

#include <memory>
#include <string>

#include <guillaume/components/borders.hpp>
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
		EXPECT_FLOAT_EQ(bound.getWidth(), 280.0f);
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
		EXPECT_GT(borders.getColor().getAlpha(), 0);
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

}	 // namespace guillaume::entities::tests
