# Audit 05 — Documentation & Onboarding

## ✅ What's good

- `docs/ARCHITECTURE.md`, `HOW_GUILLAUME_WORKS.md`, `TECHNICAL_CHOICES.md` exist and are
  accurate.
- Doxygen config is thorough (awesome-css theme, graphs, source browser) with a CI job that
  builds and publishes to GitHub Pages.
- Doxygen comments are high quality.

## ⚠️ What's problematic

- **README is 24 lines** — no quickstart, no example, no build instructions per platform, no
  API overview.
- **No examples directory, no tutorials, no migration guide.**
- **No build instructions for Android** (N/A — no Android).
- No architecture diagrams (only Doxygen-generated graphs).
- No CONTRIBUTING.md (referenced in `cmake/Docs.cmake:106` but absent).

## 🔧 Recommended actions

| Priority | Action |
|---|---|
| **High** | Write a real README: quickstart, minimal example, build/consume instructions |
| **High** | Add an `examples/` directory with a runnable desktop sample |
| **Medium** | Add a CONTRIBUTING.md and an architecture diagram |
| **Low** | Add a "Getting Started" tutorial |

## 📈 Estimated impact

DX, onboarding.
