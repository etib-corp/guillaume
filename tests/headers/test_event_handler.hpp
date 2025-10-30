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

#pragma once

#include "button.hpp"
#include "component_tree.hpp"
#include "container.hpp"
#include "event.hpp"
#include "event_handler.hpp"
#include "label.hpp"
#include <gtest/gtest.h>
#include <memory>

// Mock EventHandler for testing
class MockEventHandler : public EventHandler {
private:
  std::vector<Event> _mockEvents;
  bool _shouldContinueFlag = true;

public:
  MockEventHandler() : EventHandler() {}

  void addMockEvent(const Event &event) { _mockEvents.push_back(event); }

  void clearMockEvents() { _mockEvents.clear(); }

  void setShouldContinue(bool value) { _shouldContinueFlag = value; }

  std::vector<Event> pollEvents() override {
    auto events = _mockEvents;
    _mockEvents.clear();
    return events;
  }

  bool shouldContinue() const override { return _shouldContinueFlag; }
};

// Test component that tracks if it received an event
class TestComponent : public Component {
private:
  int _eventCount = 0;
  std::string _lastEventType;

public:
  TestComponent() : Component() {}

  void onEvent(const Event &event) override {
    _eventCount++;
    _lastEventType = event.getType();
  }

  int getEventCount() const { return _eventCount; }
  const std::string &getLastEventType() const { return _lastEventType; }
  void resetEventCount() { _eventCount = 0; }
};

class EventHandlerTest : public ::testing::Test {
protected:
  std::shared_ptr<MockEventHandler> handler;
  ComponentTree componentTree;
  std::shared_ptr<Container> root;

  void SetUp() override {
    handler = std::make_shared<MockEventHandler>();
    root = std::make_shared<Container>();
    componentTree.setRoot(root);
    handler->setComponentTree(&componentTree);
  }

  void TearDown() override {
    handler.reset();
    root.reset();
  }
};
