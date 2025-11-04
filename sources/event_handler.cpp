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

#include "event_handler.hpp"
#include "component.hpp"
#include "container.hpp"
#include "event.hpp"

void EventHandler::dispatchEvent(const Event &event) {
  auto target = event.getTarget();
  if (target) {
    // Call onEvent on the target component
    propagateEvent(target, event);
  } else {
    // If no specific target, dispatch to root
    if (_root) {
      propagateEvent(_root, event);
    }
  }
}

void EventHandler::propagateEvent(std::shared_ptr<Component> component,
                                  const Event &event) {
  if (!component) {
    return;
  }

  // Call onEvent on the component
  // The component's onEvent implementation decides if/how to propagate to
  // children
  component->onEvent(event);
}

std::shared_ptr<Component> EventHandler::findComponent(
    std::shared_ptr<Component> component,
    std::function<bool(std::shared_ptr<Component>)> predicate) {
  if (!component) {
    return nullptr;
  }

  // Check if this component matches
  if (predicate(component)) {
    return component;
  }

  // Recursively search children
  for (const auto &child : component->getChildren()) {
    auto found = findComponent(child, predicate);
    if (found) {
      return found;
    }
  }

  return nullptr;
}

std::shared_ptr<Component>
EventHandler::findComponentById(unsigned int componentId) {
  if (!_root) {
    return nullptr;
  }

  return findComponent(
      _root, [componentId](auto comp) { return comp->getID() == componentId; });
}
