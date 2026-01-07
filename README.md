# Guillaume

**A C++ UI framework designed for modern desktop and extended reality (XR) applications.**

## Key Features

## Quick Start

```cpp
```

## Documentation

- **[How Guillaume Works](docs/HOW_GUILLAUME_WORKS.md)** - Comprehensive guide to concepts, lifecycle, and usage patterns
- **[Architecture Documentation](docs/ARCHITECTURE.md)** - Detailed overview of the system's design and components
- **[Technical Choices Documentation](docs/TECHNICAL_CHOICES.md)** - Insights into design decisions and technologies

## Building

Guillaume builds as a static library along with examples and tests:

1. Clone the repository:

```bash
git clone https://github.com/etib-corp/guillaume.git
cd guillaume
```

1. Configure and build using CMake:

```bash
cmake -S . -B build -DBUILD_EXAMPLES=ON -DBUILD_TESTS=ON
cmake --build build
```

### Running Examples

To run the examples:

```bash
./build/examples/simple_application/simple_application
```

### Running Tests

To run the test suite:

```bash
./tests/test_guillaume
```

### Using Guillaume in Your Project

Guillaume builds as a static library (`libguillaume.a`) with minimal dependencies. To use it in your CMake project:

```cmake
# Link with Guillaume (core library has no external dependencies)
target_link_libraries(your_target PRIVATE guillaume)
```

## Examples

The `examples/` directory contains sample applications demonstrating Guillaume's features. Each example is organized in its own subdirectory:
