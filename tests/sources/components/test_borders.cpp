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

#include "components/test_borders.hpp"

#include <set>

namespace guillaume::components::tests
{

	TEST_F(TestBorders, OrderingComparesTheStrokeWidth)
	{
		Borders thin;
		Borders thick;

		thin.setWidth(1.0f);
		thick.setWidth(2.0f);

		EXPECT_TRUE(thin != thick);
		EXPECT_TRUE(thin < thick);
		EXPECT_FALSE(thick < thin);
	}

	TEST_F(TestBorders, OrderingAgreesWithEquality)
	{
		Borders first;
		Borders second;

		first.setBorderRadius(4.0f).setWidth(1.0f);
		second.setBorderRadius(4.0f).setWidth(1.0f);

		EXPECT_TRUE(first == second);
		EXPECT_FALSE(first < second);
		EXPECT_FALSE(second < first);
	}

	TEST_F(TestBorders, WidthIsTheLastOrderingCriterion)
	{
		Borders rounded;
		Borders thick;

		rounded.setBorderRadius(8.0f).setWidth(1.0f);
		thick.setBorderRadius(4.0f).setWidth(2.0f);

		EXPECT_TRUE(thick < rounded);
		EXPECT_FALSE(rounded < thick);
	}

	TEST_F(TestBorders, OrderedContainerKeepsBordersDifferingByWidth)
	{
		Borders thin;
		Borders thick;
		std::set<Borders> cache;

		thin.setWidth(1.0f);
		thick.setWidth(2.0f);
		cache.insert(thin);
		cache.insert(thick);

		EXPECT_EQ(cache.size(), 2u);
	}

}	 // namespace guillaume::components::tests
