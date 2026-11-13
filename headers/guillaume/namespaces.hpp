/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#pragma once

/**
 * @file
 * @brief Map of the library: what each namespace holds and where to start.
 *
 * This header declares nothing. Read it first, then follow the `@see` links.
 */

/**
 * @namespace guillaume
 * @brief A C++20 UI framework for desktop and XR, built as an
 * entity-component-system.
 *
 * The three ideas to keep in mind:
 * - A **component** is plain state attached to an entity (a size, a color,
 *   a checkbox configuration). See guillaume::components.
 * - A **system** holds the rules. It runs every frame on every entity that
 *   has the components it needs. See guillaume::systems.
 * - An **entity** is an identifier. The widget classes of
 *   guillaume::entities (Button, Checkbox, ...) are the comfortable way to
 *   create one with the right components and children.
 *
 * A program creates an Application with one or more Scene types and an
 * engine, builds widgets in Scene::onEnter, and calls Application::run.
 *
 * @code
 * class HomeScene: public guillaume::Scene
 * {
 * 	public:
 * 	using guillaume::Scene::Scene;
 *
 * 	void onEnter(void) override
 * 	{
 * 		guillaume::entities::Button::Builder builder(
 * 			getComponentRegistry(), getEntityRegistry());
 * 		builder.makeButton(nullptr, "Save", [] {},
 * 			guillaume::entities::Button::Color::Filled,
 * 			guillaume::entities::Button::Shape::Round,
 * 			guillaume::entities::Button::Size::Medium, false);
 * 	}
 * };
 * @endcode
 *
 * @see guillaume::Application, guillaume::Scene
 */

/**
 * @namespace guillaume::ecs
 * @brief The entity-component-system core, independent of any widget.
 *
 * - ecs::Entity: an identifier, a signature (which components it has) and
 *   a place in the tree.
 * - ecs::ComponentRegistry: stores every component, per type, by entity
 *   identifier, and tracks which ones changed this frame.
 * - ecs::EntityRegistry: owns entities and walks the tree.
 * - ecs::System and ecs::SystemFiller: base classes of every system.
 * - ecs::Phase: the order systems run in within a frame (Event, Measure,
 *   Layout, Render).
 *
 * @see guillaume::ecs::System::routine for what happens each frame.
 */

/**
 * @namespace guillaume::components
 * @brief State. Every class here is plain data attached to an entity.
 *
 * Components hold no rule: their setters store a value and mark the
 * component changed, nothing else. Three kinds live here:
 * - **Geometry and appearance** read by the render systems: Transform,
 *   Bound, Color, Borders, Elevation, Text, Glyph, ...
 * - **Interaction state** written by the input systems: Focus, StyleState,
 *   the hover and button interaction components, Selectable.
 * - **Widget configuration**, one per widget, named after it: Checkbox,
 *   Button, Menu, ... Each is read by the system of the same name.
 *
 * components::Hierarchy mirrors the entity tree (parent, children, depth)
 * so systems can walk it from identifiers.
 */

/**
 * @namespace guillaume::systems
 * @brief Rules. Every class here runs each frame on the entities that have
 * the components it declares.
 *
 * A system reads and writes components by entity identifier and never
 * needs an entity object. Systems are grouped by phase:
 * - **Event**: turn input into component state (Focus, hover and button
 *   interaction, Button, Selection).
 * - **Measure**: compute intrinsic sizes (MeasureText, MeasureGlyph,
 *   Typography).
 * - **Layout**: arrange and style. Layout places children; each widget
 *   system (Checkbox, Menu, ...) styles its widget and places what Layout
 *   does not.
 * - **Render**: upload geometry to the engine (the *Render systems).
 *
 * Widget systems expose their rules as static functions taking a registry
 * and an identifier (`systems::Checkbox::toggle(registry, id)`), so they
 * can be called outside the frame loop and tested without entity objects.
 * Shared helpers live in systems::style, systems::interaction and
 * systems::placement.
 *
 * @see guillaume::systems::registerCoreSystems for the registration order.
 */

/**
 * @namespace guillaume::entities
 * @brief Widgets: the classes used to create entities with the right
 * components and children.
 *
 * A widget class holds no state and no rule. Its setters write the widget's
 * component and call the widget's system so the change is visible at once.
 * Create a widget with its Builder (which also registers it in a scene) or
 * with its static `create()`.
 *
 * @code
 * auto checkbox = Checkbox::create(registry, config,
 * 	Checkbox::State::Unchecked, Checkbox::Variant::Enabled, "Accept");
 * checkbox->setOnValueChange([](Checkbox::State state) { ... });
 * @endcode
 *
 * @see guillaume::entities::SurfaceBase, guillaume::entities::ButtonBase
 */

/**
 * @namespace guillaume::event
 * @brief Delivery of engine events to the systems that subscribed to them.
 */

/**
 * @namespace guillaume::theme
 * @brief Design tokens: colors, type scale, shapes, elevation, motion and
 * spacing. Read them through guillaume::getActiveTheme and the helpers of
 * `guillaume/theme.hpp`.
 */
