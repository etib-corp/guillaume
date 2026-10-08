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

#include "entities/test_switch.hpp"

#include <memory>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/value.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<Switch>
			makeSwitch(ecs::ComponentRegistry &registry,
					   Switch::Variant variant = Switch::Variant::Enabled,
					   bool checked = false, const std::string &iconGlyph = "",
					   const std::string &label = "")
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Switch::Builder builder(registry, entityRegistry);
			Switch::Director director;

			return director.makeSwitch(builder, nullptr, variant, checked,
									   iconGlyph, label);
		}
	}	 // namespace

	TEST_F(TestSwitch, DefaultGeometryIsPill)
	{
		ecs::ComponentRegistry registry;
		auto toggle = makeSwitch(registry);

		toggle->initialize();
		toggle->update();

		const auto &bound =
			registry.getComponent<components::Bound>(toggle->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 52.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 32.0f);

		const auto &borders =
			registry.getComponent<components::Borders>(toggle->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 16.0f);
	}

	TEST_F(TestSwitch, CheckedRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto toggle = makeSwitch(registry);

		toggle->initialize();

		EXPECT_FALSE(toggle->isChecked());
		toggle->setChecked(true);
		EXPECT_TRUE(toggle->isChecked());

		toggle->toggle();
		EXPECT_FALSE(toggle->isChecked());
	}

	TEST_F(TestSwitch, ValueReflectsCheckedState)
	{
		ecs::ComponentRegistry registry;
		auto toggle = makeSwitch(registry, Switch::Variant::Enabled, true);

		toggle->initialize();
		toggle->update();

		const auto &value =
			registry.getComponent<components::Value>(toggle->getIdentifier());
		EXPECT_FLOAT_EQ(value.getValue(), 1.0f);
		EXPECT_FLOAT_EQ(value.getMax(), 1.0f);
		EXPECT_FLOAT_EQ(value.getMin(), 0.0f);
	}

	TEST_F(TestSwitch, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto toggle = makeSwitch(registry);

		toggle->setVariant(Switch::Variant::Disabled);
		EXPECT_EQ(toggle->getVariant(), Switch::Variant::Disabled);

		toggle->setVariant(Switch::Variant::WithIcon);
		EXPECT_EQ(toggle->getVariant(), Switch::Variant::WithIcon);
	}

	TEST_F(TestSwitch, LabelAddsChildContent)
	{
		ecs::ComponentRegistry registry;
		auto toggle =
			makeSwitch(registry, Switch::Variant::Enabled, false, "", "Wifi");

		toggle->initialize();
		toggle->update();

		EXPECT_EQ(toggle->getChildren().size(), 1U);
	}

	TEST_F(TestSwitch, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Switch::Builder builder(registry, entityRegistry);
		Switch::Director director;

		auto toggle = director.makeSwitch(
			builder, parent, Switch::Variant::WithIcon, true, "check", "On");

		ASSERT_NE(toggle, nullptr);
		EXPECT_EQ(toggle->getParent(), parent);
		EXPECT_EQ(toggle->getVariant(), Switch::Variant::WithIcon);
		EXPECT_TRUE(toggle->isChecked());
	}

}	 // namespace guillaume::entities::tests
