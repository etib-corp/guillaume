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
platform.

### Building

```sh
cmake -S . -B build -DBUILD_EXAMPLES=ON
cmake --build build --target guillaume_hello_world
```

### Running

```sh
./build/examples/hello_world/guillaume_hello_world
```
