# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Standard open-source documentation: `CHANGELOG.md`, `CODE_OF_CONDUCT.md`,
  `CONTRIBUTING.md`, `SECURITY.md`, `AUTHORS.md`, and `LICENSE`.
- Packaging & consumability: `install()`/`export()`, a CMake package config
  (`guillaumeConfig.cmake`) for `find_package(guillaume)`, and CPack rules.
- Benchmark harness (`BUILD_BENCHMARKS`) covering layout, redraw, event
  dispatch, and memory footprint.
- CI hardening: `ctest` execution on Linux/macOS/Windows, ASan/UBSan and
  `clang-tidy` jobs, coverage reporting, and a benchmark report job.
- Documentation: full README, Getting Started tutorial, threading model,
  architecture diagram, semver & support policy, and engine-coupling decision.

### Changed

- Replaced `file(GLOB)` with explicit source lists for reproducible builds.
- Moved include paths onto the `guillaume` target (target-scoped includes).
- `Entity::getNextIdentifier` is now atomic (thread-safe identifier generation).
- `EventBus::subscribe` now takes its listener by value.
- `SceneManager::getActiveScene()` now returns a `Scene*` instead of exposing
  the internal `std::unique_ptr<Scene>&`.
- `LocalStorage` reuses prepared SQLite statements and guards `sqlite3_errmsg`
  against a null database handle.
- `EntityRegistry::getEntity` returns `nullptr` for missing entities instead of
  throwing; `SystemRegistry::getSystemsByPhase` returns an empty vector for
  valid phases and validates the phase bounds.
- `Entity::initialize`/`update` are no longer pure virtual (default no-ops).
- Added `noexcept` to trivial getters across the ECS.

### Fixed

- Removed the shared `static firstView` in `Application::routine` (was shared
  across instances).
- Removed the empty `test_entity_registry.cpp` test stub.
- Fixed pre-existing test build failures (abstract `Entity` instantiation,
  stale test APIs, missing font asset).

## [1.0.0] - 2025-08-25

### Added

- Initial release of the Guillaume UI framework.
- Application lifecycle management.
- Entity-component-system based UI structures.
- Scenes, event handlers, and render systems.
- Local and session storage helpers.
- Theme and component metadata support.
