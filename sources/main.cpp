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

#include <memory>

#include "application.hpp"
#include "button.hpp"
#include "component.hpp"
#include "container.hpp"
#include "label.hpp"
#include "renderer.hpp"

class MyRenderer : public Renderer {
public:
  MyRenderer(void) : Renderer() {}
  ~MyRenderer(void) override = default;

  // Override the draw method to provide custom rendering logic
  void draw(std::shared_ptr<Component> component) override {
    // Example: In a real implementation, this would draw the component
    // For now, this is a placeholder
    (void)component;
  }
};

#ifndef TESTING

int main(int argc, char *argv[], char *envp[]) {
  // Create the application with a renderer
  Application my_application(std::make_unique<MyRenderer>());

  // Get the root container
  auto root = my_application.getRoot();

  // Create some components to demonstrate the architecture
  auto label = std::make_shared<Label>("Hello, Guillaume!");
  auto button = std::make_shared<Button>("Click Me");

  // Add components to the root container
  root->addChild(label);
  root->addChild(button);

  // Run the application
  my_application.run();

  return 0;
}

#endif
