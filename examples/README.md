# Guillaume Examples

This directory contains example applications demonstrating various features and usage patterns of the Guillaume GUI framework.

## Table of Contents

- [Getting Started](#getting-started)
- [Example Overview](#example-overview)
- [Building Examples](#building-examples)
- [Running Examples](#running-examples)
- [Example Details](#example-details)

## Getting Started

Before running the examples, ensure you have:

1. **Built Guillaume:** Follow the main [README.md](../README.md) for build instructions
2. **Dependencies:** All examples use the same dependencies as the main Guillaume library

## Example Overview

| Example | Description | Demonstrates |
|---------|-------------|--------------|
| `basic_window.cpp` | Simple window creation | Context setup, basic event handling |
| `button_demo.cpp` | Interactive buttons | Button components, event callbacks |
| `custom_component.cpp` | Custom component creation | Inheritance, custom rendering |
| `layout_demo.cpp` | Layout management | Containers, positioning |
| `event_handling.cpp` | Event system usage | Keyboard, mouse, custom events |
| `animation_demo.cpp` | Animation examples | Smooth transitions, updates |

## Building Examples

### Using CMake (Recommended)

Add examples to your build:

```bash
# Configure with examples
cmake -S . -B build -DBUILD_EXAMPLES=ON
cmake --build build

# Or build specific example
cmake --build build --target basic_window
```

### Manual Compilation

If examples are not integrated with the main build:

```bash
# Compile individual example
g++ -std=c++20 \
    -I../headers \
    -L../build \
    examples/basic_window.cpp \
    -lguillaume \
    -o basic_window

# Run
./basic_window
```

## Running Examples

### From Build Directory

```bash
# Run built examples
./build/basic_window
./build/button_demo
./build/custom_component
```

### With Debug Output

```bash
# Enable debug logging
GUILLAUME_LOG_LEVEL=DEBUG ./build/button_demo

# Memory checking (Linux/macOS)
valgrind --leak-check=full ./build/custom_component
```

## Example Details

### 1. Basic Window (`basic_window.cpp`)

**What it demonstrates:**
- Creating a Guillaume context
- Setting up a basic window
- Basic event handling
- Application lifecycle

**Key concepts:**
```cpp
// Create context
auto context = guigui::Context::create("Title", 800, 600);

// Set background
context->set_background_color(guigui::Color::from_rgb(240, 240, 240));

// Handle events
context->on_event([](const guigui::Event& event) {
    // Process events
});

// Run application
context->run();
```

**Learning objectives:**
- Understand Guillaume initialization
- Learn basic window management
- See simple event handling pattern

---

### 2. Button Demo (`button_demo.cpp`)

**What it demonstrates:**
- Creating and configuring button components
- Event handling (click, hover)
- Button states (enabled/disabled)
- Multiple button interactions

**Key concepts:**
```cpp
// Create button
auto button = guigui::Button::create("Click Me!");
button->set_position({100, 100});
button->set_size({150, 40});

// Add event handler
button->on_click([]() {
    std::cout << "Button clicked!" << std::endl;
});

// Add to context
context->add_component(button);
```

**Features showcased:**
- Click counting
- Toggle button behavior
- Hover effects
- Enable/disable functionality
- Visual feedback

**Learning objectives:**
- Master button component usage
- Understand event callback patterns
- Learn component state management

---

### 3. Custom Component (`custom_component.cpp`)

**What it demonstrates:**
- Creating custom components by inheriting from `Component`
- Custom hit testing (circular buttons)
- Custom rendering with primitives
- Animation effects
- Component composition (color picker)

**Key concepts:**
```cpp
class CircleButton : public guigui::Component {
public:
    CircleButton(const std::string& text, float radius);
    
    // Custom hit testing
    bool contains_point(const guigui::Vector2& point) const override;
    
protected:
    // Custom rendering
    void update_visual_state() override;
    
    // State change handling
    void on_state_changed(InteractionState old, InteractionState new) override;
};
```

**Features showcased:**
- Circular buttons with custom hit detection
- Click animations
- Color picker component
- Custom visual states
- Container usage

**Learning objectives:**
- Learn to create reusable custom components
- Understand the component lifecycle
- Master custom rendering techniques
- See animation implementation patterns

---

### 4. Layout Demo (`layout_demo.cpp`)

**What it demonstrates:**
- Container usage for organizing components
- Different layout strategies
- Responsive design principles
- Component positioning and sizing

**Key concepts:**
```cpp
// Create container
auto container = guigui::Container::create("main_container");

// Add components with layout
container->add_component(button1);
container->add_component(button2);

// Apply layout strategy
container->set_layout(guigui::LayoutType::Vertical);
```

**Features showcased:**
- Horizontal and vertical layouts
- Grid layouts
- Automatic sizing
- Nested containers
- Responsive resizing

**Learning objectives:**
- Master container and layout systems
- Learn responsive design patterns
- Understand component organization

---

### 5. Event Handling (`event_handling.cpp`)

**What it demonstrates:**
- Comprehensive event system usage
- Keyboard input handling
- Mouse event processing
- Custom event creation and dispatch

**Key concepts:**
```cpp
// Keyboard events
context->on_event([](const guigui::Event& event) {
    if (event.get_type() == guigui::EventType::KeyPress) {
        auto& key_event = static_cast<const guigui::KeyboardEvent&>(event);
        // Handle key press
    }
});

// Mouse events
component->on_mouse_down([](const guigui::MouseEvent& event) {
    // Handle mouse press
});
```

**Features showcased:**
- Keyboard shortcuts
- Mouse tracking
- Event propagation
- Custom events
- Event filtering

**Learning objectives:**
- Master the event system
- Learn input handling patterns
- Understand event flow and propagation

---

### 6. Animation Demo (`animation_demo.cpp`)

**What it demonstrates:**
- Smooth animations and transitions
- Frame-based updates
- Easing functions
- Performance considerations

**Key concepts:**
```cpp
class AnimatedComponent : public guigui::Component {
private:
    float animation_progress = 0.0f;
    bool is_animating = false;
    
    void update_animation() {
        if (is_animating) {
            animation_progress += delta_time;
            update_visual_state();
            _mark_dirty(); // Request redraw
        }
    }
};
```

**Features showcased:**
- Fade in/out effects
- Movement animations
- Color transitions
- Rotation and scaling
- Animation chaining

**Learning objectives:**
- Learn animation implementation
- Understand frame timing
- Master smooth transitions
- See performance optimization techniques

## Best Practices Demonstrated

### 1. Resource Management

Examples show proper resource management:

```cpp
// Use smart pointers
auto component = std::make_shared<Component>();

// RAII for automatic cleanup
{
    auto context = Context::create("App", 800, 600);
    // Automatic cleanup when scope ends
}
```

### 2. Event Handling Patterns

```cpp
// Capture by value for safety
button->on_click([value_copy]() {
    // Use value_copy safely
});

// Capture by reference for state
button->on_click([&state]() {
    // Modify state
});
```

### 3. Component Lifecycle

```cpp
class MyComponent : public Component {
public:
    MyComponent() : Component("unique_id") {
        // Initialize in constructor
        setup_initial_state();
    }
    
protected:
    void update_visual_state() override {
        // Clean rebuild of visuals
        clear_primitives();
        build_primitives();
    }
};
```

### 4. Error Handling

```cpp
try {
    auto context = Context::create("App", 800, 600);
    context->run();
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
}
```

## Performance Tips

Based on the examples:

1. **Minimize Redraws:** Only call `_mark_dirty()` when visuals actually change
2. **Batch Updates:** Group multiple changes before triggering updates
3. **Efficient Hit Testing:** Implement fast `contains_point()` for custom shapes
4. **Animation Optimization:** Use frame timing for smooth animations
5. **Memory Management:** Prefer stack allocation when possible

## Troubleshooting

### Common Issues

**Example won't compile:**
```bash
# Check includes and libraries
g++ -I../headers -L../build examples/basic_window.cpp -lguillaume
```

**Runtime errors:**
```bash
# Check SDL dependencies
ldd ./basic_window  # Linux
otool -L ./basic_window  # macOS
```

**Performance issues:**
```bash
# Run with profiling
perf record ./button_demo  # Linux
```

### Getting Help

If you encounter issues with examples:

1. **Check main README:** Ensure Guillaume builds correctly
2. **Review source:** Examples are extensively commented
3. **Create issue:** Report problems with reproduction steps
4. **Ask community:** Use GitHub Discussions

## Contributing Examples

Want to add your own example?

1. **Follow naming pattern:** `feature_demo.cpp`
2. **Add comprehensive comments:** Explain what and why
3. **Include in CMake:** Update build system if needed
4. **Document in README:** Add entry to this file
5. **Test thoroughly:** Ensure it builds and runs

### Example Template

```cpp
/**
 * @file my_demo.cpp
 * @brief Brief description of what this demonstrates
 * 
 * Detailed explanation of the example's purpose and
 * what users can learn from it.
 */

#include "guillaume.h"
#include <iostream>

class MyDemo {
private:
    std::shared_ptr<guigui::Context> context;
    
public:
    MyDemo() {
        // Setup
    }
    
    void run() {
        try {
            context->run();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
};

int main() {
    MyDemo demo;
    demo.run();
    return 0;
}
```

## Next Steps

After exploring the examples:

1. **Modify Examples:** Try changing colors, sizes, behaviors
2. **Combine Features:** Mix concepts from different examples
3. **Create Your Own:** Build a simple application using Guillaume
4. **Contribute:** Submit improvements or new examples
5. **Read Documentation:** Dive deeper into the [API docs](../docs/api/)

## See Also

- [Main README](../README.md) - Project overview and setup
- [API Documentation](../docs/api/) - Complete API reference
- [Contributing Guide](../docs/CONTRIBUTING.md) - How to contribute
- [Development Guide](../docs/DEVELOPMENT.md) - Development setup
