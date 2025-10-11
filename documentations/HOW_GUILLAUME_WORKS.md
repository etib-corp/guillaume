# How Guillaume Works

Guillaume is a component-based UI framework built around a **3D primitive rendering system**. This guide explains the core concepts, lifecycle, and usage patterns.

## 3D Rendering Capabilities ✨

Guillaume primitives are designed for **3D environments** with full support for:

- **3D Positioning**: All primitives use 3D coordinates (x, y, z)
- **Surface Normals**: Automatic calculation for lighting and back-face culling
- **Geometric Properties**: Area calculation, centroids, and spatial relationships
- **3D Text Rendering**: Position, rotation, and scaling in 3D space

## Core Concepts

### 1. Components and Hierarchy

Guillaume organizes UI elements in a tree structure where each component can contain both child components and rendering primitives:

```cpp
// Create a nested component structure
auto root = std::make_shared<Container>();
auto header = std::make_shared<Container>();
auto titleLabel = std::make_shared<Label>("My Application");

header->addChild(titleLabel);
root->addChild(header);
```

### 2. Primitive-Based Rendering

Instead of rendering components directly, Guillaume uses a two-phase approach:

1. **Component Render Phase**: Components generate primitives based on their current state
2. **Primitive Draw Phase**: The renderer draws each primitive to the output

```cpp
// Label generates a Text primitive during render()
std::shared_ptr<Component> Label::render() {
    _primitives.clear();
    auto textPrimitive = std::make_shared<Text>(_text, Point(0, 0, 0));
    _primitives.push_back(textPrimitive);
    return shared_from_this();
}
```

### 3. State Management

Components manage their own state and can be updated dynamically:

```cpp
auto label = std::make_shared<Label>("Initial Text");
label->setText("Updated Text");  // Updates internal state
app.update();  // Triggers re-render and redraw
```

## Application Lifecycle

### 1. Initialization

```cpp
// Create application with a custom renderer
Application app(std::make_unique<MyRenderer>());

// Build component tree
auto root = app.getRoot();
root->addChild(std::make_shared<Label>("Hello World"));
```

### 2. Initial Render

```cpp
app.run();  // Calls render() on all components, then draws primitives
```

The `run()` method:

1. Calls `render()` on the root component
2. Recursively renders all child components
3. Traverses the component tree and draws all generated primitives

### 3. Updates and Re-rendering

```cpp
// Modify component state
label->setText("New Text");

// Trigger update cycle
app.update();  // Re-renders components and redraws primitives
```

## Event Handling

### 1. Event Dispatch

Events are dispatched to components and can trigger state changes:

```cpp
auto button = std::make_shared<Button>("Click Me");
button->setOnClick([]() {
    std::cout << "Button clicked!" << std::endl;
});

// Simulate event
button->onEvent(Event("click", button));
```

### 2. Event-Driven Updates

Typical pattern for interactive components:

```cpp
auto counter = 0;
auto countLabel = std::make_shared<Label>("Count: 0");
auto incrementButton = std::make_shared<Button>("Increment");

incrementButton->setOnClick([&]() {
    counter++;
    countLabel->setText("Count: " + std::to_string(counter));
    app.update();  // Re-render after state change
});
```

## Custom Renderers

### 1. Implementing a Renderer

Guillaume provides two approaches for implementing custom renderers:

#### Option A: Override Specific Draw Methods (Recommended)

```cpp
class MyRenderer : public Renderer {
public:
    void drawText(std::shared_ptr<Text> text) override {
        std::cout << "Text: " << text->getContent()
                  << " at (" << text->getPosition().getX()
                  << ", " << text->getPosition().getY() << ")" << std::endl;
    }

    void drawRectangle(std::shared_ptr<Rectangle> rectangle) override {
        std::cout << "Rectangle center: (" << rectangle->getCenter().getX()
                  << ", " << rectangle->getCenter().getY()
                  << ", " << rectangle->getCenter().getZ() << ") width: "
                  << rectangle->getWidth() << " height: " << rectangle->getHeight()
                  << " rotation: (" << rectangle->getRotation().getX() << ", "
                  << rectangle->getRotation().getY() << ", " << rectangle->getRotation().getZ() << ")" << std::endl;
        auto points = rectangle->getPoints();
        for (size_t i = 0; i < points.size(); ++i) {
            std::cout << "  Corner " << i << ": (" << points[i].getX() << ", " << points[i].getY() << ", " << points[i].getZ() << ")" << std::endl;
        }
    }

    void drawTriangle(std::shared_ptr<Triangle> triangle) override {
        auto points = triangle->getPoints();
        std::cout << "Triangle with vertices: (" << points[0].getX()
                  << ", " << points[0].getY() << "), (" << points[1].getX()
                  << ", " << points[1].getY() << "), (" << points[2].getX()
                  << ", " << points[2].getY() << ")" << std::endl;
    }

    void drawPolygon(std::shared_ptr<Polygon> polygon) override {
        std::cout << "Polygon with " << polygon->getPoints().size()
                  << " vertices" << std::endl;
    }
};
```

#### Option B: Override Generic Draw Method

```cpp
class MyRenderer : public Renderer {
public:
    void draw(std::shared_ptr<Primitive> primitive) override {
        if (auto text = std::dynamic_pointer_cast<Text>(primitive)) {
            // Handle text rendering
        } else if (auto rect = std::dynamic_pointer_cast<Rectangle>(primitive)) {
            // Handle rectangle rendering
        }
        // Manual type checking required
    }
};
```### 2. Multiple Rendering Backends

Different renderers can output to different targets:

- **Terminal Renderer**: ASCII art and ANSI escape codes
- **GUI Renderer**: Native GUI framework calls
- **Web Renderer**: HTML/Canvas output
- **Graphics Renderer**: OpenGL/Vulkan calls

Each renderer can choose to:
- Override specific primitive methods for type safety
- Override the generic `draw` method for custom dispatch logic
- Mix both approaches as needed

## Custom Components

### 1. Creating New Components

```cpp
class ProgressBar : public Component {
private:
    float _progress = 0.0f;

public:
    ProgressBar(float progress) : Component(), _progress(progress) {}

    void setProgress(float progress) {
        _progress = std::clamp(progress, 0.0f, 1.0f);
    }

    std::shared_ptr<Component> render() override {
        _primitives.clear();

    // Background rectangle (center, width, height, rotation)
    auto bg = std::make_shared<Rectangle>(Point(100, 10, 0), 200, 20, Point(0, 0, 0));
    _primitives.push_back(bg);

    // Progress fill (center, width, height, rotation)
    auto fill = std::make_shared<Rectangle>(Point(100 * _progress / 2, 10, 0), 200 * _progress, 20, Point(0, 0, 0));
    _primitives.push_back(fill);

        // Progress text
        auto text = std::make_shared<Text>(
            std::to_string(int(_progress * 100)) + "%",
            Point(100, 10, 0)
        );
        _primitives.push_back(text);

        return shared_from_this();
    }
};
```

### 2. Custom Primitives

```cpp
class Circle : public Primitive {
private:
    Point _center;
    float _radius;

public:
    Circle(Point center, float radius) : _center(center), _radius(radius) {}

    Point getCenter() const { return _center; }
    float getRadius() const { return _radius; }
};
```

## Best Practices

### 1. Component Design

- **Single Responsibility**: Each component should have a clear, focused purpose
- **State Encapsulation**: Keep component state private and provide controlled access methods
- **Primitive Generation**: Always clear and regenerate primitives in `render()`

### 2. Renderer Design

- **Specific Methods**: Prefer overriding specific draw methods (`drawText`, `drawRectangle`) over generic `draw`
- **Type Safety**: Specific methods provide compile-time type checking and eliminate manual casts
- **Performance**: Direct method calls are faster than dynamic type checking

### 3. Performance Considerations

- **Selective Updates**: Only call `update()` when necessary after state changes
- **Primitive Caching**: Consider caching primitives when content doesn't change frequently
- **Event Optimization**: Use efficient event handling patterns for high-frequency events
- **Renderer Efficiency**: Use specific draw methods to avoid runtime type checking overhead

### 4. Error Handling

- **Null Checks**: Always validate shared pointers before use
- **State Validation**: Validate input parameters in setter methods
- **Graceful Degradation**: Handle missing or invalid data gracefully

## Example: Complete Mini-Application

```cpp
#include "application.hpp"
#include "label.hpp"
#include "button.hpp"
#include "container.hpp"

class ConsoleRenderer : public Renderer {
public:
    void drawText(std::shared_ptr<Text> text) override {
        std::cout << "[TEXT] " << text->getContent() << std::endl;
    }

    void drawRectangle(std::shared_ptr<Rectangle> rectangle) override {
        std::cout << "[RECT] Background" << std::endl;
    }

    void drawTriangle(std::shared_ptr<Triangle> triangle) override {
        std::cout << "[TRIANGLE] " << triangle->getPoints().size() << " vertices" << std::endl;
    }

    void drawPolygon(std::shared_ptr<Polygon> polygon) override {
        std::cout << "[POLYGON] " << polygon->getPoints().size() << " vertices" << std::endl;
    }
};

int main() {
    // Create application
    Application app(std::make_unique<ConsoleRenderer>());

    // Create UI
    auto root = app.getRoot();
    auto titleLabel = std::make_shared<Label>("Counter App");
    auto countLabel = std::make_shared<Label>("Count: 0");
    auto button = std::make_shared<Button>("Increment");

    // Build hierarchy
    root->addChild(titleLabel);
    root->addChild(countLabel);
    root->addChild(button);

    // Add a 3D rectangle primitive (center, width, height, rotation)
    auto rect3D = std::make_shared<Rectangle>(Point(50, 50, 10), 40, 20, Point(0, 0.5, 0));
    root->addPrimitive(rect3D);

    // Add interaction
    int counter = 0;
    button->setOnClick([&]() {
        counter++;
        countLabel->setText("Count: " + std::to_string(counter));
        app.update();
    });

    // Run application
    app.run();

    // Simulate user interactions
    for (int i = 0; i < 3; i++) {
        button->onEvent(Event("click", button));
    }

    return 0;
}
```

This example demonstrates the complete Guillaume workflow: component creation, hierarchy building, event handling, and the render/update cycle.
