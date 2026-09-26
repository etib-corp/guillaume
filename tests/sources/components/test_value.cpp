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

#include "components/test_value.hpp"

namespace guillaume::components::tests
{

	TEST_F(TestValue, ValueDefaults)
	{
		Value value;

		EXPECT_FLOAT_EQ(value.getMin(), 0.0f);
		EXPECT_FLOAT_EQ(value.getMax(), 1.0f);
		EXPECT_FLOAT_EQ(value.getValue(), 0.0f);
		EXPECT_FLOAT_EQ(value.getStep(), 0.0f);
	}

	TEST_F(TestValue, ValueClampsToRange)
	{
		Value value;

		value.setMax(10.0f);
		value.setValue(20.0f);
		EXPECT_FLOAT_EQ(value.getValue(), 10.0f);

		value.setValue(-5.0f);
		EXPECT_FLOAT_EQ(value.getValue(), 0.0f);
	}

	TEST_F(TestValue, ValueSnapsToStep)
	{
		Value value;

		value.setMax(10.0f);
		value.setStep(0.5f);
		value.setValue(2.3f);

		EXPECT_FLOAT_EQ(value.getValue(), 2.5f);
	}

	TEST_F(TestValue, ValueHandlerFiresOnlyOnChange)
	{
		Value value;
		int calls  = 0;
		float last = -1.0f;

		value.setOnChangedHandler([&calls, &last](float changed) {
			++calls;
			last = changed;
		});

		value.setValue(0.5f);
		EXPECT_EQ(calls, 1);
		EXPECT_FLOAT_EQ(last, 0.5f);

		value.setValue(0.5f);
		EXPECT_EQ(calls, 1);
	}

	TEST_F(TestValue, ValueNormalizedRoundTrip)
	{
		Value value;

		value.setMax(10.0f);
		value.setValue(5.0f);
		EXPECT_FLOAT_EQ(value.getNormalizedValue(), 0.5f);

		value.setNormalizedValue(0.25f);
		EXPECT_FLOAT_EQ(value.getValue(), 2.5f);
	}

	TEST_F(TestValue, RangeClampsAndOrders)
	{
		Range range;

		range.setMax(10.0f);
		EXPECT_FLOAT_EQ(range.getLow(), 0.0f);
		EXPECT_FLOAT_EQ(range.getHigh(), 1.0f);

		range.setHigh(8.0f);
		range.setLow(2.0f);

		EXPECT_FLOAT_EQ(range.getLow(), 2.0f);
		EXPECT_FLOAT_EQ(range.getHigh(), 8.0f);
	}

	TEST_F(TestValue, RangeClampsToBounds)
	{
		Range range;

		range.setMax(10.0f);
		range.setHigh(50.0f);
		EXPECT_FLOAT_EQ(range.getHigh(), 10.0f);

		range.setLow(-5.0f);
		EXPECT_FLOAT_EQ(range.getLow(), 0.0f);
	}

	TEST_F(TestValue, RangeHandlerFiresOnChange)
	{
		Range range;
		int calls = 0;

		range.setMax(10.0f);
		range.setHigh(8.0f);
		range.setOnChangedHandler([&calls](float, float) {
			++calls;
		});

		range.setLow(2.0f);
		EXPECT_EQ(calls, 1);

		range.setLow(2.0f);
		EXPECT_EQ(calls, 1);
	}

}	 // namespace guillaume::components::tests
