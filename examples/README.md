# Guillaume Examples

This directory contains runnable samples that demonstrate how to use the
Guillaume framework.

## hello_world

A minimal desktop sample that:

- Implements a minimal `Engine` subclass.
- Defines a `Scene` type.
- Runs the `Application` main loop.

Because Guillaume is engine-coupled (rendering is delegated to an `Engine`
implementation), this sample ships a no-op engine stub. To render actual
content, replace `NoopEngine` with a real engine implementation for your
platform, or see the `evan` example.

### Building

```sh
cmake -S . -B build -DBUILD_EXAMPLES=ON
cmake --build build --target guillaume_hello_world
```

### Running

```sh
./build/examples/hello_world/guillaume_hello_world
```

## evan

An Evan-backed sample (macOS / Linux / Windows + GLFW) that uses the
[Evan](https://github.com/etib-corp/evan) Vulkan engine as the `Engine` behind a
Guillaume `Application`, drawing a container, a button and a 3D model.

Evan requires the Vulkan SDK. The example is added to the build on macOS, Linux
and Windows, and the matching Evan platform (`BUILD_FOR_MACOS`,
`BUILD_FOR_LINUX` or `BUILD_FOR_WINDOWS`) and the GLFW backend are selected
automatically from the host. The Evan dependency stays scoped to the example,
so the `guillaume` library itself has no engine dependency.

### Building

```sh
cmake -S . -B build -DBUILD_EXAMPLES=ON
cmake --build build --target guillaume_evan_example
```

### Running

```sh
./build/examples/evan/guillaume_evan_example
```
