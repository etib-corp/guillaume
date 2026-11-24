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

## Program-wide Counters

The ECS keeps three counters that are shared by the whole program rather
than owned by one registry. All three are atomic, so they are the only parts
of the ECS that may be reached from several threads at once:

- **Entity identifiers.** `Entity::getNextIdentifier` hands out unique
  identifiers, so entities may be *created* from multiple threads.
- **Component type identifiers.** `ComponentTypeId::get<T>()` may be called
  from multiple threads, including for a type used for the first time: each
  type gets exactly one identifier and no two types share one. The order in
  which concurrent first uses are numbered is unspecified.
- **Entity tree version.** Every `EntityRegistry` shares one version counter
  to invalidate its cached breadth-first walk. Two threads may therefore
  each add to and remove from **their own, separate** entity tree without
  racing on that counter.

This does not make the registries themselves thread-safe. A given
`EntityRegistry` (and the registries nested in it), `ComponentRegistry`,
`SystemRegistry` or `System` must still be used by one thread at a time, and
handing one over to another thread needs external synchronization.
