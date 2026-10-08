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

#include "entities/test_side_sheet.hpp"

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
		std::shared_ptr<SideSheet> makeSideSheet(
			ecs::ComponentRegistry &registry,
			SideSheet::Variant variant = SideSheet::Variant::Standard,
			SideSheet::Side side	   = SideSheet::Side::Right)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			SideSheet::Builder builder(registry, entityRegistry);
			SideSheet::Director director;

			return director.makeSideSheet(builder, nullptr, variant, side,
										  "Title", "Content");
		}
	}	 // namespace

	TEST_F(TestSideSheet, GeometryIsFullHeight)
	{
		ecs::ComponentRegistry registry;
		auto sheet = makeSideSheet(registry);

		sheet->initialize();
		sheet->update();

		const auto &bound =
			registry.getComponent<components::Bound>(sheet->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 320.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 640.0f);
	}

	TEST_F(TestSideSheet, RightSideHasFlatRightCorners)
	{
		ecs::ComponentRegistry registry;
		auto sheet = makeSideSheet(registry, SideSheet::Variant::Standard,
								   SideSheet::Side::Right);

		sheet->initialize();
		sheet->update();

		const auto &borders =
			registry.getComponent<components::Borders>(sheet->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopRightRadius(), 0.0f);
		EXPECT_FLOAT_EQ(borders.getBottomRightRadius(), 0.0f);
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 16.0f);
	}

	TEST_F(TestSideSheet, LeftSideHasFlatLeftCorners)
	{
		ecs::ComponentRegistry registry;
		auto sheet = makeSideSheet(registry, SideSheet::Variant::Standard,
								   SideSheet::Side::Left);

		sheet->initialize();
		sheet->update();

		const auto &borders =
			registry.getComponent<components::Borders>(sheet->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 0.0f);
		EXPECT_FLOAT_EQ(borders.getBottomLeftRadius(), 0.0f);
		EXPECT_FLOAT_EQ(borders.getTopRightRadius(), 16.0f);
	}

	TEST_F(TestSideSheet, ModalCapturesOutsideClicks)
	{
		ecs::ComponentRegistry registry;
		auto sheet = makeSideSheet(registry, SideSheet::Variant::Modal);

		sheet->initialize();
		sheet->update();

		const auto &overlay =
			registry.getComponent<components::Overlay>(sheet->getIdentifier());
		EXPECT_TRUE(overlay.isModal());
		EXPECT_TRUE(overlay.dismissesOnOutsideClick());
	}

	TEST_F(TestSideSheet, SideAndVariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto sheet = makeSideSheet(registry);

		sheet->setSide(SideSheet::Side::Left);
		sheet->setVariant(SideSheet::Variant::Modal);

		EXPECT_EQ(sheet->getSide(), SideSheet::Side::Left);
		EXPECT_EQ(sheet->getVariant(), SideSheet::Variant::Modal);
	}

	TEST_F(TestSideSheet, ShowHideTogglesVisibility)
	{
		ecs::ComponentRegistry registry;
		auto sheet = makeSideSheet(registry);

		sheet->initialize();

		EXPECT_FALSE(sheet->isVisible());
		sheet->show();
		EXPECT_TRUE(sheet->isVisible());
		sheet->hide();
		EXPECT_FALSE(sheet->isVisible());
	}

	TEST_F(TestSideSheet, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		SideSheet::Builder builder(registry, entityRegistry);
		SideSheet::Director director;

		auto sheet =
			director.makeSideSheet(builder, parent, SideSheet::Variant::Modal,
								   SideSheet::Side::Left, "Title", "Content");

		ASSERT_NE(sheet, nullptr);
		EXPECT_EQ(sheet->getParent(), parent);
		EXPECT_EQ(sheet->getSide(), SideSheet::Side::Left);
	}

}	 // namespace guillaume::entities::tests
