---
name: run-gallery
description: Build and run the widget gallery (the evan example) to see or manually test a widget. Use after a change to rendering, layout, theming or interaction, and when asked to run, show or screenshot a widget.
---

# Run the widget gallery

The gallery (`examples/evan`) shows every widget on nine pages, rendered by
the evan engine. It is the only way to see a widget.

```sh
cmake -S . -B build -DBUILD_EXAMPLES=ON
cmake --build build --target guillaume_evan_example -j
cd build/examples/evan && GUILLAUME_GALLERY_PAGE=5 ./guillaume_evan_example
```

- It needs the Vulkan SDK, a display, and network access on the first
  configure (evan is fetched from Git).
- Run it from `build/examples/evan`: the assets are copied there and loaded
  by relative path.
- `GUILLAUME_GALLERY_PAGE` (1 to 9) opens a page directly. The pages are
  listed in `examples/README.md`.
- Every interaction prints a `[gallery] ...` line on standard output, so a
  click with no visible effect can still be checked.
- The program has no frame limit: it runs until the window is closed. When
  running it unattended, start it in the background, wait a few seconds, then
  stop it, and redirect its output to a file. Debug builds log every frame,
  so grep the file (`grep -a "\[gallery\]\|\[Error\]"`) instead of reading it.

## Known noise

A debug build with the Vulkan validation layers prints image-layout errors
and "vertex attribute not consumed" warnings on every run. They are not
caused by a widget change.

## What an agent cannot check

Clicks, hover, focus and keyboard input need a person, or a tool that drives
the screen. Say which interactions were not exercised. A new widget must
appear on a gallery page (see the `add-widget` skill).
