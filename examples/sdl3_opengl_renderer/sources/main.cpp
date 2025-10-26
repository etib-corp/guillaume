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

#include <SDL3/SDL.h>

#include "application.hpp"
#include "button.hpp"
#include "component.hpp"
#include "container.hpp"
#include "label.hpp"
#include "point.hpp"
#include "vertex.hpp"

#include "my_renderer.hpp"

int main(int argc, char *argv[], char *envp[]) {
  // Create the application with a renderer
  std::unique_ptr<Application<MyRenderer>> application = nullptr;

  try {
    application = std::make_unique<Application<MyRenderer>>();
  } catch (std::exception &exception) {
    std::cerr << "Failed to create Application: " << exception.what()
              << std::endl;
    return EXIT_FAILURE;
  }

  // Get the root container
  auto root = application->getRoot();

  // Create a nested component tree
  auto header = std::make_shared<Container>();
  auto content = std::make_shared<Container>();
  auto footer = std::make_shared<Container>();

  // Header components
  auto titleLabel = std::make_shared<Label>("Guillaume UI Demo");
  auto subtitleLabel = std::make_shared<Label>("A tiny reactive hierarchy");

  // Content components
  auto infoLabel = std::make_shared<Label>("Clicks: 0");
  auto actionButton = std::make_shared<Button>("Increment");
  auto resetButton = std::make_shared<Button>("Reset");

  // Footer components
  auto statusLabel = std::make_shared<Label>("Status: idle");

  // Compose tree
  header->addChild(titleLabel);
  header->addChild(subtitleLabel);

  content->addChild(infoLabel);
  content->addChild(actionButton);
  content->addChild(resetButton);

  footer->addChild(statusLabel);

  root->addChild(header);
  root->addChild(content);
  root->addChild(footer);

  // Local counter state simulated via Label + State inside it
  int counter = 0;

  // Wire up button behaviors by mutating label states
  actionButton->setOnClick([&]() {
    counter += 1;
    infoLabel->setText("Clicks: " + std::to_string(counter));
    statusLabel->setText("Status: incremented");
  });

  resetButton->setOnClick([&]() {
    counter = 0;
    infoLabel->setText("Clicks: 0");
    statusLabel->setText("Status: reset");
  });

  // Add 3D primitives to demonstrate all draw methods with depth
  auto trianglePrimitive = std::make_shared<Triangle>(
      Point(-0.5f, -0.5f, 0.0f),  // Bottom-left
      Point(0.0f, 0.5f, 0.0f),    // Top
      Point(0.5f, -0.5f, 0.0f)    // Bottom-right
  );
  auto trianglePos = Vector<float, 3>({-0.3f, -0.5f, 0.0f});
  trianglePrimitive->setTranslation(trianglePos);

  std::vector<Vertex> polygonVertices = {
      Vertex(Point(0.5f, 0.0f, 0.0f), Color(0.0f, 1.0f, 0.0f)), // Bottom-left
      Vertex(Point(0.25f, 0.433f, 0.0f),
             Color(0.0f, 1.0f, 0.0f)), // Bottom-right
      Vertex(Point(-0.25f, 0.433f, 0.0f), Color(0.0f, 1.0f, 0.0f)), // Right
      Vertex(Point(-0.5f, 0.0f, 0.0f), Color(0.0f, 1.0f, 0.0f)),    // Top-right
      Vertex(Point(-0.25f, -0.433f, 0.0f), Color(0.0f, 1.0f, 0.0f)), // Top-left
      Vertex(Point(0.25f, -0.433f, 0.0f), Color(0.0f, 1.0f, 0.0f)),  // Left
  };

  auto polygonPrimitive = std::make_shared<Polygon>(polygonVertices);
  auto polygonPos = Vector<float, 3>({-0.5f, 0.5f, 0.0f});
  polygonPrimitive->setTranslation(polygonPos);

  // Create a 3D rectangle with different Z coordinates
  auto rect3D = std::make_shared<Rectangle>();
  auto rect3DPos = Vector<float, 3>({0.5f, 0.0f, 0.0f});
  rect3D->setTranslation(rect3DPos);

  // Create text with 3D positioning, rotation, and scaling
  auto text3D =
      std::make_shared<Text>("3D Text Demo", Point(100, 50, 25),
                             Point(0.2f, 0.5f, 0.1f), // rotation in radians
                             1.5f                     // scale factor
      );

  root->addPrimitive(trianglePrimitive);
  root->addPrimitive(polygonPrimitive);
  root->addPrimitive(rect3D);
  // root->addPrimitive(text3D);

  // Initial render pass
  std::cout << "\n-- Initial run --" << std::endl;
  SDL_Event event;
  bool running = true;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }
    application->update();
  }

  // Simulate some user interactions and updates
  std::cout << "\n-- Simulate 3 clicks --" << std::endl;
  for (int i = 0; i < 3; ++i) {
    // Dispatch a synthetic click by directly calling the handler
    // actionButton->onEvent(Event("click", actionButton));
    // Trigger application update and redraw
    application->update();
  }

  std::cout << "\n-- Reset --" << std::endl;
  // resetButton->onEvent(Event("click", resetButton));
  application->update();

  std::cout << "\n-- One more click --" << std::endl;
  // actionButton->onEvent(Event("click", actionButton));
  application->update();

  return EXIT_SUCCESS;
}