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

#include "entities/test_loading_indicator.hpp"

#include <memory>

#include <guillaume/components/animation.hpp>
#include <guillaume/components/arc.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<LoadingIndicator>
			makeLoadingIndicator(ecs::ComponentRegistry &registry,
								 LoadingIndicator::Variant variant =
									 LoadingIndicator::Variant::Linear)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			LoadingIndicator::Builder builder(registry, entityRegistry);
			LoadingIndicator::Director director;

			return director.makeLoadingIndicator(builder, nullptr, variant);
		}
	}	 // namespace

	TEST_F(TestLoadingIndicator, LinearDefaultGeometry)
	{
		ecs::ComponentRegistry registry;
		auto loading = makeLoadingIndicator(registry);

		loading->initialize();
		loading->update();

		const auto &bound =
			registry.getComponent<components::Bound>(loading->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 200.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 4.0f);
	}

	TEST_F(TestLoadingIndicator, CircularGeometryIsSquare)
	{
		ecs::ComponentRegistry registry;
		auto loading =
			makeLoadingIndicator(registry, LoadingIndicator::Variant::Circular);

		loading->initialize();
		loading->update();

		const auto &bound =
			registry.getComponent<components::Bound>(loading->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 48.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 48.0f);
	}

	TEST_F(TestLoadingIndicator, AnimationRunsAfterInitialize)
	{
		ecs::ComponentRegistry registry;
		auto loading = makeLoadingIndicator(registry);

		loading->initialize();

		EXPECT_TRUE(loading->isRunning());
	}

	TEST_F(TestLoadingIndicator, StopAndStartControlAnimation)
	{
		ecs::ComponentRegistry registry;
		auto loading = makeLoadingIndicator(registry);

		loading->initialize();
		EXPECT_TRUE(loading->isRunning());

		loading->stop();
		EXPECT_FALSE(loading->isRunning());

		loading->start();
		EXPECT_TRUE(loading->isRunning());
	}

	TEST_F(TestLoadingIndicator, CircularBuildsArcChild)
	{
		ecs::ComponentRegistry registry;
		auto loading =
			makeLoadingIndicator(registry, LoadingIndicator::Variant::Circular);

		loading->initialize();
		loading->update();

		ASSERT_FALSE(loading->getEntities().empty());
		const auto &child = loading->getEntities().front();
		ASSERT_NE(child, nullptr);
		EXPECT_TRUE(
			registry.hasComponent<components::Arc>(child->getIdentifier()));
	}

	TEST_F(TestLoadingIndicator, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto loading = makeLoadingIndicator(registry);

		loading->setVariant(LoadingIndicator::Variant::Circular);
		EXPECT_EQ(loading->getVariant(), LoadingIndicator::Variant::Circular);

		loading->setVariant(LoadingIndicator::Variant::Linear);
		EXPECT_EQ(loading->getVariant(), LoadingIndicator::Variant::Linear);
	}

	TEST_F(TestLoadingIndicator, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		LoadingIndicator::Builder builder(registry, entityRegistry);
		LoadingIndicator::Director director;

		auto loading = director.makeLoadingIndicator(
			builder, parent, LoadingIndicator::Variant::Linear);

		ASSERT_NE(loading, nullptr);
		EXPECT_EQ(loading->getParent(), parent);
		EXPECT_EQ(loading->getVariant(), LoadingIndicator::Variant::Linear);
	}

}	 // namespace guillaume::entities::tests
