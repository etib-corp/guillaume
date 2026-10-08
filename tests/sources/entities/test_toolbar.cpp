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

#include "entities/test_toolbar.hpp"

#include <memory>

#include <utility/graphic/color.hpp>

#include <guillaume/components/borders.hpp>
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
		std::shared_ptr<Toolbar>
			makeToolbar(ecs::ComponentRegistry &registry,
						Toolbar::Variant variant = Toolbar::Variant::Docked)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Toolbar::Builder builder(registry, entityRegistry);
			Toolbar::Director director;

			return director.makeToolbar(builder, nullptr, variant);
		}
	}	 // namespace

	TEST_F(TestToolbar, DockedIsRectangular)
	{
		ecs::ComponentRegistry registry;
		auto toolbar = makeToolbar(registry, Toolbar::Variant::Docked);

		toolbar->initialize();
		toolbar->update();

		const auto &borders = registry.getComponent<components::Borders>(
			toolbar->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 0.0f);
	}

	TEST_F(TestToolbar, FloatingIsPill)
	{
		ecs::ComponentRegistry registry;
		auto toolbar = makeToolbar(registry, Toolbar::Variant::Floating);

		toolbar->initialize();
		toolbar->update();

		const auto &borders = registry.getComponent<components::Borders>(
			toolbar->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 28.0f);
	}

	TEST_F(TestToolbar, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto toolbar = makeToolbar(registry, Toolbar::Variant::Docked);

		toolbar->setVariant(Toolbar::Variant::Floating);

		EXPECT_EQ(toolbar->getVariant(), Toolbar::Variant::Floating);

		const auto &borders = registry.getComponent<components::Borders>(
			toolbar->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 28.0f);
	}

	TEST_F(TestToolbar, ChildrenAttachedAndArranged)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;

		auto firstChild	 = std::make_shared<ecs::Entity>();
		auto secondChild = std::make_shared<ecs::Entity>();

		Toolbar::Builder builder(registry, entityRegistry);
		Toolbar::Director director;

		auto toolbar =
			director.makeToolbar(builder, nullptr, Toolbar::Variant::Docked);
		toolbar->setChildren({ firstChild, secondChild });
		toolbar->initialize();
		toolbar->update();

		EXPECT_EQ(toolbar->getChildren().size(), 2U);
		EXPECT_NE(toolbar->getChildren()[0]->getIdentifier(),
				  ecs::Entity::InvalidIdentifier);
		EXPECT_EQ(firstChild->getParent(), toolbar);
	}

	TEST_F(TestToolbar, DockedUsesSurfaceContainerColor)
	{
		ecs::ComponentRegistry registry;
		auto toolbar = makeToolbar(registry, Toolbar::Variant::Docked);

		toolbar->initialize();
		toolbar->update();

		const auto &color =
			registry.getComponent<components::Color>(toolbar->getIdentifier());
		EXPECT_EQ(color.getColor(),
				  schemeColor(SchemeColorRole::SurfaceContainer));
	}

	TEST_F(TestToolbar, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Toolbar::Builder builder(registry, entityRegistry);
		Toolbar::Director director;

		auto toolbar =
			director.makeToolbar(builder, parent, Toolbar::Variant::Floating);

		ASSERT_NE(toolbar, nullptr);
		EXPECT_EQ(toolbar->getParent(), parent);
		EXPECT_EQ(toolbar->getVariant(), Toolbar::Variant::Floating);
	}

}	 // namespace guillaume::entities::tests
