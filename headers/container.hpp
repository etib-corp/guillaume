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

#include "component.hpp"

/**
 * @class Container
 * @brief Specialized component to group other components.
 *
 * The Container class manages the hierarchy and recursive rendering of
 * children.
 */
class Container : public Component {
public:
  /**
   * @brief Constructs a Container object.
   */
  Container(void) : Component() {}

  /**
   * @brief Constructs a Container object with properties.
   *
   * @param properties The properties for the container
   */
  Container(const Properties &properties) : Component(properties) {}

  /**
   * @brief Destroys the Container object.
   */
  ~Container(void) override = default;

  /**
   * @brief Renders the container and its children recursively.
   *
   * @return std::shared_ptr<Component> Pointer to the rendered component
   */
  std::shared_ptr<Component> render(void) override {
    // Render all children recursively
    for (auto &child : _children) {
      if (child) {
        child->render();
      }
    }
    return shared_from_this();
  }

  /**
   * @brief Handles events for the container.
   *
   * Events are propagated to children if needed.
   *
   * @param event The event to handle
   */
  void onEvent(const Event &event) override {
    // Propagate event to all children
    for (auto &child : _children) {
      if (child) {
        child->onEvent(event);
      }
    }
  }
};
