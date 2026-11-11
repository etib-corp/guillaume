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

#include <guillaume/theme.hpp>

#include <guillaume/components/accessibility.hpp>

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
#include <guillaume/systems/layout.hpp>

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

			return builder.makeProgressIndicator(nullptr, variant, value);
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

		const auto &bound =
			registry.getComponent<components::Bound>(progress->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 48.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 48.0f);
	}

	TEST_F(TestProgressIndicator, ChildrenAreLiftedByTheContentLayer)
	{
		ecs::ComponentRegistry registry;
		auto progress = makeProgressIndicator(registry);

		const float baseZ =
			registry
				.getComponent<components::Transform>(progress->getIdentifier())
				.getPose()
				.getPosition()
				.getZ();
		const auto depths = directChildDepths(registry, progress);

		ASSERT_FALSE(depths.empty());
		for (const float depth: depths) {
			EXPECT_GE(
				depth,
				baseZ
					+ static_cast<float>(systems::Layout::ContentLayerOffset));
		}
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

	TEST_F(TestProgressIndicator, BuilderPresetBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		ProgressIndicator::Builder builder(registry, entityRegistry);

		auto progress = builder.makeProgressIndicator(
			parent, ProgressIndicator::Variant::LinearDeterminate, 0.75f);

		ASSERT_NE(progress, nullptr);
		EXPECT_EQ(progress->getParent(), parent);
		EXPECT_EQ(progress->getVariant(),
				  ProgressIndicator::Variant::LinearDeterminate);
	}

	TEST_F(TestProgressIndicator, ValueChangesAnimateOverShortFour)
	{
		ecs::ComponentRegistry registry;
		auto progress = makeProgressIndicator(registry);
		progress->initialize();
		EXPECT_FLOAT_EQ(progress->getDisplayedValue(), 0.5f);

		progress->setValue(1.0f);
		EXPECT_FLOAT_EQ(progress->getValue(), 1.0f);
		EXPECT_FLOAT_EQ(progress->getDisplayedValue(), 0.5f);

		auto &animation = registry.getComponent<components::Animation>(
			progress->getIdentifier());
		EXPECT_FLOAT_EQ(animation.getDuration(),
						getMotionDuration(theme::MotionDuration::Short4));
		animation.advance(1.0f);
		EXPECT_FLOAT_EQ(progress->getDisplayedValue(), 1.0f);
	}

	TEST_F(TestProgressIndicator, ExposesProgressBarValue)
	{
		ecs::ComponentRegistry registry;
		auto progress = makeProgressIndicator(registry);
		progress->initialize();

		const auto &accessibility =
			registry.getComponent<components::Accessibility>(
				progress->getIdentifier());
		EXPECT_EQ(accessibility.getRole(),
				  components::AccessibilityRole::ProgressBar);
		ASSERT_TRUE(accessibility.getValue().has_value());
		EXPECT_FLOAT_EQ(accessibility.getValue()->now, 50.0f);
		EXPECT_FLOAT_EQ(accessibility.getValue()->max, 100.0f);
	}

	TEST_F(TestProgressIndicator, VisibilityAndColorOverride)
	{
		ecs::ComponentRegistry registry;
		auto progress = makeProgressIndicator(registry);
		progress->initialize();

		progress->setVisible(false);
		registry.getComponent<components::Animation>(progress->getIdentifier())
			.advance(1.0f);
		EXPECT_FLOAT_EQ(progress->getOpacity(), 0.0f);

		const auto tertiary =
			getActiveScheme().getColor(SchemeColorRole::Tertiary).getColor();
		progress->setIndicatorColor(tertiary);
		EXPECT_EQ(progress->getIndicatorColor(), tertiary);
		EXPECT_FLOAT_EQ(ProgressIndicator::IndeterminateSegment, 0.6f);
	}

}	 // namespace guillaume::entities::tests
