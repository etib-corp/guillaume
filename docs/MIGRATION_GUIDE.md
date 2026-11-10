# Migration Guide

This guide documents breaking API changes introduced for v1.0 and how to
update existing code.

## `Application` takes its engine in the constructor

**Before:** `Application<MyScene, MyScene> app(resources);` followed by
`app.setEngine(std::move(engine));`. The default scene had to be repeated in
the scene list, and calling `run()` before `setEngine()` dereferenced a null
engine.

**After:** `Application<MyScene> app(resources, std::move(engine));`. The
first template argument is the default scene and every scene is listed once
(`Application<Home, Settings>`); a repeated scene type no longer compiles. A
null engine throws `std::invalid_argument`. `setEngine()` is gone.

**Update:** Pass the engine to the constructor and remove the duplicated
default scene from the template arguments.

`Application::pollEvents()`, `update()`, `clear()` and `present()` were removed:
they only forwarded to the engine and were not used by `run()`. Call
`routine()` to run one frame.

## Systems take `utility::Engine &`

**Before:** engine-backed systems were constructed from a
`std::unique_ptr<utility::Engine> &`.

**After:** they take a `utility::Engine &`. `SceneManager` also takes the engine
by reference in its constructor; `SceneManager::setEngine()` is gone.

**Update:** Pass `*engine` when constructing a system yourself.

## `SceneManager::switchToScene` runs the scene lifecycle

**Before:** `switchToScene<T>()` only changed the active scene type.

**After:** it calls `onExit()` on the current scene and `onEnter()` on the new
one, like a transition requested with `Scene::goToScene<T>()`.

## `application.hpp` no longer includes every system

Core systems are registered by `systems::registerCoreSystems()`
(`guillaume/systems/core_systems.hpp`). Include the header of any system you
reference directly.

## Theme accessors read the active theme

`getShapeRadius()`, `getStateLayerAlpha()`, `getElevationToken()`,
`getMotionDuration()`, `getActiveTypescale()` and `getActiveScheme()` now read
`getActiveTheme()` instead of `defaultTheme`. Behavior is unchanged until you
call `setActiveTheme()`. `getActiveColor(role)` replaces the
`getActiveScheme().getColor(role).getColor()` chain.

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
- **Family defaults:** `Button` defaults to the `Text` style (Paper
  `mode="text"`); `Chip` defaults to the flat (secondary container) mode;
  `AppBar` heights are 64/112/152 dp with 4 dp padding; `Badge` defaults to
  the 20 dp variant; `LoadingIndicator` defaults to circular; `Search`
  `FullScreen` is a 72 dp inline field; `TextField` width follows its content
  (minimum 100 dp); `Divider` defaults to a hairline; `View` no longer fills
  white.
- **Checkbox / RadioButton / Switch** are pressable; their labels sit beside
  the control instead of inside it, and `Variant::Disabled` routes through
  `components::StyleState`.
