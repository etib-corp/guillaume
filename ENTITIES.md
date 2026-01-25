# **Basic UI Entities (Components Combinations)**

## **1. Button**

- **Components**:
  - `Transform` (position, size)
  - `Sprite` (background image/color)
  - `Text` (label)
  - `Clickable` (on_click callback)
  - `Hoverable` (hover effects)
  - `Border` (optional styling)
  - `Animator` (optional for animations)

- **Systems Used**:
  - `RenderSystem`, `InputSystem`, `ButtonSystem`, `AnimationSystem`

---

## **2. Label/Text**

- **Components**:
  - `Transform` (position, size)
  - `Text` (content, font, color)
  - `Opacity` (optional transparency)

- **Systems Used**:
  - `RenderSystem`, `LayoutSystem`

---

## **3. Input Field**

- **Components**:
  - `Transform` (position, size)
  - `Text` (displayed text, placeholder)
  - `InputField` (max_length, on_submit)
  - `Focusable` (keyboard input)
  - `Border` (optional styling)

- **Systems Used**:
  - `RenderSystem`, `InputFieldSystem`, `FocusSystem`

---

## **4. Slider**

- **Components**:
  - `Transform` (position, size)
  - `Slider` (min/max/current value, on_change)
  - `Draggable` (user interaction)
  - `Sprite` (track and thumb visuals)

- **Systems Used**:
  - `RenderSystem`, `SliderSystem`, `InputSystem`

---

## **5. Checkbox**

- **Components**:
  - `Transform` (position, size)
  - `Sprite` (unchecked/checked states)
  - `Clickable` (toggle state)
  - `Selected` (is_checked)

- **Systems Used**:
  - `RenderSystem`, `InputSystem`, `StateSystem`

---

## **6. Dropdown Menu**

- **Components**:
  - `Transform` (position, size)
  - `Dropdown` (options, selected_index)
  - `Clickable` (expand/collapse)
  - `Scrollable` (if options exceed height)
  - `Children` (list of option entities)

- **Systems Used**:
  - `RenderSystem`, `DropdownSystem`, `InputSystem`, `ScrollSystem`

---

## **7. Progress Bar**

- **Components**:
  - `Transform` (position, size)
  - `ProgressBar` (min/max/current value)
  - `Rectangle` (background and fill)

- **Systems Used**:
  - `RenderSystem`, `ProgressBarSystem`

---

## **8. Panel/Window**

- **Components**:
  - `Transform` (position, size)
  - `Rectangle` (background)
  - `Border` (optional styling)
  - `Children` (child entities)
  - `Draggable` (optional for movable windows)

- **Systems Used**:
  - `RenderSystem`, `LayoutSystem`, `HierarchySystem`, `DragDropSystem`

---

## **9. Tooltip**

- **Components**:
  - `Transform` (position, size)
  - `Text` (tooltip content)
  - `Tooltip` (delay, target entity)
  - `Visibility` (shown/hidden)

- **Systems Used**:
  - `RenderSystem`, `TooltipSystem`, `StateSystem`

---

## **10. Scrollable List**

- **Components**:
  - `Transform` (position, size)
  - `Scrollable` (scroll_position, content_size)
  - `Children` (list items)
  - `Layout` (arrangement of items)

- **Systems Used**:
  - `RenderSystem`, `ScrollSystem`, `LayoutSystem`

---

## **11. Toggle Switch**

- **Components**:
  - `Transform` (position, size)
  - `Sprite` (on/off states)
  - `Clickable` (toggle state)
  - `Selected` (is_on)

- **Systems Used**:
  - `RenderSystem`, `InputSystem`, `StateSystem`

---

## **12. Image/Icon**

- **Components**:
  - `Transform` (position, size)
  - `Sprite` (image texture)
  - `Opacity` (optional transparency)

- **Systems Used**:
  - `RenderSystem`

---

## **13. Modal Dialog**

- **Components**:
  - `Transform` (position, size)
  - `Rectangle` (background)
  - `Children` (buttons, text, etc.)
  - `Visibility` (shown/hidden)

- **Systems Used**:
  - `RenderSystem`, `LayoutSystem`, `HierarchySystem`, `InputSystem`

---

## **14. Tab Bar**

- **Components**:
  - `Transform` (position, size)
  - `Children` (tab buttons and content panels)
  - `Selected` (active tab)
  - `Clickable` (tab switching)

- **Systems Used**:
  - `RenderSystem`, `InputSystem`, `StateSystem`, `LayoutSystem`

---

## **15. Alert/Notification**

- **Components**:
  - `Transform` (position, size)
  - `Text` (message)
  - `Rectangle` (background)
  - `Animator` (fade-in/out)
  - `Visibility` (shown/hidden)

- **Systems Used**:
  - `RenderSystem`, `AnimationSystem`, `StateSystem`
