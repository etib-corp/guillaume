# Systems: What They Do and What Components They Need**

## **A. Core UI Systems**

| System | Required Components | Action |
| --- | --- | --- |
| **RenderSystem** | `Transform`, `Sprite`/`Text`/`Rectangle` | Renders all visible entities on screen, respecting opacity, borders, and hierarchy. |
| **InputSystem** | `Clickable`, `Hoverable`, `Draggable` | Detects mouse/keyboard input and updates interaction states (e.g., `is_clicked`). |
| **LayoutSystem** | `Transform`, `Layout`, `Anchor`, `Parent` | Calculates positions/sizes for all entities based on layout rules and anchors. |
| **StateSystem** | `Visibility`, `Enabled`, `Selected` | Updates dynamic states (e.g., hides disabled entities, highlights selected ones). |

---

## **B. Interaction Systems**

| System | Required Components | Action |
| --- | --- | --- |
| **ButtonSystem** | `Button`, `Clickable`, `Hoverable` | Triggers `on_click` and `on_hover` callbacks, updates visual feedback. |
| **SliderSystem** | `Slider`, `Draggable`, `Transform` | Updates `current_value` based on drag input, calls `on_change`. |
| **InputFieldSystem** | `InputField`, `Text`, `Focusable` | Manages text input, cursor blinking, and submission. |
| **DropdownSystem** | `Dropdown`, `Clickable`, `Scrollable` | Expands/collapses dropdown, handles option selection and scrolling. |
| **TooltipSystem** | `Tooltip`, `Hoverable`, `Transform` | Shows/hides tooltips after a delay when hovering over an entity. |

---

## **C. Animation Systems**

| System | Required Components | Action |
| --- | --- | --- |
| **AnimationSystem** | `Animator`, `Sprite`/`Transform` | Advances animation frames, updates `current_frame` and applies transformations. |
| **TweenSystem** | `Tween`, `Transform`/`Opacity` | Interpolates values (e.g., position, alpha) over time using easing functions. |

---

## **D. Utility Systems**

| System | Required Components | Action |
| --- | --- | --- |
| **HierarchySystem** | `Parent`, `Children`, `Transform` | Propagates transforms from parents to children, maintains hierarchy. |
| **ScrollSystem** | `Scrollable`, `Transform`, `Children` | Updates `scroll_position` and clips child entities outside the scrollable area. |
| **FocusSystem** | `Focusable`, `Selected` | Manages keyboard focus, handles tab navigation, and triggers focus/blur events. |
| **EventSystem** | `Clickable`, `Hoverable` | Dispatches custom events (e.g., `on_click`, `on_hover`) to other systems or scripts. |

---

## **E. Specialized Systems**

| System | Required Components | Action |
| --- | --- | --- |
| **ProgressBarSystem** | `ProgressBar`, `Transform` | Updates the visual representation of the progress bar based on `current_value`. |
| **DragDropSystem** | `Draggable`, `Transform`, `Parent` | Handles drag-and-drop logic, updates parent-child relationships on drop. |
| **LocalizationSystem** | `Text` (custom localization component) | Replaces `content` with localized strings based on language settings. |
