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

#include "test_state.hpp"
#include "state.hpp"
#include <gtest/gtest.h>

TEST(StateTest, DefaultConstructor) {
  State state;
  EXPECT_TRUE(state.getData().empty());
}

TEST(StateTest, SetAndGetValue) {
  State state;
  state.set<int>("count", 42);
  EXPECT_EQ(state.get<int>("count"), 42);
}

TEST(StateTest, SetAndGetString) {
  State state;
  state.set<std::string>("name", "Guillaume");
  EXPECT_EQ(state.get<std::string>("name"), "Guillaume");
}

TEST(StateTest, SetStateWithMap) {
  State state;
  std::map<std::string, std::any> newState;
  newState["count"] = 10;
  newState["name"] = std::string("Test");

  state.setState(newState);

  EXPECT_EQ(state.get<int>("count"), 10);
  EXPECT_EQ(state.get<std::string>("name"), "Test");
}

TEST(StateTest, SetStateMergesWithExisting) {
  State state;
  state.set<int>("count", 5);

  std::map<std::string, std::any> newState;
  newState["name"] = std::string("Merged");
  state.setState(newState);

  EXPECT_EQ(state.get<int>("count"), 5);
  EXPECT_EQ(state.get<std::string>("name"), "Merged");
}

TEST(StateTest, GetNonExistentKeyReturnsDefault) {
  State state;
  EXPECT_EQ(state.get<int>("nonexistent"), 0);
  EXPECT_EQ(state.get<std::string>("nonexistent"), "");
}

TEST(StateTest, OnStateChangeCallback) {
  State state;
  bool callbackCalled = false;

  state.setOnStateChange([&callbackCalled]() { callbackCalled = true; });

  state.set<int>("count", 42);
  EXPECT_TRUE(callbackCalled);
}

TEST(StateTest, OnStateChangeCallbackWithSetState) {
  State state;
  int callbackCount = 0;

  state.setOnStateChange([&callbackCount]() { callbackCount++; });

  std::map<std::string, std::any> newState;
  newState["value"] = 100;
  state.setState(newState);

  EXPECT_EQ(callbackCount, 1);
}
