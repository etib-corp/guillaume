#include "simple_application.hpp"

#include <properties/layout/margin.hpp>
#include <properties/layout/padding.hpp>
#include <properties/style/background_color.hpp>
#include <properties/style/opacity.hpp>

#include <iostream>

int main(void) {
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

  while (application.hasOpenWindows() && !application.shouldQuit()) {
    application.routine();
  }

  return 0;
}
