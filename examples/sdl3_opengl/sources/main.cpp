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

#include "my_event_handler.hpp"
#include "my_renderer.hpp"

int main(int argc, char *argv[], char *envp[]) {
  (void)argc; // Unused
  (void)argv; // Unused
  (void)envp; // Unused

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) == false) {
    std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    return EXIT_FAILURE;
  }

  // Create SDL window
  std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> window(
      SDL_CreateWindow("Guillaume UI Demo", 800, 600,
                       SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE),
      SDL_DestroyWindow);

  if (!window) {
    std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return EXIT_FAILURE;
  }

  // Create the application with renderer and event handler
  std::unique_ptr<Application<MyRenderer, Guillaume::MyEventHandler>>
      application = nullptr;

  try {
    application =
        std::make_unique<Application<MyRenderer, Guillaume::MyEventHandler>>();
  } catch (std::exception &exception) {
    std::cerr << "Failed to create Application: " << exception.what()
              << std::endl;
    window.reset();
    SDL_Quit();
    return EXIT_FAILURE;
  }

  // Set the window on both renderer and event handler
  auto renderer = application->getRenderer();
  auto eventHandler = application->getEventHandler();

  renderer->setWindow(window.get());
  eventHandler->setWindow(window.get());

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
  auto multiplyButton = std::make_shared<Button>("x2");
  auto decrementButton = std::make_shared<Button>("Decrement");
  auto addTenButton = std::make_shared<Button>("+10");

  // Footer components
  auto statusLabel = std::make_shared<Label>("Status: idle");
  auto eventCountLabel = std::make_shared<Label>("Total Events: 0");

  // Compose tree
  header->addChild(titleLabel);
  header->addChild(subtitleLabel);

  content->addChild(infoLabel);
  content->addChild(actionButton);
  content->addChild(decrementButton);
  content->addChild(multiplyButton);
  content->addChild(addTenButton);
  content->addChild(resetButton);

  footer->addChild(statusLabel);
  footer->addChild(eventCountLabel);

  root->addChild(header);
  root->addChild(content);
  root->addChild(footer);

  // Local counter state simulated via Label + State inside it
  int counter = 0;
  int totalEvents = 0;

  // Helper function to update event count
  auto updateEventCount = [&]() {
    totalEvents++;
    eventCountLabel->setText("Total Events: " + std::to_string(totalEvents));
  };

  // Wire up button behaviors by mutating label states
  actionButton->setOnClick([&]() {
    counter += 1;
    infoLabel->setText("Clicks: " + std::to_string(counter));
    statusLabel->setText("Status: incremented (+1)");
    updateEventCount();
    std::cout << "Increment button clicked! Counter = " << counter << std::endl;
  });

  decrementButton->setOnClick([&]() {
    counter -= 1;
    infoLabel->setText("Clicks: " + std::to_string(counter));
    statusLabel->setText("Status: decremented (-1)");
    updateEventCount();
    std::cout << "Decrement button clicked! Counter = " << counter << std::endl;
  });

  multiplyButton->setOnClick([&]() {
    counter *= 2;
    infoLabel->setText("Clicks: " + std::to_string(counter));
    statusLabel->setText("Status: multiplied (x2)");
    updateEventCount();
    std::cout << "Multiply button clicked! Counter = " << counter << std::endl;
  });

  addTenButton->setOnClick([&]() {
    counter += 10;
    infoLabel->setText("Clicks: " + std::to_string(counter));
    statusLabel->setText("Status: added ten (+10)");
    updateEventCount();
    std::cout << "+10 button clicked! Counter = " << counter << std::endl;
  });

  resetButton->setOnClick([&]() {
    counter = 0;
    infoLabel->setText("Clicks: 0");
    statusLabel->setText("Status: reset to 0");
    updateEventCount();
    std::cout << "Reset button clicked! Counter reset to 0" << std::endl;
  });

  // Main event loop - process events and render continuously
  while (application->isRunning()) {
    application->update(); // Processes events and renders
  }

  std::cout << "\nApplication closed." << std::endl;
  std::cout << "Final counter: " << counter << std::endl;
  std::cout << "Total events processed: " << totalEvents << std::endl;

  // Cleanup
  window.reset();
  SDL_Quit();

  return EXIT_SUCCESS;
}