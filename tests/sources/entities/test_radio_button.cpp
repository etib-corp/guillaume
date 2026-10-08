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

#include "entities/test_radio_button.hpp"

#include <memory>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/ring.hpp>
#include <guillaume/components/selection.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<RadioButton> makeRadio(
			ecs::ComponentRegistry &registry,
			RadioButton::Variant variant = RadioButton::Variant::Enabled,
			const std::string &label	 = "")
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			RadioButton::Builder builder(registry, entityRegistry);
			RadioButton::Director director;

			return director.makeRadioButton(builder, nullptr, variant, label);
		}
	}	 // namespace

	TEST_F(TestRadioButton, DefaultGeometryIsCircle)
	{
		ecs::ComponentRegistry registry;
		auto radio = makeRadio(registry);

		radio->initialize();
		radio->update();

		const auto &bound =
			registry.getComponent<components::Bound>(radio->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 20.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 20.0f);

		const auto &ring =
			registry.getComponent<components::Ring>(radio->getIdentifier());
		EXPECT_FLOAT_EQ(ring.getThickness(), 2.0f);
	}

	TEST_F(TestRadioButton, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto radio = makeRadio(registry);

		radio->setVariant(RadioButton::Variant::Error);
		EXPECT_EQ(radio->getVariant(), RadioButton::Variant::Error);

		radio->setVariant(RadioButton::Variant::Disabled);
		EXPECT_EQ(radio->getVariant(), RadioButton::Variant::Disabled);
	}

	TEST_F(TestRadioButton, SelectRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto radio = makeRadio(registry);

		radio->initialize();

		EXPECT_FALSE(radio->isSelected());
		radio->select(true);
		EXPECT_TRUE(radio->isSelected());

		radio->select(false);
		EXPECT_FALSE(radio->isSelected());
	}

	TEST_F(TestRadioButton, SelectedDotIsOpaque)
	{
		ecs::ComponentRegistry registry;
		auto radio = makeRadio(registry);

		radio->initialize();
		radio->select(true);

		const auto &color =
			registry.getComponent<components::Color>(radio->getIdentifier());
		EXPECT_GT(color.getColor().getAlpha(), 0);
	}

	TEST_F(TestRadioButton, LabelAddsChild)
	{
		ecs::ComponentRegistry registry;
		auto radio =
			makeRadio(registry, RadioButton::Variant::Enabled, "Choice");

		radio->initialize();
		radio->update();

		EXPECT_EQ(radio->getChildren().size(), 1U);
	}

	TEST_F(TestRadioButton, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		RadioButton::Builder builder(registry, entityRegistry);
		RadioButton::Director director;

		auto radio = director.makeRadioButton(
			builder, parent, RadioButton::Variant::Error, "Option");

		ASSERT_NE(radio, nullptr);
		EXPECT_EQ(radio->getParent(), parent);
		EXPECT_EQ(radio->getVariant(), RadioButton::Variant::Error);
	}

}	 // namespace guillaume::entities::tests
