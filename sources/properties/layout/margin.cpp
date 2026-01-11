/*
 Copyright (c) 2026 ETIB Corporation

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

#include "properties/layout/margin.hpp"

#include "component.hpp"

namespace guillaume::properties::layout {

void Margin::apply(Component &component) {
  auto boundingBox = component.getBoundingBox();

  // Margins shrink the component by adding space around it
  // Adjust position (move inward by left/top margins)
  auto newX = boundingBox.x() + _left;
  auto newY = boundingBox.y() + _top;

  // Adjust size (reduce by left+right and top+bottom margins)
  auto currentWidth = boundingBox.width();
  auto currentHeight = boundingBox.height();

  // Ensure margins don't exceed available space
  auto totalHorizontalMargin = _left + _right;
  auto totalVerticalMargin = _top + _bottom;

  auto newWidth = (currentWidth > totalHorizontalMargin)
                      ? currentWidth - totalHorizontalMargin
                      : 0;
  auto newHeight = (currentHeight > totalVerticalMargin)
                       ? currentHeight - totalVerticalMargin
                       : 0;

  // Update the bounding box
  component.setBoundingBox(
      utility::Rectangle<std::size_t>(newX, newY, newWidth, newHeight));
}

} // namespace guillaume::properties::layout
