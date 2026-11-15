---
name: add-widget
description: Add a new widget to guillaume, or add, rename or remove any header, source or test file. Use whenever a component, entity or system is created, because sources, tests and systems are registered by hand in several places.
---

# Add a widget to guillaume

A widget is three classes with the same name in three namespaces. `Badge` is
a small, complete one: copy it.

| Part | Header | Source |
| --- | --- | --- |
| Component: the data | `headers/guillaume/components/<name>.hpp` | `sources/components/<name>.cpp` |
| Entity: the builder and public API | `headers/guillaume/entities/<name>.hpp` | `sources/entities/<name>.cpp` |
| System: the per-frame behaviour | `headers/guillaume/systems/<name>.hpp` | `sources/systems/<name>.cpp` |

Template code goes in `templates/guillaume/`, mirroring `headers/`, as a
`.tpp` included at the end of the header.

## Every place to touch

Find them with `grep -rn -i "badge" headers sources templates tests cmake examples/evan/main.cpp examples/README.md`
and mirror each hit:

1. The six files above.
2. `cmake/Sources.cmake`: the three `.cpp` files. Sources are listed by hand.
3. `sources/systems/core_systems.cpp`: include the system and register it
   with `registerNewSystem`. An unregistered system never runs, and nothing
   warns about it. Its phase (Event, Measure, Layout, Render) is set in the
   system's constructor.
4. `sources/scene.cpp`: add `entities::<Name>::Builder` to the builder list.
5. `tests/sources/entities/test_<name>.cpp`, and its path in `TEST_SOURCES`
   in `tests/CMakeLists.txt`. An unlisted test is not compiled.
6. `examples/evan/main.cpp`: show the widget on a gallery page, with a
   `logger(...)` on each interaction. Update the table in
   `examples/README.md`.
7. `CHANGELOG.md`, and `docs/MIGRATION_GUIDE.md` when an existing API changes.

## Conventions

- Colours, type and shapes come from the theme (`docs/THEMING.md`); no
  hard-coded colour in a widget.
- A widget is created through its `Builder`; follow the `with...()` /
  `make...()` shape of the neighbours.
- Public declarations carry Doxygen comments with a usage example, as the
  existing headers do.
- The library has no engine dependency: only `utility` types cross the
  boundary (`docs/ENGINE_COUPLING.md`).

## Finish

Follow the `build-and-test` skill, then the `run-gallery` skill to look at the
widget. Layout and rendering are not covered by the unit tests.
