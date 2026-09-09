# Third-party dependencies

This directory contains vendored copies of all third-party dependencies used by
Guillaume. They are consumed at configure time via `FetchContent` with a local
`SOURCE_DIR`, so no network access is required to build the project.

Each dependency is pinned to a specific upstream version. To update a
dependency, replace the contents of its directory with the new upstream release
and update the version recorded below.

| Directory              | Dependency          | Version / commit                          | Upstream                                                              | License |
| ---------------------- | ------------------- | ----------------------------------------- | --------------------------------------------------------------------- | ------- |
| `sqlite3/`             | sqlite3-cmake       | `v3.49.1`                                 | https://github.com/sjinks/sqlite3-cmake                               | MIT |
| `sqlite3_ext/`         | SQLite amalgamation | `3.49.1`                                  | https://sqlite.org/                                                   | Public Domain |
| `googletest/`          | GoogleTest          | `v1.17.0`                                 | https://github.com/google/googletest                                  | BSD-3-Clause |
| `googlebenchmark/`     | Google Benchmark    | `v1.9.1`                                  | https://github.com/google/benchmark                                   | Apache-2.0 |
| `doxygen-awesome-css/` | Doxygen Awesome CSS | `v2.4.1`                                  | https://github.com/jothepro/doxygen-awesome-css                       | MIT |

## Notes

- The vendored copies are stripped of their `.git` directories and
  `.gitmodules` files to keep the repository lean.
- `sqlite3_ext/` is the SQLite amalgamation that `sqlite3-cmake` normally
  downloads at configure time; it is vendored here and referenced via a local
  `SOURCE_DIR` in `sqlite3/CMakeLists.txt`.
- Each dependency retains its own license file; refer to the individual
  `LICENSE`/`COPYING` files for the exact terms.
- The `etib-corp` sibling library (`utility`) is intentionally not vendored; it
  is fetched via `FetchContent` with a local-checkout override for development.
