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

#include <gtest/gtest.h>

#include "component.hpp"
#include "test_component.hpp"

TEST(ComponentTest, DefaultConstructor) {
  Component component;
  EXPECT_EQ(component.getID(), 1);
}

TEST(ComponentTest, ConstructorWithProperties) {
  std::map<std::string, std::any> attrs;
  attrs["name"] = std::string("test-component");
  Properties props(attrs);

  Component component(props);
  EXPECT_EQ(component.getProperties().get<std::string>("name"),
            "test-component");
}

TEST(ComponentTest, GetState) {
  Component component;
  State &state = component.getState();
  state.set<int>("value", 42);

  EXPECT_EQ(component.getState().get<int>("value"), 42);
}

TEST(ComponentTest, GetProperties) {
  std::map<std::string, std::any> attrs;
  attrs["id"] = 123;
  Properties props(attrs);

  Component component(props);
  const Properties &retrievedProps = component.getProperties();

  EXPECT_EQ(retrievedProps.get<int>("id"), 123);
}

TEST(ComponentTest, AddChild) {
  auto parent = std::make_shared<Component>();
  auto child = std::make_shared<Component>();

  parent->addChild(child);

  EXPECT_EQ(parent->getChildren().size(), 1);
  EXPECT_EQ(parent->getChildren()[0], child);
}

TEST(ComponentTest, ChildHasCorrectParent) {
  auto parent = std::make_shared<Component>();
  auto child = std::make_shared<Component>();

  parent->addChild(child);

  EXPECT_EQ(child->getParent(), parent);
}

TEST(ComponentTest, MultipleChildren) {
  auto parent = std::make_shared<Component>();
  auto child1 = std::make_shared<Component>();
  auto child2 = std::make_shared<Component>();
  auto child3 = std::make_shared<Component>();

  parent->addChild(child1);
  parent->addChild(child2);
  parent->addChild(child3);

  EXPECT_EQ(parent->getChildren().size(), 3);
}

TEST(ComponentTest, RenderReturnsItself) {
  auto component = std::make_shared<Component>();
  auto rendered = component->render();

  EXPECT_EQ(rendered, component);
}

TEST(ComponentTest, OnEventDefaultImplementation) {
  auto component = std::make_shared<Component>();
  Event event("test", component);

  // Default implementation should not crash
  EXPECT_NO_THROW(component->onEvent(event));
}

TEST(ComponentTest, UniqueIDs) {
  Component comp1;
  Component comp2;
  Component comp3;

  EXPECT_NE(comp1.getID(), comp2.getID());
  EXPECT_NE(comp2.getID(), comp3.getID());
  EXPECT_NE(comp1.getID(), comp3.getID());
}

TEST(ComponentTest, GetChildrenEmpty) {
  Component component;
  EXPECT_TRUE(component.getChildren().empty());
}

TEST(ComponentTest, GetParentNull) {
  Component component;
  EXPECT_EQ(component.getParent(), nullptr);
}
