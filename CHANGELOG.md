# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2026.1.0] - 2026-09-26

## [2026.2.0] - 2026-09-26

## [Unreleased]

### Added

- Material Design 3 component families (#39–#66): `AppBar`, `Badge`,
  `BottomSheet`, `Card`, `Carousel`, `Checkbox`, `Chip`, `DatePicker`,
  `Dialog`, `Divider`, `List`, `LoadingIndicator`, `Menu`, `NavigationBar`,
  `NavigationDrawer`, `NavigationRail`, `ProgressIndicator`, `RadioButton`,
  `Search`, `SideSheet`, `Slider`, `Snackbar`, `Switch`, `Tabs`, `TextField`,
  `TimePicker`, `Toolbar`, and `Tooltip`, each with a `Builder` + `Director`
  and `TEST_F` coverage.
- Shared entity foundations for component families: `entities::SurfaceBase`
  and `entities::SurfaceConfig`, which arrange children through
  `systems::Layout` instead of hand-rolled geometry, plus
  `entities/content_helpers.hpp`, `entities/style_helpers.hpp`,
  `entities/placement_helpers.hpp` and `entities/overlay_helpers.hpp`.
- Reusable `entities::EntityBuilderBase` (`buildEntity` hook + shared
  `registerEntity`) and `entities::EntityDirectorBase`, adopted by all 41
  entity builders/directors to remove the repeated boilerplate.
- `systems::Selection::select`, which selects a child and reconciles a
  `components::SelectionGroup` in one call.
- Registered `components::Ellipse` in `ComponentRegistry`.
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
- Rebased the button composite families (`SegmentedButton`, `SplitButton`,
  `StandardButtonGroup`, `FloatingActionButtonMenu`) onto `systems::Layout`,
  replacing their hand-rolled child positioning.
- `ButtonBase` now reuses `systems::Layout::applyLayerToPosition` and the shared
  `entities/style_helpers.hpp` helpers instead of duplicating them.
- Refactored the M3 families to remove duplication: all builders/directors use
  the shared CRTP bases; the navigation/tab/date families route exclusive
  selection through `systems::Selection`; absolutely-positioned children use
  `entities/placement_helpers.hpp`; the shared panel preset lives in
  `SurfaceConfig::panel`.
- Overlay families expose canonical `show()/hide()/isVisible()` with
  `open()/close()/isOpen()` as forwarding aliases.
- Component getters (`isVisible/isOpen/isSelected/isChecked/isRunning`,
  `getText/getValue/getLow/getHigh`) are now `const`.
- Test fixtures are consolidated into the shared
  `tests/headers/entities/family_fixture.hpp` helper.

### Fixed

- `DatePicker` day cells now render their day-number labels (the label child
  was built but never attached).
- Removed the shared `static firstView` in `Application::routine` (was shared
  across instances).
- Removed the empty `test_entity_registry.cpp` test stub.
- Fixed pre-existing test build failures (abstract `Entity` instantiation,
  stale test APIs, missing font asset).
- Updated the `evan` example to the `entities::Layout` API (the removed
  `entities::Container` was still referenced).

## [1.0.0] - 2025-08-25

### Added

- Initial release of the Guillaume UI framework.
- Application lifecycle management.
- Entity-component-system based UI structures.
- Scenes, event handlers, and render systems.
- Local and session storage helpers.
- Theme and component metadata support.
