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
example an XR runtime that drives frames).

## Common Concepts

- `Scene` instances describe one screen or mode of the application. A scene
  requests a change with `goToScene<Next>()`; the switch happens at the end of
  the frame and calls `onExit()` then `onEnter()`.
- `LocalStorage` stores persistent application state.
- `SessionStorage` stores state for the current session.
- The event system keeps input handling separate from rendering.
