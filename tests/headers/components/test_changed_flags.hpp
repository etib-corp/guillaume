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

#pragma once

#include <gtest/gtest.h>

#include <guillaume/components/divider.hpp>
#include <guillaume/components/ellipse.hpp>
#include <guillaume/components/hand_pinch_interaction.hpp>
#include <guillaume/components/hand_poke_interaction.hpp>
#include <guillaume/components/navigation_bar.hpp>
#include <guillaume/components/navigation_drawer.hpp>
#include <guillaume/components/scrollable.hpp>
#include <guillaume/components/text.hpp>

namespace guillaume::components::tests
{

	class TestChangedFlags: public ::testing::Test
	{
		protected:
		TestChangedFlags(void)			 = default;
		~TestChangedFlags(void) override = default;
		void SetUp(void) override
		{
		}
		void TearDown(void) override
		{
		}
	};

}	 // namespace guillaume::components::tests
