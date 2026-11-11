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
#include "components/test_hierarchy.hpp"

#include <memory>
#include <stdexcept>

#include "guillaume/components/bound.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity.hpp"
#include "guillaume/ecs/entity_filler.hpp"

namespace guillaume::components::tests
{
	namespace
	{
		using Node = ecs::EntityFiller<components::Bound>;

		const Hierarchy &hierarchyOf(const ecs::ComponentRegistry &registry,
									 const std::shared_ptr<Node> &node)
		{
			return registry.getComponent<Hierarchy>(node->getIdentifier());
		}
	}	 // namespace

	TEST_F(TestHierarchy, DefaultsToARootWithoutChildren)
	{
		Hierarchy hierarchy;

		EXPECT_FALSE(hierarchy.hasParent());
		EXPECT_EQ(hierarchy.getParentIdentifier(),
				  ecs::Entity::InvalidIdentifier);
		EXPECT_TRUE(hierarchy.getChildIdentifiers().empty());
		EXPECT_EQ(hierarchy.getLayer(), 0);
	}

	TEST_F(TestHierarchy, ChildrenAreUniqueAndKeepAttachOrder)
	{
		Hierarchy hierarchy;

		EXPECT_TRUE(hierarchy.addChild(7));
		EXPECT_TRUE(hierarchy.addChild(3));
		EXPECT_FALSE(hierarchy.addChild(7));
		ASSERT_EQ(hierarchy.getChildIdentifiers().size(), 2U);
		EXPECT_EQ(hierarchy.getChildIdentifiers()[0], 7U);
		EXPECT_EQ(hierarchy.getChildIdentifiers()[1], 3U);

		EXPECT_TRUE(hierarchy.removeChild(7));
		EXPECT_FALSE(hierarchy.removeChild(7));
		EXPECT_FALSE(hierarchy.hasChild(7));
		EXPECT_TRUE(hierarchy.hasChild(3));
	}

	TEST_F(TestHierarchy, SettersOnlyFlagRealChanges)
	{
		Hierarchy hierarchy;
		hierarchy.setHasChanged(false);

		hierarchy.setLayer(0).setParentIdentifier(
			ecs::Entity::InvalidIdentifier);
		EXPECT_FALSE(hierarchy.hasChanged());

		hierarchy.setLayer(2);
		EXPECT_TRUE(hierarchy.hasChanged());
	}

	TEST_F(TestHierarchy, FilledEntitiesGetTheComponentAndSignatureBit)
	{
		ecs::ComponentRegistry registry;
		auto node = std::make_shared<Node>(registry);

		EXPECT_TRUE(registry.hasComponent<Hierarchy>(node->getIdentifier()));
		const auto bit = ecs::Entity::getSignatureFromTypes<Hierarchy>();
		EXPECT_EQ(node->getSignature() & bit, bit);
	}

	TEST_F(TestHierarchy, SetParentMirrorsParentChildrenAndLayer)
	{
		ecs::ComponentRegistry registry;
		auto root  = std::make_shared<Node>(registry);
		auto child = std::make_shared<Node>(registry);

		child->setParent(root);

		const auto &childHierarchy = hierarchyOf(registry, child);
		EXPECT_EQ(childHierarchy.getParentIdentifier(), root->getIdentifier());
		EXPECT_EQ(childHierarchy.getLayer(), 1);
		EXPECT_EQ(child->getLayer(), 1);
		EXPECT_TRUE(
			hierarchyOf(registry, root).hasChild(child->getIdentifier()));
	}

	TEST_F(TestHierarchy, ReparentingMovesTheChildBetweenParents)
	{
		ecs::ComponentRegistry registry;
		auto first	= std::make_shared<Node>(registry);
		auto second = std::make_shared<Node>(registry);
		auto child	= std::make_shared<Node>(registry);

		child->setParent(first);
		child->setParent(second);

		EXPECT_FALSE(
			hierarchyOf(registry, first).hasChild(child->getIdentifier()));
		EXPECT_TRUE(
			hierarchyOf(registry, second).hasChild(child->getIdentifier()));

		child->setParent(nullptr);
		EXPECT_FALSE(
			hierarchyOf(registry, second).hasChild(child->getIdentifier()));
		EXPECT_FALSE(hierarchyOf(registry, child).hasParent());
		EXPECT_EQ(child->getLayer(), 0);
	}

	TEST_F(TestHierarchy, AttachingASubtreeUpdatesDescendantLayers)
	{
		ecs::ComponentRegistry registry;
		auto root		= std::make_shared<Node>(registry);
		auto branch		= std::make_shared<Node>(registry);
		auto leaf		= std::make_shared<Node>(registry);
		auto grandchild = std::make_shared<Node>(registry);

		leaf->setParent(branch);
		grandchild->setParent(leaf);
		ASSERT_EQ(grandchild->getLayer(), 2);

		branch->setParent(root);

		EXPECT_EQ(branch->getLayer(), 1);
		EXPECT_EQ(leaf->getLayer(), 2);
		EXPECT_EQ(grandchild->getLayer(), 3);
		EXPECT_EQ(hierarchyOf(registry, grandchild).getLayer(), 3);
	}

	TEST_F(TestHierarchy, ParentWithoutHierarchyStillGivesLayerAndIdentifier)
	{
		ecs::ComponentRegistry registry;
		auto bare  = std::make_shared<ecs::Entity>();
		auto child = std::make_shared<Node>(registry);

		child->setParent(bare);

		EXPECT_EQ(child->getParent(), bare);
		EXPECT_EQ(child->getLayer(), 1);
		EXPECT_EQ(hierarchyOf(registry, child).getParentIdentifier(),
				  bare->getIdentifier());
	}

	TEST_F(TestHierarchy, CyclesAreRejected)
	{
		ecs::ComponentRegistry registry;
		auto root  = std::make_shared<Node>(registry);
		auto child = std::make_shared<Node>(registry);
		auto leaf  = std::make_shared<Node>(registry);

		child->setParent(root);
		leaf->setParent(child);

		EXPECT_THROW(root->setParent(root), std::invalid_argument);
		EXPECT_THROW(root->setParent(leaf), std::invalid_argument);
		EXPECT_FALSE(hierarchyOf(registry, root).hasParent());
		EXPECT_EQ(leaf->getLayer(), 2);
	}

}	 // namespace guillaume::components::tests
