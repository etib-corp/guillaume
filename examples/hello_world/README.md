# Hello World

A minimal Guillaume example showing the basics of creating an application with a simple renderer.

## What it demonstrates

- Creating a minimal custom renderer
- Setting up an application instance
- Adding a simple label component to the UI
- Running the application

## Building

From the main Guillaume directory:

```bash
mkdir build && cd build
cmake ..
make
```

## Running

```bash
./examples/hello_world/hello_world
```

This example doesn't require SDL3 or OpenGL - it uses a simple console-based renderer that prints the UI text to the terminal.
