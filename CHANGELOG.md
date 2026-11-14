# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).
Versions follow the calendar scheme described in
[docs/VERSIONING.md](docs/VERSIONING.md); breaking changes are marked
**Breaking**.

## [Unreleased]

### Added

- `components::LayoutExtent`: a widget can reserve layout room beyond the
  shape it draws; `systems::Layout` adds it to the child's width.
- `components::Mesh::setFitSize()` / `entities::Mesh::setFitSize()` scale a
  3D model so its longest side has a given length in dp.
- The evan example is a widget gallery on nine pages; `GUILLAUME_GALLERY_PAGE`
  opens it on a given page.

### Changed

- **Breaking:** an entity's position is its top-left corner and content
  extends down, with y pointing up. Shapes used to be drawn upward from the
  position while text and icons were drawn downward, and vertical layouts
  stacked their first child at the bottom. Code that computes world
  positions by hand must subtract vertical offsets.
- `Scene::placeEntitiesInFrontOfView()` centres each root vertically on the
  view.
- Circular progress and loading arcs start at the top and run clockwise.
- An `Image` without a size takes its texture's size.
- A closed `Menu` draws nothing and ignores presses on its rows.
- The in-code documentation was rewritten for newcomers: every class says
  what it is, who writes and reads it and in which phase, with examples.
  `headers/guillaume/namespaces.hpp` is the map of the modules, and
  `docs/CODE_CONVENTIONS.md` has the documentation guide.
- **Breaking:** widgets are created with a static `create()` that returns
  them fully built; their constructors are private and
  `ecs::Entity::initialize()` is removed (`EntityRegistry::addEntity()` no
  longer calls it). Replace `std::make_shared<Widget>(...)` followed by
  `initialize()` with `Widget::create(...)`. Builders are unchanged.

### Fixed

- Transparent shapes were drawn solid black and translucent colors at full
  strength: the engine draws shapes with an opaque material. Fully
  transparent shapes, text and icons are no longer submitted, and
  translucent colors are composited over the scheme's Surface. Real
  blending needs an alpha-blended material in the engine.
- Labels of checkboxes, radio buttons and switches overlapped the next
  widget in a row.
- Radio buttons were filled discs, the indeterminate checkbox dash and
  progress fills were hidden behind their own shapes.
- 3D models were drawn at the world origin in their file's units, whatever
  their entity's position.
- `systems::Button` is registered by `registerCoreSystems()`; its per-frame
  sync of a button's icon and accessibility state did not run.

## [2026.6.0] - 2026-11-13

### Added

- `ecs::EntityRegistry::removeEntity()` and `clearEntities()` remove an
  entity with everything it owns, delete its components
  (`ecs::ComponentRegistry::removeEntity()`) and unlink it from its parent's
  `components::Hierarchy`. Entities parented to it go with it, wherever they
  are registered below the registry the removal started in. `List`, `Menu`, `NavigationDrawer` and the
  indicators use it when they rebuild their children, so replaced children no
  longer stay registered.

### Changed

- **Breaking:** `ecs::Entity::update()` is removed. `ecs::System::routine` no
  longer calls back into entities, and no library entity had an `update()`
  left. See `docs/MIGRATION_GUIDE.md`.
- **Breaking:** removed the unused `components::Parent`; use
  `components::Hierarchy`.

### Fixed

- `LocalStorage` waits for a concurrent writer instead of failing when
  several processes open the same storage file for the first time.
- An extended FAB with a hidden label no longer has its label color
  rewritten twice per frame.

## [2026.5.0] - 2026-11-13

### Added

- `List`, `Search`, `TextField`, `NavigationBar` and `NavigationDrawer`
  (with their destinations) follow the strict ECS. `TextField`'s
  configuration is `components::TextFieldConfiguration`; the existing
  `systems::TextField` holds its rules and moved to the Layout phase.

### Changed

- **Breaking:** `ecs::System::routine` no longer calls `Entity::update()` on
  a changed entity or on its parent. No library entity overrides `update()`
  any more; it remains as a deprecated no-op. See `docs/MIGRATION_GUIDE.md`.
- `entities::List`, `Search`, `TextField`, `NavigationBar` and
  `NavigationDrawer` are facades without fields. Visible differences:
  - Setters apply immediately instead of on the next update, and build a
    missing child at once.
  - A disabled accordion child of a `List` stays disabled when its parent
    expands; rows nested deeper than one accordion level are hidden.
  - `List::setVariant()` and `NavigationBar::addDestination()` no longer
    rebuild existing rows or destinations, so their identifiers are stable.
  - `Search::setTrailingIcon("")` hides the icon.
  - A `TextField` rewrites its accessibility label and hint every frame from
    `components::TextField`.

## [2026.4.0] - 2026-11-13

### Added

- `components::Hierarchy` holds an entity's parent identifier, child
  identifiers and layer. Every entity built on a component registry gets one,
  and `ecs::Entity::setParent()` keeps it up to date, so systems can walk the
  tree from identifiers alone. It supersedes the unused `components::Parent`.

- `components::SurfaceTone` and `systems::SurfaceTone`: the elevation and
  shadow mode of `entities::Surface` live in the component, and the system
  resolves the tonal color and the shadow from it each frame.

- `Checkbox`, `RadioButton` and `Switch` follow the strict ECS: their state
  lives in `components::Checkbox` / `RadioButton` / `Switch` (children are
  referenced by identifier) and `systems::Checkbox` / `RadioButton` / `Switch`
  style them and place their children each frame. The systems expose the
  actions as static functions (`toggle`, `press`, `setState`, `setChecked`)
  that work on an identifier.
- The button family follows the strict ECS. `components::Button` holds the
  state every button shares (content, toggle, callbacks, loading, visibility,
  color overrides, child identifiers) and `systems::Button` turns input into
  it. Each variant has a configuration component and a Layout-phase system
  that styles and lays it out: `CommonButton` (for `entities::Button`),
  `IconButton`, `FloatingActionButton`, `ExtendedFloatingActionButton`.
- `Badge`, `Chip`, `Tooltip`, `Snackbar`, `LoadingIndicator` and
  `ProgressIndicator` follow the strict ECS: each has a configuration
  component (including its transient state: fades, timers, animation
  progress) and a Layout-phase system of the same name whose static functions
  hold the rules and the actions (`show`, `hide`, `advance`, `start`,
  `stop`, ...).
- `AppBar`, `Card`, `Dialog`, `Menu`, `RadioGroup`, `SegmentedButton` and
  `FloatingActionButtonMenu` follow the strict ECS, each with a configuration
  component and a Layout-phase system of the same name.
- `components::CommonButton::setCorners()` overrides the radius of each
  corner; `systems::SegmentedButton` uses it to join its segments.
- `systems::Layout::layerOf()` returns an entity's tree depth from its
  `components::Hierarchy`.

### Changed

- `entities::Checkbox`, `RadioButton` and `Switch` are facades without fields
  and no longer override `update()`; `initialize()` only builds their child
  entities. Their `State` / `Variant` enums are now aliases of the component
  enums, so existing spellings keep compiling.
- `entities::ButtonBase`, `Button`, `IconButton`, `FloatingActionButton` and
  `ExtendedFloatingActionButton` are facades without fields. `ButtonBase` no
  longer overrides `update()` and its `applyStyleState()` / `applyGeometry()`
  virtuals are gone: subclasses outside the library must provide a variant
  system and register it with `components::Button::setRestyle()`. Enums such
  as `Button::Color` are aliases of the component enums.
- `entities::Badge`, `Chip`, `Tooltip`, `Snackbar`, `LoadingIndicator` and
  `ProgressIndicator` are facades without fields and no longer override
  `update()`. Visible differences:
  - `Chip::getChildren()` returns every child the chip owns; the system
    decides which are laid out, so an unselected filter chip reports its
    hidden check icon too.
  - The chip's accessibility label follows `setLabel()`.
  - An indicator's shape (fixed size, padding, radius, transparent container)
    is enforced every frame; a later `setColor()` or size set through
    `setSurfaceConfig()` on an indicator is overwritten.
  - An indeterminate `ProgressIndicator` is drawn before its first animation
    tick, and rebuilding an indicator's children stops the old animation.
  - `Tooltip` and `Snackbar` build a missing child as soon as a setter needs
    it, not on the next update.
- `entities::AppBar`, `Card`, `Dialog`, `Menu`, `RadioGroup`,
  `SegmentedButton` and `FloatingActionButtonMenu` are facades without fields
  and no longer override `update()`. Visible differences:
  - **Breaking:** `Card::getActions()` returns the vector by value instead of
    by `const` reference.
  - A dialog keeps the pose it was given; before, its Transform was reset to
    the default pose on every update, discarding `Builder::withPose()`.
  - Menu rows are an internal entity instead of `entities::View`, and
    `Menu::setVariant()` no longer rebuilds them.
  - Setters that need a new child (`AppBar::setTitle()`, the `Card` content
    setters, `Dialog::setTitle()` / `setMessage()`, `Menu::setItems()`) build
    it immediately, not on the next update.
  - Accessibility labels of the app bar, card and dialog titles follow later
    title changes.
  - `SegmentedButton::setSegmentDisabled()` keeps the connected corners.
- `entities::View` is a facade without fields and no longer overrides
  `update()`; no new component was needed, its state was already in
  Transform, Color, Borders and Layout. Visible differences:
  - Values written to a view's components by identifier are no longer
    overwritten by the view.
  - **Breaking:** a view no longer keeps the entities passed to
    `setEntities()` alive; they must be owned by an entity registry (which is
    what the builders do) or by the caller.
- `components::Elevation::setElevationLevel()` only marks the component
  changed when the resolved shadow differs.
- `entities::SurfaceBase` keeps no copy of its configuration and no longer
  overrides `initialize()` / `update()`: the Transform, Color, Borders and
  Layout components are the state and `systems::Layout` arranges the children.
  `getSurfaceConfig()` returns the configuration by value, read back from the
  components.
- `entities::Surface` is a facade without fields and no longer overrides
  `update()`; after a theme change the surface is restyled by
  `systems::SurfaceTone`, not by calling `update()` on it.
- `systems::Layout` reads the layer from `components::Hierarchy` instead of
  looking the entity object up.
- `ecs::Entity::setParent()` now updates the layer of every descendant when a
  subtree is attached, and throws `std::invalid_argument` when the new parent
  is the entity itself or one of its descendants.
### Fixed

- Releases are cut again. The version bump runs on the push to `main`
  instead of on the pull request event, which only had a read-only token for
  pull requests opened from a fork, and it calls the release workflow
  directly, since a tag pushed by a workflow does not start other workflows.


## [2026.3.3] - 2026-11-11

Includes the changes released as 2026.3.2 on the same day.

### Changed

- Core systems are registered by `systems::registerCoreSystems()`
  (`guillaume/systems/core_systems.hpp`); `guillaume/application.hpp` no longer
  includes every system header, so include the system headers you use.
- The hand input systems share `systems::HandEventSystem` /
  `systems::HandRaySystem` instead of six copies of the same wiring.
- Button, icon button and FAB styles resolve through lookup tables
  (`entities/button_style.hpp`, `entities/fab_style.hpp`) instead of `switch`
  blocks; the default Material 3 theme data moved to
  `sources/theme/default_theme.cpp`.
- Added `getActiveColor(role)`; removed the unused `SystemNotFoundException`.
- **Breaking:** `Application` takes the engine in its constructor and
  `setEngine()` is removed; the default scene is the first template argument
  and is no longer repeated (`Application<MyScene>`).
- **Breaking:** removed `Application::pollEvents/update/clear/present`;
  engine-backed systems and `SceneManager` take `utility::Engine &`.
- **Breaking:** entity `Director` classes are folded into their `Builder`
  (`builder.makeXxx(parent, ...)`); the director manager is removed.
- **Breaking:** `ecs::Phase` is a scoped enum (`enum class`); cast it
  explicitly where it was used as an integer.
- `LocalStorage` logs an error and reports `isPersistent() == false` when it
  falls back to in-memory storage; a parent directory that cannot be created
  now takes the same fallback instead of throwing.
- `Divider` is the first widget migrated to a strict ECS: its state lives in
  `components::Divider`, its behavior in `systems::Divider`, and
  `entities::Divider` is a facade without fields or an `update()` override.
- `Backdrop` and `Text` follow: `components::Backdrop` + `systems::Backdrop`,
  and `components::Typography` + `systems::Typography`. `entities::Text` no
  longer keeps its own copy of the content and color: the `Text` and `Color`
  components are the only source of truth, and a text is fully configured as
  soon as it is constructed (it no longer needs an `update()` call).
- **Behavior change:** `entities::Icon`, `entities::Image` and
  `entities::Mesh` no longer keep private copies of their glyph, paths and
  size and re-apply them on `update()`. The components are the only source of
  truth, so a value written directly to a component is no longer overwritten
  by the entity. `components::Image::getTexturePath()` now returns a
  reference.
- `SceneManager::switchToScene()` now calls `onExit()` / `onEnter()`.
- Theme accessors read `getActiveTheme()`; `setActiveTheme()` replaces it.

### Fixed

- `systems::Overlay` was registered twice and ran twice per frame.
- `systems::GlyphRender` and `systems::MeasureGlyph` no longer crash when the
  Material Symbols font or code point table is missing from the resource
  root; icons are skipped and an error is logged.
- `-DBUILD_DOCS=ON` now enables the documentation target; `BUILD_ALL_DOCS` is
  kept as a deprecated alias.
- GoogleTest is no longer installed by `cmake --install`; the bundled fonts
  are.

## [2026.3.1] - 2026-11-10

Versions 2026.1.0 to 2026.3.0 were tagged without changelog entries, so this
section lists everything released since 1.0.0.


### Added

- Material Design 3 component families (#39–#66): `AppBar`, `Badge`,
  `BottomSheet`, `Card`, `Carousel`, `Checkbox`, `Chip`, `DatePicker`,
  `Dialog`, `Divider`, `List`, `LoadingIndicator`, `Menu`, `NavigationBar`,
  `NavigationDrawer`, `NavigationRail`, `ProgressIndicator`, `RadioButton`,
  `Search`, `SideSheet`, `Slider`, `Snackbar`, `Switch`, `Tabs`, `TextField`,
  `TimePicker`, `Toolbar`, and `Tooltip`, each with a `Builder` + `Director`
  and `TEST_F` coverage. The families that react-native-paper does not ship
  were removed again in this release (see **Removed**).
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
- Core systems are registered by `systems::registerCoreSystems()`
  (`guillaume/systems/core_systems.hpp`); `guillaume/application.hpp` no longer
  includes every system header, so include the system headers you use.
- The hand input systems share `systems::HandEventSystem` /
  `systems::HandRaySystem` instead of six copies of the same wiring.
- Button, icon button and FAB styles resolve through lookup tables
  (`entities/button_style.hpp`, `entities/fab_style.hpp`) instead of `switch`
  blocks; the default Material 3 theme data moved to
  `sources/theme/default_theme.cpp`.
- Added `getActiveColor(role)`; removed the unused `SystemNotFoundException`.
- **Breaking:** `Application` takes the engine in its constructor and
  `setEngine()` is removed; the default scene is the first template argument
  and is no longer repeated (`Application<MyScene>`).
- **Breaking:** removed `Application::pollEvents/update/clear/present`;
  engine-backed systems and `SceneManager` take `utility::Engine &`.
- **Breaking:** entity `Director` classes are folded into their `Builder`
  (`builder.makeXxx(parent, ...)`); the director manager is removed.
- `SceneManager::switchToScene()` now calls `onExit()` / `onEnter()`.
- Theme accessors read `getActiveTheme()`; `setActiveTheme()` replaces it.
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

### Removed

- Component families with no react-native-paper equivalent (#114):
  `BottomSheet`, `Carousel`, `DatePicker`, `NavigationRail`, `SideSheet`,
  `Slider`, `Tabs`, `TimePicker` and `Toolbar`.

## [2026.3.0] - 2026-10-08

## [2026.2.0] - 2026-09-26

## [2026.1.0] - 2026-09-26

## [1.0.0] - 2025-08-25

### Added

- Initial release of the Guillaume UI framework.
- Application lifecycle management.
- Entity-component-system based UI structures.
- Scenes, event handlers, and render systems.
- Local and session storage helpers.
- Theme and component metadata support.
