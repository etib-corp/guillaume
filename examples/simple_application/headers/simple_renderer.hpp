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

#include <color.hpp>
#include <rectangle.hpp>
#include <renderer.hpp>
#include <standard_logger.hpp>
#include <vector.hpp>

namespace guillaume::simple_application {

class SimpleRenderer : public guillaume::Renderer {
private:
  utility::StandardLogger _logger;
  utility::Color<uint8_t> _drawColor;
  utility::Vector<float, 2> _scale;

public:
  SimpleRenderer(void);
  ~SimpleRenderer(void) override = default;

  void clear(void) override;
  void present(void) override;
  void setDrawColor(utility::Color<uint8_t> color) override;
  utility::Color<uint8_t> getDrawColor(void) const override;
  void drawPoint(utility::Vector<float, 2> point) override;
  void drawLine(utility::Vector<float, 2> start,
                utility::Vector<float, 2> end) override;
  void drawRect(utility::Rectangle<float> rectangle) override;
  void fillRect(utility::Rectangle<float> rectangle) override;
  void setScale(utility::Vector<float, 2> scale) override;
  utility::Vector<float, 2> getScale(void) const override;
  bool flush(void) override;
};

} // namespace guillaume::simple_application
