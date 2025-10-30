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

#include "container.hpp"
#include <memory>

/**
 * @class ComponentTree
 * @brief Encapsulates the root component of the component hierarchy.
 *
 * The ComponentTree class manages the root container component and provides
 * access to the component hierarchy.
 */
class ComponentTree {
private:
  std::shared_ptr<Container> _root; ///< The root container component

public:
  /**
   * @brief Constructs a ComponentTree object.
   */
  ComponentTree(void) : _root(std::make_shared<Container>()) {}

  /**
   * @brief Constructs a ComponentTree object with a root container.
   *
   * @param root The root container component
   */
  ComponentTree(std::shared_ptr<Container> root) : _root(root) {}

  /**
   * @brief Destroys the ComponentTree object.
   */
  ~ComponentTree(void) = default;

  /**
   * @brief Gets the root container.
   *
   * @return std::shared_ptr<Container> The root container
   */
  std::shared_ptr<Container> getRoot(void) const { return _root; }

  /**
   * @brief Sets the root container.
   *
   * @param root The new root container
   */
  void setRoot(std::shared_ptr<Container> root) { _root = root; }

  /**
   * @brief Renders the component tree.
   *
   * Triggers rendering of the root component and its children.
   */
  void render(void) {
    if (_root) {
      _root->render();
    }
  }
};
