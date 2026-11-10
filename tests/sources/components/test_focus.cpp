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

#include <guillaume/components/style_state.hpp>

#include <guillaume/components/accessibility.hpp>

#include <stdexcept>

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
	TEST(TestStyleState, DisablingClearsTransientFlags)
	{
		components::StyleState state;
		state.setHovered(true).setPressed(true).setFocused(true);
		EXPECT_EQ(state.getActiveLayer(), theme::StateLayer::Pressed);

		state.setEnabled(false);
		EXPECT_FALSE(state.isHovered());
		EXPECT_FALSE(state.isPressed());
		EXPECT_FALSE(state.isFocused());
		EXPECT_FALSE(state.getActiveLayer().has_value());

		state.setHovered(true);
		EXPECT_FALSE(state.isHovered());
	}

	TEST(TestStyleState, LayerPriorityFollowsMaterial)
	{
		components::StyleState state;
		EXPECT_FALSE(state.getActiveLayer().has_value());
		state.setHovered(true);
		EXPECT_EQ(state.getActiveLayer(), theme::StateLayer::Hovered);
		state.setFocused(true);
		EXPECT_EQ(state.getActiveLayer(), theme::StateLayer::Focused);
		state.setPressed(true);
		EXPECT_EQ(state.getActiveLayer(), theme::StateLayer::Pressed);
		state.setDragged(true);
		EXPECT_EQ(state.getActiveLayer(), theme::StateLayer::Dragged);
	}

	TEST(TestAccessibility, StoresSemantics)
	{
		components::Accessibility accessibility;
		accessibility.setRole(components::AccessibilityRole::Checkbox)
			.setLabel("Accept")
			.setHint("Accepts the terms")
			.setChecked(components::CheckedState::Mixed)
			.setExpanded(false)
			.setLiveRegion(components::LiveRegion::Polite);

		EXPECT_EQ(accessibility.getRole(),
				  components::AccessibilityRole::Checkbox);
		EXPECT_EQ(accessibility.getLabel(), "Accept");
		EXPECT_EQ(accessibility.getHint(), "Accepts the terms");
		EXPECT_EQ(accessibility.getChecked(), components::CheckedState::Mixed);
		EXPECT_EQ(accessibility.getExpanded(), false);
		EXPECT_EQ(accessibility.getLiveRegion(), components::LiveRegion::Polite);
		EXPECT_TRUE(accessibility.isFocusable());
	}

	TEST(TestAccessibility, ValidatesValueAndFocusOrder)
	{
		components::Accessibility accessibility;
		accessibility.setValue(components::AccessibilityValue { 0.0f, 100.0f,
																 40.0f });
		ASSERT_TRUE(accessibility.getValue().has_value());
		EXPECT_FLOAT_EQ(accessibility.getValue()->now, 40.0f);

		EXPECT_THROW(accessibility.setValue(components::AccessibilityValue {
						 0.0f, 1.0f, 2.0f }),
					 std::invalid_argument);
		EXPECT_THROW(accessibility.setFocusOrder(-1), std::invalid_argument);
	}

}	 // namespace guillaume::components::tests
