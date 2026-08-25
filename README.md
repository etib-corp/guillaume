# Guillaume

Guillaume is the UI framework used by XIDER. It provides the application
shell, ECS-based UI composition, scene management, event dispatching, and
storage helpers for desktop and XR user interfaces.

## What It Covers

- Application lifecycle management.
- Entity-component-system based UI structures.
- Scenes, event handlers, and render systems.
- Local and session storage helpers.
- Theme and component metadata support.

## Build Notes

Guillaume is a C++20 library built with CMake and fetched dependencies such as
Utility and SQLite3.

## Documentation

- [Architecture](docs/ARCHITECTURE.md)
- [How Guillaume Works](docs/HOW_GUILLAUME_WORKS.md)
- [Technical Choices](docs/TECHNICAL_CHOICES.md)
