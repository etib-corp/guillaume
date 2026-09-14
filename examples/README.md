# Guillaume Examples

This directory contains runnable samples that demonstrate how to use the
Guillaume framework.

## hello_world

A desktop sample that:

- Implements a concrete `Engine` subclass backed by the Evan Vulkan engine.
- Defines a `Scene` type.
- Runs the `Application` main loop.

Guillaume is engine-less: it delegates rendering and event polling to an
`Engine` implementation that consumers provide. This example fetches the
[Evan](https://github.com/etib-corp/evan) engine via FetchContent and wraps it
in a local `EvanEngine : guillaume::Engine` class. The Evan dependency is
scoped to the example only — the `guillaume` library itself has no engine
dependency.

### Building

Evan requires the Vulkan SDK and an explicit platform/backend selection.

```sh
# macOS
cmake -S . -B build -DBUILD_EXAMPLES=ON \
  -DBUILD_FOR_GLFW=ON -DBUILD_FOR_MACOS=ON
cmake --build build --target guillaume_hello_world

# Linux
cmake -S . -B build -DBUILD_EXAMPLES=ON \
  -DBUILD_FOR_GLFW=ON -DBUILD_FOR_LINUX=ON
cmake --build build --target guillaume_hello_world

# Windows
cmake -S . -B build -DBUILD_EXAMPLES=ON \
  -DBUILD_FOR_GLFW=ON -DBUILD_FOR_WINDOWS=ON
cmake --build build --target guillaume_hello_world
```

### Running

```sh
./build/examples/hello_world/guillaume_hello_world
```
