/**
 * @file context.hpp
 * @brief Main GUI context class for the Guillaume framework
 * 
 * This file contains the Context class which serves as the main entry point
 * for the Guillaume GUI framework. It manages the rendering loop, event handling,
 * and component management.
 */

#pragma once

#include <iostream>
#include <memory>

#include "guillaume/renderer.hpp"
#include "guillaume/logger.hpp"

#include "guillaume/event.hpp"
#include "guillaume/eventable.hpp"

#include "guillaume/events/keyboard_event.hpp"
#include "guillaume/events/mouse_event.hpp"
#include "guillaume/events/quit_event.hpp"
#include "guillaume/events/unknown_event.hpp"

#include "guillaume/color.hpp"
#include "guillaume/font.hpp"
#include "guillaume/rectangle.hpp"
#include "guillaume/vector.hpp"

#include "guillaume/primitives/rectangle_primitive.hpp"
#include "guillaume/primitives/text_primitive.hpp"

#include "guillaume/component.hpp"
#include "guillaume/componentable.hpp"
#include "guillaume/container.hpp"

namespace guigui {

/**
 * @class Context
 * @brief Main GUI context that manages the rendering loop and event handling
 * 
 * The Context class is the central hub of the Guillaume GUI framework. It manages
 * the rendering loop, handles events, and provides the interface for setting up
 * and running a GUI application.
 * 
 * @details The Context class:
 * - Manages a renderer instance for drawing operations
 * - Handles the main event loop and frame processing
 * - Manages the root component hierarchy
 * - Provides event routing to appropriate components
 * - Controls when the screen needs to be redrawn
 * 
 * @note This class follows RAII principles and automatically cleans up resources
 * when destroyed.
 */
class Context {

private:
    std::shared_ptr<Renderer> _renderer;           ///< Renderer instance for drawing operations
    std::unique_ptr<Font> _default_font;           ///< Default font for text rendering
    std::unique_ptr<Container> _root_component;    ///< Root component of the UI hierarchy
    bool _needs_redraw = true;                     ///< Flag indicating if screen needs redraw (starts true for initial frame)

    /**
     * @brief Internal main loop implementation
     * 
     * Handles the core event processing and rendering loop.
     * This is called internally by the run() method.
     */
    void _loop();

protected:
public:
    /**
     * @brief Constructs a new Context with the specified renderer
     * 
     * @param renderer Shared pointer to a renderer implementation
     * @throws std::runtime_error If renderer initialization fails
     * 
     * @details Initializes the context with the provided renderer and sets up
     * event handlers for keyboard, mouse, and quit events. The renderer is
     * initialized with the window title "Guillaume".
     */
    Context(std::shared_ptr<Renderer> renderer);
    
    /**
     * @brief Default destructor
     * 
     * Automatically cleans up all resources. The renderer and components
     * are automatically destroyed when the context is destroyed.
     */
    ~Context() = default;

    /**
     * @brief Sets the root component for the UI hierarchy
     * 
     * @param root_component Unique pointer to the root container component
     * 
     * @details The root component becomes the top-level container for all
     * UI elements. The context takes ownership of the component.
     */
    void set_root_component(std::unique_ptr<Container> root_component);
    
    /**
     * @brief Gets the current root component
     * 
     * @return std::unique_ptr<Container> The root component (may be null)
     * 
     * @details Returns ownership of the root component to the caller.
     * After calling this method, the context will have no root component.
     */
    std::unique_ptr<Container> get_root_component();

    /**
     * @brief Begins a frame rendering cycle
     * 
     * Prepares the renderer for drawing operations. Should be called
     * before any drawing operations in a frame.
     */
    void begin();
    
    /**
     * @brief Processes events and updates components for the current frame
     * 
     * Handles all pending events, updates component states, and performs
     * any necessary redrawing operations.
     */
    void process_frame();
    
    /**
     * @brief Ends the current frame rendering cycle
     * 
     * Finalizes the frame and presents it to the screen. Should be called
     * after all drawing operations are complete.
     */
    void end();
    
    /**
     * @brief Runs the main application loop
     * 
     * @throws std::runtime_error If the rendering loop encounters an error
     * 
     * @details Starts the main event loop which continues until a quit event
     * is received. This method blocks until the application is terminated.
     */
    void run();
    
    /**
     * @brief Marks the context as needing a redraw
     * 
     * Sets the internal flag to indicate that the screen should be redrawn
     * on the next frame. This is typically called when UI state changes.
     */
    void mark_dirty();
    
    /**
     * @brief Checks if the context needs to be redrawn
     * 
     * @return bool True if the screen needs to be redrawn, false otherwise
     * 
     * @details This can be used to optimize rendering by only drawing when
     * necessary. The flag is automatically cleared after rendering.
     */
    bool needs_redraw() const;
};
} // namespace guigui
