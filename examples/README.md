# Guillaume Examples

This directory contains runnable samples that demonstrate how to use the
Guillaume framework.

## hello_world

A minimal desktop sample that:

- Implements a minimal `Engine` subclass.
- Defines a `Scene` type.
- Runs the `Application` main loop.

Because Guillaume is engine-coupled (rendering is delegated to an `Engine`
implementation), this sample ships a no-op engine stub. To render actual
content, replace `NoopEngine` with a real engine implementation for your
platform, or see the `evan` example.

### Building

```sh
cmake -S . -B build -DBUILD_EXAMPLES=ON
cmake --build build --target guillaume_hello_world
```

### Running

```sh
./build/examples/hello_world/guillaume_hello_world
```

## evan

An Evan-backed sample (macOS / Linux / Windows + GLFW) that uses the
[Evan](https://github.com/etib-corp/evan) Vulkan engine as the `Engine` behind a
Guillaume `Application`. It is a widget gallery for manual testing: every
widget of the library, spread over nine pages.

| Page | Widgets |
|---|---|
| 1. Buttons | `Button` (each color style, icon, disabled), `IconButton` (each variant, toggle), FABs (each size and variant), extended FAB, FAB menu, `SegmentedButton` (single and multi) |
| 2. Selection | `Checkbox` (each state and variant), `RadioButton`, `RadioGroup`, `Switch`, `Chip` (each variant) |
| 3. Text and input | `Text` (type scale), `Icon`, `Image`, `TextField` (filled, outlined, error), `Search` |
| 4. Surfaces and cards | `Surface` (elevation levels, flat), `Card` (each variant, all sections) |
| 5. Dividers, badges and lists | `Divider`, `Badge`, `List` (subheader, divider, accordion) |
| 6. Feedback | `ProgressIndicator`, `LoadingIndicator`, and buttons opening a `Snackbar`, a `Dialog`, a `Menu`; a `Tooltip` on hover |
| 7. App bar and navigation bar | `AppBar`, `NavigationBar` (with a badge) |
| 8. Navigation drawers | `NavigationDrawer` (standard, collapsed) |
| 9. 3D | `Mesh` (the teapot model, scaled with `setFitSize`) |

The Previous and Next buttons at the bottom of each page switch pages. Every
interaction prints a `[gallery]` line on standard output, so a click that has
no visible effect can still be checked. Set `GUILLAUME_GALLERY_PAGE` to a page
number to open the gallery on that page:

```sh
cd build/examples/evan && GUILLAUME_GALLERY_PAGE=5 ./guillaume_evan_example
```

Evan requires the Vulkan SDK. The example is added to the build on macOS, Linux
and Windows, and the matching Evan platform (`BUILD_FOR_MACOS`,
`BUILD_FOR_LINUX` or `BUILD_FOR_WINDOWS`) and the GLFW backend are selected
automatically from the host. The Evan dependency stays scoped to the example,
so the `guillaume` library itself has no engine dependency.

### Building

```sh
cmake -S . -B build -DBUILD_EXAMPLES=ON
cmake --build build --target guillaume_evan_example
```

### Running

Run it from its build directory, where the assets are copied:

```sh
cd build/examples/evan && ./guillaume_evan_example
```
