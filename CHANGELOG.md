# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2026.1.0] - 2026-09-26

## [2026.2.0] - 2026-09-26

## [2026.3.0] - 2026-10-08

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
- Text and icon children are now consistently lifted one content layer in
  front of their container surface: `systems::Layout` exposes
  `ContentLayerOffset`/`contentLayerPose`, and
  `entities/placement_helpers.hpp` routes manual placement (`placeChild`,
  `centerChild`, `placeNormalized`) through it, so `Slider`, `Switch`,
  `ProgressIndicator`, `LoadingIndicator`, `RadioButton` and `TimePicker`
  match the already-layered `Layout` families.

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
- Unified every drawable on a single top-left origin convention:
  `Scene::placeEntitiesInFrontOfView` no longer adds a half-width centre
  offset, `ScrimRender` now rotates its quad by the transform orientation like
  the other shape renderers, and the text/glyph renderers re-anchor the glyph
  box top-left onto the entity position via `systems::anchorTextTopLeft`.
- `IconButton` and `FloatingActionButton` now position their icon (and the FAB
  label) instead of leaving it at the default pose.
- Scenes now place their root entities *after* `onEnter()` builds them.
  `SceneManager::enterActiveScene` previously called
  `placeEntitiesInFrontOfView` before `onEnter`, so entities created there
  (and their text/icon children) were never positioned and stayed at the
  origin.
- Entities are no longer moved or rotated when the view changes. The removed
  `Application::routine` view-flip re-placement (and its `_firstView` state)
  used to reposition and re-orient every root entity when the view turned
  more than 90 degrees; entities are now placed once per scene entry and left
  untouched afterwards.
- Text and glyphs now render at their entity position regardless of the
  renderer's model-matrix handling. `systems::TextRender`/`GlyphRender` emit a
  new `guillaume::TextRenderable` (a `utility::graphic::Text` whose glyph
  vertices are baked into world space with an identity pose), matching the
  shape/image renderers that already bake world coordinates. Previously the
  local-space glyph mesh relied on the engine applying
  `Renderable::getModelMatrix()`, so text collapsed to the origin.
- `systems::Layout` now arranges only the container's registered layout
  children (`components::Layout::getChildren()`, populated by
  `SurfaceBase::setChildren` and `entities::Layout::setEntities`) instead of
  rediscovering every child from the entity hierarchy. Manually positioned
  children (a switch thumb, a slider track/fill/thumb, a radio dot, clock
  markers, ...) keep their own placement again, so containers no longer
  compact or mis-position their contents.
- `components::Bound` now marks itself changed when its width or height is
  set, so a measured size written by `systems::MeasureText`/`MeasureGlyph`
  propagates to the parent and containers re-hug their text.

## [1.0.0] - 2025-08-25

### Added

- Initial release of the Guillaume UI framework.
- Application lifecycle management.
- Entity-component-system based UI structures.
- Scenes, event handlers, and render systems.
- Local and session storage helpers.
- Theme and component metadata support.
