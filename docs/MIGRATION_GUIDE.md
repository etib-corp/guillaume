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

## `Panel` children are attached by shared pointer, not identifier

**Before:** `Panel::Builder::withEntities` (and the `Panel` constructor) took a
`std::vector<ecs::Entity::Identifier>`. The identifiers were stored but never
resolved, so children were not actually attached to the panel.

**After:** The panel takes a
`std::vector<std::shared_ptr<ecs::Entity>>`. Children are parented to the
panel on initialization, lifted toward the camera by their layer depth step
to avoid z-fighting with the panel surface, and the panel `Bound` is
computed from the children bounds plus the panel padding.

**Update:** Pass the shared pointers returned by the entity builders instead
of identifiers, e.g.:

```cpp
// Before
panelDirector.makeDefaultPanel(panelBuilder, parent, pose,
                               { text->getIdentifier() });

// After
panelDirector.makeDefaultPanel(panelBuilder, parent, pose, { text });
```

