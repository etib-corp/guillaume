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

#include "test_container.hpp"
#include "component.hpp"
#include "container.hpp"
#include <gtest/gtest.h>

TEST(ContainerTest, DefaultConstructor) {
  Container container;
  EXPECT_TRUE(container.getChildren().empty());
}

TEST(ContainerTest, ConstructorWithProperties) {
  std::map<std::string, std::any> attrs;
  attrs["name"] = std::string("main-container");
  Properties props(attrs);

  Container container(props);
  EXPECT_EQ(container.getProperties().get<std::string>("name"),
            "main-container");
}

TEST(ContainerTest, AddChild) {
  auto container = std::make_shared<Container>();
  auto child = std::make_shared<Component>();

  container->addChild(child);

  EXPECT_EQ(container->getChildren().size(), 1);
  EXPECT_EQ(container->getChildren()[0], child);
}

TEST(ContainerTest, AddMultipleChildren) {
  auto container = std::make_shared<Container>();
  auto child1 = std::make_shared<Component>();
  auto child2 = std::make_shared<Component>();
  auto child3 = std::make_shared<Component>();

  container->addChild(child1);
  container->addChild(child2);
  container->addChild(child3);

  EXPECT_EQ(container->getChildren().size(), 3);
}

TEST(ContainerTest, ChildHasCorrectParent) {
  auto container = std::make_shared<Container>();
  auto child = std::make_shared<Component>();

  container->addChild(child);

  EXPECT_EQ(child->getParent(), container);
}

TEST(ContainerTest, RenderReturnsItself) {
  auto container = std::make_shared<Container>();
  auto rendered = container->render();

  EXPECT_EQ(rendered, container);
}

TEST(ContainerTest, RenderWithChildren) {
  auto container = std::make_shared<Container>();
  auto child1 = std::make_shared<Component>();
  auto child2 = std::make_shared<Component>();

  container->addChild(child1);
  container->addChild(child2);

  auto rendered = container->render();
  EXPECT_EQ(rendered, container);
}

TEST(ContainerTest, OnEventPropagation) {
  auto container = std::make_shared<Container>();
  auto child = std::make_shared<Component>();
  container->addChild(child);

  Event event("test", container);

  // Should not crash when propagating event to children
  EXPECT_NO_THROW(container->onEvent(event));
}

TEST(ContainerTest, NestedContainers) {
  auto rootContainer = std::make_shared<Container>();
  auto subContainer = std::make_shared<Container>();
  auto component = std::make_shared<Component>();

  subContainer->addChild(component);
  rootContainer->addChild(subContainer);

  EXPECT_EQ(rootContainer->getChildren().size(), 1);
  EXPECT_EQ(subContainer->getChildren().size(), 1);
  EXPECT_EQ(component->getParent(), subContainer);
  EXPECT_EQ(subContainer->getParent(), rootContainer);
}
