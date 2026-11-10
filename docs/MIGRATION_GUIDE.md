# Migration Guide

This guide documents breaking API changes introduced for v1.0 and how to
update existing code.

## `EntityRegistry::getEntity` now returns `nullptr` instead of throwing

**Before (v0.x):** `getEntity<EntityType>(id)` threw `std::runtime_error` when
the entity was not found or was not of the expected type.

**After (v1.0):** `getEntity<EntityType>(id)` returns `nullptr` when the entity
is not found or is not of the expected type. It also returns `nullptr` for
`Entity::InvalidIdentifier`.

**Update:** Check the returned pointer for `nullptr` instead of wrapping the
call in a `try`/`catch`.

```cpp
// Before
try {
    auto e = registry.getEntity<MyEntity>(id);
} catch (const std::runtime_error &) { /* handle */ }

// After
auto e = registry.getEntity<MyEntity>(id);
if (e == nullptr) { /* handle */ }
```

## `SceneManager::getActiveScene` returns a `Scene*`

**Before:** `getActiveScene()` returned `std::unique_ptr<Scene>&`, exposing
mutable ownership of an internal `unique_ptr`.

**After:** `getActiveScene()` returns a `Scene*`. Callers must not delete the
returned pointer; the scene manager retains ownership.

**Update:** Most call sites that used `->` continue to work unchanged. If you
stored the result as `std::unique_ptr<Scene>&`, change it to `Scene*`.

## `EventBus::subscribe` takes its listener by value

**Before:** `subscribe<EventType>(const Listener &listener)`.

**After:** `subscribe<EventType>(Listener listener)` (moved into the bus).

**Update:** Existing call sites that pass a lambda or `std::function` continue
to work unchanged.

## `SystemRegistry::getSystemsByPhase` validates the phase

**Before:** `getSystemsByPhase(phase)` used `std::map::at`, throwing
`std::out_of_range` for an invalid phase.

**After:** `getSystemsByPhase(phase)` returns an empty vector for a valid phase
with no systems, and throws `std::out_of_range` only for an invalid phase
value.

**Update:** No change required for valid phases.

## `Entity::initialize` / `Entity::update` are no longer pure virtual

**Before:** `Entity` was abstract; derived entities had to implement
`initialize()` and `update()`.

**After:** Both have default no-op implementations. Derived entities may still
override them.

**Update:** No change required; existing overrides continue to work.

## `noexcept` on trivial getters

Several trivial getters (e.g. `Entity::getIdentifier`, `Entity::getSignature`,
`Entity::getLayer`, `Entity::getParent`, `System::getPhase`,
`System::getSignature`) are now `noexcept`. This is source-compatible.

## `Entity::getNextIdentifier` is atomic

Identifier generation is now thread-safe. No source change is required.

## `Panel` and `Container` replaced by `Layout`

The `Panel` entity was replaced by `Container`, which is now itself replaced by
`Layout`. `Layout` keeps the surface behavior (pose, color, border radius) and
delegates child arrangement to `components::Layout` + `systems::Layout`.

**Before:** `Container` owned the row/column arrangement, padding, spacing and
margin directly.

**After:** `Layout` stores the arrangement in a `components::Layout` and exposes
the standard builder/director:

- `Layout::Axis::Horizontal` / `Layout::Axis::Vertical` (default `Horizontal`)
- `Layout::MainAxisAlignment::{Start, Center, End, SpaceBetween}`
- `Layout::CrossAxisAlignment::{Start, Center, End}`
- `spacing` — gap between adjacent children
- `padding` — space around the children
- `withFixedWidth` / `withFixedHeight` — force a dimension (auto by default)

The geometry is produced by `systems::Layout`, registered as a core system in
the Layout phase. It positions the children, applies the main/cross axis
alignments and writes the resulting size back to the container
`components::Bound`. The old `margin` parameter is gone: use an outer `Layout`
or adjust the pose instead.

**Update:** Rename `Container` to `Layout` and use the new builder/director
methods. The method names changed from `makeDefaultContainer` /
`makeColorContainer` to `makeDefaultLayout` / `makeColorLayout`:

```cpp
// Before
containerDirector.makeDefaultContainer(containerBuilder, parent, pose,
                                       { text });

// After
layoutDirector.makeDefaultLayout(layoutBuilder, parent, pose, { text });
```


## Material 3 parity defaults (milestone "M3 parity remediation")

Several defaults changed to match react-native-paper v5. See
`docs/THEMING.md` for the token layer.

- **`components::Borders`** gains a stroke width (default `0`) and its color
  now defaults to transparent. A border color alone paints nothing; call
  `setWidth()` to draw the stroke.
- **`components::Line`** can carry its own stroke color (`setColor()`) and an
  edge placement (`Placement::Top/Center/Bottom`). Without a color it still
  uses `components::Color`.
- **`entities::Text`** defaults to the `BodyMedium` role and the `OnSurface`
  color instead of 24 px white. The `fontSize` constructor/director overloads
  still work and override the role size; prefer the `theme::TypeRole`
  overloads.
- **`entities::Icon`** defaults to `OnSurface` instead of white.
- **Button family labels** (`Button`, `FloatingActionButton`,
  `ExtendedFloatingActionButton`) use `labelLarge` instead of the icon size.
- **Shapes:** icon buttons are circular at every size and chips use the 8 dp
  `Small` shape; radii come from `getShapeRadius()`.
- **`components::Elevation`** defaults to the level-1 token; use
  `setElevationLevel()` rather than raw `setLevel()/setSpread()`.
- **`ButtonBase`** no longer stores `_isDisabled` or `_accessibilityLabel`:
  `isDisabled()`/`setDisabled()` read and write `components::StyleState`, and
  the accessibility label lives in `components::Accessibility`.
- **Snackbar** auto-dismisses after `MotionScale::SnackbarDuration` (7 s, the
  Paper default) instead of 4 s.
