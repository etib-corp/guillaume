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

#include "entities/test_chip.hpp"

#include <memory>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/selection.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<Chip>
			makeChip(ecs::ComponentRegistry &registry,
					 Chip::Variant variant		  = Chip::Variant::Assist,
					 const std::string &label	  = "Label",
					 const std::string &iconGlyph = "", bool selected = false)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Chip::Builder builder(registry, entityRegistry);
			Chip::Director director;

			return director.makeChip(builder, nullptr, variant, label,
									 iconGlyph, selected);
		}
	}	 // namespace

	TEST_F(TestChip, DefaultGeometryIsPill)
	{
		ecs::ComponentRegistry registry;
		auto chip = makeChip(registry);

		chip->initialize();
		chip->update();

		const auto &bound =
			registry.getComponent<components::Bound>(chip->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 32.0f);

		const auto &borders =
			registry.getComponent<components::Borders>(chip->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 16.0f);
	}

	TEST_F(TestChip, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto chip = makeChip(registry);

		chip->setVariant(Chip::Variant::Filter);
		EXPECT_EQ(chip->getVariant(), Chip::Variant::Filter);

		chip->setVariant(Chip::Variant::Input);
		EXPECT_EQ(chip->getVariant(), Chip::Variant::Input);
	}

	TEST_F(TestChip, SelectedRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto chip = makeChip(registry);

		chip->initialize();

		EXPECT_FALSE(chip->isSelected());
		chip->setSelected(true);
		EXPECT_TRUE(chip->isSelected());

		chip->setSelected(false);
		EXPECT_FALSE(chip->isSelected());
	}

	TEST_F(TestChip, FilterSelectedUsesContainerFill)
	{
		ecs::ComponentRegistry registry;
		auto chip = makeChip(registry, Chip::Variant::Filter, "Tag");

		chip->initialize();
		chip->setSelected(true);

		const auto &color =
			registry.getComponent<components::Color>(chip->getIdentifier());
		EXPECT_GT(color.getColor().getAlpha(), 0);
	}

	TEST_F(TestChip, InputHasTrailingRemoveIcon)
	{
		ecs::ComponentRegistry registry;
		auto chip = makeChip(registry, Chip::Variant::Input, "Email");

		chip->initialize();
		chip->update();

		EXPECT_EQ(chip->getChildren().size(), 2U);
	}

	TEST_F(TestChip, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Chip::Builder builder(registry, entityRegistry);
		Chip::Director director;

		auto chip = director.makeChip(builder, parent, Chip::Variant::Filter,
									  "Tag", "check", true);

		ASSERT_NE(chip, nullptr);
		EXPECT_EQ(chip->getParent(), parent);
		EXPECT_EQ(chip->getVariant(), Chip::Variant::Filter);
		EXPECT_TRUE(chip->isSelected());
	}

}	 // namespace guillaume::entities::tests
