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

#include "entities/test_checkbox.hpp"

#include <memory>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/line.hpp>
#include <guillaume/components/selection.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<Checkbox>
			makeCheckbox(ecs::ComponentRegistry &registry,
						 Checkbox::State state	   = Checkbox::State::Unchecked,
						 Checkbox::Variant variant = Checkbox::Variant::Enabled,
						 const std::string &label  = "")
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Checkbox::Builder builder(registry, entityRegistry);
			Checkbox::Director director;

			return director.makeCheckbox(builder, nullptr, state, variant,
										 label);
		}
	}	 // namespace

	TEST_F(TestCheckbox, DefaultGeometryIsSquareBox)
	{
		ecs::ComponentRegistry registry;
		auto checkbox = makeCheckbox(registry);

		checkbox->initialize();
		checkbox->update();

		const auto &bound =
			registry.getComponent<components::Bound>(checkbox->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 18.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 18.0f);

		const auto &borders = registry.getComponent<components::Borders>(
			checkbox->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 2.0f);
	}

	TEST_F(TestCheckbox, StateRoundTripUpdatesSelectable)
	{
		ecs::ComponentRegistry registry;
		auto checkbox = makeCheckbox(registry);

		checkbox->initialize();
		checkbox->setState(Checkbox::State::Checked);

		EXPECT_EQ(checkbox->getState(), Checkbox::State::Checked);
		EXPECT_TRUE(checkbox->isChecked());

		checkbox->setState(Checkbox::State::Unchecked);
		EXPECT_EQ(checkbox->getState(), Checkbox::State::Unchecked);
		EXPECT_FALSE(checkbox->isChecked());
	}

	TEST_F(TestCheckbox, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto checkbox = makeCheckbox(registry);

		checkbox->setVariant(Checkbox::Variant::Error);
		EXPECT_EQ(checkbox->getVariant(), Checkbox::Variant::Error);

		checkbox->setVariant(Checkbox::Variant::Disabled);
		EXPECT_EQ(checkbox->getVariant(), Checkbox::Variant::Disabled);
	}

	TEST_F(TestCheckbox, IndeterminateEnablesLineStroke)
	{
		ecs::ComponentRegistry registry;
		auto checkbox = makeCheckbox(registry);

		checkbox->initialize();
		checkbox->setState(Checkbox::State::Indeterminate);

		const auto &line =
			registry.getComponent<components::Line>(checkbox->getIdentifier());
		EXPECT_GT(line.getThickness(), 0.0f);
	}

	TEST_F(TestCheckbox, LabelAddsChildContent)
	{
		ecs::ComponentRegistry registry;
		auto checkbox = makeCheckbox(registry, Checkbox::State::Checked,
									 Checkbox::Variant::Enabled, "Accept");

		checkbox->initialize();
		checkbox->update();

		EXPECT_EQ(checkbox->getChildren().size(), 2U);
	}

	TEST_F(TestCheckbox, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Checkbox::Builder builder(registry, entityRegistry);
		Checkbox::Director director;

		auto checkbox =
			director.makeCheckbox(builder, parent, Checkbox::State::Checked,
								  Checkbox::Variant::Error, "Terms");

		ASSERT_NE(checkbox, nullptr);
		EXPECT_EQ(checkbox->getParent(), parent);
		EXPECT_EQ(checkbox->getState(), Checkbox::State::Checked);
		EXPECT_EQ(checkbox->getVariant(), Checkbox::Variant::Error);
	}

}	 // namespace guillaume::entities::tests
