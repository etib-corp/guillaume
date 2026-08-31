# Audit 08 — Priority Refactorings & v1.0 Release Checklist

## Priority Refactorings (before v1.0)

1. **Packaging & consumability** — add `install()`/`export()`/CMake package config + explicit
   source lists.
   *Effort: M. Risk: Low. Benefit: High (library becomes usable as a dependency).*
2. **CI test execution + sanitizers** — run `ctest` on all OSes, add ASan/UBSan/clang-tidy.
   *Effort: M. Risk: Low. Benefit: High (catches regressions before release).*
3. **Error-model unification** — consistent `std::optional`/`std::expected` vs exceptions.
   *Effort: M. Risk: Medium (API change). Benefit: High (DX).*
4. **Thread-safety + identifier atomicity** — atomic ID counter, document threading model,
   fix `sqlite3_errmsg` null-deref.
   *Effort: S. Risk: Low. Benefit: High (stability).*
5. **Fix `static firstView` + `getActiveScene` ownership** — latent multi-instance bug and
   unsafe ownership exposure.
   *Effort: S. Risk: Low. Benefit: Medium (correctness).*

## v1.0 Release Checklist

- [ ] Add top-level `LICENSE` (MIT) file
- [ ] Add `CHANGELOG.md` and tag `v1.0.0`
- [ ] Add `CONTRIBUTING.md` and `CODE_OF_CONDUCT.md`
- [ ] Add `install()`/`export()` + CMake package config
- [ ] Replace `file(GLOB)` with explicit source lists
- [ ] Run `ctest` in CI on Linux/macOS/Windows
- [ ] Add ASan/UBSan + clang-tidy to CI
- [ ] Add coverage reporting
- [ ] Write full README (quickstart, example, build/consume)
- [ ] Add `examples/` runnable desktop sample
- [ ] Add architecture diagram + data-flow doc
- [ ] Unify error model (optional/expected vs exceptions)
- [ ] Make entity ID generation atomic; document threading model
- [ ] Fix `static firstView` and `getActiveScene` ownership
- [ ] Fix `sqlite3_errmsg` null-deref paths
- [ ] Add benchmark harness (layout, redraw, event dispatch, memory)
- [ ] Write migration guide (from beta API)
- [ ] Document semver + support policy
- [ ] **To verify:** confirm whether Android is actually a v1.0 target (currently absent)
