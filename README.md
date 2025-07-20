# Guillaume GUI Framework

**G**UI **U**nified **I**nteractive **L**ibrary for **L**ightweight **A**pplications **U**sing **M**odern **E**ngineering

Guillaume is a modern C++ library designed to simplify the development of graphical user interfaces (GUIs) and multimedia applications. Built with SDL3 as the rendering backend, it provides a robust set of primitives, event handling mechanisms, and utilities to help developers build interactive applications efficiently across multiple platforms.

## Features

- **Modern C++20**: Built with contemporary C++ standards for performance and maintainability
- **Cross-Platform**: Native support for Windows, macOS, and Linux
- **Event-Driven Architecture**: Comprehensive event handling system for user interactions
- **Modular Design**: Component-based architecture for easy extensibility
- **Hardware Accelerated**: OpenGL/Vulkan rendering through SDL3
- **Type-Safe**: Strong typing system with clear interfaces
- **Memory Safe**: RAII and smart pointer usage throughout

## Quick Start

### Prerequisites

- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- CMake 3.10 or higher
- Git (for cloning submodules)

### Installation

1. **Clone the repository with submodules:**

   ```bash
   git clone https://github.com/etib-corp/guillaume.git --recursive
   cd guillaume
   ```

2. **Build using CMake:**

   ```bash
   cmake -S . -B build
   cmake --build build
   ```

3. **Run the demo application:**

   ```bash
   ./build/guillaume
   ```

### Basic Usage

```cpp
#include "guillaume.h"

int main() {
    // Initialize Guillaume context
    auto context = guigui::Context::create("My App", 800, 600);
    
    // Create a button component
    auto button = guigui::Button::create("Click Me!");
    button->set_position({100, 100});
    button->set_size({200, 50});
    
    // Add event handler
    button->on_click([](){ 
        std::cout << "Button clicked!" << std::endl; 
    });
    
    // Add to context and run
    context->add_component(button);
    context->run();
    
    return 0;
}
```

## Architecture Overview

Guillaume follows a component-based architecture with the following core concepts:

### Core Components

- **Context**: Main application context managing the window and render loop
- **Renderer**: Abstraction layer for rendering operations
- **Component**: Base class for all UI elements
- **Event System**: Type-safe event handling and propagation
- **Primitives**: Low-level drawing operations

### Class Hierarchy

```text
Componentable (Interface)
├── Component (Base UI Element)
│   ├── Button
│   ├── Label
│   └── Container
│       ├── Panel
│       └── Window
└── Primitive (Base Drawing Element)
    ├── Rectangle
    ├── Circle
    └── Line
```

## Development Guide

### Project Structure

```text
guillaume/
├── headers/                 # Public API headers
│   └── guillaume/
│       ├── components/      # UI component headers
│       ├── events/         # Event system headers
│       └── primitives/     # Drawing primitive headers
├── sources/                # Implementation files
│   └── guillaume/
│       ├── components/
│       └── primitives/
├── external/               # Third-party dependencies
│   ├── SDL/               # SDL3 library
│   └── SDL_ttf/           # SDL_ttf for text rendering
├── docs/                  # Generated documentation
├── build/                 # Build artifacts (generated)
└── assets/               # Resources and assets
```

### Building from Source

#### Debug Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

#### Release Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

#### Build Options

```bash
# Disable SDL_ttf features if not needed
cmake -S . -B build -DSDLTTF_HARFBUZZ=OFF -DSDLTTF_FREETYPE=OFF

# Enable PlutoSVG support
cmake -S . -B build -DSDLTTF_PLUTOSVG=ON
```

### Testing

```bash
# Build and run tests
cmake --build build
./build/guillaume

# Run with logging enabled
GUILLAUME_LOG_LEVEL=DEBUG ./build/guillaume
```

### Documentation

Guillaume uses Doxygen for API documentation generation:

```bash
# Generate documentation
doxygen Doxyfile

# View documentation
open docs/html/index.html
```

## Development Workflow

### Setting Up Development Environment

1. **IDE Configuration**: Guillaume supports any C++20 compatible IDE. VS Code configuration included.

2. **Code Style**: Follow the established code style (see [CONTRIBUTING.md](docs/CONTRIBUTING.md))

3. **Commit Guidelines**: Use conventional commits for clear history

### Adding New Components

1. Create header file in `headers/guillaume/components/`
2. Implement in `sources/guillaume/components/`
3. Inherit from `Component` base class
4. Implement required virtual methods
5. Add comprehensive documentation
6. Include unit tests

### Adding New Events

1. Define event class in `headers/guillaume/events/`
2. Inherit from `Event` base class
3. Implement event data structures
4. Update event dispatcher
5. Document event usage patterns

## API Reference

### Core Classes

- [`Context`](docs/api/context.md) - Application context and window management
- [`Renderer`](docs/api/renderer.md) - Rendering abstraction layer
- [`Component`](docs/api/component.md) - Base UI component class
- [`Event`](docs/api/event.md) - Event system base class

### Components

- [`Button`](docs/api/button.md) - Interactive button component
- [`Container`](docs/api/container.md) - Component container and layout
- [`Label`](docs/api/label.md) - Text display component

### Utilities

- [`Color`](docs/api/color.md) - Color manipulation utilities
- [`Vector`](docs/api/vector.md) - 2D vector mathematics
- [`Logger`](docs/api/logger.md) - Logging system

## Examples

Comprehensive examples are available in the [examples](examples/) directory:

- [Basic Window](examples/basic_window.cpp) - Simple window creation
- [Button Interaction](examples/button_demo.cpp) - Button event handling
- [Layout System](examples/layout_demo.cpp) - Container and layout usage
- [Custom Components](examples/custom_component.cpp) - Creating custom components

## Contributing

We welcome contributions! Please see our [Contributing Guide](docs/CONTRIBUTING.md) for details on:

- Code of Conduct
- Development Process
- Pull Request Guidelines
- Code Style Requirements
- Testing Requirements

## Roadmap

See [ROADMAP.md](ROADMAP.md) for planned features and development timeline.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Support

- **Documentation**: [Online Documentation](https://etib-corp.github.io/guillaume/)
- **Issues**: [GitHub Issues](https://github.com/etib-corp/guillaume/issues)
- **Discussions**: [GitHub Discussions](https://github.com/etib-corp/guillaume/discussions)

## Acknowledgments

- SDL3 Development Team for the excellent multimedia library
- Contributors and community members
- Inspiration from other GUI frameworks like Dear ImGui and FLTK
