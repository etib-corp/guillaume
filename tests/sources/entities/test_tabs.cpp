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

#include "entities/test_tabs.hpp"

#include <memory>
#include <string>
#include <vector>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/selection.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<Tabs>
			makeTabs(ecs::ComponentRegistry &registry,
					 const std::vector<std::string> &labels,
					 Tabs::Variant variant	   = Tabs::Variant::Primary,
					 std::size_t selectedIndex = 0)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Tabs::Builder builder(registry, entityRegistry);
			Tabs::Director director;

			return director.makeTabs(builder, nullptr, variant, labels,
									 selectedIndex);
		}
	}	 // namespace

	TEST_F(TestTabs, BuildsTabChildren)
	{
		ecs::ComponentRegistry registry;
		auto tabs = makeTabs(registry, { "One", "Two", "Three" });

		tabs->initialize();
		tabs->update();

		EXPECT_EQ(tabs->getTabCount(), 3U);
	}

	TEST_F(TestTabs, DefaultGeometryIsRow)
	{
		ecs::ComponentRegistry registry;
		auto tabs = makeTabs(registry, { "One", "Two" });

		tabs->initialize();
		tabs->update();

		const auto &bound =
			registry.getComponent<components::Bound>(tabs->getIdentifier());
		EXPECT_GT(bound.getWidth(), 0.0f);
		EXPECT_GT(bound.getHeight(), 0.0f);
	}

	TEST_F(TestTabs, SelectUpdatesIndex)
	{
		ecs::ComponentRegistry registry;
		auto tabs = makeTabs(registry, { "A", "B", "C" });

		tabs->initialize();

		EXPECT_EQ(tabs->getSelectedIndex(), 0U);
		tabs->select(2);
		EXPECT_EQ(tabs->getSelectedIndex(), 2U);
	}

	TEST_F(TestTabs, SingleSelectionIsExclusive)
	{
		ecs::ComponentRegistry registry;
		auto tabs = makeTabs(registry, { "A", "B", "C" });

		tabs->initialize();
		tabs->select(1);

		const auto firstSelectable =
			registry.getComponent<components::Selectable>(
				tabs->getTabIdentifier(0));
		const auto secondSelectable =
			registry.getComponent<components::Selectable>(
				tabs->getTabIdentifier(1));

		EXPECT_FALSE(firstSelectable.isSelected());
		EXPECT_TRUE(secondSelectable.isSelected());
	}

	TEST_F(TestTabs, SelectedIndexOutOfRangeIsIgnored)
	{
		ecs::ComponentRegistry registry;
		auto tabs = makeTabs(registry, { "A", "B" });

		tabs->initialize();
		tabs->select(5);

		EXPECT_LT(tabs->getSelectedIndex(), tabs->getTabCount());
	}

	TEST_F(TestTabs, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Tabs::Builder builder(registry, entityRegistry);
		Tabs::Director director;

		auto tabs = director.makeTabs(builder, parent, Tabs::Variant::Secondary,
									  { "One", "Two" }, 1);

		ASSERT_NE(tabs, nullptr);
		EXPECT_EQ(tabs->getParent(), parent);
		EXPECT_EQ(tabs->getVariant(), Tabs::Variant::Secondary);
		EXPECT_EQ(tabs->getTabCount(), 2U);
	}

}	 // namespace guillaume::entities::tests
