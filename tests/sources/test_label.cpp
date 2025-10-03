/*
 Copyright (c) 2025 ETIB Corporation

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

#include "test_label.hpp"
#include "label.hpp"
#include <gtest/gtest.h>

TEST(LabelTest, ConstructorWithText) {
  Label label("Hello, World!");
  EXPECT_EQ(label.getText(), "Hello, World!");
}

TEST(LabelTest, ConstructorWithTextAndProperties) {
  std::map<std::string, std::any> attrs;
  attrs["fontSize"] = 16;
  Properties props(attrs);

  Label label("Styled Text", props);
  EXPECT_EQ(label.getText(), "Styled Text");
  EXPECT_EQ(label.getProperties().get<int>("fontSize"), 16);
}

TEST(LabelTest, GetText) {
  Label label("Test Label");
  EXPECT_EQ(label.getText(), "Test Label");
}

TEST(LabelTest, SetText) {
  Label label("Original Text");
  label.setText("Updated Text");
  EXPECT_EQ(label.getText(), "Updated Text");
}

TEST(LabelTest, SetTextUpdatesState) {
  Label label("Original");
  label.setText("New Text");
  EXPECT_EQ(label.getState().get<std::string>("text"), "New Text");
}

TEST(LabelTest, RenderReturnsItself) {
  auto label = std::make_shared<Label>("Render Test");
  auto rendered = label->render();
  EXPECT_EQ(rendered, label);
}

TEST(LabelTest, OnEventDoesNothing) {
  auto label = std::make_shared<Label>("Passive Label");
  Event clickEvent("click", label);

  // Labels typically don't handle events, should not crash
  EXPECT_NO_THROW(label->onEvent(clickEvent));
}

TEST(LabelTest, EmptyText) {
  Label label("");
  EXPECT_EQ(label.getText(), "");
}

TEST(LabelTest, LongText) {
  std::string longText =
      "This is a very long text that might be displayed in a label component";
  Label label(longText);
  EXPECT_EQ(label.getText(), longText);
}

TEST(LabelTest, MultipleTextUpdates) {
  Label label("First");
  label.setText("Second");
  label.setText("Third");
  label.setText("Fourth");

  EXPECT_EQ(label.getText(), "Fourth");
  EXPECT_EQ(label.getState().get<std::string>("text"), "Fourth");
}
