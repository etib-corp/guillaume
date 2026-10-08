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

#include "entities/test_time_picker.hpp"

#include <cstddef>
#include <memory>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/overlay.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<TimePicker> makeTimePicker(
			ecs::ComponentRegistry &registry,
			TimePicker::Variant variant = TimePicker::Variant::Input)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			TimePicker::Builder builder(registry, entityRegistry);
			TimePicker::Director director;

			return director.makeTimePicker(builder, nullptr, variant, 9, 30);
		}
	}	 // namespace

	TEST_F(TestTimePicker, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto picker = makeTimePicker(registry, TimePicker::Variant::Input);

		picker->setVariant(TimePicker::Variant::Clock);

		EXPECT_EQ(picker->getVariant(), TimePicker::Variant::Clock);
	}

	TEST_F(TestTimePicker, DefaultGeometry)
	{
		ecs::ComponentRegistry registry;
		auto picker = makeTimePicker(registry);

		picker->initialize();
		picker->update();

		const auto &bound =
			registry.getComponent<components::Bound>(picker->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 328.0f);

		const auto &borders =
			registry.getComponent<components::Borders>(picker->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 28.0f);
	}

	TEST_F(TestTimePicker, OverlayIsModal)
	{
		ecs::ComponentRegistry registry;
		auto picker = makeTimePicker(registry);

		picker->initialize();
		picker->update();

		const auto &overlay =
			registry.getComponent<components::Overlay>(picker->getIdentifier());
		EXPECT_TRUE(overlay.isModal());
		EXPECT_TRUE(overlay.dismissesOnOutsideClick());
		EXPECT_TRUE(overlay.dismissesOnEscape());
	}

	TEST_F(TestTimePicker, TimeAndValueRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto picker = makeTimePicker(registry);

		picker->initialize();

		picker->setTime(14, 45);

		EXPECT_EQ(picker->getHour(), 14);
		EXPECT_EQ(picker->getMinute(), 45);
		EXPECT_EQ(picker->getValue(), 14 * 60 + 45);
	}

	TEST_F(TestTimePicker, ClockBuildsTwelveMarkers)
	{
		ecs::ComponentRegistry registry;
		auto picker = makeTimePicker(registry, TimePicker::Variant::Clock);

		picker->initialize();

		EXPECT_EQ(picker->getMarkerCount(), 12U);
	}

	TEST_F(TestTimePicker, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		TimePicker::Builder builder(registry, entityRegistry);
		TimePicker::Director director;

		auto picker = director.makeTimePicker(
			builder, parent, TimePicker::Variant::Clock, 8, 15);

		ASSERT_NE(picker, nullptr);
		EXPECT_EQ(picker->getParent(), parent);
		EXPECT_EQ(picker->getVariant(), TimePicker::Variant::Clock);
	}

}	 // namespace guillaume::entities::tests
