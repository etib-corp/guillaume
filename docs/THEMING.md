# Theming and Material 3 Foundations

GUILLAUME follows Material 3 as implemented by
[`react-native-paper`](https://github.com/callstack/react-native-paper) v5.
Every visual decision an entity makes — color, typography, shape, elevation,
state feedback, motion, spacing — is read from the active `Theme` instead of
being hardcoded. Changing a token changes every consumer.

The audit that motivated this layer is `docs/audit/09-paper-parity.md`.

## Color roles

`Scheme` holds one `ExtendedColor` per `SchemeColorRole`. All six schemes
(`Light`, `Dark` and their medium/high-contrast variants) populate every role,
including the surface containers, the fixed roles and three Paper-specific
roles derived from `OnSurface`:

| Role | Value |
|---|---|
| `SurfaceDisabled` | `OnSurface` at 12% — disabled containers |
| `OnSurfaceDisabled` | `OnSurface` at 38% — disabled content |
| `Backdrop` | neutral-variant 20 at 40% — modal backdrops |

```cpp
setActiveScheme(ThemeSchemeRole::Dark);
auto surface = getActiveScheme().getColor(SchemeColorRole::Surface).getColor();
```

## Typescale (`guillaume/theme/typescale.hpp`)

`theme::Typescale` maps the 15 M3 roles plus `Default` to a `TypeToken`
(family, weight, size, line height, letter spacing), with the Paper baseline
values. `theme::fontPath(weight)` selects the bundled Roboto Regular or Medium
file; `TextRender` and `MeasureText` use it, and `MeasureText` grows the bound
to the role line height.

`entities::Text` resolves its role on every update, so changing a token
restyles existing texts:

```cpp
director.makeText(builder, parent, "Title", theme::TypeRole::TitleLarge,
                  schemeColor(SchemeColorRole::OnSurface));
text->setTypeRole(theme::TypeRole::LabelLarge);   // explicit sizes still win
```

## Design tokens (`guillaume/theme/tokens.hpp`)

| Scale | Accessor | Contents |
|---|---|---|
| `ShapeScale` | `getShapeRadius(ShapeRole)` | None 0, ExtraSmall 4, Small 8, Medium 12, Large 16, ExtraLarge 28, Full (pill/circle); scaled by `roundness` (default 4) |
| `ElevationScale` | `getElevationToken(level)`, `getElevatedSurfaceColor(level)` | Levels 0–5: tonal `SurfaceTint` opacity (0, 5, 8, 11, 12, 14%) and shadow spread/opacity |
| `StateLayerScale` | `getStateLayerAlpha(StateLayer)` | Hovered 8%, Focused 12%, Pressed 12%, Dragged 16%, DisabledContent 38%, DisabledContainer 12% |
| `MotionScale` | `getMotionDuration(MotionDuration)` | Short1–ExtraLong4 (50–1000 ms); `enter()`/`exit()`/`standard()` easings |
| `SpacingScale` | `SpacingScale::get(n)` | 4 dp grid, `MinTouchTarget = 48`, `touchMargin(extent)` |
| `FocusRing` | constants | 3 dp ring, 2 dp offset, `Secondary` color |

`components::Elevation::setElevationLevel(level)` resolves a shadow from the
scale; level 0 paints nothing.

## Shared interaction state

- `components::StyleState` is the single enabled/disabled source and carries
  the transient hover/press/focus/drag flags. `systems::StyleState` mirrors the
  interaction components into it every frame; `getActiveLayer()` returns the
  M3 state layer to paint (dragged > pressed > focused > hovered).
- Every pointer and hand system, `systems::Drag` and `systems::Focus` skip
  disabled entities (`systems::isInteractive`). Hit tests grow the bound by
  the hover `accessibilityMargin`, which small controls set to reach the 48 dp
  touch target.
- `entities::makeInteractive()` wires the full stack (hover, buttons, focus,
  style state, accessibility role and a press handler gated on the enabled
  flag); `entities::stateLayerColor()` resolves the container color for the
  current state.

## Accessibility and focus

`components::Accessibility` is the semantic description of an entity: label,
hint, `AccessibilityRole`, selected / checked (`CheckedState`) / expanded,
range value, live region, focus order and focusability. The disabled state is
not duplicated: it is read from `StyleState`.

`systems::Focus` keeps a per-entity focus flag, supports Tab / Shift-Tab
traversal (positive `focusOrder` first, then tree order, wrapping), and drops
the focus of an entity that becomes disabled. `systems::FocusRingRender`
paints the focus indicator. Both are registered by
`Application::registerCoreSystems()`.
