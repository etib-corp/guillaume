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

#include "components/test_bound.hpp"

namespace guillaume::components::tests
{
	TEST_F(TestBound, SettingSizeMarksTheComponentChanged)
	{
		Bound bound;

		EXPECT_FALSE(bound.hasChanged());

		bound.setWidth(10.0f);
		EXPECT_TRUE(bound.hasChanged());
		EXPECT_FLOAT_EQ(bound.getWidth(), 10.0f);

		bound.setHasChanged(false);
		bound.setHeight(4.0f);
		EXPECT_TRUE(bound.hasChanged());
		EXPECT_FLOAT_EQ(bound.getHeight(), 4.0f);
	}

	TEST_F(TestBound, SettingTheSameSizeDoesNotFlagAChange)
	{
		Bound bound;
		bound.setWidth(10.0f);
		bound.setHeight(4.0f);
		bound.setHasChanged(false);

		bound.setWidth(10.0f).setHeight(4.0f);
		EXPECT_FALSE(bound.hasChanged());
	}
}	 // namespace guillaume::components::tests