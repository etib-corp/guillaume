# Guillaume

Guillaume is a component-based UI framework built around a primitive rendering system. It's designed for building user interfaces across XR platforms and traditional platforms with custom rendering backends and input handling. The framework is lightweight, flexible, and easy to integrate into existing projects.

## Key Features

- **Primitive-Based Rendering**: Components generate rendering primitives, enabling clean separation between UI logic and visual output
- **Flexible Renderer Architecture**: Support for multiple rendering backends (terminal, GUI, web, graphics APIs)
- **Component Hierarchy**: Tree-structured components with state management and event handling
- **Reactive Updates**: Event-driven state changes with explicit re-rendering
- **Extensible Primitive System**: Built-in primitives (Text, Rectangle, Polygon) with support for custom primitives

## Quick Start

```cpp
#include "application.hpp"
#include "label.hpp"
#include "button.hpp"

// Create custom renderer
class MyRenderer : public Renderer {
public:
    void draw(std::shared_ptr<Primitive> primitive) override {
        if (auto text = std::dynamic_pointer_cast<Text>(primitive)) {
            std::cout << "Text: " << text->getContent() << std::endl;
        }
        // Handle other primitive types...
    }
};

int main() {
    // Create application with renderer
    Application app(std::make_unique<MyRenderer>());

    // Build UI
    auto root = app.getRoot();
    root->addChild(std::make_shared<Label>("Hello Guillaume!"));

    // Run
    app.run();
    return 0;
}
```

## Documentation

- **[How Guillaume Works](documentations/HOW_GUILLAUME_WORKS.md)** - Comprehensive guide to concepts, lifecycle, and usage patterns
- **[Architecture Documentation](documentations/ARCHITECTURE.md)** - Detailed overview of the system's design and components
- **[Technical Choices Documentation](documentations/TECHNICAL_CHOICES.md)** - Insights into design decisions and technologies

## Building

```bash
cmake --build build --config Release
./build/guillaume  # Run the demo application
```
