---
name: build-and-test
description: Configure, build, test and format guillaume. Use after any change to headers/, sources/, templates/, tests/ or the CMake files, and whenever asked to build, test or check the library.
---

# Build and test guillaume

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build -j 2>&1 | grep -E "error|Error [0-9]|Built target"
ctest --test-dir build --output-on-failure
```

- The first configure fetches `utility` from Git and needs network access.
  Everything else is vendored under `third_party/`.
- The library and its tests need no engine, window or GPU.
- Run one group with `ctest --test-dir build -R <Name> --output-on-failure`.
- The tests create `.guillaume-local-storage.db` in the working directory. It
  is ignored by Git; do not commit it.
- `-DBUILD_EXAMPLES=ON` also builds the examples. The evan example needs the
  Vulkan SDK and fetches evan; see the `run-gallery` skill.
- `tests/package_consumer/` is expected to fail until `find_package` works
  (see `README.md`, "Consuming as a dependency"). It is not part of `ctest`.

## Format

```sh
./scripts/run-clang-format.sh
```

Use the clang-format version CI installs (`.github/workflows/format.yml`):
another version formats differently.

## Report

State the test count and what was not built (examples, benchmarks, docs). See
the change checklist in `AGENTS.md`.
