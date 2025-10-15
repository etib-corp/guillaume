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

#include "test_text.hpp"
#include "primitives/text.hpp"
#include <gtest/gtest.h>

TEST(TextTest, DefaultConstructor) {
  Text text;
  EXPECT_EQ(text.getContent(), "");
  EXPECT_FLOAT_EQ(text.getPosition().x(), 0.0f);
  EXPECT_FLOAT_EQ(text.getPosition().y(), 0.0f);
  EXPECT_FLOAT_EQ(text.getPosition().z(), 0.0f);
  EXPECT_FLOAT_EQ(text.getRotation().x(), 0.0f);
  EXPECT_FLOAT_EQ(text.getRotation().y(), 0.0f);
  EXPECT_FLOAT_EQ(text.getRotation().z(), 0.0f);
  EXPECT_FLOAT_EQ(text.getScale(), 1.0f);
}

TEST(TextTest, ConstructorWithContent) {
  Text text("Hello World");
  EXPECT_EQ(text.getContent(), "Hello World");
  EXPECT_FLOAT_EQ(text.getPosition().x(), 0.0f);
  EXPECT_FLOAT_EQ(text.getPosition().y(), 0.0f);
  EXPECT_FLOAT_EQ(text.getPosition().z(), 0.0f);
  EXPECT_FLOAT_EQ(text.getRotation().x(), 0.0f);
  EXPECT_FLOAT_EQ(text.getRotation().y(), 0.0f);
  EXPECT_FLOAT_EQ(text.getRotation().z(), 0.0f);
  EXPECT_FLOAT_EQ(text.getScale(), 1.0f);
}

TEST(TextTest, ConstructorWithContentAndPosition) {
  Point position(10.0f, 20.0f, 30.0f);
  Text text("Hello World", position);
  EXPECT_EQ(text.getContent(), "Hello World");
  EXPECT_FLOAT_EQ(text.getPosition().x(), 10.0f);
  EXPECT_FLOAT_EQ(text.getPosition().y(), 20.0f);
  EXPECT_FLOAT_EQ(text.getPosition().z(), 30.0f);
  EXPECT_FLOAT_EQ(text.getRotation().x(), 0.0f);
  EXPECT_FLOAT_EQ(text.getRotation().y(), 0.0f);
  EXPECT_FLOAT_EQ(text.getRotation().z(), 0.0f);
  EXPECT_FLOAT_EQ(text.getScale(), 1.0f);
}

TEST(TextTest, ConstructorWithAllParameters) {
  Point position(10.0f, 20.0f, 30.0f);
  Point rotation(0.5f, 1.0f, 1.5f);
  float scale = 2.0f;
  Text text("Hello 3D World", position, rotation, scale);

  EXPECT_EQ(text.getContent(), "Hello 3D World");
  EXPECT_FLOAT_EQ(text.getPosition().x(), 10.0f);
  EXPECT_FLOAT_EQ(text.getPosition().y(), 20.0f);
  EXPECT_FLOAT_EQ(text.getPosition().z(), 30.0f);
  EXPECT_FLOAT_EQ(text.getRotation().x(), 0.5f);
  EXPECT_FLOAT_EQ(text.getRotation().y(), 1.0f);
  EXPECT_FLOAT_EQ(text.getRotation().z(), 1.5f);
  EXPECT_FLOAT_EQ(text.getScale(), 2.0f);
}

TEST(TextTest, SetContent) {
  Text text("Original");
  text.setContent("Updated Content");
  EXPECT_EQ(text.getContent(), "Updated Content");
}

TEST(TextTest, SetPosition) {
  Text text("Test");
  Point newPosition(100.0f, 200.0f, 300.0f);
  text.setPosition(newPosition);

  EXPECT_FLOAT_EQ(text.getPosition().x(), 100.0f);
  EXPECT_FLOAT_EQ(text.getPosition().y(), 200.0f);
  EXPECT_FLOAT_EQ(text.getPosition().z(), 300.0f);
}

TEST(TextTest, SetRotation) {
  Text text("Test");
  Point newRotation(1.57f, 3.14f, 0.0f);
  text.setRotation(newRotation);

  EXPECT_FLOAT_EQ(text.getRotation().x(), 1.57f);
  EXPECT_FLOAT_EQ(text.getRotation().y(), 3.14f);
  EXPECT_FLOAT_EQ(text.getRotation().z(), 0.0f);
}

TEST(TextTest, SetScale) {
  Text text("Test");
  text.setScale(3.5f);
  EXPECT_FLOAT_EQ(text.getScale(), 3.5f);
}

TEST(TextTest, EmptyString) {
  Text text("");
  EXPECT_EQ(text.getContent(), "");
  text.setContent("Now has content");
  EXPECT_EQ(text.getContent(), "Now has content");
}

TEST(TextTest, LongContent) {
  std::string longText = "This is a very long text that might be used to test "
                         "how the Text primitive handles longer strings with "
                         "various characters and symbols!@#$%^&*()_+";
  Text text(longText);
  EXPECT_EQ(text.getContent(), longText);
}

TEST(TextTest, SpecialCharacters) {
  std::string specialText = "Hello\nWorld\t!@#$%^&*()";
  Text text(specialText);
  EXPECT_EQ(text.getContent(), specialText);
}

TEST(TextTest, NegativePositionValues) {
  Point position(-10.5f, -20.3f, -30.7f);
  Text text("Negative Position", position);

  EXPECT_FLOAT_EQ(text.getPosition().x(), -10.5f);
  EXPECT_FLOAT_EQ(text.getPosition().y(), -20.3f);
  EXPECT_FLOAT_EQ(text.getPosition().z(), -30.7f);
}

TEST(TextTest, NegativeRotationValues) {
  Text text("Test");
  Point negativeRotation(-1.57f, -3.14f, -0.78f);
  text.setRotation(negativeRotation);

  EXPECT_FLOAT_EQ(text.getRotation().x(), -1.57f);
  EXPECT_FLOAT_EQ(text.getRotation().y(), -3.14f);
  EXPECT_FLOAT_EQ(text.getRotation().z(), -0.78f);
}

TEST(TextTest, ZeroScale) {
  Text text("Test");
  text.setScale(0.0f);
  EXPECT_FLOAT_EQ(text.getScale(), 0.0f);
}

TEST(TextTest, NegativeScale) {
  Text text("Test");
  text.setScale(-1.5f);
  EXPECT_FLOAT_EQ(text.getScale(), -1.5f);
}

TEST(TextTest, InheritsFromPrimitive) {
  Text text("Inheritance Test");
  Primitive *primitivePtr = &text;

  // Should not crash when treating as base type
  EXPECT_NO_THROW(delete new Text("Test Deletion"));
}

TEST(TextTest, MultipleUpdates) {
  Text text("Initial");

  // Multiple content updates
  text.setContent("Update 1");
  EXPECT_EQ(text.getContent(), "Update 1");

  text.setContent("Update 2");
  EXPECT_EQ(text.getContent(), "Update 2");

  // Multiple position updates
  text.setPosition(Point(1.0f, 2.0f, 3.0f));
  text.setPosition(Point(4.0f, 5.0f, 6.0f));
  EXPECT_FLOAT_EQ(text.getPosition().x(), 4.0f);
  EXPECT_FLOAT_EQ(text.getPosition().y(), 5.0f);
  EXPECT_FLOAT_EQ(text.getPosition().z(), 6.0f);
}

TEST(TextTest, DecimalPrecision) {
  Point position(1.23456f, 2.34567f, 3.45678f);
  Point rotation(0.12345f, 0.23456f, 0.34567f);
  float scale = 1.23456f;

  Text text("Precision Test", position, rotation, scale);

  EXPECT_NEAR(text.getPosition().x(), 1.23456f, 1e-5f);
  EXPECT_NEAR(text.getPosition().y(), 2.34567f, 1e-5f);
  EXPECT_NEAR(text.getPosition().z(), 3.45678f, 1e-5f);
  EXPECT_NEAR(text.getRotation().x(), 0.12345f, 1e-5f);
  EXPECT_NEAR(text.getRotation().y(), 0.23456f, 1e-5f);
  EXPECT_NEAR(text.getRotation().z(), 0.34567f, 1e-5f);
  EXPECT_NEAR(text.getScale(), 1.23456f, 1e-5f);
}
