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

#include "entities/test_bottom_sheet.hpp"

#include <memory>
#include <string>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/overlay.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<BottomSheet> makeBottomSheet(
			ecs::ComponentRegistry &registry,
			BottomSheet::Variant variant = BottomSheet::Variant::Standard)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			BottomSheet::Builder builder(registry, entityRegistry);
			BottomSheet::Director director;

			return director.makeBottomSheet(builder, nullptr, variant, "Title",
											"Content", 320.0f);
		}
	}	 // namespace

	TEST_F(TestBottomSheet, AnchoredGeometry)
	{
		ecs::ComponentRegistry registry;
		auto sheet = makeBottomSheet(registry);

		sheet->initialize();
		sheet->update();

		const auto &bound =
			registry.getComponent<components::Bound>(sheet->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 360.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 320.0f);
	}

	TEST_F(TestBottomSheet, TopCornersAreRounded)
	{
		ecs::ComponentRegistry registry;
		auto sheet = makeBottomSheet(registry);

		sheet->initialize();
		sheet->update();

		const auto &borders =
			registry.getComponent<components::Borders>(sheet->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 28.0f);
		EXPECT_FLOAT_EQ(borders.getTopRightRadius(), 28.0f);
		EXPECT_FLOAT_EQ(borders.getBottomLeftRadius(), 0.0f);
		EXPECT_FLOAT_EQ(borders.getBottomRightRadius(), 0.0f);
	}

	TEST_F(TestBottomSheet, ModalCapturesOutsideClicks)
	{
		ecs::ComponentRegistry registry;
		auto sheet = makeBottomSheet(registry, BottomSheet::Variant::Modal);

		sheet->initialize();
		sheet->update();

		const auto &overlay =
			registry.getComponent<components::Overlay>(sheet->getIdentifier());
		EXPECT_TRUE(overlay.isModal());
		EXPECT_TRUE(overlay.dismissesOnOutsideClick());
	}

	TEST_F(TestBottomSheet, StandardDoesNotCaptureInput)
	{
		ecs::ComponentRegistry registry;
		auto sheet = makeBottomSheet(registry, BottomSheet::Variant::Standard);

		sheet->initialize();
		sheet->update();

		const auto &overlay =
			registry.getComponent<components::Overlay>(sheet->getIdentifier());
		EXPECT_FALSE(overlay.isModal());
		EXPECT_FALSE(overlay.dismissesOnOutsideClick());
	}

	TEST_F(TestBottomSheet, ShowHideTogglesVisibility)
	{
		ecs::ComponentRegistry registry;
		auto sheet = makeBottomSheet(registry);

		sheet->initialize();

		EXPECT_FALSE(sheet->isVisible());
		sheet->show();
		EXPECT_TRUE(sheet->isVisible());
		sheet->hide();
		EXPECT_FALSE(sheet->isVisible());
	}

	TEST_F(TestBottomSheet, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto sheet = makeBottomSheet(registry, BottomSheet::Variant::Standard);

		sheet->setVariant(BottomSheet::Variant::Modal);

		EXPECT_EQ(sheet->getVariant(), BottomSheet::Variant::Modal);
	}

	TEST_F(TestBottomSheet, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		BottomSheet::Builder builder(registry, entityRegistry);
		BottomSheet::Director director;

		auto sheet = director.makeBottomSheet(builder, parent,
											  BottomSheet::Variant::Modal,
											  "Title", "Content", 400.0f);

		ASSERT_NE(sheet, nullptr);
		EXPECT_EQ(sheet->getParent(), parent);
		EXPECT_EQ(sheet->getVariant(), BottomSheet::Variant::Modal);
	}

}	 // namespace guillaume::entities::tests
