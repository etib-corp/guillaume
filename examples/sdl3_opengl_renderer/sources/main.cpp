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
#include <typeinfo>

#include <SDL3/SDL.h>

#include "application.hpp"
#include "button.hpp"
#include "component.hpp"
#include "container.hpp"
#include "label.hpp"
#include "primitives/polygon.hpp"
#include "primitives/rectangle.hpp"
#include "primitives/text.hpp"
#include "primitives/triangle.hpp"
#include "renderer.hpp"

class MyRenderer : public Renderer {
private:
  std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> _window{
      nullptr, SDL_DestroyWindow};
  std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)> _renderer{
      nullptr, SDL_DestroyRenderer};

public:
  MyRenderer(void) : Renderer() {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
      std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
      throw std::runtime_error("SDL_Init failed");
    }

    try {
      _window = std::unique_ptr<SDL_Window, void (*)(SDL_Window *)>(
          SDL_CreateWindow("Guillaume", 800, 600,
                           SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE),
          SDL_DestroyWindow);
    } catch (std::exception &execption) {
      std::cerr << "Failed to create SDL window: " << execption.what()
                << std::endl;
      throw std::runtime_error("Failed to create SDL window");
    }

    try {
      _renderer = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)>(
          SDL_CreateRenderer(_window.get(), NULL), SDL_DestroyRenderer);
    } catch (std::exception &execption) {
      std::cerr << "Failed to create SDL renderer: " << execption.what()
                << std::endl;
      throw std::runtime_error("Failed to create SDL renderer");
    }

    std::cout << "Renderer initialized." << std::endl;
  }

  ~MyRenderer(void) {
    SDL_Quit();

    std::cout << "Renderer destroyed." << std::endl;
  }

  void clear(void) override { std::cout << "Renderer cleared." << std::endl; }

  void present(void) override {
    std::cout << "Renderer presented." << std::endl;
  }

  // Override specific draw methods for each primitive type
  void drawText(std::shared_ptr<Text> text) override {
    std::cout << "Drawing text: \"" << text->getContent()
              << "\" at 3D position (" << text->getPosition().getX() << ", "
              << text->getPosition().getY() << ", "
              << text->getPosition().getZ() << ")"
              << " rotation (" << text->getRotation().getX() << ", "
              << text->getRotation().getY() << ", "
              << text->getRotation().getZ() << ")"
              << " scale: " << text->getScale() << std::endl;
  }

  void drawRectangle(std::shared_ptr<Rectangle> rectangle) override {
    const auto &points = rectangle->getPoints();
    std::cout << "Drawing 3D rectangle with corners:" << std::endl;
    for (size_t i = 0; i < points.size(); ++i) {
      std::cout << "  Point " << i << ": (" << points[i].getX() << ", "
                << points[i].getY() << ", " << points[i].getZ() << ")"
                << std::endl;
    }
    auto normal = rectangle->calculateNormal();
    auto centroid = rectangle->calculateCentroid();
    std::cout << "  Surface normal: (" << normal.getX() << ", " << normal.getY()
              << ", " << normal.getZ() << ")" << std::endl;
    std::cout << "  Centroid: (" << centroid.getX() << ", " << centroid.getY()
              << ", " << centroid.getZ() << ")" << std::endl;
  }

  void drawTriangle(std::shared_ptr<Triangle> triangle) override {
    const auto &points = triangle->getPoints();
    std::cout << "Drawing 3D triangle with vertices:" << std::endl;
    for (size_t i = 0; i < points.size(); ++i) {
      std::cout << "  Vertex " << i << ": (" << points[i].getX() << ", "
                << points[i].getY() << ", " << points[i].getZ() << ")"
                << std::endl;
    }
    auto normal = triangle->calculateNormal();
    auto centroid = triangle->calculateCentroid();
    std::cout << "  Surface normal: (" << normal.getX() << ", " << normal.getY()
              << ", " << normal.getZ() << ")" << std::endl;
    std::cout << "  Centroid: (" << centroid.getX() << ", " << centroid.getY()
              << ", " << centroid.getZ() << ")" << std::endl;
    std::cout << "  Area: " << triangle->calculateArea() << std::endl;
  }

  void drawPolygon(std::shared_ptr<Polygon> polygon) override {
    const auto &points = polygon->getPoints();
    std::cout << "Drawing 3D polygon with " << points.size()
              << " vertices:" << std::endl;
    for (size_t i = 0; i < points.size(); ++i) {
      std::cout << "  Vertex " << i << ": (" << points[i].getX() << ", "
                << points[i].getY() << ", " << points[i].getZ() << ")"
                << std::endl;
    }
    auto normal = polygon->calculateNormal();
    auto centroid = polygon->calculateCentroid();
    std::cout << "  Surface normal: (" << normal.getX() << ", " << normal.getY()
              << ", " << normal.getZ() << ")" << std::endl;
    std::cout << "  Centroid: (" << centroid.getX() << ", " << centroid.getY()
              << ", " << centroid.getZ() << ")" << std::endl;
  }
};

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
      Point(0, 0, 5), Point(50, 0, 10), Point(25, 43, 15));

  auto polygonPrimitive = std::make_shared<Polygon>(
      std::vector<Point>{Point(0, 0, 8), Point(30, 0, 12), Point(40, 20, 16),
                         Point(20, 30, 20), Point(-10, 20, 18)});

  // Create a 3D rectangle with different Z coordinates
  auto rect3D =
      std::make_shared<Rectangle>(Point(10, 10, 5), Point(60, 40, 15));

  // Create text with 3D positioning, rotation, and scaling
  auto text3D =
      std::make_shared<Text>("3D Text Demo", Point(100, 50, 25),
                             Point(0.2f, 0.5f, 0.1f), // rotation in radians
                             1.5f                     // scale factor
      );

  root->addPrimitive(trianglePrimitive);
  root->addPrimitive(polygonPrimitive);
  root->addPrimitive(rect3D);
  root->addPrimitive(text3D);

  // Initial render pass
  std::cout << "\n-- Initial run --" << std::endl;
  application->run();

  // Simulate some user interactions and updates
  std::cout << "\n-- Simulate 3 clicks --" << std::endl;
  for (int i = 0; i < 3; ++i) {
    // Dispatch a synthetic click by directly calling the handler
    actionButton->onEvent(Event("click", actionButton));
    // Trigger application update and redraw
    application->update();
  }

  std::cout << "\n-- Reset --" << std::endl;
  resetButton->onEvent(Event("click", resetButton));
  application->update();

  std::cout << "\n-- One more click --" << std::endl;
  actionButton->onEvent(Event("click", actionButton));
  application->update();

  return EXIT_SUCCESS;
}