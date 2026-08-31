# Audit 02 — API Design & DX

## ✅ What's good

- Small, predictable public surface; consistent `getX`/`setX` naming; fluent setters return
  `*this`.
- `std::optional` used for "not found" (`getItem`, `getFocusedEntity`) — good.
- Concepts + `requires` clauses give compile-time safety.
- Doxygen comments are thorough and include `@code` examples.

## ⚠️ What's problematic

- **Inconsistent error model:** `getItem` returns `std::optional`, but `getComponent` throws
  `EntityComponentNotFoundException`, `getEntity` throws `std::runtime_error`, and
  `getSystemsByPhase` throws despite the doc saying "returns empty vector". Mixed
  exceptions/optionals is confusing.
- **`std::shared_ptr` overuse for events** (`EventBus::publish(std::shared_ptr<Event>)`) —
  ownership semantics are unclear; the doc says "ownership transferred" but the bus stores
  listeners that hold the shared_ptr.
- `EventBus::subscribe` takes `const Listener&` (a `std::function`) — should be by
  value/move.
- `Engine::Handler` uses `std::shared_ptr<Event>&` (non-const ref) — awkward.
- `SceneManager::getActiveScene()` returns `std::unique_ptr<Scene>&` — exposes mutable
  ownership, dangerous.
- `Application::routine()` uses a **`static` local `firstView`**
  (`templates/guillaume/application.tpp:163`) — shared across all instances, a latent bug.
- No `noexcept` on most accessors; exception specifiers are inconsistent.

## 🔧 Recommended actions

| Priority | Action |
|---|---|
| **High** | Adopt a single error convention: `std::optional`/`std::expected` for lookups, exceptions only for programmer errors |
| **High** | Fix the `static firstView` in `routine()` |
| **Medium** | Change `subscribe` to take `Listener` by value; make `getActiveScene` return a pointer/reference, not `unique_ptr&` |
| **Medium** | Add `noexcept` to trivial getters |

## 📈 Estimated impact

DX, stability.
