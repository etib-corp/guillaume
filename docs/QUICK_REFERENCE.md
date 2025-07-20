# Guillaume Quick Reference

A concise reference for common Guillaume GUI framework operations and patterns.

## Table of Contents

- [Basic Setup](#basic-setup)
- [Common Components](#common-components)
- [Event Handling](#event-handling)
- [Layout and Positioning](#layout-and-positioning)
- [Styling and Colors](#styling-and-colors)
- [Custom Components](#custom-components)
- [Common Patterns](#common-patterns)
- [Troubleshooting](#troubleshooting)

## Basic Setup

### Minimal Application

```cpp
#include "guillaume.h"

int main() {
    auto context = guigui::Context::create("App", 800, 600);
    context->run();
    return 0;
}
```

### With Error Handling

```cpp
try {
    auto context = guigui::Context::create("App", 800, 600);
    context->set_background_color(guigui::Color::from_rgb(240, 240, 240));
    context->run();
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
}
```

## Common Components

### Button

```cpp
// Create button
auto button = guigui::Button::create("Click Me");
button->set_position({100, 100});
button->set_size({150, 40});

// Add click handler
button->on_click([]() {
    std::cout << "Clicked!" << std::endl;
});

// Add to context
context->add_component(button);
```

### Label

```cpp
auto label = guigui::Label::create("Hello World");
label->set_position({50, 50});
label->set_text_color(guigui::Color::black());
context->add_component(label);
```

### Container

```cpp
auto container = guigui::Container::create("main");
container->set_position({10, 10});
container->set_size({400, 300});

// Add child components
container->add_component(button);
container->add_component(label);

context->add_component(container);
```

## Event Handling

### Mouse Events

```cpp
// Click events
component->on_click([]() { /* handle click */ });

// Hover events
component->on_hover_enter([]() { /* mouse entered */ });
component->on_hover_exit([]() { /* mouse left */ });

// Mouse button events
component->on_mouse_down([]() { /* button pressed */ });
component->on_mouse_up([]() { /* button released */ });
```

### Keyboard Events

```cpp
context->on_event([](const guigui::Event& event) {
    if (event.get_type() == guigui::EventType::KeyPress) {
        auto& key_event = static_cast<const guigui::KeyboardEvent&>(event);
        
        switch (key_event.get_key()) {
            case guigui::Key::Escape:
                // Handle escape
                break;
            case guigui::Key::Enter:
                // Handle enter
                break;
        }
    }
});
```

### Custom Events

```cpp
// Create custom event
class MyEvent : public guigui::Event {
public:
    MyEvent(const std::string& data) : _data(data) {}
    std::string get_data() const { return _data; }
    
private:
    std::string _data;
};

// Dispatch custom event
auto my_event = std::make_unique<MyEvent>("custom data");
context->dispatch_event(std::move(my_event));
```

## Layout and Positioning

### Absolute Positioning

```cpp
component->set_position({100, 50});  // x=100, y=50
component->set_size({200, 30});      // width=200, height=30
```

### Container Layouts

```cpp
// Vertical layout
container->set_layout(guigui::LayoutType::Vertical);
container->set_spacing(10);  // 10px between components

// Horizontal layout
container->set_layout(guigui::LayoutType::Horizontal);

// Grid layout
container->set_layout(guigui::LayoutType::Grid);
container->set_grid_columns(3);
```

### Alignment

```cpp
// Center component in container
component->set_alignment(guigui::Alignment::Center);

// Top-right alignment
component->set_alignment(guigui::Alignment::TopRight);
```

## Styling and Colors

### Colors

```cpp
// Predefined colors
auto red = guigui::Color::red();
auto blue = guigui::Color::blue();
auto white = guigui::Color::white();

// RGB colors
auto custom = guigui::Color::from_rgb(128, 64, 255);

// RGBA with transparency
auto transparent = guigui::Color::from_rgba(255, 0, 0, 0.5f);

// HSV colors
auto hsv = guigui::Color::from_hsv(240, 1.0f, 1.0f);  // Pure blue
```

### Component Styling

```cpp
// Background color
button->set_background_color(guigui::Color::blue());

// Text color
label->set_text_color(guigui::Color::white());

// Border
component->set_border_color(guigui::Color::black());
component->set_border_width(2);
```

### Fonts

```cpp
// Default font
auto font = guigui::Font::default_font();

// Load custom font
auto custom_font = guigui::Font::load("assets/Roboto.ttf", 16);

// Apply to component
label->set_font(custom_font);
```

## Custom Components

### Basic Custom Component

```cpp
class MyComponent : public guigui::Component {
public:
    MyComponent() : Component("my_component") {
        set_size({100, 50});
        update_visual_state();
    }

protected:
    void update_visual_state() override {
        clear_primitives();
        
        // Add background
        auto bg = std::make_unique<guigui::RectanglePrimitive>(
            get_bounds(), guigui::Color::gray());
        add_primitive(std::move(bg));
    }
    
    void on_state_changed(InteractionState old_state, 
                         InteractionState new_state) override {
        // Handle state changes
        update_visual_state();
    }
};
```

### Using Custom Components

```cpp
auto my_comp = std::make_shared<MyComponent>();
my_comp->set_position({100, 100});
context->add_component(my_comp);
```

## Common Patterns

### Toggle Button

```cpp
bool is_toggled = false;
auto toggle = guigui::Button::create("OFF");

toggle->on_click([&is_toggled, toggle]() {
    is_toggled = !is_toggled;
    toggle->set_text(is_toggled ? "ON" : "OFF");
    toggle->set_background_color(
        is_toggled ? guigui::Color::green() : guigui::Color::gray()
    );
});
```

### Dynamic Component Creation

```cpp
void add_button_dynamically() {
    static int count = 0;
    
    auto button = guigui::Button::create("Button " + std::to_string(++count));
    button->set_position({50, 50 + count * 50});
    button->set_size({120, 30});
    
    button->on_click([button, count]() {
        std::cout << "Button " << count << " clicked" << std::endl;
    });
    
    context->add_component(button);
}
```

### Component Communication

```cpp
class MessageBus {
public:
    using Callback = std::function<void(const std::string&)>;
    
    void subscribe(const std::string& topic, Callback callback) {
        subscribers[topic].push_back(callback);
    }
    
    void publish(const std::string& topic, const std::string& message) {
        for (auto& callback : subscribers[topic]) {
            callback(message);
        }
    }
    
private:
    std::map<std::string, std::vector<Callback>> subscribers;
};
```

### Animation Helpers

```cpp
class Animator {
public:
    static void fade_in(std::shared_ptr<Component> comp, float duration) {
        comp->set_opacity(0.0f);
        
        // Start animation (pseudo-code)
        animate_property(comp, "opacity", 0.0f, 1.0f, duration);
    }
    
    static void slide_in(std::shared_ptr<Component> comp, 
                        const Vector2& from, const Vector2& to, float duration) {
        comp->set_position(from);
        animate_property(comp, "position", from, to, duration);
    }
};
```

## Troubleshooting

### Common Build Issues

```bash
# Missing headers
g++ -I/path/to/guillaume/headers your_file.cpp

# Linking issues  
g++ your_file.cpp -L/path/to/lib -lguillaume

# C++20 support
g++ -std=c++20 your_file.cpp
```

### Runtime Issues

```cpp
// Check if component exists
if (auto comp = context->find_component("my_id")) {
    comp->set_visible(true);
}

// Safe event handling
try {
    context->run();
} catch (const guigui::InitializationError& e) {
    std::cerr << "Failed to initialize: " << e.what() << std::endl;
}
```

### Memory Issues

```cpp
// Avoid raw pointers
// DON'T: Component* comp = new Component();
// DO: auto comp = std::make_shared<Component>();

// Proper cleanup
{
    auto context = Context::create("App", 800, 600);
    // Automatic cleanup when scope ends
}
```

### Performance Tips

```cpp
// Minimize redraws
void expensive_update() {
    // Batch multiple changes
    component->set_position({100, 100});
    component->set_size({200, 50});
    component->set_color(Color::blue());
    
    // Single redraw at the end
    component->_mark_dirty();
}

// Efficient hit testing
bool CustomComponent::contains_point(const Vector2& point) const {
    // Fast bounding box check first
    if (!get_bounds().contains(point)) return false;
    
    // Expensive precise check only if needed
    return precise_hit_test(point);
}
```

## Environment Variables

```bash
# Debug output
export GUILLAUME_LOG_LEVEL=DEBUG

# Asset paths
export GUILLAUME_ASSETS_DIR=/path/to/assets

# Force software rendering
export GUILLAUME_SOFTWARE_RENDER=1
```

## Useful Macros

```cpp
// Debug logging
#ifdef GUILLAUME_DEBUG
#define GUILLAUME_LOG(msg) std::cout << "[DEBUG] " << msg << std::endl
#else
#define GUILLAUME_LOG(msg)
#endif

// Error checking
#define GUILLAUME_CHECK(condition, message) \
    if (!(condition)) { \
        throw std::runtime_error(message); \
    }
```

## See Also

- [Full API Documentation](docs/api/)
- [Examples Directory](examples/)
- [Contributing Guide](docs/CONTRIBUTING.md)
- [Development Setup](docs/DEVELOPMENT.md)
