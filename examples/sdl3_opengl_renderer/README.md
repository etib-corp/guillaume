# Basic Demo

This comprehensive example demonstrates the advanced features of Guillaume including:

- Component hierarchy with containers, labels, and buttons
- Event handling and state management
- 3D primitive rendering (triangles, rectangles, polygons, text)
- Custom renderer implementation using SDL3 and OpenGL

## Building

From the main Guillaume directory:

```bash
mkdir build && cd build
cmake ..
make
```

## Running

```bash
./examples/basic_demo/basic_demo
```

## What it does

The demo creates a simple UI with:

- Header with title and subtitle labels
- Content area with a click counter and increment/reset buttons
- Footer with status information
- Various 3D primitives to showcase the rendering system

The example simulates user interactions by programmatically clicking buttons and shows how the UI state updates and re-renders accordingly.
