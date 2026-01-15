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

#include "component.hpp"

namespace guillaume {

void Component::setParent(std::shared_ptr<Component> parent) {
  _parent = std::move(parent);
}

void Component::processProperties(void) {
  // Sort properties by priority before applying
  std::sort(_properties.begin(), _properties.end(),
            [](const auto &a, const auto &b) { return a.first < b.first; });

  for (auto &property_pair : _properties) {
    property_pair.second->apply(*this);
  }
}

void Component::render(Renderer &renderer) {
  processProperties();

  // Only render visible components
  if (!_visible) {
    return;
  }

  // Fill the component's bounding box with its background color
  renderer.setDrawColor(_backgroundColor);
  renderer.fillRect(_boundingBox);

  // Render all children
  for (const auto &child : _children) {
    child->render(renderer);
  }
}

} // namespace guillaume