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

#include "event.hpp"
#include <functional>
#include <memory>
#include <vector>

// Forward declarations
class Component;
class ComponentTree;

/**
 * @class EventHandler
 * @brief Abstract interface for managing UI events across different backends.
 *
 * The EventHandler provides an interface for capturing, processing, and
 * dispatching events from different input sources (mouse, keyboard, touch,
 * etc.) to the component hierarchy. It can be extended to support different
 * event backends (e.g., terminal input, SDL events, web events, etc.).
 *
 * Similar to how Renderer abstracts the rendering backend, EventHandler
 * abstracts the event input backend, allowing the library to work with
 * different UI frameworks and platforms.
 */
class EventHandler {
protected:
  ComponentTree *_componentTree; ///< Pointer to the component tree

public:
  /**
   * @brief Constructs an EventHandler object.
   */
  EventHandler(void) : _componentTree(nullptr) {}

  /**
   * @brief Destroys the EventHandler object.
   */
  virtual ~EventHandler(void) = default;

  /**
   * @brief Sets the component tree for event dispatching.
   *
   * The component tree provides access to the entire component hierarchy
   * for event propagation and component lookup.
   *
   * @param tree Pointer to the component tree
   */
  void setComponentTree(ComponentTree *tree) { _componentTree = tree; }

  /**
   * @brief Gets the component tree.
   *
   * @return ComponentTree* Pointer to the component tree
   */
  ComponentTree *getComponentTree(void) const { return _componentTree; }

  /**
   * @brief Polls for events from the backend.
   *
   * This method should be overridden by derived classes to implement
   * backend-specific event polling logic (e.g., SDL_PollEvent, terminal input,
   * browser events). The implementation should capture raw events from the
   * backend and convert them to the library's Event objects.
   *
   * @return std::vector<Event> A vector of events captured from the backend
   */
  virtual std::vector<Event> pollEvents(void) = 0;

  /**
   * @brief Processes raw backend events and dispatches them to components.
   *
   * This method can be overridden by derived classes to implement custom
   * event processing logic before dispatching to components. The default
   * implementation calls pollEvents() and dispatches each event.
   *
   * Common use cases for overriding this method:
   * - Event filtering or validation
   * - Event transformation or normalization
   * - Event prioritization or throttling
   * - Gesture recognition from raw input events
   */
  virtual void processEvents(void) {
    auto events = pollEvents();
    for (const auto &event : events) {
      dispatchEvent(event);
    }
  }

  /**
   * @brief Dispatches an event to the appropriate component(s).
   *
   * This method walks the component hierarchy to find the target component
   * and propagates the event. The default implementation dispatches to the
   * event's target component, but can be overridden for custom dispatching
   * logic (e.g., event bubbling, capturing, or broadcast).
   *
   * @param event The event to dispatch
   */
  virtual void dispatchEvent(const Event &event);

  /**
   * @brief Checks if the handler should continue processing events.
   *
   * This method can be overridden to implement custom shutdown logic.
   * For example, checking for a "quit" event or application state.
   *
   * @return true if the handler should continue, false otherwise
   */
  virtual bool shouldContinue(void) const { return true; }

  /**
   * @brief Finds a component in the hierarchy by predicate.
   *
   * This utility method searches the component tree starting from the given
   * component, looking for a component that matches the predicate.
   *
   * @param component The starting component
   * @param predicate A function that returns true for the desired component
   * @return std::shared_ptr<Component> The found component, or nullptr
   */
  std::shared_ptr<Component>
  findComponent(std::shared_ptr<Component> component,
                std::function<bool(std::shared_ptr<Component>)> predicate);

  /**
   * @brief Finds a component in the hierarchy by ID.
   *
   * This utility method searches the component tree for a component
   * with the specified ID, starting from the root of the component tree.
   *
   * @param componentId The ID of the component to find
   * @return std::shared_ptr<Component> The found component, or nullptr
   */
  std::shared_ptr<Component> findComponentById(unsigned int componentId);

  /**
   * @brief Gets the root component from the component tree.
   *
   * Convenience method to access the root component.
   *
   * @return std::shared_ptr<Component> The root component, or nullptr if tree
   * is not set
   */
  std::shared_ptr<Component> getRoot(void) const;

protected:
  /**
   * @brief Propagates an event through the component hierarchy.
   *
   * This helper method recursively walks the component tree and calls
   * onEvent() on each component in the path from target to root.
   * Can be used by derived classes for custom event propagation strategies.
   *
   * @param component The current component
   * @param event The event to propagate
   */
  void propagateEvent(std::shared_ptr<Component> component, const Event &event);
};

/**
 * @class NoOpEventHandler
 * @brief Default event handler implementation that does nothing.
 *
 * This is used as a default template parameter for Application when
 * no specific event handler is needed. It implements the abstract
 * EventHandler interface with no-op methods.
 */
class NoOpEventHandler : public EventHandler {
public:
  /**
   * @brief Constructs a NoOpEventHandler object.
   */
  NoOpEventHandler(void) : EventHandler() {}

  /**
   * @brief Destroys the NoOpEventHandler object.
   */
  ~NoOpEventHandler(void) override = default;

  /**
   * @brief Returns an empty vector of events (no-op implementation).
   *
   * @return std::vector<Event> An empty vector
   */
  std::vector<Event> pollEvents(void) override { return std::vector<Event>(); }
};
