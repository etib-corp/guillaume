# GUI ILLustration Augmented Unified MEdia Engine

Guillaume is a C++ library designed to simplify the development of graphical user interfaces (GUIs) and multimedia applications. It provides a set of primitives, event handling mechanisms, and utilities to help developers build interactive applications efficiently. The library aims to support both desktop and AR/VR platforms.

## Features

- **Cross-Platform**: Runs on desktop and AR/VR devices.
- **Event Handling**: Robust system for managing user interactions.
- **Graphical Primitives**: Includes a variety of rendering primitives.
- **Extensible**: Easily add custom primitives and events.

## Development

Guillaume is actively developed and welcomes contributions. Its modular architecture makes it straightforward to add new features or modify existing ones.

## Installation

To install Guillaume, clone the repository and include the headers in your project. CMake is recommended for managing dependencies and building the library.

1. Clone the repository:

    ```bash
    git clone https://github.com/etib-corp/guillaume.git
    cd guillaume
    ```

2. Build the library with CMake:

    ```bash
    cmake -S . -B build
    cmake --build build
    ```

3. Run the test application:

    ```bash
    ./build/guillaume
    ```
