# Technical Choices

This document summarizes key technical decisions for Guillaume.

## Programming Languages

Guillaume is written in `C++20`.

Reasons:

- High performance and low-level control.
- Mature ecosystem for graphics and systems programming.
- Strong fit for cross-platform native applications.

## Libraries

- Utility: Shared helpers and data structures.
- SQLite: Embedded storage engine for local persistence.
- GoogleTest: Unit testing framework for verification.

## Build System

Guillaume uses CMake.

- Source files are under `sources`.
- Headers are under `headers`.
- External dependencies are integrated with `FetchContent`.

This setup keeps the project portable and reproducible across environments.
