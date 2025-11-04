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

#include <iostream>
#include <memory>
#include <utility>

#include "container.hpp"
#include "event_handler.hpp"
#include "primitives/polygon.hpp"
#include "primitives/rectangle.hpp"
#include "primitives/text.hpp"
#include "primitives/triangle.hpp"
#include "renderer.hpp"

/**
 * @class Application
 * @brief Entry point of the application.
 *
 * The Application class holds the root component and manages the global
 * lifecycle, rendering, and event handling.
 */

template <typename RendererType, typename EventHandlerType>
class Application {
  static_assert(std::is_base_of<Renderer, RendererType>::value,
                "RendererType must be derived from Renderer");
  static_assert(std::is_base_of<EventHandler, EventHandlerType>::value,
                "EventHandlerType must be derived from EventHandler");

protected:
  std::shared_ptr<RendererType> _renderer; ///< Shared pointer to the renderer
  std::shared_ptr<EventHandlerType>
      _eventHandler; ///< Shared pointer to the event handler

private:
  std::shared_ptr<Container> _root; ///< The root container component
  bool _running = false;            ///< Tracks if the app is in the run loop

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
      // Dispatch to specific draw methods based on primitive type
      if (auto text = std::dynamic_pointer_cast<Text>(primitive)) {
        _renderer->drawText(text);
      } else if (auto rectangle =
                     std::dynamic_pointer_cast<Rectangle>(primitive)) {
        _renderer->drawRectangle(rectangle);
      } else if (auto triangle =
                     std::dynamic_pointer_cast<Triangle>(primitive)) {
        _renderer->drawTriangle(triangle);
      } else if (auto polygon = std::dynamic_pointer_cast<Polygon>(primitive)) {
        _renderer->drawPolygon(polygon);
      }
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
   */
  Application(void)
      : _renderer(nullptr), _eventHandler(nullptr),
        _root(std::make_shared<Container>()) {
    try {
      _renderer = std::make_shared<RendererType>();
      _eventHandler = std::make_shared<EventHandlerType>();
      // Set the root component reference in the event handler
      _eventHandler->setRoot(_root);
    } catch (std::exception &exception) {
      std::cerr << "Failed to create renderer or event handler: "
                << exception.what() << std::endl;
      throw std::runtime_error("Failed to create renderer or event handler");
    }

    _running = true;
  }

  /**
   * @brief Destroys the Application object.
   */
  ~Application(void) = default;

  /**
   * @brief Gets the renderer.
   *
   * @return std::shared_ptr<RendererType> The renderer
   */
  std::shared_ptr<RendererType> getRenderer(void) const { return _renderer; }

  /**
   * @brief Gets the event handler.
   *
   * @return std::shared_ptr<EventHandlerType> The event handler
   */
  std::shared_ptr<EventHandlerType> getEventHandler(void) const {
    return _eventHandler;
  }

  /**
   * @brief Gets the root container.
   *
   * @return std::shared_ptr<Container> The root container
   */
  std::shared_ptr<Container> getRoot(void) const {
    return _root;
  }

  /**
   * @brief Sets the root container.
   *
   * @param root The new root container
   */
  void setRoot(std::shared_ptr<Container> root) {
    _root = root;
    // Update the root reference in the event handler
    if (_eventHandler) {
      _eventHandler->setRoot(_root);
    }
  }

  /**
   * @brief Runs the application.
   *
   * This method starts the application lifecycle and performs a frame of the
   * main loop using the renderer: clear, render, draw, and present.
   * It also processes events from the event handler.
   * If you need continuous rendering, call this method in your own
   * loop or extend it to manage events and timing.
   */
  void run(void) {
    _running = true;

    // Process events before rendering
    if (_eventHandler) {
      _eventHandler->processEvents();
    }

    if (_renderer) {
      _renderer->clear();
    }

    // Compute the virtual tree for this frame
    if (_root) {
      _root->render();
    }
    // Draw the computed primitives/components
    drawTree(_root);

    if (_renderer) {
      _renderer->present();
    }
  }

  /**
   * @brief Updates the application state.
   *
   * This method is called to update the application and trigger re-rendering.
   * It processes events and re-renders the component tree.
   */
  void update(void) {
    // Process events
    if (_eventHandler) {
      _eventHandler->processEvents();
    }

    if (_eventHandler->shouldContinue() == false) {
      _running = false;
    }

    if (_renderer) {
      _renderer->clear();
    }

    // Update the root component and trigger re-render
    if (_root) {
      _root->render();
    }
    // Re-draw after the update
    drawTree(_root);

    if (_renderer) {
      _renderer->present();
    }
  }

  /**
   * @brief Stop the application run loop (if managed externally).
   */
  void stop(void) { _running = false; }

  /**
   * @brief Check if the application is running.
   *
   * @return true if running, false otherwise
   */
  bool isRunning(void) const { return _running; }
};
