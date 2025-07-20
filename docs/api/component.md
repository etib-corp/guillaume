# Component API Reference

The `Component` class is the base class for all interactive UI elements in Guillaume, providing core functionality for rendering, event handling, and state management.

## Overview

```cpp
#include "guillaume/component.hpp"

namespace guigui {
    class Component : public Componentable;
}
```

The Component class provides the foundation for all interactive UI elements with:

- Mouse event handling and interaction states
- Primitive-based rendering system  
- Callback system for user interactions
- Bounds-based hit testing
- Visibility and enabled state management

## Interaction States

Components support different visual states based on user interaction:

```cpp
enum class InteractionState {
    NORMAL,   // Default state, no user interaction
    HOVERED,  // Mouse is over the component
    PRESSED,  // Component is being pressed
    DISABLED  // Component is disabled
};
```

## Class Declaration

```cpp
class Component : public Componentable {
public:
    // Constructor
    Component(const ComponentIdentifier& id);
    virtual ~Component() = default;
    
    // Core interface implementation
    ComponentIdentifier get_identifier() const override;
    void draw() override;
    void set_renderer(std::shared_ptr<Renderer> renderer) override;
    
    // Visibility and state
    void set_visible(bool visible) override;
    bool is_visible() const override;
    void set_enabled(bool enabled) override;
    bool is_enabled() const override;
    
    // Position and bounds
    void set_position(const Vector2& position);
    Vector2 get_position() const;
    void set_size(const Vector2& size);
    Vector2 get_size() const;
    Rectangle get_bounds() const;
    
    // Mouse event handling
    void handle_mouse_event(const MouseEvent& event) override;
    bool contains_point(const Vector2& point) const;
    
    // Interaction state
    InteractionState get_interaction_state() const;
    
    // Event callbacks
    void on_click(std::function<void()> callback);
    void on_hover_enter(std::function<void()> callback);
    void on_hover_exit(std::function<void()> callback);
    void on_mouse_down(std::function<void()> callback);
    void on_mouse_up(std::function<void()> callback);
    
protected:
    // Primitive management
    void add_primitive(std::unique_ptr<Primitive> primitive);
    void clear_primitives();
    const std::vector<std::unique_ptr<Primitive>>& get_primitives() const;
    
    // State change notifications
    virtual void on_state_changed(InteractionState old_state, 
                                InteractionState new_state);
    virtual void update_visual_state();
    
    // Internal event handlers
    virtual void _on_click();
    virtual void _on_hover_enter();
    virtual void _on_hover_exit();
    virtual void _on_mouse_down();
    virtual void _on_mouse_up();
};
```

## Constructor

### Component()

Creates a new component with the specified identifier.

```cpp
Component(const ComponentIdentifier& id);
```

**Parameters:**

- `id`: Unique identifier for the component

**Example:**
```cpp
class MyComponent : public guigui::Component {
public:
    MyComponent() : Component("my_component_" + std::to_string(counter++)) {
        // Initialize component
    }
};
```

## Position and Bounds

### set_position() / get_position()

Sets or gets the component's position in the parent coordinate system.

```cpp
void set_position(const Vector2& position);
Vector2 get_position() const;
```

**Example:**
```cpp
component->set_position({100, 50});
auto pos = component->get_position();
```

### set_size() / get_size()

Sets or gets the component's size.

```cpp
void set_size(const Vector2& size);
Vector2 get_size() const;
```

**Example:**
```cpp
component->set_size({200, 30});
auto size = component->get_size();
```

### get_bounds()

Gets the component's bounding rectangle in parent coordinates.

```cpp
Rectangle get_bounds() const;
```

**Returns:** Rectangle representing the component's bounds

**Example:**
```cpp
auto bounds = component->get_bounds();
std::cout << "Component at (" << bounds.x << ", " << bounds.y 
          << ") size " << bounds.width << "x" << bounds.height << std::endl;
```

## State Management

### set_visible() / is_visible()

Controls component visibility.

```cpp
void set_visible(bool visible) override;
bool is_visible() const override;
```

**Example:**
```cpp
component->set_visible(false); // Hide component
if (component->is_visible()) {
    // Component is visible
}
```

### set_enabled() / is_enabled()

Controls whether the component responds to user interaction.

```cpp
void set_enabled(bool enabled) override;
bool is_enabled() const override;
```

**Example:**
```cpp
component->set_enabled(false); // Disable interaction
if (component->is_enabled()) {
    // Component accepts user input
}
```

### get_interaction_state()

Gets the current interaction state of the component.

```cpp
InteractionState get_interaction_state() const;
```

**Returns:** Current interaction state

**Example:**
```cpp
auto state = component->get_interaction_state();
switch (state) {
    case InteractionState::HOVERED:
        // Handle hover state
        break;
    case InteractionState::PRESSED:
        // Handle pressed state
        break;
}
```

## Event Handling

### Mouse Event Callbacks

Components support various mouse event callbacks:

```cpp
void on_click(std::function<void()> callback);
void on_hover_enter(std::function<void()> callback);
void on_hover_exit(std::function<void()> callback);
void on_mouse_down(std::function<void()> callback);
void on_mouse_up(std::function<void()> callback);
```

**Example:**
```cpp
component->on_click([]() {
    std::cout << "Component clicked!" << std::endl;
});

component->on_hover_enter([]() {
    std::cout << "Mouse entered component" << std::endl;
});

component->on_hover_exit([]() {
    std::cout << "Mouse left component" << std::endl;
});
```

### contains_point()

Tests if a point is within the component's bounds.

```cpp
bool contains_point(const Vector2& point) const;
```

**Parameters:**

- `point`: Point to test (in parent coordinates)

**Returns:** `true` if point is within bounds

**Example:**
```cpp
Vector2 mouse_pos = get_mouse_position();
if (component->contains_point(mouse_pos)) {
    // Mouse is over component
}
```

## Rendering System

### Primitive Management

Components use a primitive-based rendering system:

```cpp
protected:
    void add_primitive(std::unique_ptr<Primitive> primitive);
    void clear_primitives();
    const std::vector<std::unique_ptr<Primitive>>& get_primitives() const;
```

**Example:**
```cpp
class MyComponent : public Component {
protected:
    void update_visual_state() override {
        clear_primitives();
        
        // Add background rectangle
        auto bg = std::make_unique<RectanglePrimitive>(
            get_bounds(), 
            get_background_color()
        );
        add_primitive(std::move(bg));
        
        // Add more primitives as needed
    }
};
```

### Visual State Updates

Override these methods to customize rendering:

```cpp
virtual void on_state_changed(InteractionState old_state, 
                            InteractionState new_state);
virtual void update_visual_state();
```

**Example:**
```cpp
void MyComponent::on_state_changed(InteractionState old_state, 
                                 InteractionState new_state) {
    // Update colors based on state
    switch (new_state) {
        case InteractionState::HOVERED:
            set_background_color(Color::light_blue());
            break;
        case InteractionState::PRESSED:
            set_background_color(Color::dark_blue());
            break;
        default:
            set_background_color(Color::gray());
            break;
    }
    
    update_visual_state();
}
```

## Creating Custom Components

To create a custom component, inherit from Component and implement the necessary methods:

```cpp
class MyButton : public guigui::Component {
public:
    MyButton(const std::string& text) 
        : Component("button_" + std::to_string(counter++))
        , _text(text) {
        set_size({100, 30});
        update_visual_state();
    }
    
    void set_text(const std::string& text) {
        _text = text;
        update_visual_state();
        _mark_dirty();
    }
    
    std::string get_text() const {
        return _text;
    }

protected:
    void update_visual_state() override {
        clear_primitives();
        
        // Background
        Color bg_color;
        switch (get_interaction_state()) {
            case InteractionState::HOVERED:
                bg_color = Color::from_rgb(200, 200, 255);
                break;
            case InteractionState::PRESSED:
                bg_color = Color::from_rgb(150, 150, 255);
                break;
            case InteractionState::DISABLED:
                bg_color = Color::from_rgb(128, 128, 128);
                break;
            default:
                bg_color = Color::from_rgb(240, 240, 240);
                break;
        }
        
        auto background = std::make_unique<RectanglePrimitive>(
            get_bounds(), bg_color
        );
        add_primitive(std::move(background));
        
        // Text
        auto text_primitive = std::make_unique<TextPrimitive>(
            _text, get_position(), Color::black()
        );
        add_primitive(std::move(text_primitive));
    }

private:
    std::string _text;
    static int counter;
};
```

## Performance Considerations

- Override `update_visual_state()` efficiently to minimize primitive creation
- Use `_mark_dirty()` to trigger re-rendering only when necessary
- Cache expensive computations in member variables
- Minimize the number of primitives for better rendering performance

## Thread Safety

The Component class is **not thread-safe**. All operations should be performed on the main UI thread.

## Common Patterns

### Toggle Component

```cpp
class ToggleButton : public Component {
public:
    ToggleButton() : Component("toggle"), _toggled(false) {
        on_click([this]() {
            set_toggled(!_toggled);
        });
    }
    
    void set_toggled(bool toggled) {
        if (_toggled != toggled) {
            _toggled = toggled;
            update_visual_state();
            _mark_dirty();
            
            if (_on_toggle) {
                _on_toggle(_toggled);
            }
        }
    }
    
    void on_toggle(std::function<void(bool)> callback) {
        _on_toggle = callback;
    }

private:
    bool _toggled;
    std::function<void(bool)> _on_toggle;
};
```

### Animated Component

```cpp
class AnimatedComponent : public Component {
protected:
    void update_visual_state() override {
        // Update animation state
        update_animation();
        
        // Render based on current animation frame
        render_current_frame();
        
        // Request next frame if animating
        if (is_animating()) {
            _mark_dirty();
        }
    }
};
```

## See Also

- [Componentable API](componentable.md) - Base interface
- [Button API](button.md) - Button component implementation
- [Container API](container.md) - Container component
- [Event API](event.md) - Event system
- [Primitive API](primitive.md) - Rendering primitives
