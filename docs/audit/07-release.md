# Audit 07 — Release Preparation

## ✅ What's good

- Version is set (`1.0.0`), docs CI publishes to Pages, issue templates exist.

## ⚠️ What's problematic

- **No LICENSE file** (headers say MIT but no top-level license).
- **No CHANGELOG, no migration guide, no release checklist.**
- **No packaging** (no install/export, no vcpkg/Conan, no CPack).
- **No CONTRIBUTING.md, no CODE_OF_CONDUCT.**
- No tags/releases; single commit.
- No post-v1 roadmap documented.

## 🔧 Recommended actions

| Priority | Action |
|---|---|
| **High** | Add LICENSE, CHANGELOG, CONTRIBUTING, CODE_OF_CONDUCT |
| **High** | Add install/export + CPack packaging |
| **Medium** | Tag v1.0.0, add release workflow, write migration guide |
| **Medium** | Document semver + support policy |

## 📈 Estimated impact

Release readiness, adoption.
