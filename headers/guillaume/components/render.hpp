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

#include <filesystem>
#include <functional>
#include <optional>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity.hpp"
#include "guillaume/renderer.hpp"

namespace guillaume::components {

/**
 * @brief Component representing renderable entities.
 * @see systems::Render
 */
class Render : public ecs::Component {
  public:
    /**
     * @brief Type alias for render event handlers.
     * The handler takes a reference to the ComponentRegistry and the entity
     * identifier as parameters.
     */
    using Handler =
        std::function<void(ecs::ComponentRegistry &componentRegistry,
                           const ecs::Entity::Identifier &entityIdentifier,
                           Renderer &renderer)>; ///< Render event handler type

  private:
    Handler _normal = nullptr;  ///< Render event handler
    Handler _hovered = nullptr; ///< Hover render event handler
    Handler _clicked = nullptr; ///< Clicked render event handler
    Handler _active = nullptr;  ///< Active render event handler

  public:
    /**
     * @brief Default constructor for the Render component.
     */
    Render(void) = default;

    /**
     * @brief Default destructor for the Render component.
     */
    ~Render(void) = default;

    /**
     * @brief Set the normal state render function.
     * @param handler The function that will be called when the component is in
     * normal state.
     * @return Reference to this Render component for chaining.
     */
    Render &setNormalHandler(const Handler &handler) {
        _normal = handler;
        return *this;
    }

    /**
     * @brief Set the hovered state render function.
     * @param handler The function that will be called when the component is in
     * hovered state.
     * @return Reference to this Render component for chaining.
     */
    Render &setHoveredHandler(const Handler &handler) {
        _hovered = handler;
        return *this;
    }

    /**
     * @brief Set the clicked state render function.
     * @param handler The function that will be called when the component is
     * in clicked state.
     * @return Reference to this Render component for chaining.
     */
    Render &setClickedHandler(const Handler &handler) {
        _clicked = handler;
        return *this;
    }

    /**
     * @brief Set the active state render function.
     * @param handler The function that will be called when the component is
     * in active state.
     * @return Reference to this Render component for chaining.
     */
    Render &setActiveHandler(const Handler &handler) {
        _active = handler;
        return *this;
    }

    /**
     * @brief Get the normal state render function.
     * @return The function that will be called when the component is in
     * normal state.
     */
    const Handler &getNormalHandler(void) const { return _normal; }

    /**
     * @brief Get the hovered state render function.
     * @return The function that will be called when the component is in
     * hovered state.
     */
    const Handler &getHoveredHandler(void) const { return _hovered; }

    /**
     * @brief Get the clicked state render function.
     * @return The function that will be called when the component is in
     * clicked state.
     */
    const Handler &getClickedHandler(void) const { return _clicked; }

    /**
     * @brief Get the active state render function.
     * @return The function that will be called when the component is in
     * active state.
     */
    const Handler &getActiveHandler(void) const { return _active; }
};
} // namespace guillaume::components