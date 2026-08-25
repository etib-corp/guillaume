# Guillaume Technical Choices

## Language

Guillaume is written in C++20 for the same portability and performance reasons
as the rest of the workspace.

## Dependencies

- Utility provides shared helpers, math types, graphics types, and events.
- SQLite3 is used for embedded local storage.

## Build Model

Guillaume is built with CMake and keeps its public headers under `headers` and
its implementation files under `sources`.

## Design Choice

The framework favors a compact native API over framework-specific abstractions,
which keeps it suitable for desktop and XR UIs.
