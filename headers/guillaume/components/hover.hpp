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

#include <functional>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components {

/**
 * @brief Component representing a hoverable entity.
 * @see systems::Hover
 */
class Hover : public ecs::Component {
  public:
    using Handler = std::function<void(void)>; ///< Hover event handler type

  private:
    Handler _onHover;   ///< Hover enter event handler
    Handler _onUnhover; ///< Hover leave event handler
    bool _isHovered{false};

  public:
    /**
     * @brief Default constructor for the Hover component.
     */
    Hover(void) = default;

    /**
     * @brief Default destructor for the Hover component.
     */
    ~Hover(void) = default;

    /**
     * @brief Set the onHover event handler.
     * @param handler The function to call on hover events.
     */
    void setOnHoverHandler(const Handler &handler) { _onHover = handler; }

    /**
     * @brief Set the onUnhover event handler.
     * @param handler The function to call when the cursor leaves the entity.
     */
    void setOnUnhoverHandler(const Handler &handler) { _onUnhover = handler; }

    /**
     * @brief Get the onHover event handler.
     * @return The onHover event handler.
     */
    Handler getOnHoverHandler(void) const { return _onHover; }

    /**
     * @brief Get the onUnhover event handler.
     * @return The onUnhover event handler.
     */
    Handler getOnUnhoverHandler(void) const { return _onUnhover; }

    /**
     * @brief Check if the entity is currently hovered.
     * @return True when the cursor is inside the entity bounds.
     */
    bool getIsHovered(void) const { return _isHovered; }

    /**
     * @brief Set current hovered state.
     * @param isHovered Current hovered state.
     */
    void setIsHovered(bool isHovered) { _isHovered = isHovered; }
};

} // namespace guillaume::components