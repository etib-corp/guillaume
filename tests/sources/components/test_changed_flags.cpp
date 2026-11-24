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

#include "components/test_changed_flags.hpp"

namespace guillaume::components::tests
{

	TEST_F(TestChangedFlags, TextFontSizeMarksOnlyOnChange)
	{
		Text text;

		text.setFontSize(32.0f);
		EXPECT_TRUE(text.hasChanged());

		text.setHasChanged(false);
		text.setFontSize(32.0f);
		EXPECT_FALSE(text.hasChanged());
	}

	TEST_F(TestChangedFlags, TextCursorPositionMarksOnlyOnChange)
	{
		Text text;

		text.setContent("abc");
		text.setCursorPosition(1);
		text.setHasChanged(false);

		text.setCursorPosition(1);
		EXPECT_FALSE(text.hasChanged());

		text.setCursorPosition(2);
		EXPECT_TRUE(text.hasChanged());

		// A position beyond the end is clamped before it is compared.
		text.setCursorPosition(3);
		text.setHasChanged(false);
		text.setCursorPosition(99);
		EXPECT_FALSE(text.hasChanged());
	}

	TEST_F(TestChangedFlags, EllipseSegmentsMarkOnlyOnChange)
	{
		Ellipse ellipse;

		ellipse.setSegments(12);
		EXPECT_TRUE(ellipse.hasChanged());

		ellipse.setHasChanged(false);
		ellipse.setSegments(12);
		EXPECT_FALSE(ellipse.hasChanged());

		// A count below 1 is stored as 1 before it is compared.
		ellipse.setSegments(1);
		ellipse.setHasChanged(false);
		ellipse.setSegments(0);
		EXPECT_FALSE(ellipse.hasChanged());
	}

	TEST_F(TestChangedFlags, DividerColorMarksOnlyOnChange)
	{
		Divider divider;
		const utility::graphic::Color32Bit color(10, 20, 30, 255);

		divider.setColor(color);
		EXPECT_TRUE(divider.hasChanged());

		divider.setHasChanged(false);
		divider.setColor(color);
		EXPECT_FALSE(divider.hasChanged());
	}

	TEST_F(TestChangedFlags, HandPinchMarksOnlyOnChange)
	{
		HandPinchInteraction interaction;

		interaction.setHasChanged(false);
		interaction.setPinching(false);
		EXPECT_FALSE(interaction.hasChanged());

		interaction.setPinching(true);
		EXPECT_TRUE(interaction.hasChanged());

		interaction.setHasChanged(false);
		interaction.setPinching(true);
		EXPECT_FALSE(interaction.hasChanged());
	}

	TEST_F(TestChangedFlags, HandPokeMarksOnlyOnChange)
	{
		HandPokeInteraction interaction;

		interaction.setHasChanged(false);
		interaction.setPoking(false);
		EXPECT_FALSE(interaction.hasChanged());

		interaction.setPoking(true);
		EXPECT_TRUE(interaction.hasChanged());

		interaction.setHasChanged(false);
		interaction.setPoking(true);
		EXPECT_FALSE(interaction.hasChanged());
	}

	TEST_F(TestChangedFlags, ScrollableSettersMarkOnlyOnChange)
	{
		Scrollable scrollable;
		const utility::math::Vector2F vector({ 10.0f, 20.0f });

		scrollable.setContentOffset(vector);
		EXPECT_TRUE(scrollable.hasChanged());
		scrollable.setHasChanged(false);
		scrollable.setContentOffset(vector);
		EXPECT_FALSE(scrollable.hasChanged());

		scrollable.setContentSize(vector);
		EXPECT_TRUE(scrollable.hasChanged());
		scrollable.setHasChanged(false);
		scrollable.setContentSize(vector);
		EXPECT_FALSE(scrollable.hasChanged());

		scrollable.setScrollStep(vector);
		EXPECT_TRUE(scrollable.hasChanged());
		scrollable.setHasChanged(false);
		scrollable.setScrollStep(vector);
		EXPECT_FALSE(scrollable.hasChanged());
	}

	TEST_F(TestChangedFlags, ScrollableClampMarksOnlyWhenTheOffsetMoves)
	{
		Scrollable scrollable;
		utility::graphic::SizeF viewport;

		viewport.setWidth(100.0f);
		viewport.setHeight(100.0f);
		scrollable.setContentSize(utility::math::Vector2F({ 300.0f, 300.0f }));
		scrollable.setContentOffset(utility::math::Vector2F({ 50.0f, 50.0f }));
		scrollable.setHasChanged(false);

		scrollable.clampOffset(viewport);
		EXPECT_FALSE(scrollable.hasChanged());

		scrollable.setContentOffset(utility::math::Vector2F({ 500.0f, -5.0f }));
		scrollable.setHasChanged(false);
		scrollable.clampOffset(viewport);
		EXPECT_TRUE(scrollable.hasChanged());
		EXPECT_FLOAT_EQ(scrollable.getContentOffset().x, 200.0f);
		EXPECT_FLOAT_EQ(scrollable.getContentOffset().y, 0.0f);
	}

	TEST_F(TestChangedFlags, NavigationBarSelectionGuardDoesNotMark)
	{
		NavigationBar bar;

		bar.setHasChanged(false);
		bar.setUpdatingSelection(true);
		EXPECT_TRUE(bar.isUpdatingSelection());
		EXPECT_FALSE(bar.hasChanged());

		bar.setUpdatingSelection(false);
		EXPECT_FALSE(bar.isUpdatingSelection());
		EXPECT_FALSE(bar.hasChanged());
	}

	TEST_F(TestChangedFlags, NavigationDrawerSelectionGuardDoesNotMark)
	{
		NavigationDrawer drawer;

		drawer.setHasChanged(false);
		drawer.setUpdatingSelection(true);
		EXPECT_TRUE(drawer.isUpdatingSelection());
		EXPECT_FALSE(drawer.hasChanged());

		drawer.setUpdatingSelection(false);
		EXPECT_FALSE(drawer.isUpdatingSelection());
		EXPECT_FALSE(drawer.hasChanged());
	}

}	 // namespace guillaume::components::tests
