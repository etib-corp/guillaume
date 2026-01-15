#include "simple_application.hpp"

#include <iostream>

namespace guillaume::simple_application {

SimpleApplication::SimpleApplication()
    : guillaume::Application<SimpleWindow, SimpleRenderer,
                             SimpleEventHandler>() {
  guillaume::Metadata metadata;
  metadata.setName("Guillaume Simple Application");
  metadata.setVersion("1.0.0");
  metadata.setIdentifier("guillaume.simple");
  metadata.setCreator("Guillaume example");
  metadata.setCopyright("(c) 2026");
  metadata.setUrl("https://example.com");
  metadata.setType("example");
  setMetadata(metadata);
}

} // namespace guillaume::simple_application