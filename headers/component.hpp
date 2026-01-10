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

#pragma once

#include <memory>
#include <vector>

#include <rectangle.hpp>

namespace guillaume {

/**
 * @brief Component base class.
 */
class Component {
private:
  utility::Rectangle<float> _boundingBox; ///< Component bounding box

public:
  /**
   * @brief Default destructor
   */
  virtual ~Component(void) = default;

  /**
   * @brief Set the bounding box of the component.
   * @param boundingBox The bounding box as a Rectangle<float>.
   */
  void setBoundingBox(const utility::Rectangle<float> &boundingBox) {
    _boundingBox = boundingBox;
  }

  /**
   * @brief Get the bounding box of the component.
   * @return The bounding box as a Rectangle<float>.
   */
  utility::Rectangle<float> getBoundingBox(void) const { return _boundingBox; }

  /**
   * @brief Render the component.
   */
  virtual void render(void) = 0;
};

} // namespace guillaume
