# Threading Model

Guillaume is primarily **single-threaded**. This document describes the
concurrency guarantees so consumers can structure their applications safely.

## Main Loop

The application main loop (`Application::run`) executes on a single thread:

```
input → update → render → present
```

All ECS registries, the event bus, and systems are used from this owner
thread. They are **not** thread-safe and must not be accessed concurrently
from other threads.

## Storage

`Storage` is the only module safe for cross-thread use:

- `LocalStorage` is guarded by an internal mutex and serializes all SQLite
  access.
- `SessionStorage` is likewise synchronized.

You may call `setItem`/`getItem`/`removeItem`/`clear` from any thread.

## Guidance for Background Work

If you need to perform work off the main thread (e.g. loading assets, network
I/O):

1. Do the work on a worker thread.
2. **Dispatch the result back onto the main loop** (e.g. via an event posted
   to the `EventBus`).
3. Only touch ECS registries, scenes, or systems from the main thread.

Do not mutate entities, components, or scenes from worker threads.

## Entity Identifiers

`Entity::getNextIdentifier` uses an atomic counter, so identifier generation
is safe even if entities are created from multiple threads. However, the
registries that own those entities are still single-threaded.
