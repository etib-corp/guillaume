# **1. Components: What They Contain**

## **A. Visual Components**

| Component | Fields |
| --- | --- |
| **Transform** | `x: float`, `y: float`, `width: float`, `height: float`, `rotation: float` |
| **Sprite** | `texture_id: string`, `color: RGBA`, `sprite_sheet: bool`, `frame: int` |
| **Text** | `content: string`, `font: string`, `size: float`, `color: RGBA`, `alignment: enum` |
| **Rectangle** | `fill_color: RGBA`, `border_color: RGBA`, `border_thickness: float` |
| **Border** | `color: RGBA`, `thickness: float`, `radius: float` |
| **Opacity** | `alpha: float` (0.0 to 1.0) |

---

## **B. Interaction Components**

| Component | Fields |
| --- | --- |
| **Clickable** | `on_click: function`, `is_clicked: bool`, `click_sound: string` |
| **Hoverable** | `hover_color: RGBA`, `on_hover: function`, `is_hovered: bool` |
| **Draggable** | `is_dragging: bool`, `drag_offset_x: float`, `drag_offset_y: float` |
| **Focusable** | `is_focused: bool`, `on_focus: function`, `on_blur: function` |

---

## **C. Layout Components**

| Component | Fields |
| --- | --- |
| **Layout** | `flex_direction: enum`, `margin: {top, right, bottom, left}`, `padding: float` |
| **Anchor** | `anchor_point: enum` (e.g., top_left, center), `offset_x: float`, `offset_y: float` |
| **Scrollable** | `scroll_position: float`, `content_size: float`, `scroll_speed: float` |

---

## **D. State Components**

| Component | Fields |
| --- | --- |
| **Visibility** | `is_visible: bool` |
| **Enabled** | `is_enabled: bool` |
| **Selected** | `is_selected: bool` |
| **Tooltip** | `tooltip_text: string`, `delay: float` |

---

## **E. Animation Components**

| Component | Fields |
| --- | --- |
| **Animator** | `current_frame: int`, `frames: array`, `speed: float`, `loop: bool` |
| **Tween** | `target_value: float`, `duration: float`, `easing: enum`, `progress: float` |

---

## **F. Functional Components**

| Component | Fields |
| --- | --- |
| **Button** | `label: string`, `on_click: function` |
| **Slider** | `min_value: float`, `max_value: float`, `current_value: float`, `on_change: function` |
| **InputField** | `placeholder: string`, `max_length: int`, `content: string`, `on_submit: function` |
| **ProgressBar** | `min_value: float`, `max_value: float`, `current_value: float`, `color: RGBA` |
| **Dropdown** | `options: array`, `selected_index: int`, `on_select: function` |

---

## **G. Hierarchy Components**

| Component | Fields |
| --- | --- |
| **Parent** | `parent_id: entity_id` |
| **Children** | `child_ids: array<entity_id>` |
