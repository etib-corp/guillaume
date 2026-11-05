# Audit 09 — Material 3 Parity vs. react-native-paper

> **Post-audit codebase update (2026-11-05):** `split_button`,
> `standard_button_group`, `toolbar`, `carousel`, `slider`, `date_picker`,
> `time_picker`, `bottom_sheet`, `side_sheet`, `navigation_rail`, and `tabs`
> were deleted because they have no react-native-paper counterpart. The
> `Layout` primitive was renamed `View`, and `Model` was renamed `Mesh`.
> Findings below are the pre-removal snapshot (42 units); 31 units remain.

## Reference and method

| Item | Value |
|---|---|
| Reference | [`callstack/react-native-paper`](https://github.com/callstack/react-native-paper) tag **`v5.15.4`** |
| Reference commit | `14ef44a9bde55bbc47f8c298877696564889cea8` (`chore: release 5.15.4`, 2026-11-02) |
| Guillaume revision | `42ee010` (`fix: arrange only registered layout children; flag bound changes`, 2026-10-16) |
| Audit date | 2026-11-05 |
| Scope | 42 units: the 41 entity test families under `tests/sources/entities/` plus the `Surface` primitive |
| Method | Static source comparison of Paper TypeScript props/defaults/tokens/tests against Guillaume public headers, builders, directors, entity implementations, systems, and `TEST_F` suites |
| Runtime verification | None. No renderer screenshots or Paper test execution was performed; geometry and visual findings are derived from source constants. |

Paper's React Native-only concerns are marked **N/A** throughout: `testID`, `hitSlop`, ripple/`background` configuration, `ref`/`touchableRef`, `style` objects, `contentStyle`/`labelStyle`, `theme` override props, `aria-*` naming, and platform-specific props (Android/iOS/web). Semantic equivalents that *do* apply are audited: accessibility label/hint/role/state, disabled behavior, focus/keyboard behavior, long press, and touch targets.

### Status and severity vocabulary

**Verdict**

| Verdict | Meaning |
|---|---|
| **Match** | Equivalent API, variants, tokens, states, and behavior at the level Guillaume models them. |
| **Partial** | Exists and is recognizably the same component, but misses props, variants, states, tokens, or composition. |
| **Diverges** | Implemented with a different default, color role, geometry, or behavior than Paper/M3. |
| **Missing** | No equivalent public entity. |
| **No Paper ref** | Paper `v5.15.4` has no direct counterpart; audited against M3 and the closest Paper primitive. |

**Severity**

| Severity | Meaning |
|---|---|
| **P0** | Visibly broken output or a core contract that is not rendered/usable today. |
| **P1** | Systemic parity gap in API, default variant, color role, state, or accessibility. |
| **P2** | Secondary state, composition, motion, or geometry gap. |
| **P3** | Cleanup, RN-only difference, or a family with no Paper reference. |

---

## Executive scorecard

### By verdict

| Verdict | Count | Families |
|---|---:|---|
| Match | 1 | `divider` |
| Partial | 22 | `badge`, `button`, `card`, `checkbox`, `dialog`, `extended_floating_action_button`, `floating_action_button`, `floating_action_button_menu`, `icon`, `list`, `loading_indicator`, `menu`, `navigation_bar`, `navigation_drawer`, `progress_indicator`, `radio_button`, `search`, `segmented_button`, `snackbar`, `switch`, `text_field`, `tooltip` |
| Diverges | 4 | `icon_button`, `chip`, `app_bar`, `text` |
| Missing | 1 | `surface` (public entity; `SurfaceBase`/`SurfaceConfig` exist) |
| No Paper ref | 14 | `split_button`, `standard_button_group`, `toolbar`, `carousel`, `slider`, `date_picker`, `time_picker`, `image`, `layout`, `model`, `bottom_sheet`, `navigation_rail`, `side_sheet`, `tabs` |
| **Total** | **42** | |

### By severity

| Severity | Count | Families |
|---|---:|---|
| P0 | 2 | `text_field`, `text` |
| P1 | 20 | `button`, `icon_button`, `floating_action_button`, `floating_action_button_menu`, `chip`, `checkbox`, `radio_button`, `switch`, `app_bar`, `search`, `list`, `menu`, `navigation_bar`, `navigation_drawer`, `tabs`, `dialog`, `card`, `surface`, `snackbar`, `tooltip` |
| P2 | 13 | `extended_floating_action_button`, `segmented_button`, `navigation_rail`, `bottom_sheet`, `side_sheet`, `badge`, `loading_indicator`, `progress_indicator`, `carousel`, `slider`, `date_picker`, `time_picker`, `icon` |
| P3 | 7 | `split_button`, `standard_button_group`, `toolbar`, `divider`, `image`, `layout`, `model` |
| **Total** | **42** | |

### Highest-impact findings

1. **The default Light and Dark schemes leave 20 of 49 color roles uninitialized.** They default to opaque black, so elevated cards, dialogs, menus, search bars, sheets, containers, and dividers render black or near-black today.
2. **`components::Borders` has a stroke color but no stroke width, and no render system reads its color.** Outlined cards, outlined text fields, tab indicators, and every "border" visual are therefore not painted. `LineRender` uses `components::Color`, so the filled text-field underline is painted with the surface fill and is invisible.
3. **There is no Material 3 typescale.** Every entity passes raw pixel sizes; `Text` and `Icon` default to 24 px and opaque white.
4. **There is no shared state-layer model.** Hover/press/disabled alphas are duplicated byte literals in a handful of entities. Only the button families (and families that compose them) produce hover/press feedback; `Chip`, `List`, `Menu`, `Drawer`, `NavigationBar`, `Tabs`, and `Card` do not.
5. **There is no semantic accessibility layer.** `accessibilityLabel` exists only as write-only string fields on the button-family entities (`ButtonBase`, `IconButton`, `FAB`, `ExtendedFAB`, `FABMenu`); no role, hint, checked/selected/disabled state, focus order, or announcement exists anywhere.
6. **`Surface` is not a public entity.** Paper's `Surface` (elevation + flat/elevated mode) is the base of most M3 containers; Guillaume exposes only a CRTP base and a config struct.
7. **Several "interactive" families are inert.** `Chip`, `Checkbox`, `RadioButton`, `List` rows, `Menu` items, `AppBar` actions, and `NavigationBar`/`Drawer` destinations are static entities with no press handlers or semantic state.
8. **Default variant mismatches.** `Button` defaults to `Filled` (Paper: `text`); `Badge` defaults to `Dot` (Paper: 20 dp badge); `LoadingIndicator` defaults to `Linear` (Paper: circular); `Search::FullScreen` is a 640-high modal (Paper `view` is a 72-high inline field).
9. **Elevation is shadow-only.** Paper's M3 elevation is a tonal surface tint plus a shadow; Guillaume has no tonal overlay and only `Card` even owns `components::Elevation`.
10. **Focus is pointer-only and not registered.** `systems::Focus` reacts to mouse/hand events, has no Tab traversal or focus order, and `registerCoreSystems()` is declared but never defined or used in examples.

---

## Cross-cutting findings

### CC-01 — Default schemes are missing 20 M3 color roles (P0)

`initializeLightScheme` (`sources/theme.cpp:270-365`) and `initializeDarkScheme` (`sources/theme.cpp:678-773`) set 29 roles and stop at `InversePrimary`. The following 20 roles are never set in either default scheme:

`OutlineVariant`, `PrimaryFixed`, `OnPrimaryFixed`, `PrimaryFixedDim`,
`OnPrimaryFixedVariant`, `SecondaryFixed`, `OnSecondaryFixed`,
`SecondaryFixedDim`, `OnSecondaryFixedVariant`, `TertiaryFixed`,
`OnTertiaryFixed`, `TertiaryFixedDim`, `OnTertiaryFixedVariant`,
`SurfaceDim`, `SurfaceBright`, `SurfaceContainerLowest`, `SurfaceContainerLow`,
`SurfaceContainer`, `SurfaceContainerHigh`, `SurfaceContainerHighest`.

`ExtendedColor` default-constructs its color, and `utility::graphic::Color` default-constructs to **opaque black** (`build/_deps/utility-src/headers/utility/graphic/color.hpp:131-139`). The medium- and high-contrast schemes do set these roles (`sources/theme.cpp:462-518`, `616-672`, `870-926`, `1024-1080`), so only the two default schemes are affected.

Twenty-three direct uses across 15 entity sources read the missing roles:

- `sources/entities/card.cpp:119,125,133`
- `sources/entities/dialog.cpp:164`
- `sources/entities/menu.cpp:142`
- `sources/entities/search.cpp:161`
- `sources/entities/toolbar.cpp:138`
- `sources/entities/date_picker.cpp:366`, `time_picker.cpp:163`
- `sources/entities/bottom_sheet.cpp:211,213`, `side_sheet.cpp:170`
- `sources/entities/navigation_drawer.cpp:380`
- `sources/entities/divider.cpp:51`
- `sources/entities/button.cpp:319,346,350`
- `sources/entities/floating_action_button.cpp:192-193`
- `sources/entities/extended_floating_action_button.cpp:195-196`
- `sources/entities/carousel.cpp:277`

**Fix:** populate all 49 roles in the default schemes, using the existing medium-contrast values as the baseline; add `SurfaceDisabled`, `OnSurfaceDisabled`, and `Backdrop` while there. Add a `test_theme.cpp` that asserts every role is non-default.

### CC-02 — `Borders` color is never rendered (P0)

`components::Borders` stores four corner radii and a `Color32Bit` (`headers/guillaume/components/borders.hpp:44-53`, `:130-136`) but no stroke width. `RectangleRender` requires `Borders`, extracts only the average radius, and fills the mesh from `components::Color` (`sources/systems/rectangle_render.cpp:262-330`). Nothing calls `Borders::getColor()`.

`LineRender` paints from `components::Color` (`sources/systems/line_render.cpp:60-78`), not from `Borders`.

Consequences:

- `Card::Variant::Outlined` sets a transparent fill and an `OutlineVariant` border (`sources/entities/card.cpp:130-135`) — the border never appears.
- `TextField::Variant::Outlined` sets `Borders` to `Outline`/`Error` (`sources/entities/text_field.cpp:202-209`) — the outline never appears.
- `TextField::Variant::Filled` sets a `Line` with thickness 1 but never sets a line color (`sources/entities/text_field.cpp:213`); `LineRender` uses the entity fill (`SurfaceVariant`), so the underline is invisible.
- `Tabs::setIndicatorColor` writes `Borders::setColor` (`sources/entities/tabs.cpp:166-175`) — the active indicator never appears.
- `test_text_field.cpp:98-100` asserts `borders.getColor().getAlpha() > 0`, which passes even though nothing renders; `test_card.cpp` similarly asserts the unused border color.

**Fix:** either extend `Borders` with a stroke width and teach the rectangle renderer to emit a stroke (separate geometry or an inset outline mesh), or introduce a dedicated `components::Outline`/`systems::OutlineRender`. Do not use `Borders::getColor()` as a data-only contract.

### CC-03 — No Material 3 typescale (P0)

`Theme` contains only name/description, schemes, and palettes (`headers/guillaume/theme.hpp:405-499`). A repo-wide search for `typescale`, `typography`, `letterSpacing`, `lineHeight`, `fontFamily`, or `fontWeight` returns no hits in `headers/` or `sources/`. Paper defines 13 roles plus `default` with family, weight, letter spacing, line height, and size (`src/styles/themes/v3/tokens.tsx:111-219`).

Current defaults:

- `components::Text` font size 24 (`headers/guillaume/components/text.hpp:44`).
- `entities::Text` builder 24 and opaque white (`sources/entities/text.cpp:48-49`).
- `entities::Icon` builder 24 and opaque white (`sources/entities/icon.cpp:49-50`).
- Only `fonts/Roboto-Regular.ttf` is shipped, so the M3 medium weight (500) cannot be rendered.

Thirty-three `buildText(...)` call sites pass raw sizes. Examples: 22 for dialog/sheet/app-bar titles (`dialog.cpp:190`, `side_sheet.cpp:197`, `app_bar.cpp:159`, `bottom_sheet.cpp:251`), 16 for list headlines (`list.cpp:182`), 14 for labels (`button_base.cpp:370`, `menu.cpp:191`, `snackbar.cpp:169`), 12 for supporting text (`text_field.cpp:266`), and off-scale 45 for clock digits (`time_picker.cpp:264-272`).

**Fix:** add `theme::Typescale` with the M3 roles, map `Text`/`Icon` to roles rather than sizes, ship Roboto Medium, and replace raw sizes with role lookups. This is the single largest visual-parity change.

### CC-04 — No shape scale or `roundness` token (P1)

Paper's `roundness` is a first-class theme field (`LightTheme.tsx:11`) multiplied by component-specific factors (`Dialog.tsx:110` → 7×, `Card.tsx:270` → 3×, `Snackbar.tsx:308` → 1×). Guillaume has no shape token; radii are literals per entity:

- `dialog.cpp:67` 28
- `chip.cpp:58` 16
- `text_field.cpp:208` 4
- `navigation_rail.cpp:106` 16
- `surface.cpp:105` 28
- `button.cpp:211-245` a five-value size/shape switch
- `icon_button.cpp:167-179` 8/12/16
- `floating_action_button.cpp:167-180` 12/16/28
- `card.cpp` 12 via `SurfaceConfig`

**Fix:** add `theme::ShapeScale` (None/ExtraSmall/Small/Medium/Large/ExtraLarge/Full) driven by `roundness`; have `SurfaceConfig` accept a shape role instead of a float where possible.

### CC-05 — No elevation token scale or tonal overlay (P1)

`components::Elevation` hardcodes level 1, 4 layers, spread 4, and a `(0,0,0,64)` shadow color (`headers/guillaume/components/elevation.hpp:45-50`). `systems::ElevationRender` only builds nested shadow meshes. `SchemeColorRole::SurfaceTint` (`theme.hpp:41`) is never read outside `theme.cpp`.

Paper maps elevation 0–5 to a surface tint at alpha 0.05/0.08/0.11/0.12/0.14 (`src/styles/themes/v3/LightTheme.tsx:53-63`) plus a shadow. Only `Card` owns `components::Elevation` (`headers/guillaume/entities/card.hpp:54`); `FAB`, `Menu`, `Dialog`, `Snackbar`, and `Tooltip` have no elevation at all.

**Fix:** add `theme::ElevationScale` with tint + shadow levels; make `components::Elevation` read the scale; add it to FAB, Menu, Dialog, Snackbar, and Tooltip.

### CC-06 — No shared state-layer tokens (P1)

Paper centralizes opacity in `ref.opacity`: hover 0.08, focus 0.12, pressed 0.12, dragged 0.16, disabled content 0.38 (`src/styles/themes/v3/tokens.tsx:103-108`). Guillaume's only helper is `entities::stateLayer(base, overlay, alpha)` (`headers/guillaume/entities/style_helpers.hpp:54-60`), and every caller passes a raw byte:

- Hover: `20U` in `button.cpp:426`, `icon_button.cpp:318`, `floating_action_button.cpp:277`, `extended_floating_action_button.cpp:280`.
- Pressed: `31U` in `button.cpp:417`, `icon_button.cpp:314`, `floating_action_button.cpp:270`, `extended_floating_action_button.cpp:273`.
- Disabled container: `31U` in `button_base.cpp:286,302`; but `97U` (0.38) is used as a **fill** in `checkbox.cpp:152,164` and `switch.cpp:201`.
- Disabled content: `97U` in `button_base.cpp:294`, `checkbox.cpp:182`, `radio_button.cpp:179,202,229`, `switch.cpp:216,224,245`.
- No dragged state anywhere, even though `DragInteraction` is used by slider, carousel, and bottom sheet.
- Focus has no visual state.

Only `Button`, `IconButton`, `FAB`, `ExtendedFAB`, and partially `Checkbox`/`Switch`/`RadioButton` implement any state styling. `Chip`, `List`, `Menu`, `Drawer`, `NavigationBar`, `Tabs`, and `Card` do not.

**Fix:** add `theme::StateLayerScale` (Rest/Hovered/Focused/Pressed/Dragged/Disabled) and a shared `components::StyleState` + `systems::StyleState`. Derive state from the existing interaction components and skip disabled entities.

### CC-07 — No semantic accessibility layer (P1)

A repo-wide search for `accessibilityRole`, `accessibilityHint`, `accessibilityState`, `aria-`, or `AccessibilityRole` yields no hits in `headers/`, `sources/`, or `tests/`. Labels exist only as write-only string fields on the button-family entities: `ButtonBase` (`headers/guillaume/entities/button_base.hpp:156-167`), `IconButton` (`icon_button.hpp:71`), `FloatingActionButton` (`floating_action_button.hpp:71`), `ExtendedFloatingActionButton` (`extended_floating_action_button.hpp:72`), and `FloatingActionButtonMenu` (`floating_action_button_menu.hpp:76`). The Button label is round-tripped in tests (`tests/sources/entities/test_button.cpp:345-357`) but none are consumed by a system or renderer.

Paper uses roles/labels/hints/states across 57 files. Missing capabilities:

| Capability | Paper examples | Guillaume |
|---|---|---|
| Label | Button, Chip, FAB, IconButton | String on `Button` family only, unused |
| Hint | Button, IconButton | Absent |
| Role | `button`, `checkbox`, `radio`, `switch`, `tab`, `header`, `alert`, `progressbar`, `menuitem` | Absent |
| Disabled state | `accessibilityState.disabled` | Absent |
| Selected/checked | `accessibilityState.selected/checked` | Absent |
| Expanded/collapsed | FAB group, accordions | Absent |
| Live region/alert | Snackbar, Banner | Absent |
| Value | Slider, progress | Absent |
| Focus order | RN tree order + `accessible` | Absent |

**Fix:** add `components::Accessibility` (label, hint, role enum, selected/checked/disabled/expanded, min/max/now, focus order) and a system that maps it to the engine's accessibility backend when one exists. Even before a backend exists, the component gives every entity a single semantic source of truth.

### CC-08 — Focus is pointer-only, unregistered, and without order (P1)

`systems::Focus` derives focus from `MouseButton`, `HandButton`, `HandPinch`, and `HandPoke` (`sources/systems/focus.cpp:130-195`). It has:

- no keyboard Tab/Shift-Tab traversal (`systems::KeyboardControl` is only for text editing: `headers/guillaume/systems/keyboard_control.hpp:41-42`),
- no focus order or `tabIndex` equivalent,
- no per-entity `isFocused` flag (`headers/guillaume/components/focus.hpp:38-43` only stores two handlers),
- no focus ring or focus state layer,
- no registration: `Application::registerCoreSystems()` is declared (`headers/guillaume/application.hpp:144`) but has no definition, and no example references `systems::Focus`.

**Fix:** add `components::Accessibility::focusOrder`, a keyboard traversal path, a per-entity focus flag, a focus-ring token, and register the focus system in the application composition root.

### CC-09 — Disabled is not a first-class state (P1)

Three incompatible models coexist:

1. `ButtonBase::_isDisabled` early-returns handlers (`sources/entities/button_base.cpp:160-173, 305-336`).
2. `Variant::Disabled` on `Checkbox`, `RadioButton`, and `Switch` (e.g. `sources/entities/checkbox.cpp:138`, `switch.cpp:197`).
3. Nothing on `Chip`, `ListItem`, `MenuItem`, `AppBar` actions, `NavigationBar`/`Drawer` destinations, `Tab`, or `SegmentedButton`.

The shared interaction components have no enabled flag, so disabled entities still receive hover/press events and can take focus; only entity-local handlers filter. `Switch` binds release handlers unconditionally and `toggle()` has no disabled guard (`sources/entities/switch.cpp:291-294, 337-347`), so a disabled switch still toggles.

**Fix:** make `components::StyleState` own `enabled`; have interaction systems and focus skip disabled entities; make every family's disabled variant route through it.

### CC-10 — No motion duration tokens (P1)

`motion::Easing` defines Linear/Standard/Decelerate/Accelerate/Emphasized (`headers/guillaume/motion/easing.hpp:31-37`) but lacks `EmphasizedDecelerate`/`EmphasizedAccelerate`, and there are no duration tokens. Magic numbers:

- `components/animation.hpp:51` → 0.3 s
- `loading_indicator.cpp:259`, `progress_indicator.cpp:351,389` → 1.5 s
- `snackbar.cpp` → `Overlay::setAutoDismiss(4.0f)`
- `systems/animation.cpp:59` → 0.1 s clamp

**Fix:** add `theme::MotionScale` (short1/2/4, medium1/2/4, long1/2/4; easing per role) and replace the literals.

### CC-11 — Palette and schemes are two disagreeing sources of truth (P2)

`initializePalettes` builds a rust/orange primary palette: `Primary.T40 = (171,53,15)` (`sources/theme.cpp:1102-1104`), while `initializeLightScheme` uses the M3 purple baseline `Primary = (103,80,164)` (`sources/theme.cpp:272-273`). The schemes do not read from the palettes; every role is duplicated as a literal. `Palette::Tone` also lacks Paper's 95 and 99 tones (`headers/guillaume/theme.hpp:287-299`), which Paper uses for `surface` and `inverseOnSurface`.

**Fix:** generate schemes from palettes, add T95/T99, or explicitly document that palettes are brand palettes and schemes are baseline. As written, a consumer cannot know which is authoritative.

### CC-12 — Theme switching leaves stale entity colors (P2)

`defaultTheme` is a static-init global and `activeSchemeRole` is an anonymous-namespace non-atomic global (`sources/theme.cpp:1306-1321`). Entities snapshot `schemeColor()` at build/style time (e.g. `card.cpp:119`, `dialog.cpp:164`), and there is no theme-change revision or re-style system. Switching light→dark leaves existing trees with the old colors. There is no per-scene theme scope or thread safety.

**Fix:** add a `themeRevision()` counter and a `systems::ThemeSync` that re-applies `applyStyleState()`/`applyVariant()` when it changes; guard the active role or make it scene-owned.

### CC-13 — Touch targets below the M3 48 dp minimum (P2)

- `IconButton::Size::Small` is 32×32 (`sources/entities/icon_button.cpp:136-142`).
- `FloatingActionButton::Size::Small` is 40×40 (`sources/entities/floating_action_button.cpp:142-149`).
- Hover accessibility margins default to `{0, 0}` (`headers/guillaume/components/mouse_hover_interaction.hpp:54-57`, `hand_hover_interaction.hpp:52-55`).
- Checkbox is 18×18 and radio 20×20 with no 36–40 dp outer target (`sources/entities/checkbox.cpp:64-65`, `radio_button.cpp:109-110`).

**Fix:** add a `theme::MinTouchTarget = 48` token and an interaction/hit-slop margin, then apply it to icon buttons, FABs, checkboxes, radios, and switches.

### CC-14 — Foundation test coverage is absent (P1)

- No `test_theme.cpp`, no `test_surface.cpp`.
- `tests/CMakeLists.txt` uses an explicit list and does not include either.
- No test asserts light/dark role completeness, typescale values, shape/elevation/state tokens, or palette/scheme consistency.
- Entity tests assert the same magic numbers as production (e.g. `test_button.cpp:310` alpha 31, `test_chip.cpp:64,68` height 32/radius 16, `test_switch.cpp:65,66,70` 52/32/16, `test_app_bar.cpp:69,70,91-93`, `test_date_picker.cpp:72,76`).
- `test_text_field.cpp:98-100` and `test_card.cpp` assert border color alpha, which is a false positive because the border is not rendered.
- No test covers hover/press state layers, disabled visual state, focus traversal, or accessibility semantics.

**Fix:** add foundation tests first, then convert entity assertions to token lookups so token changes do not require editing 30 test files.

---

## Component matrix

Paper column: the closest Paper `v5.15.4` source(s). `—` means no direct counterpart.

| Guillaume family | Paper | Verdict | Sev | Top gap |
|---|---|---|---|---|
| `app_bar` | `Appbar/*` | Diverges | P1 | Heights 56/104 vs 64/112; actions are inert icons; no header role |
| `badge` | `Badge.tsx` | Partial | P2 | No `visible`/animation/size; defaults to 6 dp dot vs Paper 20 dp label |
| `bottom_sheet` | `Modal.tsx` (closest) | No Paper ref | P2 | Modal uses `SurfaceContainerHigh`; hardcoded 800×600 scrim; no handle/detents/motion |
| `button` | `Button/*` | Partial | P1 | Defaults to Filled (Paper text); no loading; label font tied to icon size; no elevation |
| `carousel` | — | No Paper ref | P2 | No hero/multi-browse, snapping, or viewport mask |
| `card` | `Card/*` | Partial | P1 | No press/disabled state; outlined border not painted; no Content/Actions/Cover/Title |
| `checkbox` | `Checkbox/*` | Partial | P1 | Non-interactive; label inside 18 dp box; no role/checked state/touch target |
| `chip` | `Chip/*` | Diverges | P1 | Non-interactive; always outlined; 16 dp vs 8 dp radius; unselected filter shows check |
| `date_picker` | — | No Paper ref | P2 | No selected/today visuals, weekday header, or month navigation |
| `dialog` | `Dialog/*`, `Modal.tsx` | Partial | P1 | No scrim/transition; actions stack vertically; title 22 vs 24 |
| `divider` | `Divider.tsx` | Match | P3 | Default 1 dp vs hairline; no explicit `bold` |
| `extended_floating_action_button` | `FAB/FAB.tsx` | Partial | P2 | Radius 28 vs 16; label 24/36 vs 14; no elevation/motion |
| `floating_action_button` | `FAB/FAB.tsx` | Partial | P1 | No elevation/mode/visibility/loading; label 24 vs 14 |
| `floating_action_button_menu` | `FAB/FABGroup.tsx` | Partial | P1 | Labels never rendered; no scrim/state callback; closed actions stay clickable |
| `icon` | `Icon.tsx`, `MaterialCommunityIcon.tsx` | Partial | P2 | Default white vs `OnSurface`; `Glyph::Style` has no render effect |
| `icon_button` | `IconButton/*` | Diverges | P1 | Filled/Tonal/Standard-selected color roles wrong; rounded square vs round |
| `image` | `Avatar/AvatarImage.tsx` | No Paper ref | P3 | Only a texture path; no size/fit/load-error state |
| `layout` | — | No Paper ref | P3 | No wrap/per-edge padding/flex-grow; default white fill |
| `list` | `List/*` | Partial | P1 | Rows inert; supporting text beside headline; no Section/Subheader/Accordion |
| `loading_indicator` | `ActivityIndicator.tsx` | Partial | P2 | Default Linear vs circular; no hide-on-stop/color/size override |
| `menu` | `Menu/*` | Partial | P1 | Items inert; variant ignored; no elevation; label 14 vs 16 |
| `model` | — | No Paper ref | P3 | No M3 relevance; no load/error state |
| `navigation_bar` | `BottomNavigation/*` | Partial | P1 | No indicator pill; transparent container; wrong focused label role |
| `navigation_drawer` | `Drawer/*` | Partial | P1 | No active pill; no scrim; no Section/CollapsedItem |
| `navigation_rail` | `BottomNavigation`/`Drawer` (closest) | No Paper ref | P2 | FAB is a bare icon; no indicator pill; expanded labels centered |
| `progress_indicator` | `ProgressBar.tsx` | Partial | P2 | No visibility/animations; indeterminate 40% vs 60%; no a11y value |
| `radio_button` | `RadioButton/*` | Partial | P1 | Non-interactive/no group; dot 8 vs 10; label overflows 20 dp surface |
| `search` | `Searchbar.tsx` | Partial | P1 | No clear/loading affordances; `FullScreen` is a 640 modal vs 72 inline view |
| `segmented_button` | `SegmentedButtons/*` | Partial | P2 | No per-segment icon/disabled/a11y/check; outline breaks on selected |
| `side_sheet` | `Modal.tsx` (closest) | No Paper ref | P2 | No scrim or slide transition; no dismiss callback |
| `slider` | — | No Paper ref | P2 | No disabled/state layers; wrong inactive-track role; range low thumb not draggable |
| `snackbar` | `Snackbar.tsx` | Partial | P1 | No `visible/show/hide`/dismiss/duration; action is not pressable |
| `split_button` | `Button` + `IconButton` | No Paper ref | P3 | Halves mismatched in height; no separator/disabled/a11y |
| `standard_button_group` | `Button` (closest) | No Paper ref | P3 | Buttons get empty click handlers; no per-button configuration |
| `surface` | `Surface.tsx` | Missing | P1 | No public entity; no elevation→background or flat/elevated mode |
| `switch` | `Switch/*` | Partial | P1 | Disabled still toggles; no `onValueChange`; label overlaps 52 dp track |
| `tabs` | `BottomNavigation`/`SegmentedButtons` (closest) | No Paper ref | P1 | Active indicator never drawn; secondary label uses wrong role |
| `text` | `Typography/Text.tsx` | Diverges | P0 | No typescale/variant; default 24 px white; hardcoded Roboto Regular |
| `text_field` | `TextInput/*`, `HelperText/*` | Partial | P0 | Outline/underline never render; no floating label/disabled/dense/affix |
| `time_picker` | — | No Paper ref | P2 | No field boxes, AM/PM/24 h, selected hour, or clock hand |
| `toolbar` | `Appbar` (closest) | No Paper ref | P3 | No 64 dp height preset, elevation, slot model, or a11y |
| `tooltip` | `Tooltip/*` | Partial | P1 | No trigger/anchor/delays; label is `OnSurface` on `InverseSurface` |

---

## Detailed findings

### Actions and selection

#### `app_bar` — Diverges / P1

- **Paper:** `Appbar/Appbar.tsx`, `AppbarHeader.tsx`, `AppbarContent.tsx`, `AppbarAction.tsx`, `AppbarBackAction.tsx`, `utils.ts`.
- **API:** No `mode='center-aligned'`, `elevated`, action callbacks/disabled, logo, or per-child `isLeading`. `Variant::Bottom` is a Guillaume extension.
- **Variants/defaults:** Default `Small` is correct. Heights are 56/104/152/80 (`sources/entities/app_bar.cpp:131-145`) vs Paper's 64/112/152 (`Appbar/utils.ts:108-113`); small/medium are 8 dp short.
- **Tokens:** Background `Surface` matches. Actions use `OnSurface` (`app_bar.cpp:149`) vs Paper `onSurfaceVariant`. Title is always 22 vs `titleLarge` 22 / `headlineSmall` 24 / `headlineMedium` 28 per mode. No elevation.
- **States:** Navigation icon and actions are plain `Icon` entities with no handlers (`app_bar.cpp:151-175`); nothing is pressable, disabled, or focusable.
- **Layout:** Fixed default width 360, padding 16, spacing 8; Paper uses 4 dp horizontal padding, title margin 12, and multi-row column layouts for medium/large.
- **A11y:** No header role, action labels, or disabled state.
- **Tests:** 8 tests cover geometry, variant heights, child counts, surface color, and director; no interactivity, a11y, or per-mode typography.
- **Evidence:** `sources/entities/app_bar.cpp:57-62,131-175,197-211`; Paper `Appbar/utils.ts:105-120`, `AppbarContent.tsx:165-175`.
- **Fix:** fix small/medium heights, make actions/back real icon buttons, add mode-specific title typography and header semantics.

#### `badge` — Partial / P2

- **Paper:** `Badge.tsx`.
- **API:** No `visible`, `size`, or color override; only `Variant` + `label`.
- **Defaults:** Guillaume defaults to `Dot` (6 dp), which suppresses the label; Paper defaults to a 20 dp badge whose label is whatever children are provided (empty if none).
- **Tokens:** Both use `Error`/`OnError`. Label is `size*0.75` and padding 0 vs Paper `size*0.5` and 3 dp horizontal padding.
- **States:** No opacity animation for `visible`; Paper animates 1↔0 over 150 ms.
- **Layout:** No corner anchoring (`alignSelf: flex-end` in Paper); placement depends on the parent layout.
- **Tests:** 6 tests cover dot/small/large geometry, Error color, variant, director; no visibility, size, or contrast.
- **Evidence:** `headers/guillaume/entities/badge.hpp:60,69-70`; `sources/entities/badge.cpp:102-153,180-196`; Paper `Badge.tsx:15,56,81-117`.
- **Fix:** add `visible` + animation and a numeric size/color override while keeping `Dot`.

#### `bottom_sheet` — No Paper ref / P2

- **Closest Paper:** `Modal.tsx`, `Surface.tsx`.
- **API:** No `onDismiss`, dismissable flag, half/full detents, peek height, drag handle, or scrollable content.
- **Tokens:** Standard uses `SurfaceContainerLow` (correct); modal uses `SurfaceContainerHigh` (`bottom_sheet.cpp:211`) whereas M3 modal sheets use `surfaceContainerLow`.
- **Scrim:** Hardcoded 800×600 at `Color32Bit(0,0,0,82)` (`bottom_sheet.cpp:66-71`) instead of a viewport-sized `Scrim`/`Backdrop`.
- **States:** Drag threshold 120 but no snap-back or animated dismiss. No motion.
- **Tests:** 7 tests cover 360×320 geometry, top-only radius, overlay flags, show/hide, variant, director; no drag/scrim/motion.
- **Evidence:** `sources/entities/bottom_sheet.cpp:46,66-71,102-112,195-279`.
- **Fix:** use `SurfaceContainerLow`, viewport scrim from the `Backdrop` role, add a 32×4 drag handle, and animate open/close.

#### `button` — Partial / P1

- **Paper:** `Button/Button.tsx`, `Button/utils.tsx`.
- **API:** Missing `loading`, `compact`, `uppercase`, custom `buttonColor`/`textColor`, `accessibilityHint`, `accessibilityRole`, and long-press. `withIcon`, `withTrailingIcon`, disabled, and toggle are genuine equivalents.
- **Defaults:** Guillaume defaults to `Color::Filled` (`headers/guillaume/entities/button.hpp:250`); Paper defaults to `mode='text'` (`Button.tsx:183`). `Segmented` and the five-size axis are Guillaume extensions.
- **Tokens:** Color roles for Filled/Tonal/Outlined/Text match Paper. Label font is the icon size, 15/18/24/30 (`button.cpp:675-683`), instead of `labelLarge` 14; default icon is 15 vs 18. `Elevated` maps to `SurfaceContainerLow` but has no shadow. `Shape::Round` is radius 100 vs M3's constant 20.
- **States:** Hover/press alphas 20/31 match M3 0.08/0.12. No loading state. Disabled matches. No focus visual.
- **Layout:** No 64 dp min width or 40 dp min height; Small is ~34–37 high from padding 8. Label margins differ from Paper's 16/24.
- **A11y:** Label only; no role, hint, or disabled/selected state.
- **Tests:** 15 tests cover toggle/selected/morph, disabled-ignores-release, trailing icon, label round-trip, layers, director; no default color, color-role mapping, loading, min size, or hover/press layers.
- **Evidence:** `headers/guillaume/entities/button.hpp:250`; `sources/entities/button.cpp:211-245,258-274,319,415-427,675-683,729-757`; Paper `Button.tsx:183,283-284,314,390-400`, `utils.tsx:56-76,108-134`.
- **Fix:** add loading/hint/role/long-press, switch the default to `Text`, decouple label typography from icon size, and give `Elevated` a real shadow.

#### `carousel` — No Paper ref / P2

- **Closest Paper:** none; M3 carousel spec applies.
- **API:** Variants Basic/Centered/Uncontained; no peek/snap configuration, nested scroll coordination, item builder, or mask.
- **Tokens:** Placeholder items use `SurfaceContainerHighest` with 12 dp radius; no hero weighting or M3 mask/fade.
- **States:** Index changes only by drag-delta rounding (`carousel.cpp:410-424`); no fling/snap animation, no disabled/focus/hover.
- **Layout:** Horizontal strip via `systems::Layout`; no viewport masking or partial-item alignment.
- **Tests:** 6 tests cover geometry, counts, clamping, content size, variant, director; drag behavior untested.
- **Evidence:** `headers/guillaume/entities/carousel.hpp:72-76`; `sources/entities/carousel.cpp:47-49,102-122,223-321,404-427`.
- **Fix:** add snapping/fling, hero/multi-browse variants, and viewport masking.

#### `card` — Partial / P1

- **Paper:** `Card/*`, `Surface.tsx`.
- **API:** No press/long-press/disabled, `contentStyle`, or `Card.Content/.Actions/.Cover/.Title` composition; only pose/variant/width/height/radius.
- **Variants:** Defaults match (Elevated). Paper's third mode is `contained`; Guillaume calls it `Filled`. Paper has no fixed width; Guillaume forces 240.
- **Tokens:** Radius 12 matches. Contained uses `SurfaceVariant`; Filled uses `SurfaceContainerHighest`. Outlined uses transparent fill + `OutlineVariant`, but the border is not rendered (CC-02). Elevated uses `SurfaceContainerLow` + a shadow rather than Paper's `elevation.level1` tint.
- **States:** No pressed elevation animation (Paper 1→2 over 150 ms), no hover/focus, no disabled.
- **Layout:** No cover/title/content/actions slots; title min height and 40 dp leading slot are absent.
- **A11y:** No `accessible` forwarding or disabled state.
- **Tests:** 6 tests cover width/radius/elevation/variant/director; they assert the unused border color, so the missing outline is undetected.
- **Evidence:** `headers/guillaume/entities/card.hpp:54-60,96-124`; `sources/entities/card.cpp:47-56,108-138`.
- **Fix:** render outlined borders, add press/disabled interaction with elevation animation, and add Content/Actions/Cover/Title composition.

#### `checkbox` — Partial / P1

- **Paper:** `Checkbox/Checkbox.tsx`, `CheckboxAndroid.tsx`, `utils.ts`.
- **API:** No `onPress`, `onLongPress`, disabled behavior, color overrides, or accessibility label. `State` maps to Paper `status`; `Variant::Error` is a Guillaume extension.
- **Tokens:** Checked `Primary` + `OnPrimary` matches Paper. Disabled uses onSurface 38%; error uses `Error`/`OnError`.
- **States:** No interaction component at all (`headers/guillaume/entities/checkbox.hpp:60`); only `setState` works. No hover/press/ripple; no 36–40 dp target.
- **Layout:** The 14 px label is a child of the fixed 18×18 surface (`checkbox.cpp:64-65,245-262`), so it overflows or overlaps.
- **A11y:** No role, checked/disabled state, or label.
- **Tests:** 6 tests cover geometry, state/variant round-trip, indeterminate stroke, child count, director; no interaction, disabled behavior, or label geometry.
- **Evidence:** `headers/guillaume/entities/checkbox.hpp:60`; `sources/entities/checkbox.cpp:56-65,130-189,245-262`.
- **Fix:** make it interactive and target-sized, add role/checked/disabled semantics, and move the label outside the control bound.

#### `chip` — Diverges / P1

- **Paper:** `Chip/Chip.tsx`, `Chip/helpers.tsx`.
- **API:** No `onPress`, `onLongPress`, `onClose`, `closeIcon`, disabled, mode, elevated, compact, selectedColor, showSelectedCheck/Overlay, or accessibility label.
- **Defaults:** Guillaume always renders a transparent container with an `Outline` border (`chip.cpp:152-156`) regardless of variant; Paper defaults to `flat` with a `secondaryContainer` fill and transparent border, and only `outlined` adds an outline.
- **Tokens:** Radius 16 vs Paper 8; height 32/icon 18/label 14 match. Leading icon is `OnSurfaceVariant`; Paper uses `Primary` for a supplied icon. Filter builds a check glyph even when unselected (`chip.cpp:227-242`); Paper shows it only when selected.
- **States:** No press/hover/focus/disabled — `Chip` owns only `Selectable` (`chip.hpp:59`). The Input close icon is decorative and unclickable.
- **Layout:** Padding 12 uniform vs Paper's asymmetric 16/8 padding; no avatar slot or compact mode.
- **A11y:** No label, selected/disabled state, or close-button label.
- **Tests:** 6 tests cover height/radius, variants, selection, filter fill, child count, director; no interaction, close, disabled, or color matrix.
- **Evidence:** `sources/entities/chip.cpp:58,65,146-156,227-254`; `headers/guillaume/entities/chip.hpp:59`; Paper `Chip.tsx:182,254,279-282,357-391,411-438`.
- **Fix:** add press/close/disabled/label semantics, align flat vs outlined colors, and use the 8 dp radius.

#### `date_picker` — No Paper ref / P2

- **Closest Paper:** none; M3 date picker spec applies.
- **API:** SingleDate/DateRange, year/month, callbacks; missing min/max/selectable dates, input mode, mode toggle, confirm/cancel.
- **Tokens:** 328 px panel, radius 28, `SurfaceContainerHigh` — matches M3. Day labels are all `OnSurface`; no selected-day `Primary`/`OnPrimary`, no today outline.
- **States:** Selected/range states are not visually distinguishable; no disabled/hover/focus.
- **Layout:** 42-cell grid, 4 dp spacing; no weekday header or leading/trailing month-day distinction.
- **Tests:** 7 tests cover geometry, modal overlay, year/month/selection round-trip, 42 cells, labels, director; no range callback or selection visuals.
- **Evidence:** `headers/guillaume/entities/date_picker.hpp:76-86`; `sources/entities/date_picker.cpp:57,262-278,364-366,380-480`.
- **Fix:** render selected/today states, add a weekday header and month navigation.

#### `dialog` — Partial / P1

- **Paper:** `Dialog/*`, `Modal.tsx`.
- **API:** No `dismissable`, `dismissableBackButton`, `onDismiss`, runtime actions setter, scrollable content, icon override, or elevation.
- **Variants:** Alert default plus Simple/Confirmation/FullScreen. Paper has one Dialog with composable Title/Content/Actions/Icon/ScrollArea.
- **Tokens:** Width 320, radius 28, `SurfaceContainerHigh`; Paper uses content-derived width, radius 28, `elevation.level3`. Title is 22 vs Paper `headlineSmall` 24. Message 14/`OnSurfaceVariant` matches. Icon is `Primary` left-aligned vs Paper `Secondary` centered with 24 dp top padding.
- **States:** Modal overlay and Escape/outside dismiss work, but no scrim is rendered and no open/close transition.
- **Layout:** Padding 24/spacing 16; actions are appended into the vertical column (`dialog.cpp:235-237`) instead of a right-aligned horizontal row.
- **A11y:** No header role, modal semantics, or live region.
- **Tests:** 7 tests cover geometry, overlay flags, show/hide, variants, director; no scrim, action layout, or dismissal callback.
- **Evidence:** `sources/entities/dialog.cpp:60-67,105,140-198,235-237`; Paper `Dialog.tsx:55,110-131,166-175,187`, `DialogActions.tsx:66-83`.
- **Fix:** render a themed scrim, lay actions out horizontally and right-aligned, and bump the title to 24.

#### `divider` — Match / P3

- **Paper:** `Divider.tsx`.
- **API:** Paper `leftInset`/`horizontalInset`/`bold`; Guillaume `FullWidth`/`Inset`/`Middle`, `withInset`, `withThickness` — semantic equivalent.
- **Tokens:** Both use `OutlineVariant`. Guillaume defaults to 1 dp; Paper defaults to a hairline. `bold` maps to `withThickness(1)`.
- **Layout:** Insets are 16 in both.
- **Tests:** 6 tests cover full width, leading-only/both insets, thickness→Line, color, director. Paper has no Divider test.
- **Evidence:** `headers/guillaume/entities/divider.hpp:67,130-142`; `sources/entities/divider.cpp:47-57,130-183`; Paper `Divider.tsx:15-25,55-99`.
- **Fix:** default to a hairline and expose an explicit `bold` flag if needed.

#### `extended_floating_action_button` — Partial / P2

- **Paper:** `FAB/FAB.tsx` (extended), `FAB/utils.ts`.
- **API:** No mode, visible, loading, custom size, long press, or collapse/expand animation. The three-size axis is invented; Paper's extended FAB is always 56 high.
- **Tokens:** Variant palette matches. Medium radius 28 vs Paper 16; label uses icon size 24/36 vs `labelLarge` 14. No elevation.
- **States:** State layers match. No loader/visibility.
- **Layout:** Height 56 and horizontal inset 16 match; width is content-derived.
- **Tests:** 3 tests cover child creation, label mutation, director; no geometry/typography.
- **Evidence:** `sources/entities/extended_floating_action_button.cpp:158-184,309-315,366-375`.
- **Fix:** set the medium radius to 16, render `labelLarge`, and add elevation.

#### `floating_action_button` — Partial / P1

- **Paper:** `FAB/FAB.tsx`, `FAB/utils.ts`.
- **API:** Missing mode (flat/elevated), visible, loading, customSize, long press, accessibility state/label default.
- **Tokens:** Variant colors match Paper. Surface uses `SurfaceContainerHigh`/`Low` rather than Paper's `elevation.level3`. Sizes 40/56/96 and radii 12/16/28 match M3. No `components::Elevation` at all (`headers/guillaume/entities/floating_action_button.hpp:42`). Label uses icon size 24/24/36 vs `labelLarge` 14.
- **States:** Loading and invisible states missing; disabled colors match.
- **A11y:** No role/state; no label default.
- **Tests:** 5 tests cover children, positioning, disabled-ignores-release, label, director; no size/radius/color/elevation.
- **Evidence:** `sources/entities/floating_action_button.cpp:142-182,190-229,296,311-313`; Paper `FAB.tsx:199,205,256,266,289,321`, `utils.ts:191-255,364-414`.
- **Fix:** add `Elevation` (level 3 elevated, 0 flat/disabled), flat/elevated mode, and label typography.

#### `floating_action_button_menu` — Partial / P1

- **Paper:** `FAB/FABGroup.tsx`, `FAB/utils.ts`.
- **API:** `Action.label` exists but is never read — the builder hardcodes `""` (`floating_action_button_menu.cpp:70`). Missing `onStateChange`, controlled open, backdrop dismiss, variant, long press, and a live `withAccessibilityLabel` (stored at `:78,98`, never forwarded).
- **Tokens:** Trigger is hardcoded `Surface`/Medium (`:196-197`) vs Paper's default primary. No scrim, no label cards, no stacked-FAB elevation/stagger.
- **States:** Closed actions are removed from layout but keep their handlers and stale transform, so they remain clickable at old positions. No `accessibilityState.expanded`.
- **Layout:** Vertical spacing 16/padding 0 vs Paper's item margin 16 and horizontal 24/16.
- **Tests:** 4 tests cover action count, open/toggle flags, director; no labels, scrim, animation, or closed-state hit testing.
- **Evidence:** `sources/entities/floating_action_button_menu.cpp:70,78,98,103-110,162-221`; Paper `FABGroup.tsx:110-114,255-297,392-428,430-461,489-505`.
- **Fix:** render/announce action labels, add a scrim and `onStateChange`, and disable/hide actions while closed.

#### `icon` — Partial / P2

- **Paper:** `Icon.tsx`, `MaterialCommunityIcon.tsx`, `CrossFadeIcon.tsx`.
- **API:** Glyph name/font size/color/style; no image/function source or RTL direction.
- **Tokens:** Paper defaults to `onSurface`; Guillaume defaults to opaque white (`icon.cpp:50`). `Glyph::Style` (Outlined/Rounded/Sharp) is stored and used in the measurement cache key, but `GlyphRender` always loads Material Symbols Outlined (`sources/systems/glyph_render.cpp:39-43,100-105`), so Rounded/Sharp never render differently.
- **States:** None in either (parent components handle disabled).
- **Layout:** Paper sizes frames explicitly; Guillaume derives the bound from glyph measurement.
- **Tests:** 4 tests cover measure/render, setters, director; no default color or style-render effect.
- **Evidence:** `headers/guillaume/entities/icon.hpp:85-113,153-155`; `sources/entities/icon.cpp:46-52`; Paper `Icon.tsx:14-27,64-81,103-165`.
- **Fix:** default to `OnSurface`, make `Style` select the font or remove the prop, and add image/function sources.

#### `icon_button` — Diverges / P1

- **Paper:** `IconButton/IconButton.tsx`, `utils.ts`.
- **API:** Missing loading, animated cross-fade, accessibility hint, icon/container color overrides, and controlled selected state.
- **Defaults:** Paper with no mode has no container; selected is a `primary` icon. Guillaume `Standard` selected fills `secondaryContainer`/`OnSecondaryContainer`. `Filled` unselected is `Primary`/`OnPrimary` vs Paper `surfaceVariant` container/`primary` icon. `Tonal` unselected is `SecondaryContainer`/`OnSecondaryContainer` vs Paper `surfaceVariant`/`onSurfaceVariant`. `Outlined` matches.
- **Tokens:** Shape is a rounded square 8/12/16 vs Paper fully round; sizes 32/40/48 vs Paper 40 default; icons 18/24/30 vs Paper 24.
- **States:** No loading/animated transition; hover/press alphas match. No disabled a11y state.
- **Tests:** 7 tests cover toggle, centering, disabled-ignores-release, label, director; no color/shape matrix.
- **Evidence:** `sources/entities/icon_button.cpp:134-174,181-275`; Paper `IconButton/utils.ts:34-128`, `IconButton.tsx:156-161`.
- **Fix:** correct Filled/Tonal/Standard-selected roles and use a fully round container.

#### `image` — No Paper ref / P3

- **Closest Paper:** `Avatar/AvatarImage.tsx`.
- **API:** Texture path only; no size/aspect/fit/tint/radius/placeholder/load callbacks, and no `getTexturePath` getter.
- **Tokens:** Paper's Avatar.Image uses a circular `Primary` placeholder; Guillaume has no placeholder/fallback and skips render when the texture is missing.
- **Layout:** Depends on externally set `Bound`; no explicit square/size.
- **Tests:** 6 tests cover path set/update, director, mesh creation, missing texture; no size/fit/failure visual.
- **Evidence:** `headers/guillaume/entities/image.hpp:84,128-143`; `sources/entities/image.cpp:66-96`; Paper `AvatarImage.tsx:7-62,88-113`.
- **Fix:** add size/aspect/content-scale and a load-error placeholder.

#### `layout` — No Paper ref / P3

- **Closest Paper:** none (flexbox `View`/`Surface`).
- **API:** Axis, main/cross alignment, spacing, padding, fixed width/height, children. No flex-grow/shrink, wrap, min/max size, per-edge padding, or SpaceAround/SpaceEvenly.
- **Tokens:** No theme roles; default fill is opaque white (`layout.cpp:53`) and radius 16 is hardcoded.
- **Layout:** Single-axis arrangement only; no wrap, baseline, RTL, or percentages.
- **Tests:** 18 tests (the best-covered family) cover z-order, bounds, padding/spacing, axis, fixed sizing, alignments, change flags; no wrap/RTL/percent (unsupported).
- **Evidence:** `headers/guillaume/entities/layout.hpp:66-72,231-244`; `sources/entities/layout.cpp:50-65,400-416`.
- **Fix:** add per-axis padding/wrap/min-max only if flex-like containers are required.

#### `model` — No Paper ref / P3

- **Closest Paper:** none; 3D model primitive with no M3 analogue.
- **API:** Model path + optional texture path, director; no visibility/load/error state, and no scale/rotation/anchoring beyond `components::Transform`.
- **Tokens:** No M3 color/shape/typography applies.
- **States:** No load/error state; missing assets are silently skipped at render time.
- **Tests:** 7 tests cover constructor config, path setters, update re-application, director, measure, mesh render, and missing-model skip.
- **Evidence:** `headers/guillaume/entities/model.hpp:86-119`; `sources/entities/model.cpp:47-116`; `tests/sources/entities/test_model.cpp:66-211`.
- **Fix:** none for M3 parity; add an asset-load error state if the engine needs one.

#### `list` — Partial / P1

- **Paper:** `List/ListItem.tsx`, `List.tsx`, `ListSection.tsx`, `ListSubheader.tsx`, `ListAccordion.tsx`, `ListIcon.tsx`, `ListImage.tsx`.
- **API:** Item has headline/supporting/leading/trailing only; no `onPress`, disabled, numberOfLines, left/right render props, Section, Subheader, Accordion, or Image.
- **Variants:** Heights hardcoded 48/64/88 vs Paper's content-derived ~52/72/92.
- **Tokens:** Title 16/`OnSurface`, supporting 14/`OnSurfaceVariant`, icons 24/`OnSurfaceVariant` match.
- **States:** Rows are inert `Layout`s; no press/hover/focus/selected/disabled.
- **Layout:** Row is horizontal with 16 padding/spacing, so headline and supporting text are siblings side-by-side instead of stacked title/description.
- **A11y:** No row role/label.
- **Tests:** 6 tests cover width, vertical stacking, counts, IDs; the 96 height for two single-line rows encodes the hardcoded 48.
- **Evidence:** `headers/guillaume/entities/list.hpp:69-74,113-132,201-233`; `sources/entities/list.cpp:112-123,146-213`.
- **Fix:** add press/disabled semantics, stack title/description in a content column, then add Section/Subheader/Accordion.

#### `loading_indicator` — Partial / P2

- **Paper:** `ActivityIndicator.tsx`.
- **API:** Variant, size (circular only), start/stop; no color, hide-when-stopped, or size enum.
- **Defaults:** Guillaume defaults to `Linear` 200×4; Paper is circular, default 24. Guillaume circular defaults to 48. Stroke is fixed 4 vs Paper `size/10`.
- **States:** `stop()` freezes the last frame and leaves it visible; no fade/hide.
- **A11y:** No `progressbar` role or busy state.
- **Tests:** 8 tests cover geometry, z-order, running, stop/start, arc child, variant, director; no hide-on-stop/color/a11y.
- **Evidence:** `headers/guillaume/entities/loading_indicator.hpp:61-74,110-117`; `sources/entities/loading_indicator.cpp:46-53,132-148,334-364`.
- **Fix:** add `hidesWhenStopped` with a fade, color/size override, and progressbar semantics.

#### `menu` — Partial / P1

- **Paper:** `Menu/Menu.tsx`, `MenuItem.tsx`, `utils.ts`.
- **API:** Items have label/leading icon only; no per-item press, disabled, dense, trailing icon, or a11y. Menu has no anchor/anchorPosition/onDismiss/elevation/mode.
- **Variants:** Dropdown/Context/Cascading are enum values that do not change behavior; Paper's real axis is `mode: elevated|flat` + `elevation`.
- **Tokens:** Radius 4, `SurfaceContainer` background (close to Paper `elevation.level2`), but no shadow. Item label 14 vs `bodyLarge` 16; icon 20 vs 24; container padding 0 vs 8.
- **States:** No press/hover/focus/disabled on items.
- **Layout:** Fixed width 200 vs Paper's 112–280 content-derived width; item height 48 and padding 12 match.
- **A11y:** No `menuitem` role/state or modal overlay semantics.
- **Tests:** 7 tests cover width, counts, overlay flags, open/close, variant; no color/disabled/press.
- **Evidence:** `headers/guillaume/entities/menu.hpp:72-75,127-128,201-264`; `sources/entities/menu.cpp:42,127-201`.
- **Fix:** add per-item press/disabled/trailing semantics, an elevation shadow, and `bodyLarge` label size.

#### `navigation_bar` — Partial / P1

- **Paper:** `BottomNavigation/BottomNavigationBar.tsx`, `BottomNavigation/utils.ts`.
- **API:** `addDestination(icon,label)`, select, counts; no focused/unfocused icon pair, badge, per-tab callbacks, labeled/compact toggles, or color overrides.
- **Variants:** `WithLabels` default and `WithoutLabels` match Paper `labeled`.
- **Tokens:** Bar height 80 matches. Active icon `OnSecondaryContainer`, inactive `OnSurfaceVariant` match. Focused label uses `OnSecondaryContainer` vs Paper `onSurface`. No active indicator pill. The container is left transparent because `applyVariant` never calls `setColor`, so the default `SurfaceConfig` color `{0,0,0,0}` remains.
- **States:** Selection is exclusive and works; no hover/focus/pressed/disabled per destination.
- **Layout:** SpaceBetween + 8 padding vs Paper equal flex items.
- **A11y:** No tab role, selected state, labels, or long press.
- **Tests:** 6 tests cover geometry, counts, exclusivity; no colors, indicator, or callbacks.
- **Evidence:** `sources/entities/navigation_bar.cpp:51,96-97,120-129,183-194,239-247,350-365`; Paper `BottomNavigationBar.tsx:219,507-511,716-758,966-1000`.
- **Fix:** add the `secondaryContainer` indicator pill, set the container to `SurfaceContainer`, and use `onSurface` for the focused label.

#### `navigation_drawer` — Partial / P1

- **Paper:** `Drawer/DrawerItem.tsx`, `DrawerCollapsedItem.tsx`, `DrawerSection.tsx`.
- **API:** Destinations with icon/label, selection, open/close; no per-item active/disabled/onPress, right slot, a11y, Section, or CollapsedItem.
- **Variants:** Standard/Modal; Paper has no drawer container, only items/sections.
- **Tokens:** 320×640, inner-edge radius 16, item height 56/radius 28/icon 24/label 14 match. Active state changes content color but paints no `secondaryContainer` pill.
- **States:** No disabled/hover/press; modal has no scrim.
- **Layout:** Item padding 16/spacing 12 matches; no section grouping or divider.
- **A11y:** No button role or selected state.
- **Tests:** 7 tests cover geometry, radii, counts, selection, overlay flags; no active color/sections.
- **Evidence:** `sources/entities/navigation_drawer.cpp:56,100-111,177-190,243-244,354-409`; Paper `DrawerItem.tsx:113-127,142-169,185-215`.
- **Fix:** paint the active destination pill and add a modal scrim; add Sections/Divider/CollapsedItem.

#### `navigation_rail` — No Paper ref / P2

- **Closest Paper:** `BottomNavigation` indicator/selection, `DrawerItem` label styling, `FAB`.
- **API:** Destinations, optional FAB, select/counts; no per-destination active/disabled/onPress, badge, label visibility, or expanded animation.
- **Tokens:** Collapsed 80/expanded 256 match M3. Selected `OnSecondaryContainer`/unselected `OnSurfaceVariant` match. No 56×32 indicator pill. The FAB is a bare 24 px icon, not a FAB surface.
- **States:** Exclusive selection works; no hover/press/focus/disabled.
- **Layout:** Padding 8/spacing 8; expanded destinations still center content, so labels stay under the icon instead of beside it.
- **Tests:** 6 tests cover widths, counts, index, variant; no FAB/indicator/alignment.
- **Evidence:** `sources/entities/navigation_rail.cpp:50-51,90-98,182-196,247-250,357-380`.
- **Fix:** compose a real FAB, paint the indicator pill, align expanded labels to start.

#### `progress_indicator` — Partial / P2

- **Paper:** `ProgressBar.tsx`.
- **API:** Four variants, value, width, size; no visibility, color override, external animated value, or fill style.
- **Variants:** Paper uses `indeterminate` boolean and is linear-only; Guillaume adds circular determinate/indeterminate.
- **Tokens:** Track `SurfaceVariant` and fill `Primary` match; height 4 matches; 2 dp rounding is Guillaume-only. Indeterminate segment is 40% vs Paper 60%.
- **States:** No visible fade, no animated determinate transition (Paper 200 ms), indeterminate loops at 1.5 s vs Paper 2 s.
- **A11y:** No progressbar role or value.
- **Tests:** 8 tests cover geometry, z-order, clamping, arc sweep, loop, variant, director; no visibility/animation/color/a11y.
- **Evidence:** `headers/guillaume/entities/progress_indicator.hpp:64-79,214-233`; `sources/entities/progress_indicator.cpp:49-55,351,389-411,422-495`.
- **Fix:** add visibility, 200 ms value transitions, and progress a11y values.

#### `radio_button` — Partial / P1

- **Paper:** `RadioButton/*`.
- **API:** No `onPress`, disabled behavior, value, or group API. Generic `SelectionGroup` exists but `RadioButton` never attaches a group member.
- **Tokens:** Ring 20 dp/thickness 2 matches; inner dot 8 vs Paper 10.
- **States:** No interaction components; selection only via `select()`. No press animation or peer-deselect.
- **Layout:** No margin/target; label is a child of the fixed 20 dp surface and overlaps.
- **A11y:** No radio role/checked state or radiogroup.
- **Tests:** 7 tests cover geometry, child layer, variants, selection, dot alpha, child count, director; no interaction/group/a11y.
- **Evidence:** `sources/entities/radio_button.cpp:66-67,101-110,169-186`.
- **Fix:** add press/role/group semantics, 10 dp dot, and a 36–40 dp target.

#### `search` — Partial / P1

- **Paper:** `Searchbar.tsx`, `TextInput/*`, `Divider.tsx`, `ActivityIndicator.tsx`.
- **API:** Variant/placeholder/text + open/close; no icon override, clear icon/callback, trailing icon, loading, elevation, divider, or a11y labels. Leading icon is hardcoded to `"search"`.
- **Variants:** `SearchBar` 360×56/radius 28 matches Paper bar; `FullScreen` is 640 high/radius 0 and modal, whereas Paper `view` is a 72-high inline field with a divider.
- **Tokens:** `SurfaceContainerHigh` vs Paper `elevation.level3`; placeholder/text colors not set; no active color.
- **States:** No clear/loading/hover/focus; no transition on open/close.
- **Layout:** Fixed 360 width; no trailing affordance slot.
- **A11y:** No search role or button labels.
- **Tests:** 7 tests cover geometry, placeholder, text, overlay modality; no clear/loading/icon-press.
- **Evidence:** `headers/guillaume/entities/search.hpp:111-125,201-263`; `sources/entities/search.cpp:68-72,145-210,255-288`.
- **Fix:** add clear/loading affordances and model `view` as the 72-high inline layout.

#### `segmented_button` — Partial / P2

- **Paper:** `SegmentedButtons/*`.
- **API:** Label-only segments; no per-segment icon/disabled/a11y/check, density, controlled value, or value payload.
- **Defaults:** Single/Multi selection exists, but single-select pressing the selected segment toggles it off; Paper commits the pressed value.
- **Tokens:** Checked `SecondaryContainer`/`OnSecondaryContainer` matches; unchecked text `OnSurfaceVariant` vs Paper `onSurface`. Selected segment drops its border, breaking the outer 1 dp outline. No check icon.
- **States:** No disabled segment, no pressed/hover distinction, no icon/check transition.
- **Layout:** Paper enforces flex/equal widths and minWidth 76 with 16/9 padding; Guillaume sizes to text and uses button medium padding (~45 high).
- **A11y:** No button role or checked/disabled state.
- **Tests:** 8 tests cover segment count, layout, corners, selection, deselect, director; no per-segment state/colors.
- **Evidence:** `sources/entities/segmented_button.cpp:110-148,244-260`; `sources/entities/button.cpp:500-519,540-544`; Paper `SegmentedButtonItem.tsx:218-274`.
- **Fix:** add per-segment config/a11y/check, equalize widths, keep the outline on selected.

#### `side_sheet` — No Paper ref / P2

- **Closest Paper:** `Modal.tsx`, `DrawerItem`.
- **API:** Standard/Modal, side, title/content, show/hide; no onDismiss, dismissable flag, width setter, slide transition, scrollable content, or focus trap.
- **Tokens:** 320×640, inner-edge radius 16, modal `SurfaceContainerLow`/standard `Surface` match M3; no elevation.
- **States:** Modal sets overlay flags but creates no scrim and has no open/close animation.
- **Layout:** 16 padding/12 spacing; edge anchoring only through radii, pose is caller-supplied.
- **A11y:** No modal semantics or live region.
- **Tests:** 7 tests cover geometry, side/variant radii, overlay flags, show/hide, director; no scrim/motion/width.
- **Evidence:** `sources/entities/side_sheet.cpp:67-70,144-190,193-219`.
- **Fix:** add a scrim and edge-slide transition, expose a dismiss callback.

#### `slider` — No Paper ref / P2

- **Closest Paper:** none; M3 slider spec applies.
- **API:** Continuous/Discrete/Range, value, step, width, callback; no disabled, value label, change start/end, or step count.
- **Tokens:** Track 4/radius 2, thumb 20, `Primary` fill/thumb, `SurfaceVariant` inactive track; M3 inactive is `surfaceContainerHighest`. No halo/focus ring/value indicator/stop indicators.
- **States:** Drag-only; no hover/focus/pressed/disabled visuals; no state layers. Range mode moves only the high thumb and both thumbs render `Primary`.
- **A11y:** No adjustable role or min/max/current values.
- **Tests:** 10 tests cover geometry, z-order, child positions, clamping, discrete snapping, range low/high, children, variant, director; no drag simulation or range-thumb independence.
- **Evidence:** `headers/guillaume/entities/slider.hpp:74-78`; `sources/entities/slider.cpp:49-51,158-178,314-443,520-565`.
- **Fix:** add disabled/state layers, M3 inactive-track role, and independent low/high dragging.

#### `snackbar` — Partial / P1

- **Paper:** `Snackbar.tsx`.
- **API:** Pose/variant/message/action string only; no visible/show/hide, onDismiss, duration, icon, or elevation.
- **Variants:** SingleLine/MultiLine/WithAction with fixed 48/68/48; Paper has one auto-growing layout.
- **Tokens:** `InverseSurface` background, `InverseOnSurface` text, `InversePrimary` action, radius 4 match. No elevation. Paper text is `bodyMedium`; Guillaume has no line height.
- **States:** Sets overlay auto-dismiss 4 s but never shows the overlay or registers a dismiss handler, so auto-dismiss/visibility are inert. Action is a plain `Text`, not pressable.
- **Layout:** Fixed 344×48/68, centered, padding 16; Paper is full-width bottom with safe-area and 8 margin.
- **A11y:** No live region or close label.
- **Tests:** 7 tests cover heights, background, overlay flags, child count, variant, director; no show/hide/dismiss/duration/action press.
- **Evidence:** `headers/guillaume/entities/snackbar.hpp:63,96-122`; `sources/entities/snackbar.cpp:48-63,119-177`; `sources/systems/overlay.cpp:198-217`.
- **Fix:** add visible/show/hide, onDismiss, configurable duration, wire to overlay, make action pressable.

#### `split_button` — No Paper ref / P3

- **Closest Paper:** `Button` + `IconButton`.
- **API:** label/onClick/onMenuOpen only; no disabled, variant, size, leading icon, a11y, or controlled menu.
- **Tokens:** Action is Filled/Medium and chevron Filled/Medium; no separator/gap, and the two halves have mismatched heights (Button label-derived ~45 vs IconButton 40).
- **States:** Only composed Button/IconButton states; no whole-button disabled or chevron expanded state.
- **Layout:** Zero spacing/padding; radius computed from action height only.
- **Tests:** 4 tests manually replace and invoke handlers; wiring/geometry effectively untested.
- **Evidence:** `sources/entities/split_button.cpp:99-107,143-170`.
- **Fix:** equalize part heights, add a separator/gap, expose variant/disabled/a11y.

#### `standard_button_group` — No Paper ref / P3

- **Closest Paper:** `Button`.
- **API:** `labels[]` + `gap`; every child gets an empty click handler, so the group cannot perform actions. No per-button variant/icon/disabled/callback/a11y.
- **Tokens:** Always Outlined/Round/Medium; default gap 8.
- **Layout:** Horizontal layout, no wrap/overflow/equal widths.
- **Tests:** 3 tests cover count/gap/director; no click wiring (empty by construction) or variants.
- **Evidence:** `sources/entities/standard_button_group.cpp:52,86-93,129-141`.
- **Fix:** accept per-button configs/callbacks or existing `Button` entities.

#### `surface` — Missing / P1

- **Paper:** `Surface.tsx`.
- **API:** Paper's `Surface` is public with `elevation` 0–5 (default 1) and `mode` flat/elevated (default elevated). Guillaume has no `Surface` entity or builder; only `SurfaceConfig` and the CRTP `SurfaceBase` (`headers/guillaume/entities/surface.hpp:57-76,180-269`).
- **Tokens:** Paper maps elevation 0–5 to `colors.elevation.level0..5` and a two-layer shadow table. Guillaume has no elevation→background mapping; shadow geometry is configurable via `components::Elevation`.
- **States:** No flat/elevated state or animated elevation.
- **Tests:** No `test_surface.cpp`.
- **Evidence:** `headers/guillaume/entities/surface.hpp:57-76,180-269`; `templates/guillaume/entities/surface.tpp:167-205`; Paper `Surface.tsx:19,44,268,273,291-302`.
- **Fix:** add a real `Surface` entity with `elevation`/`mode` and resolve its background from elevation tokens.

#### `switch` — Partial / P1

- **Paper:** `Switch/Switch.tsx`, `utils.ts`.
- **API:** No `onValueChange`, color override, or label/a11y integration. Paper delegates geometry to the native switch; Guillaume's 52×32 track/24 thumb matches M3.
- **Tokens:** Checked `Primary`/`OnPrimary` matches; off track `SurfaceVariant`/`Outline` vs Paper's off tint; disabled 0.12/0.38 matches.
- **States:** Disabled switches still toggle — release handlers are bound unconditionally and `toggle()` has no disabled guard (`switch.cpp:291-294,337-347`). No hover/press layer.
- **Layout:** Track geometry is correct; the label is a child of the fixed 52 dp surface, so it overlaps.
- **A11y:** No switch role/checked state/label.
- **Tests:** 8 tests cover geometry, value mapping, layers, layout-child isolation, variant, label, director; no interaction/disabled toggle.
- **Evidence:** `sources/entities/switch.cpp:50-53,200-211,291-294,337-347`.
- **Fix:** guard toggle/handlers when disabled and expose an onValueChange-style callback.

#### `tabs` — No Paper ref / P1

- **Closest Paper:** `BottomNavigation` selected indicator/tab semantics, `SegmentedButtons` checked roles.
- **API:** Labels/selected index/select; no icons, badges, scrollable/fixed mode, disabled tabs, per-tab callback, or content panel.
- **Tokens:** Primary/Secondary variants; selected label uses `Primary`/`Secondary`, but M3 secondary tabs use `onSurface` for the label and `secondary` only for the indicator. The indicator itself is never drawn: `setIndicatorColor` writes `Borders` color (`tabs.cpp:166-175`), which no renderer reads, and no `Line` entity exists.
- **States:** Exclusive selection works; no hover/focus/pressed/disabled; no scroll-into-view.
- **Layout:** 48-high tabs with padding 16, no indicator strip/baseline, no min/max width.
- **A11y:** No tab/tablist role or selected state.
- **Tests:** 6 tests cover child count, bounds, index, exclusivity; no color/indicator.
- **Evidence:** `sources/entities/tabs.cpp:49,84-91,166-175,341-361`; `sources/systems/rectangle_render.cpp:262-330`.
- **Fix:** emit a real 3 dp active indicator (`components::Line`) and use `onSurface` for the secondary selected label.

#### `text` — Diverges / P0

- **Paper:** `Typography/Text.tsx`, `tokens.tsx`.
- **API:** content/fontSize/color only; no variant (15 M3 roles), weight, line height, letter spacing, family, alignment, or max-font-size/numberOfLines semantics.
- **Defaults:** Guillaume default is 24 px and opaque white (`sources/entities/text.cpp:48-49`); Paper default is `brandRegular` 400 with `onSurface`. There is no typescale in `Theme`.
- **Tokens:** No role mapping; `TextRender` hardcodes Roboto Regular (`sources/systems/text_render.cpp:37,74-83`). Only one font asset ships.
- **A11y:** No text semantics.
- **Tests:** 4 tests cover measure/render, setters, director; no typography role/default color.
- **Evidence:** `headers/guillaume/entities/text.hpp:101,138`; `sources/entities/text.cpp:45-50`; Paper `Text.tsx:18-39,97-169`, `tokens.tsx:123-219`.
- **Fix:** add M3 typescale to `Theme`, map `Text` to a role, and default to `OnSurface`.

#### `text_field` — Partial / P0

- **Paper:** `TextInput/*`, `HelperText/*`.
- **API:** Variant/placeholder/leading/trailing/supporting/error/text; missing disabled, editable, dense, multiline, prefix/suffix affixes, icon color/press, onChange/focus/blur, and runtime error text. The component supports `prefix`/`suffix`/`errorText`/`labelFloating`, but the entity never wires them.
- **Defaults:** Filled default maps to Paper `flat`. Height 56 matches. Width is hard-fixed to 280 vs Paper's intrinsic min 100.
- **Tokens:** Filled background `SurfaceVariant` matches; outlined radius 4 matches. **The filled underline is invisible** because `Line` thickness is set but its color is never set; `LineRender` paints with the entity fill. **The outlined border is never rendered** because `RectangleRender` ignores `Borders::color` (CC-02). No focused/active color, no floating label, no placeholder suppression, no error wiggle.
- **Layout:** Horizontal row with 16 padding/8 spacing; supporting text is appended into the same row (`text_field.cpp:263-287`) instead of below the field.
- **A11y:** No label association, error announcement, or icon button semantics.
- **Tests:** 8 tests cover 280×56 geometry, line thickness, border alpha (`test_text_field.cpp:98-100` false positive), text/error round-trip; no disabled/dense/focus/floating label.
- **Evidence:** `headers/guillaume/entities/text_field.hpp:116-158,247-303`; `sources/entities/text_field.cpp:64-72,182-215,217-288`; `sources/systems/line_render.cpp:60-78`; `sources/systems/rectangle_render.cpp:262-330`.
- **Fix:** render the underline/border from a real stroke, implement the floating label and disabled state, and wire affixes.

#### `time_picker` — No Paper ref / P2

- **Closest Paper:** none; M3 time picker spec applies.
- **API:** Input/Clock, hour/minute, callback, getValue, marker count; no AM/PM, 24-hour toggle, validation, mode toggle, or confirm/cancel.
- **Tokens:** 328 wide/radius 28/`SurfaceContainerHigh` matches. Input mode renders "HH : MM" as three 45 px texts; clock mode renders 12 numbers at 18 px with no selected hour or clock hand.
- **Layout:** Markers on a circle; no 256 dp dial, center dot, or 96×80 field boxes.
- **A11y:** None.
- **Tests:** 7 tests cover geometry, z-order, modal overlay, time/value round-trip, 12 markers, director; no input rendering/AM-PM/selection.
- **Evidence:** `headers/guillaume/entities/time_picker.hpp:68-79`; `sources/entities/time_picker.cpp:50-53,77-92,152-163,230-315`.
- **Fix:** add M3 field boxes with AM/PM + 24 h state and selected-hour/clock-hand visuals.

#### `toolbar` — No Paper ref / P3

- **Closest Paper:** `Appbar`.
- **API:** Generic children/spacing/padding/fixed size, Docked/Floating; no height preset, slot model, label, or a11y.
- **Tokens:** Docked `SurfaceContainer`/radius 0; Floating `SurfaceContainerHigh`/radius 28 but no elevation.
- **Layout:** Default spacing 8/padding 16, content-driven height; Paper app bar is fixed 64 with 4 dp horizontal padding.
- **Tests:** 6 tests cover radii, variant, child attachment, color, director; no height/padding/elevation/a11y.
- **Evidence:** `sources/entities/toolbar.cpp:52-56,127-144`.
- **Fix:** add a 64 dp height preset, floating elevation, and a slot/a11y model.

#### `tooltip` — Partial / P1

- **Paper:** `Tooltip/Tooltip.tsx`, `utils.ts`.
- **API:** Variant/text/title/fixedWidth; no trigger/anchor child, show/hide/visible, delay props, or `setTitle`.
- **Variants:** Plain/Rich; Rich is a Guillaume/M3 extension. Defaults: 32 high/radius 4 match; padding 8 uniform vs Paper 16 horizontal.
- **Tokens:** Background `InverseSurface`, but text uses `OnSurface` (`tooltip.cpp:167-179`) instead of `InverseOnSurface` — dark-on-dark.
- **States:** Overlay is configured non-modal/no outside dismiss/escape dismiss, but it is never shown. No trigger, hover, focus, or delays.
- **Layout:** No anchor placement or overflow clamping.
- **A11y:** No live region.
- **Tests:** 6 tests cover radius, background, overlay flags, variant, director; no show/hide/delays/placement.
- **Evidence:** `headers/guillaume/entities/tooltip.hpp:63,103-129`; `sources/entities/tooltip.cpp:124-179`; Paper `Tooltip.tsx:20-45,118-196,207-256`.
- **Fix:** add trigger/anchor placement with enter/leave delays and use `InverseOnSurface`.

---

## Paper-only gap list

These Paper `v5.15.4` components have no Guillaume entity. They are not part of the 41-family parity count, but they are the remaining coverage gap.

| Paper component | Guillaume base / recommended composition |
|---|---|
| `Avatar` (`AvatarImage`, `AvatarIcon`, `AvatarText`) | `entities::Image` + `entities::Icon` + `entities::Text` in a circular `SurfaceBase` with `components::Clip`/`Borders`. |
| `Banner` | `SurfaceBase<components::Overlay>` with inline (non-modal) overlay + two action entities; reuse the Snackbar pattern. |
| `DataTable` (`DataTable`, `DataTableRow`, `DataTableCell`, `DataTableTitle`, `DataTableHeader`) | Nested `entities::Layout` rows/cells + `entities::Divider`; no grid primitive exists. |
| `HelperText` | `entities::Text` with an error/info variant; lift the existing TextField supporting-text path. |
| `ToggleButton` (`ToggleButton`, `ToggleButtonGroup`, `ToggleButtonRow`) | `entities::SegmentedButton`/`Button` with toggle + `components::Selectable`/`SelectionGroup`. |
| `Portal` + `Modal` | `components::Overlay` + `components::Scrim` + `systems::Overlay`; today only `BottomSheet` creates a scrim. Extract a reusable `Portal`/`Modal` entity. |
| `TouchableRipple` | `components::MouseButtonInteraction` + hover + focus + the `applyStyleState()` pattern from `button_base.cpp:305-352`; extract a `components::StateLayer`/`systems::StateLayer`. |
| `CrossFadeIcon` / `MaterialCommunityIcon` | `entities::Icon` + `components::Glyph` + `components::Animation` (alpha/color lerp). |

---

## Prioritized remediation backlog

> Tracking: GitHub milestone **#4 — [M3 parity remediation (Paper v5.15.4)](https://github.com/etib-corp/guillaume/milestone/4)**. The 10 workstream issues are #104–#113.

### P0 — fix before other parity work

1. **Populate the 20 missing roles in default Light/Dark schemes** (`sources/theme.cpp:270-365,678-773`); add `SurfaceDisabled`, `OnSurfaceDisabled`, `Backdrop`. Add `test_theme.cpp` covering all roles.
2. **Fix border/underline rendering** (`sources/systems/rectangle_render.cpp:262-330`, `components/borders.hpp`, `components/line_render.cpp`). This unblocks outlined cards, outlined text fields, tab indicators, and every outlined/divider visual.
3. **Add `theme::Typescale`** and map `Text`/`Icon` to roles; ship Roboto Medium; replace the 33 raw `buildText` sizes and the 24 px/white defaults.

### P1 — systemic parity

4. **State-layer tokens + `components::StyleState` + `systems::StyleState`**; replace `20U`/`31U`/`97U` literals and add hover/press/focus/dragged to Chip, List, Menu, Drawer, Nav, Tabs, Card.
5. **Shape scale** (`roundness`) and replace per-entity radii.
6. **Accessibility component** (role/label/hint/state/value/focus order) and make `ButtonBase` feed it.
7. **Unify disabled semantics** and make interaction/focus skip disabled entities; fix `Switch` toggling while disabled.
8. **Elevation scale + tonal overlay**; add `Elevation` to FAB, Menu, Dialog, Snackbar, Tooltip.
9. **Motion scale**; replace 0.3/1.5/4.0/0.1 s literals and add emphasized decelerate/accelerate.
10. **Focus traversal** (Tab/Shift-Tab, order, ring) and register the focus system in the composition root.
11. **Family P1 fixes**, in this order: `text_field` (also P0), `button`, `icon_button`, `chip`, `checkbox`/`radio_button`/`switch`, `app_bar`, `list`, `menu`, `navigation_bar`/`drawer`, `tabs`, `dialog`, `card`, `surface`, `snackbar`, `tooltip`.

### P2 — secondary behavior and composition

12. `extended_floating_action_button`, `segmented_button`, `navigation_rail`, `bottom_sheet`, `side_sheet` geometry/motion.
13. `badge`, `loading_indicator`, `progress_indicator` visibility/animation.
14. `carousel` snapping/hero variants, `slider` disabled/state layers/range thumbs, `date_picker`/`time_picker` selected states and navigation.
15. `icon` default color/style rendering; `image` size/fit/load fallback.
16. Touch targets to 48 dp; hover accessibility margins.
17. Palette/scheme unification; theme-change invalidation (`themeRevision` + `ThemeSync`).

### P3 — cleanup

18. `divider` hairline default / `bold` flag.
19. `split_button`/`standard_button_group`/`toolbar` composition and a11y.
20. `layout` flexibility (wrap/per-edge padding) only if required.
21. `model` load/error state; `image` non-M3 assets.
22. Convert `ThemeSchemeRole`/`ThemePaletteRole` to `enum class`; remove or wire dead `SurfaceTint`/`Background` roles; align `Scrim` default with `Backdrop`.

---

## Test coverage map

41 entity test files, **280 `TEST_F` cases**. Coverage is strong on geometry, child counts, z-order, round-trips, and directors. It is weak on defaults, colors, states, accessibility, and foundations.

| Area | Existing coverage | Missing |
|---|---|---|
| Theme/tokens | None | No `test_theme.cpp`; role completeness, typescale, shape, elevation, state alphas, palette consistency |
| Surface | None | No `test_surface.cpp`; elevation/mode/background |
| Borders | Implicit in entity tests | No render test; tests assert `Borders::getColor()` alpha even though it is unused (`test_text_field.cpp:98-100`, `test_card.cpp`) |
| State layers | Button alpha `31` (`test_button.cpp:310`) | Hover/press colors, disabled visual states, dragged, focus |
| Accessibility | Label round-trip on Button | Role/hint/state/focus order; none on other families |
| Focus | Handler wiring (`tests/sources/components/test_focus.cpp`, `tests/sources/systems/test_focus.cpp`) | Keyboard traversal, focus order, disabled exclusion, focus ring |
| Default variants | Mostly not asserted | Button default Filled, Badge default Dot, Loading default Linear, Search FullScreen |
| Transitions/motion | Indeterminate loop, some show/hide flags | Snackbar show/dismiss/duration, Tooltip delays, modal transitions, FAB visibility, badge visibility |
| Composition | Child counts | Card Content/Actions/Cover/Title, List Section/Accordion, TextInput affixes, Dialog actions row |
| Token drift | Tests hardcode production magic numbers | Token-based assertions; see `test_button.cpp:310`, `test_chip.cpp:64,68`, `test_switch.cpp:65,66,70`, `test_app_bar.cpp:69,70,91-93`, `test_date_picker.cpp:72,76`, `test_list.cpp:79,92,133` |

Per-family test counts:

| Family | Tests | Family | Tests |
|---|---:|---|---:|
| `app_bar` | 8 | `navigation_bar` | 6 |
| `badge` | 6 | `navigation_drawer` | 7 |
| `bottom_sheet` | 7 | `navigation_rail` | 6 |
| `button` | 15 | `progress_indicator` | 8 |
| `card` | 6 | `radio_button` | 7 |
| `carousel` | 6 | `search` | 7 |
| `checkbox` | 6 | `segmented_button` | 8 |
| `chip` | 6 | `side_sheet` | 7 |
| `date_picker` | 7 | `slider` | 10 |
| `dialog` | 7 | `snackbar` | 7 |
| `divider` | 6 | `split_button` | 4 |
| `extended_floating_action_button` | 3 | `standard_button_group` | 3 |
| `floating_action_button` | 5 | `switch` | 8 |
| `floating_action_button_menu` | 4 | `tabs` | 6 |
| `icon` | 4 | `text` | 4 |
| `icon_button` | 7 | `text_field` | 8 |
| `image` | 6 | `time_picker` | 7 |
| `layout` | 18 | `toolbar` | 6 |
| `list` | 6 | `tooltip` | 6 |
| `loading_indicator` | 8 | `model` | 7 |
| `menu` | 7 | | |

---

## Appendix A — Mapping table

| Guillaume | Paper | Notes |
|---|---|---|
| `AppBar` | `Appbar` + subcomponents | Family mapping |
| `Badge` | `Badge` | |
| `BottomSheet` | `Modal` (closest) | No direct Paper component |
| `Button` | `Button` | |
| `Card` | `Card` + Content/Title/Actions/Cover | |
| `Carousel` | — | M3 carousel spec |
| `Checkbox` | `Checkbox` + `Checkbox.Item` | |
| `Chip` | `Chip` | |
| `DatePicker` | — | M3 date picker spec |
| `Dialog` | `Dialog` + `Modal` | |
| `Divider` | `Divider` | |
| `ExtendedFloatingActionButton` | `FAB` extended | |
| `FloatingActionButton` | `FAB` | |
| `FloatingActionButtonMenu` | `FAB.Group` | |
| `Icon` | `Icon`/`MaterialCommunityIcon`/`CrossFadeIcon` | |
| `IconButton` | `IconButton` | |
| `Image` | `Avatar.Image` (closest) | Not a Paper primitive |
| `Layout` | — | Flexbox analogue |
| `List` | `List` + Item/Accordion/Section/Subheader/Icon/Image | |
| `LoadingIndicator` | `ActivityIndicator` | |
| `Menu` | `Menu` + `MenuItem` | |
| `Model` | — | 3D primitive; no M3 relevance |
| `NavigationBar` | `BottomNavigation` | |
| `NavigationDrawer` | `Drawer` + Item/Section/CollapsedItem | |
| `NavigationRail` | — (closest `BottomNavigation`/`Drawer`) | M3 rail spec |
| `ProgressIndicator` | `ProgressBar` | Paper is linear-only |
| `RadioButton` | `RadioButton` + Group/Item | |
| `Search` | `Searchbar` | |
| `SegmentedButton` | `SegmentedButtons` | |
| `SideSheet` | `Modal` (closest) | No direct Paper component |
| `Slider` | — | M3 slider spec |
| `Snackbar` | `Snackbar` | |
| `SplitButton` | `Button` + `IconButton` (closest) | No Paper component |
| `StandardButtonGroup` | `Button` (closest) | No Paper component |
| `Surface` | `Surface` | Guillaume has only base/config |
| `Switch` | `Switch` | |
| `Tabs` | — (closest `BottomNavigation`/`SegmentedButtons`) | M3 tabs spec |
| `Text` | `Typography/Text` | |
| `TextField` | `TextInput` + `HelperText` | |
| `TimePicker` | — | M3 time picker spec |
| `Toolbar` | `Appbar` (closest) | No Paper component |
| `Tooltip` | `Tooltip` | |

## Appendix B — RN-only N/A rules

The following Paper concepts were deliberately excluded from parity scoring because they are React Native/platform concerns with no direct renderer-agnostic equivalent in Guillaume:

- `testID`, `ref`, `touchableRef`, `nativeID`
- `hitSlop` and ripple/`background` configuration
- `style`, `contentStyle`, `labelStyle`, `outlineStyle`, `underlineStyle`, `fillStyle`
- `theme` prop overrides (Guillaume uses `setActiveScheme`)
- `aria-*` prop naming (semantic equivalents are covered under accessibility)
- `keyboardShouldPersistTaps`, `statusBarHeight`, `safeAreaInsets`
- Android/iOS/web-specific props (`numberOfLines` on Android, `selectionColor`/`cursorColor` platform differences, native switch props, `Animated.Value`)
- `accessible`, `accessibilityViewIsModal`, `accessibilityLiveRegion` are treated as semantic requirements only where Guillaume has an equivalent backend; today it does not, so they are listed as gaps under accessibility rather than silently marked N/A.

## Appendix C — Audit limitations

- Static source comparison only; no runtime screenshots, Paper test execution, or Guillaume render capture.
- Paper's React Native layout is flexbox-based; Guillaume's `systems::Layout` is a single-axis arranger. Some Paper geometry is content-driven and can only be approximated from constants.
- Paper `v6.0.0-alpha.1` (main) was intentionally not used: it removes `HelperText`/`ToggleButton` and restructures tokens. Findings are pinned to `v5.15.4`.
- The 10 families marked "No Paper ref" are not parity failures by themselves; they are gaps against the M3 specification or against the closest Paper primitive.
- Severity is about remediation order, not about whether a component is usable today.
