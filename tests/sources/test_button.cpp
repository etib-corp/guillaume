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

#include "test_button.hpp"
#include "button.hpp"
#include <gtest/gtest.h>

TEST(ButtonTest, ConstructorWithLabel) {
  Button button("Click Me");
  EXPECT_EQ(button.getLabel(), "Click Me");
}

TEST(ButtonTest, ConstructorWithLabelAndProperties) {
  std::map<std::string, std::any> attrs;
  attrs["disabled"] = false;
  Properties props(attrs);

  Button button("Submit", props);
  EXPECT_EQ(button.getLabel(), "Submit");
  EXPECT_EQ(button.getProperties().get<bool>("disabled"), false);
}

TEST(ButtonTest, GetLabel) {
  Button button("Test Button");
  EXPECT_EQ(button.getLabel(), "Test Button");
}

TEST(ButtonTest, SetLabel) {
  Button button("Original");
  button.setLabel("Updated");
  EXPECT_EQ(button.getLabel(), "Updated");
}

TEST(ButtonTest, SetLabelUpdatesState) {
  Button button("Original");
  button.setLabel("New Label");
  EXPECT_EQ(button.getState().get<std::string>("label"), "New Label");
}

TEST(ButtonTest, SetOnClick) {
  Button button("Click");
  bool clicked = false;

  button.setOnClick([&clicked]() { clicked = true; });

  EXPECT_FALSE(clicked);
}

TEST(ButtonTest, OnEventWithClick) {
  auto button = std::make_shared<Button>("Click");
  bool clicked = false;

  button->setOnClick([&clicked]() { clicked = true; });

  Event clickEvent("click", button);
  button->onEvent(clickEvent);

  EXPECT_TRUE(clicked);
}

TEST(ButtonTest, OnEventWithNonClickEvent) {
  auto button = std::make_shared<Button>("Hover");
  bool clicked = false;

  button->setOnClick([&clicked]() { clicked = true; });

  Event hoverEvent("hover", button);
  button->onEvent(hoverEvent);

  EXPECT_FALSE(clicked);
}

TEST(ButtonTest, OnEventWithoutCallback) {
  auto button = std::make_shared<Button>("No Callback");
  Event clickEvent("click", button);

  // Should not crash without a callback set
  EXPECT_NO_THROW(button->onEvent(clickEvent));
}

TEST(ButtonTest, RenderReturnsItself) {
  auto button = std::make_shared<Button>("Render Test");
  auto rendered = button->render();
  EXPECT_EQ(rendered, button);
}

TEST(ButtonTest, MultipleClicks) {
  auto button = std::make_shared<Button>("Multi Click");
  int clickCount = 0;

  button->setOnClick([&clickCount]() { clickCount++; });

  Event clickEvent("click", button);
  button->onEvent(clickEvent);
  button->onEvent(clickEvent);
  button->onEvent(clickEvent);

  EXPECT_EQ(clickCount, 3);
}

TEST(ButtonTest, RenderGeneratesPrimitives) {
  auto button = std::make_shared<Button>("Test Button");
  button->render();

  const auto &primitives = button->getPrimitives();
  EXPECT_EQ(primitives.size(), 2); // Should have Rectangle + Text
}

TEST(ButtonTest, RenderGeneratesRectanglePrimitive) {
  auto button = std::make_shared<Button>("Test Button");
  button->render();

  const auto &primitives = button->getPrimitives();
  ASSERT_GE(primitives.size(), 1);

  // First primitive should be Rectangle (background)
  auto rectangle = std::dynamic_pointer_cast<Rectangle>(primitives[0]);
  EXPECT_NE(rectangle, nullptr);

  if (rectangle) {
    // Verify Rectangle properties from Button implementation
    EXPECT_FLOAT_EQ(rectangle->getCenter().x(), 50.0f);
    EXPECT_FLOAT_EQ(rectangle->getCenter().y(), 15.0f);
    EXPECT_FLOAT_EQ(rectangle->getCenter().z(), 0.0f);
    EXPECT_FLOAT_EQ(rectangle->getWidth(), 100.0f);
    EXPECT_FLOAT_EQ(rectangle->getHeight(), 30.0f);
    EXPECT_FLOAT_EQ(rectangle->getRotation().x(), 0.0f);
    EXPECT_FLOAT_EQ(rectangle->getRotation().y(), 0.0f);
    EXPECT_FLOAT_EQ(rectangle->getRotation().z(), 0.0f);
  }
}

TEST(ButtonTest, RenderGeneratesTextPrimitive) {
  auto button = std::make_shared<Button>("Test Button");
  button->render();

  const auto &primitives = button->getPrimitives();
  ASSERT_GE(primitives.size(), 2);

  // Second primitive should be Text (label)
  auto text = std::dynamic_pointer_cast<Text>(primitives[1]);
  EXPECT_NE(text, nullptr);

  if (text) {
    // Verify Text properties from Button implementation
    EXPECT_EQ(text->getContent(), "Test Button");
    EXPECT_FLOAT_EQ(text->getPosition().x(), 10.0f);
    EXPECT_FLOAT_EQ(text->getPosition().y(), 15.0f);
    EXPECT_FLOAT_EQ(text->getPosition().z(), 0.0f);
  }
}

TEST(ButtonTest, RenderClearsPrimitivesBeforeRegeneration) {
  auto button = std::make_shared<Button>("Test Button");

  // First render
  button->render();
  EXPECT_EQ(button->getPrimitives().size(), 2);

  // Second render should clear and regenerate
  button->render();
  EXPECT_EQ(button->getPrimitives().size(), 2);
}

TEST(ButtonTest, RenderUpdatesTextWhenLabelChanges) {
  auto button = std::make_shared<Button>("Original Label");
  button->render();

  // Change label and re-render
  button->setLabel("New Label");
  button->render();

  const auto &primitives = button->getPrimitives();
  ASSERT_GE(primitives.size(), 2);

  auto text = std::dynamic_pointer_cast<Text>(primitives[1]);
  ASSERT_NE(text, nullptr);
  EXPECT_EQ(text->getContent(), "New Label");
}

TEST(ButtonTest, RenderUpdatesFromState) {
  auto button = std::make_shared<Button>("Original");

  // Modify state directly
  button->getState().set<std::string>("label", "State Updated");
  button->render();

  const auto &primitives = button->getPrimitives();
  ASSERT_GE(primitives.size(), 2);

  auto text = std::dynamic_pointer_cast<Text>(primitives[1]);
  ASSERT_NE(text, nullptr);
  EXPECT_EQ(text->getContent(), "State Updated");
  EXPECT_EQ(button->getLabel(),
            "State Updated"); // Internal _label should be updated
}
