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

#include "entities/test_progress_indicator.hpp"

#include <cmath>
#include <memory>

#include <guillaume/components/animation.hpp>
#include <guillaume/components/arc.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/ring.hpp>
#include <guillaume/components/value.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<ProgressIndicator> makeProgressIndicator(
			ecs::ComponentRegistry &registry,
			ProgressIndicator::Variant variant =
				ProgressIndicator::Variant::LinearDeterminate,
			float value = 0.5f)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			ProgressIndicator::Builder builder(registry, entityRegistry);
			ProgressIndicator::Director director;

			return director.makeProgressIndicator(builder, nullptr, variant,
												  value);
		}

		std::shared_ptr<ecs::Entity>
			findChildWithArc(const std::shared_ptr<ProgressIndicator> &progress,
							 ecs::ComponentRegistry &registry)
		{
			for (const auto &entity: progress->getEntities()) {
				if (entity != nullptr
					&& registry.hasComponent<components::Arc>(
						entity->getIdentifier())) {
					return entity;
				}
			}
			return nullptr;
		}
	}	 // namespace

	TEST_F(TestProgressIndicator, LinearDefaultGeometry)
	{
		ecs::ComponentRegistry registry;
		auto progress = makeProgressIndicator(registry);

		progress->initialize();
		progress->update();

		const auto &bound =
			registry.getComponent<components::Bound>(progress->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 200.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 4.0f);
	}

	TEST_F(TestProgressIndicator, CircularGeometryIsSquare)
	{
		ecs::ComponentRegistry registry;
		auto progress = makeProgressIndicator(
			registry, ProgressIndicator::Variant::CircularDeterminate, 0.25f);

		progress->initialize();
		progress->update();

		const auto &bound =
			registry.getComponent<components::Bound>(progress->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 48.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 48.0f);
	}

	TEST_F(TestProgressIndicator, ValueClampsAndRoundTrips)
	{
		ecs::ComponentRegistry registry;
		auto progress = makeProgressIndicator(
			registry, ProgressIndicator::Variant::LinearDeterminate, 0.0f);

		progress->setValue(3.0f);
		EXPECT_FLOAT_EQ(progress->getValue(), 1.0f);

		progress->setValue(-2.0f);
		EXPECT_FLOAT_EQ(progress->getValue(), 0.0f);

		const auto &value =
			registry.getComponent<components::Value>(progress->getIdentifier());
		EXPECT_FLOAT_EQ(value.getMin(), 0.0f);
		EXPECT_FLOAT_EQ(value.getMax(), 1.0f);
	}

	TEST_F(TestProgressIndicator, DeterminateCircularArcTracksValue)
	{
		ecs::ComponentRegistry registry;
		auto progress = makeProgressIndicator(
			registry, ProgressIndicator::Variant::CircularDeterminate, 0.5f);

		progress->initialize();
		progress->update();

		auto fill = findChildWithArc(progress, registry);
		ASSERT_NE(fill, nullptr);

		const auto &arc =
			registry.getComponent<components::Arc>(fill->getIdentifier());
		EXPECT_NEAR(arc.getSweepAngle(), 3.14159265358979323846f, 1e-3f);
	}

	TEST_F(TestProgressIndicator, IndeterminateBuildsAnimatedFill)
	{
		ecs::ComponentRegistry registry;
		auto progress = makeProgressIndicator(
			registry, ProgressIndicator::Variant::CircularIndeterminate, 0.0f);

		progress->initialize();
		progress->update();

		auto fill = findChildWithArc(progress, registry);
		ASSERT_NE(fill, nullptr);

		const auto &animation =
			registry.getComponent<components::Animation>(fill->getIdentifier());
		EXPECT_TRUE(animation.isLooping());
		EXPECT_TRUE(animation.isPlaying());
	}

	TEST_F(TestProgressIndicator, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto progress = makeProgressIndicator(registry);

		progress->setVariant(ProgressIndicator::Variant::CircularDeterminate);
		EXPECT_EQ(progress->getVariant(),
				  ProgressIndicator::Variant::CircularDeterminate);

		progress->setVariant(ProgressIndicator::Variant::LinearIndeterminate);
		EXPECT_EQ(progress->getVariant(),
				  ProgressIndicator::Variant::LinearIndeterminate);
	}

	TEST_F(TestProgressIndicator, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		ProgressIndicator::Builder builder(registry, entityRegistry);
		ProgressIndicator::Director director;

		auto progress = director.makeProgressIndicator(
			builder, parent, ProgressIndicator::Variant::LinearDeterminate,
			0.75f);

		ASSERT_NE(progress, nullptr);
		EXPECT_EQ(progress->getParent(), parent);
		EXPECT_EQ(progress->getVariant(),
				  ProgressIndicator::Variant::LinearDeterminate);
	}

}	 // namespace guillaume::entities::tests
