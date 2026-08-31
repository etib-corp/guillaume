# Audit 06 — Tests & CI/CD

## ✅ What's good

- 50 GoogleTest cases across ECS, components, systems, events, entities, storage.
- Tests use real fixtures (e.g., `TestEntityRegistryContainer`, `DummyEntity`).
- CI builds on Linux, macOS, Windows (`.github/workflows/build.yml`).

## ⚠️ What's problematic

- **CI never runs the tests** — `build.yml` only configures and builds; no `ctest`, no test
  step.
- **No visual regression tests** (no rendering backend to test against).
- **No lint, no static analysis, no sanitizers** in CI.
- **No Android CI** (no emulator/device farm).
- Some test files are empty stubs (`tests/sources/ecs/test_entity_registry.cpp` just includes
  another file).
- No coverage measurement.

## 🔧 Recommended actions

| Priority | Action |
|---|---|
| **High** | Add `ctest` execution to CI on all three OSes |
| **High** | Add sanitizers (ASan/UBSan) + a static analyzer (clang-tidy) to CI |
| **Medium** | Add coverage reporting |
| **Medium** | Add a visual-regression harness once a rendering backend exists (**To verify**) |
| **Low** | Remove/complete empty test stubs |

## 📈 Estimated impact

Stability, confidence.
