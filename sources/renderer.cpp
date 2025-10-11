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

#include "renderer.hpp"
#include "primitives/text.hpp"
#include "primitives/rectangle.hpp"
#include "primitives/triangle.hpp"
#include "primitives/polygon.hpp"
#include <memory>

void Renderer::draw(std::shared_ptr<Primitive> primitive) {
  // Dispatch to specific draw methods based on primitive type
  if (auto text = std::dynamic_pointer_cast<Text>(primitive)) {
    drawText(text);
  } else if (auto rectangle = std::dynamic_pointer_cast<Rectangle>(primitive)) {
    drawRectangle(rectangle);
  } else if (auto triangle = std::dynamic_pointer_cast<Triangle>(primitive)) {
    drawTriangle(triangle);
  } else if (auto polygon = std::dynamic_pointer_cast<Polygon>(primitive)) {
    drawPolygon(polygon);
  }
  // If no specific type matches, the base draw method does nothing
}
