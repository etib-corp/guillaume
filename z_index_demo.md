# Z-Index Rendering Demonstration

The Guillaume GUI framework now supports z-index based rendering for proper depth ordering of components. This feature allows components to be rendered in layers, with higher z-index values appearing on top of lower ones.

## Implementation Details

### Key Changes Made:

1. **Component Class Enhancement**:
   - Added `_z_index` member variable with default value 0
   - Added `set_z_index(int z_index)` and `get_z_index() const` methods
   - Z-index changes mark the component as dirty for re-rendering

2. **Container Class Modification**:
   - Replaced direct vector sorting with index-based sorting to avoid unique_ptr issues
   - Added `_render_order` vector to store sorted indices
   - Added `_render_order_dirty` flag to track when reordering is needed
   - Modified `draw()` to render children in z-index order (lowest to highest)
   - Modified `handle_mouse_event()` to process events in reverse z-index order (highest to lowest)

3. **Main Application Update**:
   - Created three overlapping buttons with different z-index values:
     - Background button: z-index 0 (gray, renders behind others)
     - Middle button: z-index 1 (blue, renders in middle layer)  
     - Top button: z-index 2 (red, renders on top)

## Usage Example

```cpp
// Create buttons with different z-index values
auto background_button = std::make_unique<guigui::Button>(...);
background_button->set_z_index(0);  // Renders behind

auto middle_button = std::make_unique<guigui::Button>(...);
middle_button->set_z_index(1);      // Renders in middle

auto top_button = std::make_unique<guigui::Button>(...);
top_button->set_z_index(2);         // Renders on top

// Add to container (order doesn't matter due to z-index sorting)
container->add_child(std::move(middle_button));    // Added second but z-index 1
container->add_child(std::move(top_button));       // Added third but z-index 2 (top)
container->add_child(std::move(background_button)); // Added first but z-index 0 (back)
```

## Visual Result

When running the application, you'll see three overlapping buttons:
- The red "Top" button appears on top (z-index 2)
- The blue "Middle" button appears in the middle layer (z-index 1)
- The gray "Background" button appears behind the others (z-index 0)

Mouse events are processed correctly with higher z-index components receiving events first, ensuring proper interaction behavior.

## Performance Considerations

- Z-index sorting only occurs when needed (when components are added or z-index values change)
- Uses efficient index-based sorting instead of moving actual components
- Render order is cached and only updated when marked dirty
- No performance impact when z-index values remain static

## Benefits

1. **Proper Visual Layering**: Components can be visually layered without worrying about add order
2. **Correct Event Handling**: Mouse events are processed top-to-bottom for natural interaction
3. **Efficient Rendering**: Automatic sorting ensures correct draw order with minimal overhead
4. **Easy to Use**: Simple API with sensible defaults (z-index 0 for all components)
5. **Dynamic Updates**: Z-index can be changed at runtime to rearrange component layers
