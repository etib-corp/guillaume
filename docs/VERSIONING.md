# Versioning & Support Policy

This document describes how Guillaume is versioned and supported.

## Versioning

Guillaume uses calendar versioning: `YEAR.MINOR.PATCH` (for example
`2026.3.1`). The version is bumped automatically when a pull request is merged
into `main`, from the pull request title
(see [Commit Conventions](COMMIT_CONVENTIONS.md)):

| PR title starts with | Effect |
|---|---|
| `feat` | `MINOR` + 1, `PATCH` reset to 0 |
| `fix` | `PATCH` + 1 |
| anything else (`refactor`, `docs`, `chore`, …) | no new version |

The first release of a new calendar year resets `MINOR` and `PATCH` to 0
before applying the bump.

The current version is declared in `CMakeLists.txt`; every version is tagged
`vYEAR.MINOR.PATCH` and listed in `CHANGELOG.md`.

## Compatibility

The version number does not encode compatibility: a breaking change can land
in any release. Instead:

- Breaking changes are marked **Breaking** in `CHANGELOG.md`.
- Each one has a before/after entry in `docs/MIGRATION_GUIDE.md`.
- Pull requests that break the API carry a `!` in their title
  (`refactor!: ...`).

Pin an exact tag when you depend on Guillaume, and read the changelog before
moving to a newer one.

## Support

- Only the latest release is supported. Fixes land on `main` and ship in the
  next release; they are not backported.
- Security issues are handled per `SECURITY.md`.

## Release Cadence

A release is cut on every merged `feat` or `fix` pull request. There is no
fixed calendar schedule.
