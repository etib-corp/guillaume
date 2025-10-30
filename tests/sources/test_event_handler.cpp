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

#include "test_event_handler.hpp"

TEST_F(EventHandlerTest, ConstructorInitializesHandler) {
  EXPECT_NE(handler, nullptr);
  EXPECT_EQ(handler->getComponentTree(), &componentTree);
}

TEST_F(EventHandlerTest, SetAndGetComponentTree) {
  ComponentTree newTree;
  handler->setComponentTree(&newTree);
  EXPECT_EQ(handler->getComponentTree(), &newTree);
}

TEST_F(EventHandlerTest, PollEventsReturnsEmpty) {
  auto events = handler->pollEvents();
  EXPECT_TRUE(events.empty());
}

TEST_F(EventHandlerTest, PollEventsReturnsMockEvents) {
  auto component = std::make_shared<TestComponent>();
  Event event1("click", component);
  Event event2("hover", component);

  handler->addMockEvent(event1);
  handler->addMockEvent(event2);

  auto events = handler->pollEvents();
  EXPECT_EQ(events.size(), 2);
  EXPECT_EQ(events[0].getType(), "click");
  EXPECT_EQ(events[1].getType(), "hover");

  // Events should be cleared after polling
  auto emptyEvents = handler->pollEvents();
  EXPECT_TRUE(emptyEvents.empty());
}

TEST_F(EventHandlerTest, DispatchEventToTargetComponent) {
  auto testComp = std::make_shared<TestComponent>();
  root->addChild(testComp);

  Event event("click", testComp);
  handler->dispatchEvent(event);

  EXPECT_EQ(testComp->getEventCount(), 1);
  EXPECT_EQ(testComp->getLastEventType(), "click");
}

TEST_F(EventHandlerTest, DispatchEventToRootWhenNoTarget) {
  // Create a fresh test setup
  auto newHandler = std::make_shared<MockEventHandler>();
  auto testRoot = std::make_shared<Container>();
  auto testChild = std::make_shared<TestComponent>();
  testRoot->addChild(testChild);

  ComponentTree testTree;
  testTree.setRoot(testRoot);
  newHandler->setComponentTree(&testTree);

  Event event("click", nullptr);
  newHandler->dispatchEvent(event);

  // The event should propagate to the child (once from root propagation)
  EXPECT_EQ(testChild->getEventCount(), 1);
  EXPECT_EQ(testChild->getLastEventType(), "click");
}

TEST_F(EventHandlerTest, GetRootFromComponentTree) {
  EXPECT_EQ(handler->getRoot(), root);
}

TEST_F(EventHandlerTest, PropagateEventToChildren) {
  auto parent =
      std::make_shared<Container>(); // Use Container which propagates events
  auto child1 = std::make_shared<TestComponent>();
  auto child2 = std::make_shared<TestComponent>();

  parent->addChild(child1);
  parent->addChild(child2);

  Event event("test", parent);
  handler->dispatchEvent(event);

  // Container propagates events to children in its onEvent
  EXPECT_EQ(child1->getEventCount(), 1);
  EXPECT_EQ(child2->getEventCount(), 1);
}

TEST_F(EventHandlerTest, ProcessEventsCallsPollAndDispatch) {
  auto testComp = std::make_shared<TestComponent>();
  root->addChild(testComp);

  Event event1("click", testComp);
  Event event2("hover", testComp);
  handler->addMockEvent(event1);
  handler->addMockEvent(event2);

  handler->processEvents();

  EXPECT_EQ(testComp->getEventCount(), 2);
}

TEST_F(EventHandlerTest, FindComponentByPredicate) {
  auto comp1 = std::make_shared<TestComponent>();
  auto comp2 = std::make_shared<TestComponent>();
  auto comp3 = std::make_shared<TestComponent>();

  root->addChild(comp1);
  comp1->addChild(comp2);
  comp1->addChild(comp3);

  auto found =
      handler->findComponent(root, [comp2](auto c) { return c == comp2; });

  EXPECT_NE(found, nullptr);
  EXPECT_EQ(found, comp2);
}

TEST_F(EventHandlerTest, FindComponentByPredicateReturnsNullWhenNotFound) {
  auto comp1 = std::make_shared<TestComponent>();
  root->addChild(comp1);

  auto notInTree = std::make_shared<TestComponent>();
  auto found = handler->findComponent(
      root, [notInTree](auto c) { return c == notInTree; });

  EXPECT_EQ(found, nullptr);
}

TEST_F(EventHandlerTest, FindComponentById) {
  auto comp1 = std::make_shared<TestComponent>();
  auto comp2 = std::make_shared<TestComponent>();

  root->addChild(comp1);
  root->addChild(comp2);

  unsigned int targetId = comp2->getID();
  auto found = handler->findComponentById(targetId);

  EXPECT_NE(found, nullptr);
  EXPECT_EQ(found->getID(), targetId);
}

TEST_F(EventHandlerTest, FindComponentByIdReturnsNullWhenNotFound) {
  auto comp1 = std::make_shared<TestComponent>();
  root->addChild(comp1);

  unsigned int nonExistentId = 99999;
  auto found = handler->findComponentById(nonExistentId);

  EXPECT_EQ(found, nullptr);
}

TEST_F(EventHandlerTest, ShouldContinueReturnsTrue) {
  EXPECT_TRUE(handler->shouldContinue());
}

TEST_F(EventHandlerTest, ShouldContinueCanBeFalse) {
  handler->setShouldContinue(false);
  EXPECT_FALSE(handler->shouldContinue());
}

TEST_F(EventHandlerTest, ButtonClickEventHandling) {
  bool wasClicked = false;
  auto button = std::make_shared<Button>("Test Button");
  button->setOnClick([&wasClicked]() { wasClicked = true; });

  root->addChild(button);

  Event clickEvent("click", button);
  handler->dispatchEvent(clickEvent);

  EXPECT_TRUE(wasClicked);
}

TEST_F(EventHandlerTest, NestedComponentEventPropagation) {
  auto level1 = std::make_shared<Container>(); // Use Container for propagation
  auto level2 = std::make_shared<Container>(); // Use Container for propagation
  auto level3 = std::make_shared<TestComponent>();

  root->addChild(level1);
  level1->addChild(level2);
  level2->addChild(level3);

  Event event("test", level1);
  handler->dispatchEvent(event);

  // Container components propagate events to children
  EXPECT_EQ(level3->getEventCount(), 1);
}
