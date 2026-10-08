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

#include "entities/test_app_bar.hpp"

#include <memory>
#include <string>

#include <utility/graphic/color.hpp>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>
#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<AppBar>
			makeAppBar(ecs::ComponentRegistry &registry,
					   AppBar::Variant variant = AppBar::Variant::Small)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			AppBar::Builder builder(registry, entityRegistry);
			AppBar::Director director;

			return director.makeAppBar(builder, nullptr, variant);
		}
	}	 // namespace

	TEST_F(TestAppBar, SmallVariantGeometry)
	{
		ecs::ComponentRegistry registry;
		auto appBar = makeAppBar(registry, AppBar::Variant::Small);

		appBar->initialize();
		appBar->update();

		const auto &bound =
			registry.getComponent<components::Bound>(appBar->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 360.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 56.0f);
	}

	TEST_F(TestAppBar, LargeVariantIsTallerThanSmall)
	{
		ecs::ComponentRegistry registry;
		auto small = makeAppBar(registry, AppBar::Variant::Small);
		small->initialize();
		small->update();

		auto large = makeAppBar(registry, AppBar::Variant::Large);
		large->initialize();
		large->update();

		const float smallHeight =
			registry.getComponent<components::Bound>(small->getIdentifier())
				.getHeight();
		const float largeHeight =
			registry.getComponent<components::Bound>(large->getIdentifier())
				.getHeight();

		EXPECT_FLOAT_EQ(largeHeight, 152.0f);
		EXPECT_GT(largeHeight, smallHeight);
	}

	TEST_F(TestAppBar, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto appBar = makeAppBar(registry, AppBar::Variant::Small);

		appBar->setVariant(AppBar::Variant::Bottom);

		EXPECT_EQ(appBar->getVariant(), AppBar::Variant::Bottom);

		const auto &bound =
			registry.getComponent<components::Bound>(appBar->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 80.0f);
	}

	TEST_F(TestAppBar, TitleOnlyBuildsTextChild)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;

		AppBar::Builder builder(registry, entityRegistry);
		AppBar::Director director;

		auto appBar =
			director.makeAppBar(builder, nullptr, AppBar::Variant::Small);
		appBar->setTitle("Title");
		appBar->initialize();
		appBar->update();

		EXPECT_EQ(appBar->getChildren().size(), 1U);
		EXPECT_NE(appBar->getChildren()[0]->getIdentifier(),
				  ecs::Entity::InvalidIdentifier);
	}

	TEST_F(TestAppBar, BuilderAttachesNavigationAndActions)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;

		AppBar::Builder builder(registry, entityRegistry);
		builder.withVariant(AppBar::Variant::Small)
			.withTitle("Title")
			.withNavigationIcon("menu")
			.addAction("search")
			.addAction("more_vert");

		auto appBar = builder.registerEntity(nullptr);
		appBar->initialize();
		appBar->update();

		EXPECT_EQ(appBar->getChildren().size(), 4U);

		for (const auto &child: appBar->getChildren()) {
			EXPECT_NE(child->getIdentifier(), ecs::Entity::InvalidIdentifier);
		}
	}

	TEST_F(TestAppBar, UsesSurfaceColor)
	{
		ecs::ComponentRegistry registry;
		auto appBar = makeAppBar(registry, AppBar::Variant::Small);

		appBar->initialize();
		appBar->update();

		const auto &color =
			registry.getComponent<components::Color>(appBar->getIdentifier());
		EXPECT_EQ(color.getColor(), schemeColor(SchemeColorRole::Surface));
	}

	TEST_F(TestAppBar, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		AppBar::Builder builder(registry, entityRegistry);
		AppBar::Director director;

		auto appBar =
			director.makeAppBar(builder, parent, AppBar::Variant::Medium);

		ASSERT_NE(appBar, nullptr);
		EXPECT_EQ(appBar->getParent(), parent);
		EXPECT_EQ(appBar->getVariant(), AppBar::Variant::Medium);
	}

}	 // namespace guillaume::entities::tests
