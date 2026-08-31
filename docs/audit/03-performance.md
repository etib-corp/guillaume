# Audit 03 — Performance & Resources

## ✅ What's good

- ECS uses SoA-style `ComponentStorage` with per-component `hasChanged` dirty flags — enables
  lazy re-render.
- `SessionStorage`/`LocalStorage` are mutex-guarded; SQLite statements are
  prepared/finalized per call (correct, if not optimal).

## ⚠️ What's problematic

- **No benchmarks, no profiling, no allocation strategy.** No pooling, no batching, no lazy
  layout.
- `getEntityWithSignature` does a **linear scan + `dynamic_pointer_cast`** per entity per
  system per frame (`headers/guillaume/ecs/entity_registry.hpp:110`) — O(N·M) per frame.
- `getEntitiesBreadthFirst()` allocates a new `vector<shared_ptr>` on every call — per-frame
  allocation churn.
- Event dispatch uses `std::map<std::type_index, vector<function>>` + `shared_ptr` —
  heap-heavy hot path.
- `LocalStorage` prepares/finalizes a statement on every `setItem`/`getItem` — no
  prepared-statement reuse.
- No Android-specific considerations (memory limits, thermal throttling, background
  rendering) — **To verify** whether these matter given no Android target.

## 🔧 Recommended actions

| Priority | Action |
|---|---|
| **High** | Add a benchmark harness (layout, redraw, event dispatch, memory) before claiming performance |
| **Medium** | Cache signature queries / index entities by signature; reuse traversal buffers |
| **Medium** | Reuse prepared SQLite statements |
| **Low** | Consider an event object pool |

## 📈 Estimated impact

Performance, stability.
