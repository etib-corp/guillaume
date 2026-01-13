#include "simple_application.hpp"

#include <SDL3/SDL.h>
#include <properties/layout/margin.hpp>
#include <properties/layout/padding.hpp>
#include <properties/style/background_color.hpp>
#include <properties/style/opacity.hpp>

#include <iostream>

int main(void) {
  try {
    guillaume::simple_application::SimpleApplication application;
    application.addWindow("main");

    auto &window = application.getWindow("main");
    window.setTitle("Guillaume SDL3 Demo Window");
    window.setBoundingBox(utility::Rectangle<std::size_t>{0, 0, 1024, 768});

    // Configure main window with properties
    window
        .addProperty(std::make_unique<guillaume::properties::layout::Margin>(
            10, 10, 10, 10))
        .addProperty(
            std::make_unique<guillaume::properties::style::BackgroundColor>(
                utility::Color<uint8_t>(255, 255, 255)))
        .addProperty(
            std::make_unique<guillaume::properties::style::Opacity>(0.95f));

    // Create a header component
    auto header = std::make_shared<guillaume::Component>();
    header->setBoundingBox(utility::Rectangle<std::size_t>{0, 0, 1024, 100});
    header
        ->addProperty(
            std::make_unique<guillaume::properties::style::BackgroundColor>(
                utility::Color<uint8_t>(0, 0, 255)))
        .addProperty(std::make_unique<guillaume::properties::layout::Padding>(
            15, 15, 10, 10));
    window.addChild(header);

    // Create a content component
    auto content = std::make_shared<guillaume::Component>();
    content->setBoundingBox(utility::Rectangle<std::size_t>{0, 100, 1024, 580});
    content
        ->addProperty(
            std::make_unique<guillaume::properties::style::BackgroundColor>(
                utility::Color<uint8_t>(255, 255, 255)))
        .addProperty(std::make_unique<guillaume::properties::layout::Padding>(
            20, 20, 20, 20));
    window.addChild(content);

    // Create a sidebar component
    auto sidebar = std::make_shared<guillaume::Component>();
    sidebar->setBoundingBox(utility::Rectangle<std::size_t>{0, 100, 200, 580});
    sidebar
        ->addProperty(
            std::make_unique<guillaume::properties::style::BackgroundColor>(
                utility::Color<uint8_t>(0, 255, 0)))
        .addProperty(std::make_unique<guillaume::properties::layout::Padding>(
            10, 10, 10, 10));
    window.addChild(sidebar);

    // Create a footer component
    auto footer = std::make_shared<guillaume::Component>();
    footer->setBoundingBox(utility::Rectangle<std::size_t>{0, 680, 1024, 88});
    footer
        ->addProperty(
            std::make_unique<guillaume::properties::style::BackgroundColor>(
                utility::Color<uint8_t>(200, 200, 200)))
        .addProperty(std::make_unique<guillaume::properties::layout::Padding>(
            10, 10, 5, 5))
        .addProperty(
            std::make_unique<guillaume::properties::style::Opacity>(0.9f));
    window.addChild(footer);

    // Show the window
    window.show();

    std::cout << "Guillaume SDL3 Demo Window" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << "  M - Maximize window" << std::endl;
    std::cout << "  N - Minimize window" << std::endl;
    std::cout << "  R - Restore window" << std::endl;
    std::cout << "  SPACE - Change background color" << std::endl;

    // Get renderer for drawing
    auto &renderer = window.getRenderer();

    // Color cycle for demonstration
    std::vector<utility::Color<uint8_t>> colors = {
        {30, 30, 30, 255},  // Dark gray
        {50, 50, 100, 255}, // Dark blue
        {50, 100, 50, 255}, // Dark green
        {100, 50, 50, 255}, // Dark red
    };
    size_t colorIndex = 0;

    // Main event loop using SDL3
    bool running = true;
    SDL_Event event;

    while (running && !window.shouldClose()) {
      // Handle SDL events
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
          running = false;
          break;

        case SDL_EVENT_KEY_DOWN:
          switch (event.key.key) {
          case SDLK_ESCAPE:
            running = false;
            break;
          case SDLK_SPACE:
            colorIndex = (colorIndex + 1) % colors.size();
            std::cout << "Background color changed (index: " << colorIndex
                      << ")" << std::endl;
            break;
          case SDLK_M:
            window.maximize();
            break;
          case SDLK_N:
            window.minimize();
            break;
          case SDLK_R:
            window.restore();
            break;
          }
          break;

        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
          window.requestClose();
          break;
        }
      }

      // Clear with current background color
      renderer.setDrawColor(colors[colorIndex]);
      renderer.clear();

      // Draw header (blue rectangle)
      renderer.setDrawColor({0, 0, 255, 255});
      renderer.fillRect(utility::Rectangle<std::size_t>{0, 0, 1024, 100});

      // Draw content area (white rectangle)
      renderer.setDrawColor({255, 255, 255, 255});
      renderer.fillRect(utility::Rectangle<std::size_t>{0, 100, 1024, 580});

      // Draw sidebar (green rectangle)
      renderer.setDrawColor({0, 255, 0, 255});
      renderer.fillRect(utility::Rectangle<std::size_t>{0, 100, 200, 580});

      // Draw footer (gray rectangle)
      renderer.setDrawColor({200, 200, 200, 255});
      renderer.fillRect(utility::Rectangle<std::size_t>{0, 680, 1024, 88});

      // Draw some decorative elements
      renderer.setDrawColor({255, 255, 0, 255}); // Yellow
      renderer.drawLine({0, 100}, {1024, 100});  // Line under header
      renderer.drawLine({200, 100}, {200, 680}); // Line next to sidebar
      renderer.drawLine({0, 680}, {1024, 680});  // Line above footer

      // Present the rendered frame
      renderer.present();

      // Cap frame rate at ~60 FPS
      SDL_Delay(16);
    }

    std::cout << "Exiting Guillaume SDL3 Demo..." << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}