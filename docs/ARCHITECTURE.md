# Architecture

Guillaume follows an Entity-Component-System (ECS) architecture to separate
state, behavior, and orchestration.

## Core Concepts

- Entity: A uniquely identified object in the scene graph (button, text, icon).
- Component: Data attached to an entity (transform, bounds, text content, etc.).
- System: Logic that processes entities matching a component signature.

This design makes the framework easier to extend and maintain.

## Components

- `Transform`: Position, orientation, and scale.
- `Bounds`: Width and height.
- `Color`: Fill or display color.
- `Border`: Border color and thickness.
- `Text`: Label content and font properties.
- `Click`: Click callback and click-related state.
- `Hover`: Hover callback and hover-related state.
- `Focus`: Focus gain/loss behavior.
- `Icon`: Icon identifier and rendering metadata.

## Systems

- `RectangleRender`: Draws rectangle-based entities.
- `TextRender`: Draws text entities.
- `IconRender`: Draws icon entities.
- `Interaction`: Handles click, hover, and focus events.
- `KeyboardControl`: Handles keyboard interactions.
- `MeasureText`: Computes text bounds and updates `Bounds`.

## Entity Hierarchy

Entities can own child entities. This enables composition patterns such as a
button containing both text and icon children.
