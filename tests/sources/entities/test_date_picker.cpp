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

#include "entities/test_date_picker.hpp"

#include <cstddef>
#include <memory>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/overlay.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>
#include <guillaume/ecs/parent_entity.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<DatePicker> makeDatePicker(
			ecs::ComponentRegistry &registry,
			DatePicker::Variant variant = DatePicker::Variant::SingleDate)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			DatePicker::Builder builder(registry, entityRegistry);
			DatePicker::Director director;

			return director.makeDatePicker(builder, nullptr, variant, 2026, 1);
		}
	}	 // namespace

	TEST_F(TestDatePicker, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto picker = makeDatePicker(registry, DatePicker::Variant::SingleDate);

		picker->setVariant(DatePicker::Variant::DateRange);

		EXPECT_EQ(picker->getVariant(), DatePicker::Variant::DateRange);
	}

	TEST_F(TestDatePicker, DefaultGeometry)
	{
		ecs::ComponentRegistry registry;
		auto picker = makeDatePicker(registry);

		picker->initialize();
		picker->update();

		const auto &bound =
			registry.getComponent<components::Bound>(picker->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 328.0f);

		const auto &borders =
			registry.getComponent<components::Borders>(picker->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 28.0f);
	}

	TEST_F(TestDatePicker, OverlayIsModal)
	{
		ecs::ComponentRegistry registry;
		auto picker = makeDatePicker(registry);

		picker->initialize();
		picker->update();

		const auto &overlay =
			registry.getComponent<components::Overlay>(picker->getIdentifier());
		EXPECT_TRUE(overlay.isModal());
		EXPECT_TRUE(overlay.dismissesOnOutsideClick());
		EXPECT_TRUE(overlay.dismissesOnEscape());
	}

	TEST_F(TestDatePicker, YearMonthAndSelectedDayRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto picker = makeDatePicker(registry);

		picker->initialize();

		picker->setYear(2024);
		picker->setMonth(6);

		EXPECT_EQ(picker->getYear(), 2024);
		EXPECT_EQ(picker->getMonth(), 6);
		EXPECT_EQ(picker->getSelectedDay(), picker->getSelectedStartDay());
	}

	TEST_F(TestDatePicker, BuildsFortyTwoDayCells)
	{
		ecs::ComponentRegistry registry;
		auto picker = makeDatePicker(registry);

		picker->initialize();

		EXPECT_EQ(picker->getDayCellCount(), 42U);
	}

	TEST_F(TestDatePicker, DayCellsHaveNumberLabels)
	{
		ecs::ComponentRegistry registry;
		auto picker = makeDatePicker(registry);

		picker->initialize();

		std::size_t labelledCells = 0;

		for (const auto &entity: picker->getEntitiesBreadthFirst()) {
			const auto parent = entity->getParent();

			if (parent == nullptr
				|| !registry.hasComponent<components::Selectable>(
					parent->getIdentifier())) {
				continue;
			}

			if (registry.hasComponent<components::Text>(
					entity->getIdentifier())) {
				++labelledCells;
			}
		}

		// January 2026 has 31 days, each rendered with a text label.
		EXPECT_EQ(labelledCells, 31U);
	}

	TEST_F(TestDatePicker, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		DatePicker::Builder builder(registry, entityRegistry);
		DatePicker::Director director;

		auto picker = director.makeDatePicker(
			builder, parent, DatePicker::Variant::DateRange, 2025, 3);

		ASSERT_NE(picker, nullptr);
		EXPECT_EQ(picker->getParent(), parent);
		EXPECT_EQ(picker->getVariant(), DatePicker::Variant::DateRange);
	}

}	 // namespace guillaume::entities::tests
