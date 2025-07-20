/**
 * @file component.hpp
 * @brief Base component class for the Guillaume GUI framework
 * 
 * This file contains the Component class which serves as the base class for all
 * interactive UI elements in the Guillaume framework. It provides core functionality
 * for rendering, event handling, and state management.
 */

#pragma once

#include "guillaume/componentable.hpp"
#include "guillaume/primitive.hpp"
#include "guillaume/renderer.hpp"
#include "guillaume/events/mouse_event.hpp"
#include "guillaume/vector.hpp"
#include "guillaume/rectangle.hpp"
#include "guillaume/logger.hpp"

#include <map>
#include <functional>

namespace guigui {

/**
 * @class Component
 * @brief Base class for all interactive GUI components
 * 
 * The Component class provides the foundation for all interactive UI elements
 * in the Guillaume framework. It handles mouse events, manages visual state,
 * and provides a system for rendering using primitives.
 * 
 * @details Key features:
 * - Mouse event handling and interaction states
 * - Primitive-based rendering system
 * - Callback system for user interactions
 * - Bounds-based hit testing
 * - Visibility and enabled state management
 * 
 * @note This class is designed to be inherited by specific component types
 * like Button, TextBox, etc.
 */
class Component : public Componentable {
public:
    /**
     * @enum InteractionState
     * @brief Defines the possible interaction states for a component
     */
    enum class InteractionState {
        NORMAL,   ///< Default state, no user interaction
        HOVERED,  ///< Mouse cursor is over the component
        PRESSED   ///< Component is being pressed/clicked
    };

    // Callback function types
    using HoverCallback = std::function<void(Component&, bool)>;        ///< Callback for hover events (component, is_entering)
    using ClickCallback = std::function<void(Component&)>;              ///< Callback for click events
    using MouseEventCallback = std::function<void(Component&, const MouseEvent&)>; ///< Callback for raw mouse events

private:
    std::map<std::string, std::unique_ptr<Primitivable>> _primitives; ///< Collection of rendering primitives
    InteractionState _interaction_state = InteractionState::NORMAL;   ///< Current interaction state
    Rectangle _bounds;                                                 ///< Component bounds for interaction detection
    
    // Event callbacks
    HoverCallback _on_hover_change;         ///< Callback for hover state changes
    ClickCallback _on_click;                ///< Callback for click events
    MouseEventCallback _on_mouse_event;     ///< Callback for raw mouse events

protected:
    std::shared_ptr<Renderer> _renderer;    ///< Renderer instance for drawing operations
    bool _is_visible = true;                ///< Whether the component is visible
    bool _is_enabled = true;                ///< Whether the component is enabled for interaction
    ComponentIdentifier _identifier;        ///< Unique identifier for this component
    int _z_index = 0;                       ///< Z-index for depth ordering (higher values render on top)

    /**
     * @brief Adds a rendering primitive to the component
     * 
     * @param name Unique name for the primitive
     * @param primitive The primitive to add
     * 
     * @details Primitives are used to render the visual representation of the component.
     * Each primitive has a unique name and can be updated independently.
     */
    void _add_primitive(std::string name, std::unique_ptr<Primitivable> primitive);
    
    /**
     * @brief Updates an existing primitive or adds a new one
     * 
     * @param name Name of the primitive to update
     * @param primitive The new primitive implementation
     */
    void _update_primitive(std::string name, std::unique_ptr<Primitivable> primitive);
    
    /**
     * @brief Executes all primitives to render the component
     * 
     * Calls the execute method on all registered primitives in order.
     * This should be called during the render phase.
     */
    void _execute_primitives();
    
    // Virtual methods for interaction handling (can be overridden by derived classes)
    
    /**
     * @brief Called when the interaction state changes
     * 
     * @param old_state Previous interaction state
     * @param new_state New interaction state
     * 
     * @details This virtual method allows derived classes to respond to state changes.
     * It's called automatically when the interaction state is modified.
     */
    virtual void _on_interaction_state_changed(InteractionState old_state, InteractionState new_state);
    
    /**
     * @brief Called when the mouse enters the component bounds
     * 
     * Override this method in derived classes to provide custom hover enter behavior.
     */
    virtual void _on_hover_enter();
    
    /**
     * @brief Called when the mouse exits the component bounds
     * 
     * Override this method in derived classes to provide custom hover exit behavior.
     */
    virtual void _on_hover_exit();
    
    /**
     * @brief Called when a mouse button is pressed over the component
     * 
     * @param event The mouse event containing button and position information
     * 
     * Override this method in derived classes to provide custom press behavior.
     */
    virtual void _on_mouse_press(const MouseEvent& event);
    
    /**
     * @brief Called when a mouse button is released over the component
     * 
     * @param event The mouse event containing button and position information
     * 
     * Override this method in derived classes to provide custom release behavior.
     */
    virtual void _on_mouse_release(const MouseEvent& event);
    
    /**
     * @brief Called when a click is detected (press followed by release)
     * 
     * Override this method in derived classes to provide custom click behavior.
     */
    virtual void _on_click_detected();

public:
    /**
     * @brief Default constructor
     * 
     * Creates a component with default bounds (0,0,0,0).
     */
    Component();
    
    /**
     * @brief Constructor with specified bounds
     * 
     * @param bounds Initial bounds for the component
     * 
     * Creates a component with the specified rectangular bounds.
     */
    Component(const Rectangle& bounds);
    
    /**
     * @brief Virtual destructor
     * 
     * Ensures proper cleanup of derived classes.
     */
    virtual ~Component() = default;

    // Componentable interface
    
    /**
     * @brief Sets the renderer for this component
     * 
     * @param renderer Shared pointer to the renderer instance
     * 
     * @details The renderer is used for all drawing operations. This method
     * is typically called by the parent container or context.
     */
    void set_renderer(std::shared_ptr<Renderer> renderer) override;
    
    /**
     * @brief Sets the visibility of the component
     * 
     * @param visible True to make the component visible, false to hide it
     * 
     * @details Invisible components are not rendered and do not receive events.
     */
    void set_visible(bool visible) override;
    
    /**
     * @brief Checks if the component is visible
     * 
     * @return bool True if the component is visible, false otherwise
     */
    bool is_visible() const override;
    
    /**
     * @brief Sets the enabled state of the component
     * 
     * @param enabled True to enable the component, false to disable it
     * 
     * @details Disabled components are rendered but do not respond to user interaction.
     */
    void set_enabled(bool enabled) override;
    
    /**
     * @brief Checks if the component is enabled
     * 
     * @return bool True if the component is enabled, false otherwise
     */
    bool is_enabled() const override;
    
    /**
     * @brief Gets the unique identifier for this component
     * 
     * @return ComponentIdentifier The component's unique identifier
     */
    ComponentIdentifier get_identifier() const override;

    // Interaction system
    
    /**
     * @brief Handles a mouse event
     * 
     * @param event The mouse event to process
     * 
     * @details This method processes mouse events and updates the interaction state
     * accordingly. It also triggers appropriate callbacks and virtual methods.
     */
    virtual void handle_mouse_event(const MouseEvent& event) override;
    
    /**
     * @brief Checks if a point is inside the component's bounds
     * 
     * @param point The point to test
     * @return bool True if the point is inside the component, false otherwise
     * 
     * @details Used for hit testing to determine if mouse events should be
     * processed by this component.
     */
    virtual bool is_point_inside(const Vector& point) const;
    
    // Bounds management
    
    /**
     * @brief Sets the bounds of the component
     * 
     * @param bounds New rectangular bounds for the component
     * 
     * @details The bounds define the component's position and size, and are used
     * for rendering and hit testing.
     */
    void set_bounds(const Rectangle& bounds);
    
    /**
     * @brief Gets the current bounds of the component
     * 
     * @return Rectangle The component's current bounds
     */
    Rectangle get_bounds() const;
    
    // Interaction state
    
    /**
     * @brief Sets the interaction state of the component
     * 
     * @param state New interaction state
     * 
     * @details This method updates the interaction state and triggers the
     * appropriate state change callbacks.
     */
    void set_interaction_state(InteractionState state);
    
    /**
     * @brief Gets the current interaction state
     * 
     * @return InteractionState The current interaction state
     */
    InteractionState get_interaction_state() const;
    
    // Event callbacks
    
    /**
     * @brief Sets the hover callback function
     * 
     * @param callback Function to call when hover state changes
     * 
     * @details The callback receives the component reference and a boolean
     * indicating whether the mouse is entering (true) or exiting (false) the component.
     */
    void set_hover_callback(HoverCallback callback);
    
    /**
     * @brief Sets the click callback function
     * 
     * @param callback Function to call when the component is clicked
     * 
     * @details The callback receives the component reference.
     */
    void set_click_callback(ClickCallback callback);
    
    /**
     * @brief Sets the mouse event callback function
     * 
     * @param callback Function to call for all mouse events
     * 
     * @details The callback receives the component reference and the raw mouse event.
     * This is called for all mouse events, not just clicks.
     */
    void set_mouse_event_callback(MouseEventCallback callback);
    
    // Convenience methods
    
    /**
     * @brief Checks if the component is currently hovered
     * 
     * @return bool True if the component is in the HOVERED state
     */
    bool is_hovered() const;
    
    /**
     * @brief Checks if the component is currently pressed
     * 
     * @return bool True if the component is in the PRESSED state
     */
    bool is_pressed() const;
    
    // Z-index management for depth ordering
    
    /**
     * @brief Sets the z-index of the component
     * 
     * @param z_index Z-index value (higher values render on top)
     * 
     * @details Components with higher z-index values are rendered on top of
     * components with lower z-index values. Default z-index is 0.
     */
    void set_z_index(int z_index);
    
    /**
     * @brief Gets the current z-index of the component
     * 
     * @return int The component's current z-index
     */
    int get_z_index() const;
};

} // namespace guigui