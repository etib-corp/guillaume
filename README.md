# Guillaume

Guillaume is a C++20 UI framework for modern desktop and extended reality (XR)
applications. It provides the application shell, an ECS-based UI composition
model, scene management, event dispatching, and storage helpers.

## Key Concepts

- **Application** — owns the engine, scene manager, event bus, and system
  registry; drives the main loop (see the
  [frame lifecycle](docs/ARCHITECTURE.md#frame-lifecycle)).
- **ECS** — entities, components, and systems compose the UI. Systems run in
  ordered phases (Event, Measure, Layout, Render) and traverse the entity
  hierarchy.
- **Scenes** — self-contained UI states with their own entity/component
  registries; the scene manager handles transitions.
- **Events** — an `EventBus` routes typed events to subscribed listeners and
  systems.
- **Storage** — `LocalStorage` (SQLite-backed, persistent) and
  `SessionStorage` (in-memory) key-value helpers.
- **Theme & metadata** — component metadata and theming support.

## Quickstart

A minimal application requires a `utility::Engine` implementation and at least
one `Scene` type:

```cpp
#include <utility/engine.hpp>

#include <guillaume/application.hpp>
#include <guillaume/scene.hpp>

class MyEngine : public utility::Engine {
    // Implement the pure virtual rendering/event methods.
};

class MyScene : public guillaume::Scene {
    // Build entities in onEnter().
};

int main() {
    auto resources = std::make_shared<utility::RessourceProvider>(...);
    guillaume::Application<MyScene> app(resources,
                                        std::make_unique<MyEngine>());
    return app.run();
}
```

See the [Getting Started](docs/GETTING_STARTED.md) tutorial and the
[examples](examples/) directory for a complete walkthrough.

## Building

### Dependencies

- CMake 3.10+
- A C++20 compiler
- Doxygen + Graphviz (only for `-DBUILD_DOCS=ON`)

SQLite3, GoogleTest, Google Benchmark and the Doxygen theme are vendored under
`third_party/`. The sibling `utility` library is fetched with CMake
`FetchContent`, so the first configure needs network access.

### Configure and build

```sh
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Optional options:

- `-DBUILD_TESTING=ON` — build and run the test suite.
- `-DBUILD_DOCS=ON` — build Doxygen documentation.
- `-DBUILD_BENCHMARKS=ON` — build the benchmark harness.

## Consuming as a dependency

The supported way today is to build Guillaume as part of your project:

```cmake
include(FetchContent)
FetchContent_Declare(guillaume
    GIT_REPOSITORY https://github.com/etib-corp/guillaume.git
    GIT_TAG v2026.3.1
)
FetchContent_MakeAvailable(guillaume)
target_link_libraries(my_app PRIVATE guillaume)
```

Guillaume loads its bundled fonts as `fonts/Roboto-Regular.ttf` and
`fonts/Roboto-Medium.ttf` through the `utility::RessourceProvider` you pass to
`Application`, so the resource root must contain the repository's `fonts/`
directory (installed to `share/guillaume/fonts`).

`cmake --install` also writes a `guillaumeConfig.cmake`, but
`find_package(guillaume)` does not work yet: the installed `utility` package
asks for a `glm` package that is not installed alongside it.
`tests/package_consumer/` is a minimal project that reproduces this and will
pass once it is fixed in `utility`.

## API Overview

The public API lives under `headers/guillaume/`. Doxygen documentation is
published to [GitHub Pages](https://etib-corp.github.io/guillaume).

## Documentation

- [Architecture](docs/ARCHITECTURE.md)
- [How Guillaume Works](docs/HOW_GUILLAUME_WORKS.md)
- [Getting Started](docs/GETTING_STARTED.md)
- [Theming](docs/THEMING.md)
- [Threading Model](docs/THREADING_MODEL.md)
- [Versioning & Support](docs/VERSIONING.md)
- [Engine Coupling](docs/ENGINE_COUPLING.md)
- [Migration Guide](docs/MIGRATION_GUIDE.md)
- [Technical Choices](docs/TECHNICAL_CHOICES.md)
- [Code Conventions](docs/CODE_CONVENTIONS.md)
- [Commit Conventions](docs/COMMIT_CONVENTIONS.md)

## Contributing

We welcome contributions! See the
[Contributing Guidelines](CONTRIBUTING.md) for how to get involved.

## License

Guillaume is released under the [MIT License](LICENSE). See
[CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) for our community standards and
[SECURITY.md](SECURITY.md) for reporting vulnerabilities.
