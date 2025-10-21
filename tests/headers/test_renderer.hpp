/*
 Mock utilities for renderer tests.
*/

#pragma once

#include "primitives/polygon.hpp"
#include "primitives/rectangle.hpp"
#include "primitives/text.hpp"
#include "primitives/triangle.hpp"
#include "renderer.hpp"
#include <memory>

class MockRenderer : public Renderer {
public:
  mutable int drawCallCount = 0; // optional: total draw() calls
  mutable int drawTextCallCount = 0;
  mutable int drawRectangleCallCount = 0;
  mutable int drawTriangleCallCount = 0;
  mutable int drawPolygonCallCount = 0;

  mutable std::shared_ptr<Text> lastText = nullptr;
  mutable std::shared_ptr<Rectangle> lastRectangle = nullptr;
  mutable std::shared_ptr<Triangle> lastTriangle = nullptr;
  mutable std::shared_ptr<Polygon> lastPolygon = nullptr;

  void draw(std::shared_ptr<Primitive> primitive) override {
    drawCallCount++;
    Renderer::draw(primitive);
  }

  void drawText(std::shared_ptr<Text> text) override {
    drawTextCallCount++;
    lastText = text;
  }

  void drawRectangle(std::shared_ptr<Rectangle> rectangle) override {
    drawRectangleCallCount++;
    lastRectangle = rectangle;
  }

  void drawTriangle(std::shared_ptr<Triangle> triangle) override {
    drawTriangleCallCount++;
    lastTriangle = triangle;
  }

  void drawPolygon(std::shared_ptr<Polygon> polygon) override {
    drawPolygonCallCount++;
    lastPolygon = polygon;
  }

  void reset() {
    drawCallCount = 0;
    drawTextCallCount = 0;
    drawRectangleCallCount = 0;
    drawTriangleCallCount = 0;
    drawPolygonCallCount = 0;
    lastText = nullptr;
    lastRectangle = nullptr;
    lastTriangle = nullptr;
    lastPolygon = nullptr;
  }
};

// Keep TestRenderer alias here for test files that include this header.
using TestRenderer = MockRenderer;
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
