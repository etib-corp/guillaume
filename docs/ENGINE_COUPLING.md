# Engine Coupling Decision

## Status

**Accepted** — Guillaume is engine-coupled for v1.0. No platform/rendering
abstraction layer will be introduced before v1.0.

## Context

The original audit brief described an "engine-less" library, but Guillaume is
engine-coupled: `Engine` is an abstract interface
(`headers/guillaume/engine.hpp`) and `Application`/`SceneManager`/systems call
`addMesh`/`addText`/`addModel`/`clear`/`present`. There is no rendering
backend, windowing, or platform abstraction shipped in this repository.

## Decision

We **accept the engine-coupled design** and document it explicitly:

- Rendering and event polling are delegated to an `Engine` implementation that
  consumers provide for their platform.
- Guillaume provides the application shell, ECS-based UI composition, scene
  management, event dispatching, and storage.
- The `Engine` interface is the single integration point for a platform.

## Rationale

- The current design is functional and keeps the framework focused on UI
  composition rather than platform plumbing.
- A platform/rendering abstraction layer is a large effort and is better
  scoped as a post-v1.0 roadmap item.
- Consumers already integrate via a single `Engine` subclass.

## Consequences

- A runnable sample requires a real `Engine` implementation (see
  `examples/hello_world`, which ships a no-op engine stub).
- Visual-regression testing is deferred until a rendering backend is available
  (see the visual-regression issue).

## Visual-Regression Testing (Deferred)

There is **no visual-regression harness** in v1.0. Rendering is delegated to an
external `Engine` abstraction, and no rendering backend is shipped in this
repository, so there is nothing to render and diff against golden images.

Once a rendering backend exists (or an external reference engine is used), a
visual-regression harness should be added that renders scenes offscreen and
diffs against golden images, wired into CI. This is tracked as a post-v1.0
roadmap item.

## Future Work

If "engine-less" becomes a stated product goal, a thin platform/rendering
abstraction layer should be introduced as a post-v1.0 effort.
