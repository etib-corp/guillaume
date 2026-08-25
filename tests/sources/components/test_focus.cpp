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

#include "components/test_focus.hpp"

namespace guillaume::components::tests
{
	TEST_F(TestFocus, Constructor)
	{
		EXPECT_NO_THROW({ Focus focus; });
	}

	TEST_F(TestFocus, SetOnFocusGainedHandler)
	{
		Focus focus;
		bool called = false;

		focus.setOnFocusGainedHandler([&called]() {
			called = true;
		});

		auto handler = focus.getOnFocusGainedHandler();
		EXPECT_TRUE(handler);
		EXPECT_FALSE(called);

		handler();
		EXPECT_TRUE(called);
	}

	TEST_F(TestFocus, SetOnFocusLostHandler)
	{
		Focus focus;
		bool called = false;

		focus.setOnFocusLostHandler([&called]() {
			called = true;
		});

		auto handler = focus.getOnFocusLostHandler();
		EXPECT_TRUE(handler);
		EXPECT_FALSE(called);

		handler();
		EXPECT_TRUE(called);
	}

	TEST_F(TestFocus, HandlersAreIndependent)
	{
		Focus focus;
		bool gainedCalled = false;
		bool lostCalled	  = false;

		focus.setOnFocusGainedHandler([&gainedCalled]() {
			gainedCalled = true;
		});

		focus.setOnFocusLostHandler([&lostCalled]() {
			lostCalled = true;
		});

		auto gainedHandler = focus.getOnFocusGainedHandler();
		auto lostHandler   = focus.getOnFocusLostHandler();

		EXPECT_TRUE(gainedHandler);
		EXPECT_TRUE(lostHandler);

		gainedHandler();
		EXPECT_TRUE(gainedCalled);
		EXPECT_FALSE(lostCalled);

		lostHandler();
		EXPECT_TRUE(lostCalled);
	}

	TEST_F(TestFocus, DefaultHandlersAreNull)
	{
		Focus focus;

		auto gainedHandler = focus.getOnFocusGainedHandler();
		auto lostHandler   = focus.getOnFocusLostHandler();

		EXPECT_FALSE(gainedHandler);
		EXPECT_FALSE(lostHandler);
	}
}	 // namespace guillaume::components::tests
