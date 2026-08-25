# Guillaume Architecture

Guillaume is organized around an entity-component-system model so UI state,
presentation, and interaction logic stay separate.

## Main Building Blocks

- Entities represent UI objects such as buttons, labels, panels, and icons.
- Components store data such as transform, bounds, text, color, and borders.
- Systems apply behavior such as rendering, interaction, and text measurement.
- Scenes group entities into screens and let the application switch between
 them.

## Runtime Flow

1. The application create an engine.
2. Scenes populate the UI tree using the storage objects passed in by the app.
3. Systems render and update entities every frame.
4. Input events update component state and drive transitions.

## Design Goal

Keep the UI framework composable enough for complex interfaces while keeping
the public API small and predictable.
