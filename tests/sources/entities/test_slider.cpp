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

#include "entities/test_slider.hpp"

#include <memory>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/range.hpp>
#include <guillaume/components/value.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<Slider>
			makeSlider(ecs::ComponentRegistry &registry,
					   Slider::Variant variant = Slider::Variant::Continuous)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Slider::Builder builder(registry, entityRegistry);
			Slider::Director director;

			return director.makeSlider(builder, nullptr, variant, 0.0f, 100.0f,
									   25.0f);
		}
	}	 // namespace

	TEST_F(TestSlider, DefaultGeometry)
	{
		ecs::ComponentRegistry registry;
		auto slider = makeSlider(registry);

		slider->initialize();
		slider->update();

		const auto &bound =
			registry.getComponent<components::Bound>(slider->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 200.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 20.0f);
	}

	TEST_F(TestSlider, ValueClampsToRange)
	{
		ecs::ComponentRegistry registry;
		auto slider = makeSlider(registry);

		slider->initialize();

		slider->setValue(200.0f);
		EXPECT_FLOAT_EQ(slider->getValue(), 100.0f);

		slider->setValue(-50.0f);
		EXPECT_FLOAT_EQ(slider->getValue(), 0.0f);

		slider->setValue(42.0f);
		EXPECT_FLOAT_EQ(slider->getValue(), 42.0f);
	}

	TEST_F(TestSlider, DiscreteSnapsToStep)
	{
		ecs::ComponentRegistry registry;
		auto slider = makeSlider(registry, Slider::Variant::Discrete);

		slider->initialize();

		const auto &value =
			registry.getComponent<components::Value>(slider->getIdentifier());
		EXPECT_GT(value.getStep(), 0.0f);

		slider->setValue(53.0f);
		EXPECT_NEAR(slider->getValue(), 50.0f, 1e-3f);
	}

	TEST_F(TestSlider, RangeExposesLowAndHigh)
	{
		ecs::ComponentRegistry registry;
		auto slider = makeSlider(registry, Slider::Variant::Range);

		slider->initialize();
		slider->update();

		const auto &range =
			registry.getComponent<components::Range>(slider->getIdentifier());
		EXPECT_FLOAT_EQ(range.getMin(), 0.0f);
		EXPECT_FLOAT_EQ(range.getMax(), 100.0f);
		EXPECT_FLOAT_EQ(slider->getLow(), range.getLow());
		EXPECT_FLOAT_EQ(slider->getHigh(), range.getHigh());
	}

	TEST_F(TestSlider, BuildsTrackAndThumbChildren)
	{
		ecs::ComponentRegistry registry;
		auto slider = makeSlider(registry);

		slider->initialize();
		slider->update();

		EXPECT_GE(slider->getEntities().size(), 3U);
	}

	TEST_F(TestSlider, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto slider = makeSlider(registry);

		slider->setVariant(Slider::Variant::Range);
		EXPECT_EQ(slider->getVariant(), Slider::Variant::Range);

		slider->setVariant(Slider::Variant::Discrete);
		EXPECT_EQ(slider->getVariant(), Slider::Variant::Discrete);
	}

	TEST_F(TestSlider, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Slider::Builder builder(registry, entityRegistry);
		Slider::Director director;

		auto slider = director.makeSlider(
			builder, parent, Slider::Variant::Continuous, 0.0f, 10.0f, 5.0f);

		ASSERT_NE(slider, nullptr);
		EXPECT_EQ(slider->getParent(), parent);
		EXPECT_EQ(slider->getVariant(), Slider::Variant::Continuous);
	}

}	 // namespace guillaume::entities::tests
