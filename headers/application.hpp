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
#include "renderer.hpp"
#include <memory>

/**
 * @class Application
 * @brief Entry point of the application.
 *
 * The Application class holds the root component and manages the global
 * lifecycle.
 */
class Application {
private:
  std::unique_ptr<Renderer> _renderer; ///< Unique pointer to the renderer
  std::shared_ptr<Container> _root;    ///< The root container component

protected:
  /**
   * @brief Recursively draw the component tree using the renderer.
   *
   * This walks the component hierarchy starting from the given component,
   * draws all primitives in each component, and then recurses to children.
   *
   * @param component The starting component to draw
   */
  void drawTree(const std::shared_ptr<Component> &component) {
    if (!_renderer || !component)
      return;

    // Draw all primitives in this component
    for (const auto &primitive : component->getPrimitives()) {
      _renderer->draw(primitive);
    }

    // Then recursively draw all children
    for (const auto &child : component->getChildren()) {
      drawTree(child);
    }
  }
public:
  /**
   * @brief Constructs an Application object.
   *
   * @param renderer Unique pointer to the renderer
   */
  Application(std::unique_ptr<Renderer> renderer)
      : _renderer(std::move(renderer)), _root(std::make_shared<Container>()) {}

  /**
   * @brief Destroys the Application object.
   */
  ~Application(void) = default;

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
   * @brief Runs the application.
   *
   * This method starts the application lifecycle and enters the main loop.
   */
  void run(void) {
    // Initialize the application
    if (_root) {
      _root->render();
      // After computing the virtual tree, draw it with the renderer
      drawTree(_root);
    }

    // Main application loop would go here
    // For now, this is a placeholder for the actual implementation
  }

  /**
   * @brief Updates the application state.
   *
   * This method is called to update the application and trigger re-rendering.
   */
  void update(void) {
    // Update the root component and trigger re-render
    if (_root) {
      _root->render();
      // Re-draw after the update
      drawTree(_root);
    }
  }

  /**
   * @brief Gets the renderer.
   *
   * @return Renderer* Pointer to the renderer
   */
  Renderer *getRenderer(void) const { return _renderer.get(); }
};
