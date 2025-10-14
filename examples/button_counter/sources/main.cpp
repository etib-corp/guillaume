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
#include "button.hpp"
#include "label.hpp"
#include "container.hpp"
#include "renderer.hpp"
#include "primitives/text.hpp"
#include "primitives/rectangle.hpp"

class CounterRenderer : public Renderer {
public:
  CounterRenderer(void) : Renderer() {
    std::cout << "Counter renderer initialized." << std::endl;
  }

  ~CounterRenderer(void) {
    std::cout << "Counter renderer destroyed." << std::endl;
  }

  void clear(void) override {
    std::cout << "\n=== UI Update ===" << std::endl;
  }

  void present(void) override {
    std::cout << "=== End Update ===\n" << std::endl;
  }

  void drawText(std::shared_ptr<Text> text) override {
    std::cout << "Text: " << text->getContent() << std::endl;
  }

  void drawRectangle(std::shared_ptr<Rectangle> rectangle) override {
    std::cout << "Button background rendered" << std::endl;
  }

  // Minimal implementations for other primitive types
  void drawTriangle(std::shared_ptr<Triangle> triangle) override {}
  void drawPolygon(std::shared_ptr<Polygon> polygon) override {}
};

int main() {
  // Create the application with our counter renderer
  std::unique_ptr<Application<CounterRenderer>> application = nullptr;

  try {
    application = std::make_unique<Application<CounterRenderer>>();
  } catch (std::exception &exception) {
    std::cerr << "Failed to create Application: " << exception.what() << std::endl;
    return EXIT_FAILURE;
  }

  // Get the root container
  auto root = application->getRoot();

  // Create UI components
  auto titleLabel = std::make_shared<Label>("Button Counter Example");
  auto counterLabel = std::make_shared<Label>("Count: 0");
  auto incrementButton = std::make_shared<Button>("Increment");
  auto decrementButton = std::make_shared<Button>("Decrement");
  auto resetButton = std::make_shared<Button>("Reset");

  // Add components to root
  root->addChild(titleLabel);
  root->addChild(counterLabel);
  root->addChild(incrementButton);
  root->addChild(decrementButton);
  root->addChild(resetButton);

  // Counter state
  int counter = 0;

  // Set up button event handlers
  incrementButton->setOnClick([&]() {
    counter++;
    counterLabel->setText("Count: " + std::to_string(counter));
  });

  decrementButton->setOnClick([&]() {
    counter--;
    counterLabel->setText("Count: " + std::to_string(counter));
  });

  resetButton->setOnClick([&]() {
    counter = 0;
    counterLabel->setText("Count: 0");
  });

  // Initial render
  std::cout << "Starting Button Counter example..." << std::endl;
  application->run();

  // Simulate user interactions
  std::cout << "\n--- Simulating button clicks ---" << std::endl;

  incrementButton->onEvent(Event("click", incrementButton));
  application->update();

  incrementButton->onEvent(Event("click", incrementButton));
  application->update();

  decrementButton->onEvent(Event("click", decrementButton));
  application->update();

  incrementButton->onEvent(Event("click", incrementButton));
  application->update();

  resetButton->onEvent(Event("click", resetButton));
  application->update();

  std::cout << "Button Counter example completed!" << std::endl;
  return EXIT_SUCCESS;
}
