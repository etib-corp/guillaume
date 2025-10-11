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
#include "primitive.hpp"
#include "properties.hpp"
#include "state.hpp"
#include <memory>
#include <vector>

/**
 * @class Component
 * @brief Base class for all UI components.
 *
 * Manages local state and properties, defines rendering and event handling.
 */
class Component : public std::enable_shared_from_this<Component> {
private:
  typedef unsigned int ComponentID; ///< Type definition for ComponentID

  ComponentID _id; ///< The unique identifier for the component

  ComponentID generateID(void) const {
    static ComponentID currentID = 0;
    return ++currentID;
  }

protected:
  State _state;                       ///< The component's state
  Properties _properties;             ///< The component's properties
  std::shared_ptr<Component> _parent; ///< Pointer to the parent component
  std::vector<std::shared_ptr<Component>>
      _children; ///< Vector of child components
  std::vector<std::shared_ptr<Primitive>>
      _primitives; ///< Vector of primitives for rendering

  /**
   * @brief Set the Parent object
   *
   * @param parent A shared pointer to the parent component
   */
  void setParent(const std::shared_ptr<Component> &parent) { _parent = parent; }

public:
  /**
   * @brief Constructs a Component object.
   */
  Component(void) : _id(generateID()), _state(), _properties() {
    // Note: State change callback is not set here to avoid bad_weak_ptr
    // when Component is created on the stack. Use setStateChangeCallback()
    // if needed when Component is managed by shared_ptr.
  }

  /**
   * @brief Constructs a Component object with properties.
   *
   * @param properties The properties for the component
   */
  Component(const Properties &properties)
      : _id(generateID()), _state(), _properties(properties) {
    // Note: State change callback is not set here to avoid bad_weak_ptr
    // when Component is created on the stack. Use setStateChangeCallback()
    // if needed when Component is managed by shared_ptr.
  }

  /**
   * @brief Destroy the Component object
   *
   */
  virtual ~Component(void) = default;

  /**
   * @brief Get the ID object
   *
   * @return ComponentID
   */
  ComponentID getID(void) const { return _id; }

  /**
   * @brief Add a child component
   *
   * @param child A shared pointer to the child component
   */
  virtual void addChild(const std::shared_ptr<Component> &child) {
    child->setParent(shared_from_this());
    _children.push_back(child);
  }

  /**
   * @brief Add a primitive for rendering
   *
   * @param primitive A shared pointer to the primitive
   */
  virtual void addPrimitive(const std::shared_ptr<Primitive> &primitive) {
    _primitives.push_back(primitive);
  }

  /**
   * @brief Renders the component.
   *
   * This method should be overridden by derived classes to implement
   * their specific rendering logic.
   *
   * @return std::shared_ptr<Component> Pointer to the rendered component
   */
  virtual std::shared_ptr<Component> render(void) {
    // Default implementation returns itself
    return shared_from_this();
  }

  /**
   * @brief Handles events.
   *
   * This method should be overridden by derived classes to implement
   * their specific event handling logic.
   *
   * @param event The event to handle
   */
  virtual void onEvent(const Event &event) {
    // Default implementation does nothing
    (void)event;
  }

  /**
   * @brief Gets the component's state.
   *
   * @return State& Reference to the component's state
   */
  State &getState(void) { return _state; }

  /**
   * @brief Gets the component's properties.
   *
   * @return const Properties& Reference to the component's properties
   */
  const Properties &getProperties(void) const { return _properties; }

  /**
   * @brief Gets the component's children.
   *
   * @return const std::vector<std::shared_ptr<Component>>& Reference to the
   * children vector
   */
  const std::vector<std::shared_ptr<Component>> &getChildren(void) const {
    return _children;
  }

  /**
   * @brief Gets the component's parent.
   *
   * @return std::shared_ptr<Component> Pointer to the parent component
   */
  std::shared_ptr<Component> getParent(void) const { return _parent; }

  /**
   * @brief Gets the component's primitives.
   *
   * @return const std::vector<std::shared_ptr<Primitive>>& Reference to the
   * primitives vector
   */
  const std::vector<std::shared_ptr<Primitive>> &getPrimitives(void) const {
    return _primitives;
  }
};
