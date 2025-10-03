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

#include <memory>

// Forward declaration
class Component;

/**
 * @class Renderer
 * @brief Represents the renderer of an Application.
 *
 * The Renderer is responsible for rendering components to the screen.
 * It can be extended to support different rendering backends
 * (e.g., terminal, GUI, web).
 */
class Renderer {
private:
protected:
public:
  /**
   * @brief Constructs a Renderer object.
   */
  Renderer(void) {}

  /**
   * @brief Destroys the Renderer object.
   */
  virtual ~Renderer(void) = default;

  /**
   * @brief Draws a component to the screen.
   *
   * This is a virtual method that should be overridden by derived classes
   * to implement specific rendering logic for different backends.
   *
   * @param component The component to render
   */
  virtual void draw(std::shared_ptr<Component> component) {
    // Default implementation does nothing
    // Derived classes should override this method
    (void)component;
  }
};
