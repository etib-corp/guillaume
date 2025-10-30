# SDL3 OpenGL Interactive Demo

This comprehensive example demonstrates the advanced features of Guillaume including:

- **Component hierarchy** with containers, labels, and buttons
- **Interactive event handling** with MyEventHandler
- **Real-time state management** and UI updates
- **3D primitive rendering** (triangles, rectangles, polygons, text)
- **Custom renderer** implementation using SDL3 and OpenGL
- **Mouse and keyboard input** handling

## Features

### Event Handler Integration

The example includes a complete `MyEventHandler` implementation that:

- Captures SDL3 events (mouse clicks, keyboard input, window events)
- Converts SDL events to Guillaume Event objects
- Dispatches events to the component hierarchy
- Handles quit events (ESC key or window close)

### Interactive Components

- **Increment Button**: Click the top half of the window to increment counter
- **Reset Button**: Click the bottom half of the window to reset counter
- **Real-time Feedback**: Labels update immediately on button clicks
- **Visual Primitives**: 3D shapes rendered with OpenGL

## Building

From the main Guillaume directory:

```bash
mkdir build && cd build
cmake .. -DBUILD_EXAMPLES=ON
make sdl3_opengl_renderer
```

## Running

```bash
./examples/sdl3_opengl/sdl3_opengl_renderer
```

## Controls

- **Mouse Click (Top Half)**: Increment counter
- **Mouse Click (Bottom Half)**: Reset counter
- **ESC Key**: Quit application
- **Close Window**: Quit application

## What it does

The demo creates an interactive UI with:

1. **Header** with title and subtitle labels
2. **Content area** with:
   - Dynamic click counter label
   - Increment button (responds to clicks)
   - Reset button (responds to clicks)
3. **Footer** with status information
4. **3D primitives** showcasing the rendering system:
   - Triangle (with transformations)
   - Hexagon polygon (with transformations)
   - 3D rectangle

The example demonstrates the full event loop:
1. MyEventHandler polls SDL events
2. Events are converted to Guillaume Events
3. Events are dispatched to components
4. Button onClick handlers update state
5. Labels re-render with new text
6. Renderer draws the updated UI

## Code Structure

```
sdl3_opengl/
├── headers/
│   ├── my_renderer.hpp          # Custom OpenGL renderer
│   └── sdl3_event_handler.hpp   # SDL3 event handler
├── sources/
│   ├── main.cpp                 # Application entry point
│   ├── my_renderer.cpp          # Renderer implementation
│   └── sdl3_event_handler.cpp   # Event handler implementation
└── README.md                     # This file
```

## Key Classes

### MyEventHandler

Concrete implementation of EventHandler that:
- Polls SDL3 events in `pollEvents()`
- Implements hit-testing to find components at mouse positions
- Tracks quit state via `shouldContinue()`
- Provides window access for event context

### MyRenderer

Custom renderer that:
- Initializes SDL3 window and OpenGL context
- Implements primitive drawing methods
- Handles 3D transformations and projections
- Manages OpenGL resources (shaders, buffers, VAOs)

## Event Flow

```
User Input (Mouse/Keyboard)
    ↓
SDL3 Event System
    ↓
MyEventHandler::pollEvents()
    ↓
Guillaume Event Objects
    ↓
EventHandler::dispatchEvent()
    ↓
Component::onEvent()
    ↓
Button::onClick() callback
    ↓
Label::setText() (state update)
    ↓
Application::update()
    ↓
Renderer::draw*() methods
    ↓
OpenGL/SDL3 Display
```

## Extending the Example

### Custom Hit Testing

The current hit testing is simplified (divides window into regions). For production use, implement proper bounding box checking:

```cpp
std::shared_ptr<Component> 
MyEventHandler::findComponentAtPosition(float x, float y) {
  // 1. Transform screen coordinates to world coordinates
  // 2. Traverse component tree depth-first (back to front)
  // 3. Check each component's bounding box
  // 4. Return the topmost component at position
}
```

### Additional Event Types

Add more event handlers in `MyEventHandler::pollEvents()`:

```cpp
case SDL_EVENT_MOUSE_WHEEL:
  // Handle scroll events
  break;

case SDL_EVENT_TEXT_INPUT:
  // Handle text input for text fields
  break;
```

### Custom Components

Create custom interactive components:

```cpp
class Slider : public Component {
  void onEvent(const Event& event) override {
    if (event.getType() == "drag") {
      // Update slider value
    }
  }
};
```

## See Also

- [EventHandler Documentation](../../../docs/EVENT_HANDLER.md)
- [Architecture Overview](../../../docs/ARCHITECTURE.md)
- [Guillaume Main README](../../../README.md)
