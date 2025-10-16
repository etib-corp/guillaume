# Technical choices

Guillaume is a flexible, lightweight UI library for building user interfaces on XR and traditional platforms. Its design emphasizes a small footprint, predictable performance, and clean integration with custom rendering and input stacks.

## Programming Language

Guillaume is implemented in modern C++ (targeting C++20) to balance performance with safety and clarity. This enables:

- Low-latency code paths and deterministic memory control where needed.
- Broad portability and easy interop with existing engines and native toolchains.
- Use of modern language features to improve maintainability without sacrificing performance.

## Why not use an existing UI library?

Mature UI frameworks (for example, Qt or React Native) were evaluated. They commonly:

- Assume control over rendering and input pipelines, which conflicts with XR engines and custom renderers.
- Constrain frame scheduling and threading models, making XR latency budgets harder to meet.
- Optimize for 2D application UIs rather than spatial layouts, custom hit-testing, or XR interaction models.
- Make deep integration with domain-specific asset and scene pipelines more complex.

Additionally, they can be heavyweight and may not provide the flexibility needed for custom rendering systems.

## Script Language

Like QT with QML or Reat with JSX, Guillaume uses a declarative approach to define user interfaces.
For this purpose, the library will use a script language to describe the UI structure and behavior.

The choice of script language is still under consideration, with options including Python and Lua.

## Rendering

Guillaume is designed to be rendering-agnostic, allowing developers to integrate it with their custom rendering systems.

This flexibility enables the library to be used across different platforms and rendering technologies.

## Input Handling

Guillaume does not impose any specific input handling mechanism. Instead, it provides a flexible interface for developers to integrate their own input handling systems.

This approach ensures that the library can be adapted to various input devices and interaction paradigms.
