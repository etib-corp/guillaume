# How Guillaume Works

Guillaume exposes a small application template that owns the engine, storage,
and scene manager objects.

## Lifecycle

1. Construct an `Application` specialization with the engine, storage,
   and scene types you need.
2. Use `pollEvents()` to process input and platform activity.
3. Call `update()` to advance application state.
4. Call `clear()` and `present()` to render a frame.

## Common Concepts

- `Scene` instances describe one screen or mode of the application.
- `LocalStorage` stores persistent application state.
- `SessionStorage` stores state for the current session.
- The event system keeps input handling separate from rendering.

## Practical Usage

Guillaume is designed so higher-level applications can compose their own scene
set and own the main loop without reimplementing the plumbing.
