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

#include "ecs/test_travelers.hpp"

namespace guillaume::ecs::tests
{
	TEST_F(TestTravelers, LevelOrderVisitsEveryEntityOnce)
	{
		const auto order = LevelOrderTraveler().travel(_registry);

		ASSERT_EQ(order.size(), 4U);
		EXPECT_LT(indexOf(order, _parent), order.size());
		EXPECT_LT(indexOf(order, _leafRoot), order.size());
		EXPECT_LT(indexOf(order, _firstChild), order.size());
		EXPECT_LT(indexOf(order, _secondChild), order.size());
	}

	TEST_F(TestTravelers, LevelOrderVisitsRootsBeforeChildren)
	{
		const auto order = LevelOrderTraveler().travel(_registry);

		const auto deepestRoot =
			std::max(indexOf(order, _parent), indexOf(order, _leafRoot));

		EXPECT_LT(deepestRoot, indexOf(order, _firstChild));
		EXPECT_LT(deepestRoot, indexOf(order, _secondChild));
	}

	TEST_F(TestTravelers, LevelOrderKeepsSiblingInsertionOrder)
	{
		const auto order = LevelOrderTraveler().travel(_registry);

		EXPECT_LT(indexOf(order, _parent), indexOf(order, _leafRoot));
		EXPECT_LT(indexOf(order, _firstChild), indexOf(order, _secondChild));
	}

	TEST_F(TestTravelers, ReverseLevelOrderIsTheExactReverse)
	{
		auto forward	   = LevelOrderTraveler().travel(_registry);
		const auto reverse = ReverseLevelOrderTraveler().travel(_registry);

		std::reverse(forward.begin(), forward.end());

		EXPECT_EQ(reverse, forward);
	}

	TEST_F(TestTravelers, ReverseLevelOrderVisitsChildrenBeforeParent)
	{
		const auto order = ReverseLevelOrderTraveler().travel(_registry);

		EXPECT_LT(indexOf(order, _firstChild), indexOf(order, _parent));
		EXPECT_LT(indexOf(order, _secondChild), indexOf(order, _parent));
	}

	TEST_F(TestTravelers, TraversalSeesEntitiesAddedAfterAPreviousTraversal)
	{
		ASSERT_EQ(LevelOrderTraveler().travel(_registry).size(), 4U);

		auto lateChild = std::make_shared<Entity>();
		lateChild->setParent(_parent);
		_parent->addEntity(lateChild);

		const auto order = LevelOrderTraveler().travel(_registry);

		ASSERT_EQ(order.size(), 5U);
		EXPECT_LT(indexOf(order, _parent), indexOf(order, lateChild));
	}

	TEST_F(TestTravelers, ConstTraversalMatchesMutableTraversal)
	{
		const Registry &constRegistry = _registry;

		const auto mutableOrder = LevelOrderTraveler().travel(_registry);
		const auto constOrder	= LevelOrderTraveler().travel(constRegistry);

		ASSERT_EQ(constOrder.size(), mutableOrder.size());
		for (std::size_t index = 0; index < constOrder.size(); ++index) {
			EXPECT_EQ(constOrder[index].get(), mutableOrder[index].get());
		}
	}
}	 // namespace guillaume::ecs::tests
