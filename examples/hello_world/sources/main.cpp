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

#include <iostream>
#include <memory>

#include "application.hpp"
#include "label.hpp"
#include "primitives/polygon.hpp"
#include "primitives/rectangle.hpp"
#include "primitives/text.hpp"
#include "primitives/triangle.hpp"
#include "renderer.hpp"

class SimpleRenderer : public Renderer {
public:
  SimpleRenderer(void) : Renderer() {
    std::cout << "Simple renderer initialized." << std::endl;
  }

  ~SimpleRenderer(void) {
    std::cout << "Simple renderer destroyed." << std::endl;
  }

  void clear(void) override {
    std::cout << "--- Rendering Frame ---" << std::endl;
  }

  void present(void) override { std::cout << "--- End Frame ---" << std::endl; }

  void draw(std::shared_ptr<Primitive> primitive) override {
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
  }

  void drawText(std::shared_ptr<Text> text) override {
    std::cout << "Rendering text: \"" << text->getContent() << "\""
              << std::endl;
  }

  // Minimal implementations for other primitive types
  void drawRectangle(std::shared_ptr<Rectangle> rectangle) override {}
  void drawTriangle(std::shared_ptr<Triangle> triangle) override {}
  void drawPolygon(std::shared_ptr<Polygon> polygon) override {}
};

int main() {
  // Create the application with our simple renderer
  std::unique_ptr<Application<SimpleRenderer>> application = nullptr;

  try {
    application = std::make_unique<Application<SimpleRenderer>>();
  } catch (std::exception &exception) {
    std::cerr << "Failed to create Application: " << exception.what()
              << std::endl;
    return EXIT_FAILURE;
  }

  // Get the root container
  auto root = application->getRoot();

  // Create a simple label
  auto helloLabel = std::make_shared<Label>("Hello, Guillaume!");

  // Add it to the root
  root->addChild(helloLabel);

  // Run the application
  std::cout << "Starting Hello World example..." << std::endl;
  application->run();

  std::cout << "Hello World example completed!" << std::endl;
  return EXIT_SUCCESS;
}
