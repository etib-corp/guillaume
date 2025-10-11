# Architecture

The architecture of our UI component library is designed to be modular, extensible, and easy to understand. Below is a detailed breakdown of the main components and their interactions.

## Core Component System

The foundation of our architecture is built around a hierarchical component system:

```mermaid
classDiagram
  class Component {
    <<abstract>>
    +state: State
    +properties: Properties
    +parent: shared_ptr~Component~
    +children: vector~shared_ptr~Component~~
    +primitives: vector~shared_ptr~Primitive~~
    +render() shared_ptr~Component~
    +onEvent(event: Event) void
    +addChild(child: shared_ptr~Component~) void
    +addPrimitive(primitive: shared_ptr~Primitive~) void
    +getPrimitives() vector~shared_ptr~Primitive~~
  }

  class State {
    +data: map~string, any~
    +setState(newState: map~string, any~) void
    +set(key: string, value: T) void
    +get(key: string) T
  }

  class Properties {
    +attributes: map~string, any~
    +get(key: string) T
    +has(key: string) bool
  }

  Component "1" *-- "1" State : "manages"
  Component "1" *-- "1" Properties : "has"
  Component "1" *-- "*" Primitive : "contains"
```

**Component** serves as the abstract base class for all UI elements. It manages local state and properties while defining the contract for rendering and event handling. Each component maintains references to its parent and children, enabling a tree-like structure.

**State** holds the dynamic data of a component. When `setState` is called, it triggers a re-render of the component and its children, ensuring the UI stays synchronized with the data.

**Properties** contains immutable attributes passed to the component, typically from its parent. This separation allows for clear data flow and component reusability.

## Event System

```mermaid
classDiagram
  class Event {
    +type: string
    +target: shared_ptr~Component~
    +data: any
    +getType() string
    +getTarget() shared_ptr~Component~
    +getData() T
    +hasData() bool
  }

  class Component {
    +onEvent(event: Event) void
  }

  Event --> Component : "targets"
```

The **Event** system handles user interactions and other asynchronous operations. Events bubble up through the component hierarchy, allowing for flexible event handling patterns.

## Primitive System

The library includes a primitive system for low-level geometric shapes that can be used for custom rendering:

```mermaid
classDiagram
  class Primitive {
    <<abstract>>
    +Primitive()
    +~Primitive() virtual
  }

  class Polygon {
    -_points: vector~Point~
    +Polygon()
    +Polygon(points: vector~Point~)
    +addPoint(point: Point) void
    +getPoints() vector~Point~
  }

  class Triangle {
    +Triangle()
    +Triangle(p1: Point, p2: Point, p3: Point)
  }

  class Rectangle {
    +Rectangle()
    +Rectangle(center: Point, width: float, height: float, rotation: Point)
    +Rectangle(topLeft: Point, topRight: Point, bottomRight: Point, bottomLeft: Point)
    +getCenter() Point
    +getWidth() float
    +getHeight() float
    +getRotation() Point
  }

  class Text {
    -_content: string
    -_position: Point
    +Text()
    +Text(content: string)
    +Text(content: string, position: Point)
    +getContent() string
    +setContent(content: string) void
    +getPosition() Point
    +setPosition(position: Point) void
  }

  Primitive <|-- Polygon
  Primitive <|-- Text
  Polygon <|-- Triangle
  Polygon <|-- Rectangle
```

**Primitive** serves as the abstract base class for all drawing primitives. It provides a common interface for geometric shapes that can be rendered by the Renderer.

**Polygon** is a fundamental primitive that represents a shape defined by a series of points (vertices). It stores the vertices and provides methods to manipulate them.

**Triangle** is a specialized polygon with exactly three vertices, useful for mesh-based rendering and geometric operations.

**Rectangle** is a specialized polygon that can be defined by:

- a center point, width, height, and 3D rotation (Euler angles),
- or by four corner points for full 3D control.

It automatically computes the four vertices in 3D space, supporting arbitrary orientation and depth.

**Text** is a specialized primitive that represents text content at a specific position. This allows components like Labels and Buttons to generate text primitives for rendering instead of handling text rendering directly. Text primitives contain the content string and position information.

## Concrete Components

```mermaid
classDiagram
  class Component {
    <<abstract>>
  }

  class Container {
    +children: vector~shared_ptr~Component~~
    +render() shared_ptr~Component~
    +addChild(child: shared_ptr~Component~) void
    +onEvent(event: Event) void
  }

  class Button {
    +label: string
    +onClick: function~void()~
    +render() shared_ptr~Component~
    +onEvent(event: Event) void
    +setOnClick(callback: function) void
    +getLabel() string
    +setLabel(label: string) void
  }

  class Label {
    +text: string
    +render() shared_ptr~Component~
    +onEvent(event: Event) void
    +getText() string
    +setText(text: string) void
  }

  Component <|-- Container
  Component <|-- Button
  Component <|-- Label
  Container "1" *-- "*" Component : "contains"
```

**Container** is a specialized component for grouping other components. It manages the hierarchy and handles recursive rendering of its children. Containers typically don't generate primitives themselves but orchestrate their children.

**Button** represents an interactive element with click handling capabilities. During rendering, it generates both Rectangle primitives (for the button background) and Text primitives (for the button label), demonstrating how components can produce multiple primitives.

**Label** is a simple text display component that generates Text primitives during rendering. It typically doesn't handle user interactions but can be styled and positioned within the layout.

## Application and Rendering

```mermaid
classDiagram
  class Application {
    +root: shared_ptr~Container~
    +renderer: unique_ptr~Renderer~
    +run() void
    +update() void
    # drawTree(component: shared_ptr~Component~) void
    +getRenderer() Renderer*
    +getRoot() shared_ptr~Container~
    +setRoot(root: shared_ptr~Container~) void
  }

  class Renderer {
    <<abstract>>
    +~Renderer() virtual
    +initialize() void
    +shutdown() void
    +clear() void
    +present() void
    +draw(primitive: shared_ptr~Primitive~) void
    +drawText(text: shared_ptr~Text~) void
    +drawRectangle(rectangle: shared_ptr~Rectangle~) void
    +drawTriangle(triangle: shared_ptr~Triangle~) void
    +drawPolygon(polygon: shared_ptr~Polygon~) void
  }

  class Container {
  }

  Application "1" -- "1" Container : "root"
  Application "1" *-- "1" Renderer : "owns"
  Renderer --> Primitive : "renders"
```

**Application** serves as the entry point and manages the global lifecycle. It owns the root container and the renderer, coordinating updates and the rendering pipeline. The rendering pipeline is a two-phase process:

1. **Virtual render**: `Component::render()` recursively calls `render()` on children to compute/update component state and generate primitives for the current frame.
2. **Physical draw**: `Application::drawTree()` traverses the component hierarchy and calls `Renderer::draw(primitive)` on each primitive to emit the actual output.

**Renderer** is an abstract class responsible for drawing primitives to the screen. This abstraction allows the library to support different rendering backends (terminal, GUI frameworks, web canvas, etc.) without changing the component logic. The renderer only deals with primitives, never components directly.

The Renderer provides both a generic `draw(primitive)` method and specific methods for each primitive type (`drawText`, `drawRectangle`, `drawTriangle`, `drawPolygon`). The generic method automatically dispatches to the appropriate specific method based on the primitive type, eliminating manual type checking in derived renderers.

### Lifecycle and Flow

The typical lifecycle involves initial run followed by updates triggered by events or state changes. The high-level sequence is:

```mermaid
sequenceDiagram
  participant App as Application
  participant Root as Root Container
  participant C as Components
  participant R as Renderer

  App->>Root: render()
  loop render subtree
    Root->>C: render()
    C-->>Root: shared_ptr<Component>
  end
  App->>App: drawTree(root)
  loop traverse hierarchy
    loop for each primitive in component
      App->>R: draw(primitive)
    end
  end

  Note over App,Root: On update()
  App->>Root: render()
  App->>App: drawTree(root)
```

This split allows you to compute layout/state separately from the concrete drawing backend.

## Architecture Benefits

- **Modularity**: Each component is self-contained with clear responsibilities
- **Extensibility**: New components can be easily added by inheriting from the base Component class
- **Separation of Concerns**: State, properties, and rendering are handled separately
- **Primitive-Based Rendering**: Components generate primitives during render, and renderers only handle primitives. This creates a clean separation where components manage logic/state while primitives handle visual representation
- **Flexible Rendering**: The abstract Renderer allows for multiple output targets, with a clear separation between virtual render (state/structure) and physical draw (output)
- **Type-Safe Rendering**: Specific draw methods for each primitive type eliminate manual type checking and provide compile-time safety
- **Event-Driven**: Reactive updates through the event and state system
- **Composable Graphics**: Components can generate multiple primitives for complex visuals (e.g., Button creates both Rectangle and Text primitives)
- **Extensible Primitive System**: Low-level primitive system enables custom rendering and advanced graphics capabilities

## Notes on State and Updates

- Setting state via `State::set` or `setState` can be used to trigger re-renders when integrated with an application-level update strategy. In the current setup, components update their internal values during `render()`, and the application explicitly calls `update()` to re-run render and draw.
- Event handlers (e.g., `Button::onEvent` for "click") can mutate component state or other components (e.g., `Label::setText`), after which `Application::update()` should be invoked to redraw.

## Primitive Generation

Components generate primitives during their `render()` method:

- **Label**: Creates a single Text primitive with its content
- **Button**: Creates a Rectangle primitive (background) and Text primitive (label)
- **Container**: Typically creates no primitives, purely structural

The `render()` method should clear existing primitives and regenerate them based on current state, ensuring the visual representation stays synchronized with component data.
