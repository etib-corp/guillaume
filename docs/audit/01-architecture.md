# Audit 01 — Architecture & Maintainability

## ✅ What's good

- Clean ECS separation: `Entity` / `Component` / `System` / `SystemRegistry` /
  `ComponentRegistry` are well-factored (`headers/guillaume/ecs/`).
- Template + `.tpp` split keeps public headers readable; concepts (`InheritFromEngine`,
  `InheritFromScene`, `IsOneOf`) enforce constraints at compile time.
- `Storage` abstraction with `LocalStorage` (SQLite) and `SessionStorage` (in-memory) is
  clean and thread-safe.
- Scene lifecycle (`onEnter`/`onExit`), phase-based system execution, and traversal
  strategies (level-order / reverse) are well-designed.
- Consistent MIT-style license headers on every file.

## ⚠️ What's problematic

- **Engine coupling contradicts the "engine-less" goal.** `Engine` is a pure abstract
  interface, but `Application`/`SceneManager`/systems hold `std::unique_ptr<Engine>&` and
  call `addMesh`/`addText`/`addModel`. There is no rendering backend, no platform
  abstraction layer, no windowing. The library cannot render without an external engine.
- **No install/export/package rules** in CMake — the library cannot be consumed as a
  dependency.
- `file(GLOB_RECURSE SOURCES ...)` (`CMakeLists.txt:65`) is fragile; new files silently
  change the build.
- `include_directories()` (global) instead of `target_include_directories()` on the target —
  leaks include paths to consumers.
- `Entity::getNextIdentifier()` uses a **non-atomic static counter**
  (`sources/ecs/entity.cpp:30`) — not thread-safe.
- `Entity::Signature` is a hardcoded `std::bitset<64>` — a scalability ceiling.
- `SceneManager` holds a raw `Engine*` (`headers/guillaume/scene_manager.hpp:76`) — lifetime
  coupling, easy to dangle.

## 🔧 Recommended actions

| Priority | Action |
|---|---|
| **High** | Add `install()`/`export()`/`GNUInstallDirs` + CMake package config so the library is consumable |
| **High** | Replace `file(GLOB)` with explicit source lists |
| **Medium** | Make `getNextIdentifier()` atomic (`std::atomic<Identifier>`) |
| **Medium** | Replace global `include_directories` with target-scoped includes |
| **Low** | Document the engine-coupling decision explicitly (or introduce a thin platform-abstraction layer if engine-less is truly the goal) |

## 📈 Estimated impact

DX (consumability), maintenance, stability.
