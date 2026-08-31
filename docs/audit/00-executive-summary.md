# Guillaume — Pre-v1.0 Audit: Executive Summary

> **Date:** 2026-08-28
> **Scope:** End-of-beta audit of the Guillaume UI framework, preparing for v1.0.
> **Method:** Static review of the public API, architecture, build system, tests, CI, and docs.
> **Part:** 00 of 8 — see also the per-domain reports in this directory.

---

## ⚠️ Critical Scope Note

The audit brief describes an **engine-less GUI library** (Vulkan/Metal/DirectX/OpenGL,
Android targets, layout, accessibility, packaging). The actual repository is **Guillaume**,
an **ECS-based UI framework for desktop + XR** that is explicitly **engine-coupled** (it
wraps an abstract `Engine` interface). The two do not match. Key divergences:

| Brief assumption | Actual state |
|---|---|
| Engine-less (low-level rendering backends) | **Engine-coupled**: `Engine` is an abstract interface (`headers/guillaume/engine.hpp:66`) with `addMesh`/`addText`/`addModel`/`clear`/`present`; rendering is delegated to an external engine, not implemented here |
| Android target | **No Android support anywhere** (no NDK, JNI, Gradle, AAR, toolchain) |
| Layout, accessibility | **Absent** — no layout engine, no accessibility module |
| Vulkan/Metal/DirectX/OpenGL | **None** — rendering is via the `Engine` abstraction + `utility` graphics types |
| Packaging (vcpkg/Conan/AAR) | **None** — no `install()`/`export()`/CPack rules in CMake |
| v1.0 beta | Single commit, `VERSION 1.0.0` already set, no changelog, no license file |

This report audits the **actual** codebase. Where the brief's assumptions do not exist,
items are marked **To verify** rather than invented.

---

## Executive Summary

Guillaume is a compact, well-documented C++20 ECS-based UI framework for desktop and XR.
The core architecture (ECS separation, event bus, scene management, storage abstraction) is
sound and the public API is small and predictable. However, it is **not** the "engine-less,
multi-backend, Android-targeting" library described in the brief — it is an engine-coupled
framework with no Android story, no packaging, no install rules, no license file, and no
examples.

The single most important pre-v1.0 risk is **release-readiness infrastructure** (packaging,
licensing, CI test execution, examples), not core architecture.

**Recommendation: NO-GO for v1.0 as currently scoped.** The code is beta-quality, but the
release surface (packaging, docs, license, CI) is not. A 4–6 week hardening pass is required.

---

## Report Index

| # | Domain | File |
|---|---|---|
| 00 | Executive summary | `00-executive-summary.md` |
| 01 | Architecture & Maintainability | `01-architecture.md` |
| 02 | API Design & DX | `02-api.md` |
| 03 | Performance & Resources | `03-performance.md` |
| 04 | Security & Robustness | `04-security.md` |
| 05 | Documentation & Onboarding | `05-docs.md` |
| 06 | Tests & CI/CD | `06-tests-ci.md` |
| 07 | Release Preparation | `07-release.md` |
| 08 | Priority Refactorings & v1.0 Checklist | `08-priority-refactorings.md` |
