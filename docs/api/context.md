# Context API Reference

The `Context` class is the main entry point for Guillaume applications, managing the window, rendering loop, and event handling.

## Overview

```cpp
#include "guillaume/context.hpp"

namespace guigui {
    class Context;
}
```

The Context class serves as the central hub for Guillaume applications, providing:
- Window management and creation
- Main rendering loop
- Event handling and dispatch
- Component management
- Resource initialization

## Class Declaration

```cpp
class Context : public Eventable {
public:
    // Factory method
    static std::unique_ptr<Context> create(const std::string& title, 
                                         int width, int height);
    
    // Core lifecycle methods
    void run();
    void stop();
    bool is_running() const;
    
    // Component management
    void add_component(std::shared_ptr<Component> component);
    void remove_component(const ComponentIdentifier& id);
    
    // Event handling
    void handle_event(const Event& event) override;
    
    // Rendering control
    void set_background_color(const Color& color);
    Color get_background_color() const;
    
    // Window properties
    void set_title(const std::string& title);
    std::string get_title() const;
    void set_size(int width, int height);
    Vector2 get_size() const;
    
    // Renderer access
    std::shared_ptr<Renderer> get_renderer() const;
};
```

## Factory Methods

### create()

Creates a new Context instance with the specified window properties.

```cpp
static std::unique_ptr<Context> create(const std::string& title, 
                                     int width, int height);
```

**Parameters:**
- `title`: Window title
- `width`: Window width in pixels
- `height`: Window height in pixels

**Returns:** Unique pointer to the created Context

**Example:**
```cpp
auto context = guigui::Context::create("My Application", 800, 600);
```

## Core Methods

### run()

Starts the main application loop. This method blocks until the application is terminated.

```cpp
void run();
```

**Example:**
```cpp
context->run(); // Blocks until application exits
```

### stop()

Signals the application to stop running. The run() method will return after the current frame.

```cpp
void stop();
```

### is_running()

Checks if the application is currently running.

```cpp
bool is_running() const;
```

**Returns:** `true` if the application is running, `false` otherwise

## Component Management

### add_component()

Adds a component to the context for rendering and event handling.

```cpp
void add_component(std::shared_ptr<Component> component);
```

**Parameters:**
- `component`: Shared pointer to the component to add

**Example:**
```cpp
auto button = Button::create("Click Me");
context->add_component(button);
```

### remove_component()

Removes a component from the context by its identifier.

```cpp
void remove_component(const ComponentIdentifier& id);
```

**Parameters:**
- `id`: Unique identifier of the component to remove

## Window Properties

### set_title() / get_title()

Sets or gets the window title.

```cpp
void set_title(const std::string& title);
std::string get_title() const;
```

**Example:**
```cpp
context->set_title("Updated Title");
std::cout << "Current title: " << context->get_title() << std::endl;
```

### set_size() / get_size()

Sets or gets the window size.

```cpp
void set_size(int width, int height);
Vector2 get_size() const;
```

**Example:**
```cpp
context->set_size(1024, 768);
auto size = context->get_size();
std::cout << "Window size: " << size.x << "x" << size.y << std::endl;
```

## Rendering Control

### set_background_color() / get_background_color()

Sets or gets the background color for the window.

```cpp
void set_background_color(const Color& color);
Color get_background_color() const;
```

**Example:**
```cpp
context->set_background_color(Color::from_rgb(64, 128, 255));
```

### get_renderer()

Gets access to the underlying renderer for custom drawing operations.

```cpp
std::shared_ptr<Renderer> get_renderer() const;
```

**Returns:** Shared pointer to the renderer

**Example:**
```cpp
auto renderer = context->get_renderer();
renderer->draw_rectangle({10, 10}, {100, 50}, Color::red());
```

## Event Handling

The Context class inherits from `Eventable` and handles various types of events:

- **Mouse Events**: Click, move, scroll
- **Keyboard Events**: Key press, release
- **Window Events**: Resize, close
- **Custom Events**: Application-specific events

### handle_event()

Processes an event and dispatches it to appropriate handlers.

```cpp
void handle_event(const Event& event) override;
```

## Usage Examples

### Basic Application

```cpp
#include "guillaume.h"

int main() {
    // Create context
    auto context = guigui::Context::create("My App", 800, 600);
    
    // Set background color
    context->set_background_color(guigui::Color::from_rgb(240, 240, 240));
    
    // Create and add components
    auto button = guigui::Button::create("Click Me!");
    button->set_position({100, 100});
    button->on_click([]() {
        std::cout << "Button clicked!" << std::endl;
    });
    
    context->add_component(button);
    
    // Run application
    context->run();
    
    return 0;
}
```

### Event Handling

```cpp
// Custom event handler
context->on_event([](const guigui::Event& event) {
    if (event.get_type() == guigui::EventType::KeyPress) {
        auto& key_event = static_cast<const guigui::KeyboardEvent&>(event);
        if (key_event.get_key() == guigui::Key::Escape) {
            // Handle escape key
        }
    }
});
```

### Dynamic Component Management

```cpp
// Add component dynamically
auto label = guigui::Label::create("Dynamic Label");
context->add_component(label);

// Remove component later
context->remove_component(label->get_identifier());
```

## Thread Safety

The Context class is **not thread-safe**. All operations should be performed on the main thread. For multi-threaded applications, use appropriate synchronization mechanisms.

## Performance Considerations

- Components are rendered in the order they were added
- Minimize component additions/removals during the render loop
- Use `set_visible(false)` instead of removing components temporarily
- Consider using containers for complex layouts

## Error Handling

The Context class may throw exceptions in the following cases:

- **std::runtime_error**: Failed to initialize SDL or create window
- **std::invalid_argument**: Invalid parameters passed to methods
- **std::bad_alloc**: Out of memory conditions

Always wrap Context creation in try-catch blocks for robust error handling:

```cpp
try {
    auto context = guigui::Context::create("My App", 800, 600);
    context->run();
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
}
```

## See Also

- [Component API](component.md) - Base component class
- [Renderer API](renderer.md) - Rendering operations
- [Event API](event.md) - Event system
- [Container API](container.md) - Component containers
