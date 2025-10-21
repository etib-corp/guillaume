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

// Forward declarations
class Component;
class Primitive;
class Text;
class Rectangle;
class Triangle;
class Polygon;

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
   * @brief Clears the rendering target.
   *
   * This method is called to clear the current rendering target
   * (e.g., the screen or a texture) before drawing new content.
   */
  virtual void clear(void) {}

  /**
   * @brief Presents the rendered content to the display.
   *
   * This method is called after all drawing commands have been issued
   * to present the final rendered content to the screen.
   */
  virtual void present(void) {}

  /**
   * @brief Draws a primitive to the screen.
   *
   * This method dispatches to the appropriate specific draw method based on
   * the primitive type. Derived classes can override either this method or
   * the specific draw methods (drawText, drawRectangle, etc.).
   *
   * @param primitive The primitive to render
   */
  virtual void draw(std::shared_ptr<Primitive> primitive) = 0;

  /**
   * @brief Draws a text primitive in 3D space.
   *
   * This is a virtual method that can be overridden by derived classes
   * to implement specific 3D text rendering logic for different backends.
   * The text includes 3D position, rotation, and scale information.
   *
   * @param text The text primitive to render in 3D space
   */
  virtual void drawText(std::shared_ptr<Text> text) = 0;

  /**
   * @brief Draws a rectangle primitive in 3D space.
   *
   * This is a virtual method that can be overridden by derived classes
   * to implement specific 3D rectangle rendering logic for different backends.
   * The rectangle maintains full 3D positioning for all corners.
   *
   * @param rectangle The rectangle primitive to render in 3D space
   */
  virtual void drawRectangle(std::shared_ptr<Rectangle> rectangle) = 0;

  /**
   * @brief Draws a triangle primitive in 3D space.
   *
   * This is a virtual method that can be overridden by derived classes
   * to implement specific 3D triangle rendering logic for different backends.
   * The triangle includes surface normal calculation and area computation.
   *
   * @param triangle The triangle primitive to render in 3D space
   */
  virtual void drawTriangle(std::shared_ptr<Triangle> triangle) = 0;
  /**
   * @brief Draws a polygon primitive in 3D space.
   *
   * This is a virtual method that can be overridden by derived classes
   * to implement specific 3D polygon rendering logic for different backends.
   * The polygon includes surface normal calculation and centroid computation.
   *
   * @param polygon The polygon primitive to render in 3D space
   */
  virtual void drawPolygon(std::shared_ptr<Polygon> polygon) = 0;
};
