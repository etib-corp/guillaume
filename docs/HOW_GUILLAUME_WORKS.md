# How Guillaume Works

Guillaume exposes an `Application` template that owns the engine, the scene
manager, the event bus and the system registry.

## Lifecycle

1. Construct `Application<DefaultScene, OtherScenes...>` with a resource
   provider and a `utility::Engine`. The constructor registers the core
   systems, creates every scene and calls `onEnter()` on the default one.
2. Call `run()`. It loops over `routine()` until a quit event arrives.
3. Each `routine()` call is one frame; the exact order is in the
   [frame lifecycle](ARCHITECTURE.md#frame-lifecycle).

Call `routine()` yourself instead of `run()` when the host owns the loop (for
example an XR runtime that drives frames), and stop when `shouldQuit()`
returns true.

## Common Concepts

- `Scene` instances describe one screen or mode of the application. A scene
  owns its entities and the `ecs::ComponentRegistry` holding their state, and
  builds its widgets in `onEnter()` with the builders from
  `getBuilderManager()`. A scene requests a change with `goToScene<Next>()`;
  the switch happens at the end of the frame and calls `onExit()` then
  `onEnter()`.
- Entities are thin facades over their components: the framework never calls
  them back. Systems, owned by the application and shared by every scene, do
  the per-frame work on the active scene in four phases: Event, Measure,
  Layout, Render.
- `LocalStorage` stores persistent application state, by default in
  `guillaume/local-storage.db` under the per-user data directory of the
  platform (`LocalStorage::defaultStoragePath()`).
- `SessionStorage` stores state for the current session.
- The event system keeps input handling separate from rendering: engine
  events are published on the application's `event::EventBus`, queued by the
  systems subscribed to their type, and consumed in the Event phase.
