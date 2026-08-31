# Audit 04 — Security & Robustness

## ✅ What's good

- `LocalStorage` guards against null DB and falls back to in-memory; SQL uses bound
  parameters (no injection).
- `Storage::setItem(const char*)` null-checks.
- `Application::run()` wraps the loop in try/catch and logs errors.

## ⚠️ What's problematic

- **No input validation** on `Entity::Identifier` lookups — `getComponent` on a
  stale/removed entity throws (acceptable) but `getEntityWithSignature` silently skips.
- **Non-atomic identifier counter** — race condition if entities are created on multiple
  threads.
- **No thread-safety documentation.** Only storage is mutex-guarded; ECS registries, event
  bus, and systems are **not** thread-safe. No stated threading model.
- `sqlite3_errmsg(_database)` is called after `_database` may be null in some error paths
  (`sources/local_storage.cpp:56,83`) — potential null deref.
- No JNI/Android boundary (N/A — no Android).

## 🔧 Recommended actions

| Priority | Action |
|---|---|
| **High** | Document the threading model (single-threaded main loop; storage is the only thread-safe module) |
| **High** | Make identifier generation atomic |
| **Medium** | Fix the `sqlite3_errmsg` null-deref paths |
| **Medium** | Add bounds/validity checks on entity lookups |

## 📈 Estimated impact

Stability, security.
