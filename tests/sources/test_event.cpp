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

#include "test_event.hpp"
#include "component.hpp"
#include "event.hpp"
#include <gtest/gtest.h>

TEST(EventTest, ConstructorWithTypeAndTarget) {
  auto component = std::make_shared<Component>();
  Event event("click", component);

  EXPECT_EQ(event.getType(), "click");
  EXPECT_EQ(event.getTarget(), component);
  EXPECT_FALSE(event.hasData());
}

TEST(EventTest, ConstructorWithData) {
  auto component = std::make_shared<Component>();
  int clickData = 42;
  Event event("click", component, clickData);

  EXPECT_EQ(event.getType(), "click");
  EXPECT_EQ(event.getTarget(), component);
  EXPECT_TRUE(event.hasData());
  EXPECT_EQ(event.getData<int>(), 42);
}

TEST(EventTest, GetTypeReturnsCorrectValue) {
  auto component = std::make_shared<Component>();
  Event event("hover", component);

  EXPECT_EQ(event.getType(), "hover");
}

TEST(EventTest, GetTargetReturnsCorrectComponent) {
  auto component1 = std::make_shared<Component>();
  auto component2 = std::make_shared<Component>();

  Event event1("click", component1);
  Event event2("click", component2);

  EXPECT_EQ(event1.getTarget(), component1);
  EXPECT_EQ(event2.getTarget(), component2);
}

TEST(EventTest, HasDataReturnsFalseWhenNoData) {
  auto component = std::make_shared<Component>();
  Event event("click", component);

  EXPECT_FALSE(event.hasData());
}

TEST(EventTest, HasDataReturnsTrueWhenDataPresent) {
  auto component = std::make_shared<Component>();
  Event event("click", component, std::string("data"));

  EXPECT_TRUE(event.hasData());
}

TEST(EventTest, GetDataWithStringData) {
  auto component = std::make_shared<Component>();
  std::string data = "Hello, Guillaume!";
  Event event("message", component, data);

  EXPECT_EQ(event.getData<std::string>(), "Hello, Guillaume!");
}

TEST(EventTest, GetDataWithCustomStruct) {
  struct MousePosition {
    int x;
    int y;
    bool operator==(const MousePosition &other) const {
      return x == other.x && y == other.y;
    }
  };

  auto component = std::make_shared<Component>();
  MousePosition pos{100, 200};
  Event event("mousemove", component, pos);

  auto retrievedPos = event.getData<MousePosition>();
  EXPECT_EQ(retrievedPos.x, 100);
  EXPECT_EQ(retrievedPos.y, 200);
}
